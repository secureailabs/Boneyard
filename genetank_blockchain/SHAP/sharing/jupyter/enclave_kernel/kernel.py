from ipykernel.kernelbase import Kernel
#from pexpect import replwrap, EOF
#import pexpect
import http.client
import json

#from subprocess import check_output
import os
import base64

import re

__version__ = '0.0.2'

version_pat = re.compile(r'version (\d+(\.\d+)+)')


def place_value(number):
    return ("{:,}".format(number))

def getFileList(fileArray):
    output = ""
    for file in fileArray:
        size = file["size"]
        sizeStr = place_value(size)
        if file["folder"] == 0:
            folder = "F "
        else:
            folder = "D "
        output = output+folder+" "+sizeStr.rjust(12, ' ')+" "+file["name"]+"\n"
    return output
        
class EnclaveKernel(Kernel):
    implementation = 'enclave_kernel'
    implementation_version = __version__

    @property
    def language_version(self):
        m = version_pat.search(self.banner)
        return m.group(1)

    _banner = None

    @property
    def banner(self):
        if self._banner is None:
            # check_output(['bash', '--version']).decode('utf-8')
            self._banner = "Secure AI Labs"
        return self._banner

    language_info = {'name': 'Python',
                     'codemirror_mode': 'shell',
                     'mimetype': 'text/x-sh',
                     'file_extension': '.sh'}

    def __init__(self, **kwargs):
        Kernel.__init__(self, **kwargs)
        self._enclavePort = int(os.getenv('ENCLAVE_PORT'))
        self._jupyterId = os.getenv('JUPYTER_ID')
        self._jupyterToken = os.getenv('JUPYTER_TOKEN')
        self._username = os.getenv('JUPYTER_USER')
        self._enclave = ''
        self._subEnclave = ''
        self._currentDir = '/'
        self._start_enclave()

    def _start_enclave(self):
        # Register Bash function to write image data to temporary file
        # self.bashwrapper.run_command(image_setup_cmd)
        return

    def process_output(self, output):
        if not self.silent:
            #image_filenames, output = extract_image_filenames(output)

            # Send standard output
            stream_content = {'name': 'stdout', 'text': output}
            self.send_response(self.iopub_socket, 'stream', stream_content)

    def do_execute(self, code, silent, store_history=True,
                   user_expressions=None, allow_stdin=False):
        self.silent = silent
        if not code.strip():
            return {'status': 'ok', 'execution_count': self.execution_count,
                    'payload': [], 'user_expressions': {}}

        #interrupted = False

        try:
            # Note: timeout=None tells IREPLWrapper to do incremental
            # output.  Also note that the return value from
            # run_command is not needed, because the output was
            # already sent by IREPLWrapper.
            exitcode = self.run_command(code.rstrip())
        except Exception:
            exitcode = 1

        if exitcode:
            error_content = {
                'ename': '',
                'evalue': str(exitcode),
                'traceback': []
            }
            self.send_response(self.iopub_socket, 'error', error_content)

            error_content['execution_count'] = self.execution_count
            error_content['status'] = 'error'
            return error_content
        else:
            return {'status': 'ok', 'execution_count': self.execution_count,
                    'payload': [], 'user_expressions': {}}

    def send_text_resp(self, resp):
        stream_content = {'name': 'stdout', 'text': resp}
        self.send_response(self.iopub_socket, 'stream', stream_content)

    def show_status(self):
        result = "User: " + self._username + "\nHub Enclave: " + \
            self._enclave + "\nSub Enclave :" + self._subEnclave + "\n"
        self.send_text_resp(result)

    def getNewPath(self, path):
        newPath = self._currentDir
        if len(path) == 0:
            pass
        elif path[0] == '/':
            newPath = path
            path = ""
        else:
            while path[0] == '.':
                if path.startswith("./"):
                    path = path[2:]
                elif path.startswith("../"):
                    i = newPath.rfind('/')
                    if (i > 0):
                        newPath = newPath[0:i]
                    elif (i == 0):
                        newPath = '/'
                    else:
                        break
                    path = path[3:]
                    if len(path) == 0:
                        break
                elif path.startswith(".."):
                    i = newPath.rfind('/')
                    if (i > 0):
                        newPath = newPath[0:i]
                    elif (i == 0):
                        newPath = '/'
                    path = path[2:0]
                    break
                else:
                    break

        if newPath == '/': 
            newPath = newPath + path
        elif len(path)>0:
            newPath = newPath + "/" + path
        return newPath

    def run_command(self, cmd):
        op_code = "run"
        filename = ""
        code = cmd
        codeLen = len(code)
        if cmd.startswith("#%"):
            cmd = cmd[2:].strip()
            if cmd.startswith("config"):
                config = json.loads(cmd[6:])
                try:
                    self._enclave = config['Enclave']
                    self._subEnclave = config['SubEnclave']
                except KeyError:
                    self.send_text_resp("Bad key(s).")
                self.show_status()
                return 0
            elif cmd.startswith("status"):
                self.show_status()
                return 0
            elif cmd.startswith("ls"):
                op_code = "ls"
                cmd = cmd[2:].strip()
                filename = self.getNewPath(cmd)
            elif cmd.startswith("cd"):
                op_code = "cd"
                cmd = cmd[2:].strip()
                filename = self.getNewPath(cmd)
            elif cmd.startswith("rm"):
                op_code = "rm"
                cmd = cmd[2:].strip()
                filename = self.getNewPath(cmd)
            elif cmd.startswith("upload"):
                op_code = "upload"
                cmd = cmd[6:]
                i = cmd.find("\n")
                filename = cmd[:i].strip()
                filename = self.getNewPath(filename)
                code = cmd[(i+1):]
                codeLen = len(code)
                code = str(base64.b64encode(code.encode("utf-8")),"utf-8")
            else:
                self.send_text_resp("Error: Unknown #% command: "+cmd)
                return 0

        connection = http.client.HTTPConnection('localhost',
                                                self._enclavePort, timeout=300)

        headers = {'Content-type': 'application/json'}
        content = {'opCode': op_code,
                   'filename': filename,
                   'code': code,
                   'codeLen': codeLen,
                   'id': self._jupyterId,
                   'token': self._jupyterToken,
                   'enclave': self._enclave,
                   'subEnclave': self._subEnclave
                   }
        json_data = json.dumps(content)

        connection.request('POST', '/enclaveKernel', json_data, headers)

        response = connection.getresponse()
        resp = response.read()
        respdec = resp.decode()
        # self.send_text_resp(respdec)
        respjson = json.loads(respdec)
        response.close()
        connection.close()
        error = True
        output = ''
        try:
            output = "Error: "+respjson["error"]
        except KeyError:
            error = False

        if not error:
            output = ""
            if (op_code.startswith("cd")):
                self._currentDir = filename
                output = filename
            elif (op_code.startswith("ls") or op_code.startswith("cd")):
                output = filename + ":\n" + getFileList(respjson["result"])
            else:
                output = respjson["result"]

        self.send_text_resp(output)

        if 'image' in respjson:
            data = {
                'data': {
                    # 'image/svg+xml': base64.b64decode(respjson["image"]).decode('utf-8')
                    'image/png': respjson["image"]},
                'metadata': {
                    # 'image/png': {
                    #    'width': 800,
                    #    'height': 700
                    # }
                }
            }
            self.send_response(self.iopub_socket, 'display_data', data)
        return 0

// Copyright by Contributors

#include <dmlc/logging.h>
#include <errno.h>
extern "C" {
//#include <sys/stat.h>
#include "../../../../../Include/enclave_wrapper/sys.h"
#include "../../../../../Include/enclave_wrapper/io.h"
}
#ifndef _MSC_VER
extern "C" {
#include <sys/types.h>
//#include <dirent.h>
}
#else
#include <Windows.h>
#define stat _stat64
#endif

#include "./local_filesys.h"

#if defined(__FreeBSD__) && DMLC_USE_FOPEN64
#define fopen64 std::fopen
#endif


namespace dmlc {
namespace io {
/*! \brief implementation of file i/o stream */
class FileStream : public SeekStream {
 public:
  explicit FileStream(FILE *fp, bool use_stdio)
      : fp_(fp), use_stdio_(use_stdio) {}
  virtual ~FileStream(void) {
    this->Close();
  }
  virtual size_t Read(void *ptr, size_t size) {
    return fread(ptr, 1, size, fp_);
  }
  virtual void Write(const void *ptr, size_t size) {
    //CHECK(std::fwrite(ptr, 1, size, fp_) == size)
    //    << "FileStream.Write incomplete";
	fwrite(ptr, 1, size, fp_);
  }
  virtual void Seek(size_t pos) {
//#ifndef _MSC_VER
//    CHECK(!std::fseek(fp_, static_cast<long>(pos), SEEK_SET));  // NOLINT(*)
//#else
//    CHECK(!_fseeki64(fp_, pos, SEEK_SET));
//#endif
	  fseek(fp_, static_cast<long>(pos), SEEK_SET);
  }
  virtual size_t Tell(void) {
//#ifndef _MSC_VER
//    return std::ftell(fp_);
//#else
//    return _ftelli64(fp_);
//#endif
	  ftell(fp_);
  }
  virtual bool AtEnd(void) const {
    //return std::feof(fp_) != 0;
	  return feof(fp_) != 0;
  }
  inline void Close(void) {
//    if (fp_ != NULL && !use_stdio_) {
//      std::fclose(fp_); fp_ = NULL;
//    }
	  fclose(fp_);
  }

 private:
  //std::FILE *fp_;
  FILE *fp_;
  bool use_stdio_;
};

FileInfo LocalFileSystem::GetPathInfo(const URI &path) {
  struct stat sb;
  if (stat(path.name.c_str(), &sb) == -1) {
    int errsv = errno;
    LOG(FATAL) << "LocalFileSystem.GetPathInfo " << path.name
               << " Error:" << strerror(errsv);
  }
  FileInfo ret;
  ret.path = path;
  ret.size = sb.st_size;

//  if ((sb.st_mode & S_IFMT) == S_IFDIR) {
    ret.type = kDirectory;
//  } else {
//    ret.type = kFile;
//  }
  return ret;
}

void LocalFileSystem::ListDirectory(const URI &path, std::vector<FileInfo> *out_list) {
//#ifndef _MSC_VER
//  DIR *dir = opendir(path.name.c_str());
//  if (dir == NULL) {
//    int errsv = errno;
//    LOG(FATAL) << "LocalFileSystem.ListDirectory " << path.str()
//               <<" error: " << strerror(errsv);
//  }
//  out_list->clear();
//  struct dirent *ent;
//  /* print all the files and directories within directory */
//  while ((ent = readdir(dir)) != NULL) {
//    if (!strcmp(ent->d_name, ".")) continue;
//    if (!strcmp(ent->d_name, "..")) continue;
//    URI pp = path;
//    if (pp.name[pp.name.length() - 1] != '/') {
//      pp.name += '/';
//    }
//    pp.name += ent->d_name;
//    out_list->push_back(GetPathInfo(pp));
//  }
//  closedir(dir);
//#else
//  WIN32_FIND_DATA fd;
//  std::string pattern = path.name + "/*";
//  HANDLE handle = FindFirstFile(pattern.c_str(), &fd);
//  if (handle == INVALID_HANDLE_VALUE) {
//    int errsv = GetLastError();
//    LOG(FATAL) << "LocalFileSystem.ListDirectory " << path.str()
//               << " error: " << strerror(errsv);
//  }
//  do {
//    if (strcmp(fd.cFileName, ".") && strcmp(fd.cFileName, "..")) {
//      URI pp = path;
//      char clast = pp.name[pp.name.length() - 1];
//      if (pp.name == ".") {
//        pp.name = fd.cFileName;
//      } else if (clast != '/' && clast != '\\') {
//        pp.name += '/';
//        pp.name += fd.cFileName;
//      }
//      out_list->push_back(GetPathInfo(pp));
//    }
//  }  while (FindNextFile(handle, &fd));
//  FindClose(handle);
//#endif
}

SeekStream *LocalFileSystem::Open(const URI &path,
                                  const char* const mode,
                                  bool allow_null) {
  bool use_stdio = false;
  FILE *fp = NULL;
#ifdef _MSC_VER
  const int fname_length = MultiByteToWideChar(CP_UTF8, 0, path.name.c_str(), -1, nullptr, 0);
  CHECK(fname_length > 0) << " LocalFileSystem::Open \"" << path.str()
                          << "\": " << "Invalid character sequence.";
  std::wstring fname(fname_length, 0);
  MultiByteToWideChar(CP_UTF8, 0, path.name.c_str(), -1, &fname[0], fname_length);

  const int mode_length = MultiByteToWideChar(CP_UTF8, 0, mode, -1, nullptr, 0);
  std::wstring wmode(mode_length, 0);
  MultiByteToWideChar(CP_UTF8, 0, mode, -1, &wmode[0], mode_length);

  using namespace std;
#ifndef DMLC_DISABLE_STDIN
  if (!wcscmp(fname.c_str(), L"stdin")) {
    use_stdio = true; fp = stdin;
  }
  if (!wcscmp(fname.c_str(), L"stdout")) {
    use_stdio = true; fp = stdout;
  }
#endif
  if (!wcsncmp(fname.c_str(), L"file://", 7)) { fname = fname.substr(7); }
  if (!use_stdio) {
    std::wstring flag(wmode.c_str());
    if (flag == L"w") flag = L"wb";
    if (flag == L"r") flag = L"rb";
#if DMLC_USE_FOPEN64
    fp = _wfopen(fname.c_str(), flag.c_str());
#else
    fp = fopen(fname, flag.c_str());
#endif
  }
#else
  const char *fname = path.name.c_str();
  using namespace std;
#ifndef DMLC_DISABLE_STDIN
  if (!strcmp(fname, "stdin")) {
    use_stdio = true; fp = stdin;
  }
  if (!strcmp(fname, "stdout")) {
    use_stdio = true; fp = stdout;
  }
#endif
  if (!strncmp(fname, "file://", 7)) fname += 7;
  if (!use_stdio) {
    std::string flag = mode;
    if (flag == "w") flag = "wb";
    if (flag == "r") flag = "rb";
#if DMLC_USE_FOPEN64
    fp = fopen64(fname, flag.c_str());
#else
    fp = fopen(fname, flag.c_str());
#endif
  }
#endif
  if (fp != NULL) {
    return new FileStream(fp, use_stdio);
  } else {
    CHECK(allow_null) << " LocalFileSystem::Open \"" << path.str() << "\": " << strerror(errno);
    return NULL;
  }
}
SeekStream *LocalFileSystem::OpenForRead(const URI &path, bool allow_null) {
  return Open(path, "r", allow_null);
}
}  // namespace io
}  // namespace dmlc

import os
import sys
import json
import doctest
import unittest

from test import test_support

# import json with and without accelerations
cjson = test_support.import_fresh_module('json', fresh=['_json'])
pyjson = test_support.import_fresh_module('json', blocked=['_json'])

# create two base classes that will be used by the other tests
class PyTest(unittest.TestCase):
    json = pyjson
    loads = staticmethod(pyjson.loads)
    dumps = staticmethod(pyjson.dumps)

@unittest.skipUnless(cjson, 'requires _json')
class CTest(unittest.TestCase):
    if cjson is not None:
        json = cjson
        loads = staticmethod(cjson.loads)
        dumps = staticmethod(cjson.dumps)

# test PyTest and CTest checking if the functions come from the right module
class TestPyTest(PyTest):
    def test_pyjson(self):
        self.assertEqual(self.json.scanner.make_scanner.__module__,
                         'json.scanner')
        self.assertEqual(self.json.decoder.scanstring.__module__,
                         'json.decoder')
        self.assertEqual(self.json.encoder.encode_basestring_ascii.__module__,
                         'json.encoder')

class TestCTest(CTest):
    def test_cjson(self):
        self.assertEqual(self.json.scanner.make_scanner.__module__, '_json')
        self.assertEqual(self.json.decoder.scanstring.__module__, '_json')
        self.assertEqual(self.json.encoder.c_make_encoder.__module__, '_json')
        self.assertEqual(self.json.encoder.encode_basestring_ascii.__module__,
                         '_json')


here = os.path.dirname(__file__)

def test_suite():
    suite = additional_tests()
    loader = unittest.TestLoader()
    test_list = ['test_check_circular.py', 'test_decode.py', 'test_default.py', 'test_dump.py', 'test_encode_basestring_ascii.py', 'test_fail.py', 'test_float.py', 'test_indent.py', 'test_pass1.py', 'test_pass2.py', 'test_pass3.py', 'test_recursion.py', 'test_scanstring.py', 'test_separators.py', 'test_speedups.py', 'test_tool.py', 'test_unicode.py']
    for fn in test_list:
        if fn.startswith("test") and fn.endswith(".py"):
            modname = "json.tests." + fn[:-3]
            __import__(modname)
            module = sys.modules[modname]
            suite.addTests(loader.loadTestsFromModule(module))
    return suite

def additional_tests():
    suite = unittest.TestSuite()
    for mod in (json, json.encoder, json.decoder):
        suite.addTest(doctest.DocTestSuite(mod))
    suite.addTest(TestPyTest('test_pyjson'))
    suite.addTest(TestCTest('test_cjson'))
    return suite

def main():
    suite = test_suite()
    runner = unittest.TextTestRunner()
    runner.run(suite)

if __name__ == '__main__':
    sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    main()

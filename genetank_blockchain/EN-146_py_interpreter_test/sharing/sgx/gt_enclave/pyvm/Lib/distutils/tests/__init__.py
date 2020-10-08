"""Test suite for distutils.

This test suite consists of a collection of test modules in the
distutils.tests package.  Each test module has a name starting with
'test' and contains a function test_suite().  The function is expected
to return an initialized unittest.TestSuite instance.

Tests for the command classes in the distutils.command package are
included in distutils.tests as well, instead of using a separate
distutils.command.tests package, since command identification is done
by import rather than matching pre-defined names.

"""

import os
import sys
import unittest
from test.test_support import run_unittest


here = os.path.dirname(__file__) or os.curdir


def test_suite():
    suite = unittest.TestSuite()
    tests_list = ['test_archive_util.py', 'test_bdist_dumb.py', 'test_bdist_msi.py', 'test_bdist.py', 'test_bdist_rpm.py', 'test_bdist_wininst.py', 'test_build_clib.py',
    'test_build_ext.py', 'test_build.py', 'test_build_py.py', 'test_build_scripts.py', 'test_ccompiler.py', 'test_check.py', 'test_clean.py', 'test_cmd.py',
    'test_config_cmd.py', 'test_config.py', 'test_core.py', 'test_dep_util.py', 'test_dir_util.py', 'test_dist.py', 'test_filelist.py',
    'test_file_util.py', 'test_install_data.py', 'test_install_headers.py', 'test_install_lib.py', 'test_install.py', 'test_install_scripts.py',
    'test_msvc9compiler.py', 'test_register.py', 'test_sdist.py', 'test_spawn.py', 'test_sysconfig.py', 'test_text_file.py', 'test_unixccompiler.py',
    'test_upload.py', 'test_util.py', 'test_versionpredicate.py', 'test_version.py',
    ]
    for fn in tests_list:
        if fn.startswith("test") and fn.endswith(".py"):
            modname = "distutils.tests." + fn[:-3]
            __import__(modname)
            module = sys.modules[modname]
            suite.addTest(module.test_suite())
    return suite


if __name__ == "__main__":
    run_unittest(test_suite())

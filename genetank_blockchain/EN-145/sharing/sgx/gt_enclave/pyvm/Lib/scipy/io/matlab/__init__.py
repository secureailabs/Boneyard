"""
Utilities for dealing with MATLAB(R) files

Notes
-----
MATLAB(R) is a registered trademark of The MathWorks, Inc., 3 Apple Hill
Drive, Natick, MA 01760-2098, USA.

"""
from __future__ import division, print_function, absolute_import

# Matlab file read and write utilities
from .mio import loadmat, savemat, whosmat
from .byteordercodes import *

__all__ = ['loadmat', 'savemat', 'whosmat', 'byteordercodes', 'sys_is_le', 'native_code', 'swapped_code', 'aliases', 'to_numpy_code']

#from scipy._lib._testutils import PytestTester
#test = PytestTester(__name__)
#del PytestTester

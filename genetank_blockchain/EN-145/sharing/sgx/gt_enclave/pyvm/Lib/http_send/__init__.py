# -*- coding: utf-8 -*-

#   __
#  /__)  _  _     _   _ _/   _
# / (   (- (/ (/ (- _)  /  _)
#          /

"""
http_rest library
~~~~~~~~~~~~~~~~~~~~~

The http_rest is a Python Restful API developed with C/C++.
Basic GET usage is the same as requests library:

   >>> import http_rest as requests
   >>> r = requests.get('https://www.python.org')
   >>> r.status_code
   200
   >>> 'Python is a programming language' in r.content
   True

... or POST:

   >>> payload = dict(key1='value1', key2='value2')
   >>> r = requests.post('http://httpbin.org/post', data=payload)
   >>> print(r.text)
   {
     ...
     "form": {
       "key2": "value2",
       "key1": "value1"
     },
     ...
   }

The other HTTP methods are supported - see `requests.api`. Full documentation
is at <http://python-requests.org>.

:copyright: (c) 2015 by Kenneth Reitz.
:license: Apache 2.0, see LICENSE for more details.

"""

__title__ = 'http_rest'
__version__ = '0.1.0'
__build__ = 0x202001
__author__ = 'ShifaSz'
__license__ = 'Apache 2.0'
__copyright__ = 'Copyright 2015 Kenneth Reitz'

from .api import request, get, head, post, patch, put, delete, options
from .sessions import session, Session


#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(ZLIBSRCDIR\)\/base\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include -I../ -I../../ *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(ZLIBSRCDIR\)\/base\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(ZLIBSRCDIR\)\/base\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(ZLIBSRCDIR\)\/base\/&/g' filedep.mk


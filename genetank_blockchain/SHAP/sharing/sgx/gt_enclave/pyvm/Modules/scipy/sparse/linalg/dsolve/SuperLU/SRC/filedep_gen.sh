#! /bin/bash

ls *.c >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/scipy\/sparse\/linalg\/dsolve\/SuperLU\/SRC\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
gcc -M -I./ -I../../../../../Include -I../../../../include -I../_lib/src/ -I../../../../numpy/core/include -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/scipy\/sparse\/linalg\/dsolve\/SuperLU\/SRC\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/scipy\/sparse\/linalg\/dsolve\/SuperLU\/SRC\/&/g' filedep.mk


#! /bin/bash

ls *.cpp >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/matplotlib\/tri\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
gcc -M -I./ -I../ -I../../../Include/ -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.cpp > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/matplotlib\/tri\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/matplotlib\/tri\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/matplotlib\/tri\/&/g' filedep.mk


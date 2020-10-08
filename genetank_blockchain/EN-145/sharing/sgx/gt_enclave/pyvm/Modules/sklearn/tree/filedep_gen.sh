#! /bin/bash

ls *.c >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/sklearn\/tree\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
gcc -M -I./ -I../../../Include -I../../numpy/core/include -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/sklearn\/tree\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/sklearn\/tree\/&/g' filedep.mk


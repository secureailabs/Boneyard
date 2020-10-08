#! /bin/bash

ls *.cpp >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/sklearn\/cluster\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > filecxx.mk
rm tmp1
g++ -M -I./ -I../../../Include -I../../../../include -I../_lib/ -I../../numpy/core/include -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.cpp > filedepcxx.mk
sed -i 's/\busr[^ ]*//g'  filedepcxx.mk
sed -i 's/\/ \\//g' filedepcxx.mk
sed -i 's/\/ //g' filedepcxx.mk
sed -i '/^ *$/d' filedepcxx.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedepcxx.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/sklearn\/cluster\/\1\.c/g' filedepcxx.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/sklearn\/cluster\/&/g' filedepcxx.mk


#! /bin/bash

ls *.cc >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/xgboost\/rabit\/src\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
g++ -std=c++11 -M -I./ -I../ -I../include/ -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.cc > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/xgboost\/rabit\/src\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/xgboost\/rabit\/src\/&/g' filedep.mk


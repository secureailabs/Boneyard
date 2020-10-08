#! /bin/bash

ls *.cc >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/xgboost\/src\/objective\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
g++ -std=c++11 -M -I./ -I../../include -I../../rabit/include/ -I../../dmlc-core/include/ -I/home/accuser/sgxsdk/include -DPy_BUILD_CORE *.cc > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/xgboost\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/xgboost\/src\/objective\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/xgboost\/src\/objective\/&/g' filedep.mk


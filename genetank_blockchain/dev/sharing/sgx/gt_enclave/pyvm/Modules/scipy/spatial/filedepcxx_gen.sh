#! /bin/bash

ls *.cxx >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/scipy\/spatial\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > filecxx.mk
rm tmp1
g++ -M -I./ -I../../../Include -I../../../../include -I../_lib/src/ -I../../numpy/core/include -I/home/accuser/sgxsdk/include -I/home/jingwei/genetank_blockchain/sharing/sgx/gt_enclave/pyvm/Modules/scipy/spatial/ckdtree/src -DPy_BUILD_CORE *.cxx > filedepcxx.mk
sed -i 's/\busr[^ ]*//g'  filedepcxx.mk
sed -i 's/\/ \\//g' filedepcxx.mk
sed -i 's/\/ //g' filedepcxx.mk
sed -i '/^ *$/d' filedepcxx.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedepcxx.mk
sed -i 's/ \(.*\)\.c/\$\(MODULESRCDIR\)\/scipy\/spatial\/\1\.c/g' filedepcxx.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/scipy\/spatial\/&/g' filedepcxx.mk


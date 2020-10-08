#! /bin/bash

#ls *.cpp >tmp1
#sed -i 's/^/\$\(MODULESRCDIR\)\/matplotlib\//g' tmp1
#sed -i 's/$/ \\/g' tmp1
#cat tmp1 > files.mk
#rm tmp1
gcc -M -I./matplotlib -I../Include -I../../include -I../../trusted_libraries -I./matplotlib/extern -I./matplotlib/extern/libqhull -I./matplotlib/extern/agg24-svn/include -I./numpy/core/include -I/home/accuser/sgxsdk/include  -I../../../gt_enclave -I../../../gt_app/include -DPy_BUILD_CORE matplotlib/*.cpp > mfiledep.mk
sed -i 's/\busr[^ ]*//g'  mfiledep.mk
sed -i 's/\/ //g' mfiledep.mk
sed -i '/^ *$/d' mfiledep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' mfiledep.mk
sed -i 's/ \(.*\)\.cpp/\$\(MODULESRCDIR\)\/\1\.cpp/g' mfiledep.mk
sed -i 's/.*\.o/\$\(PY_BUILD\)\/\$\(MODULESRCDIR\)\/matplotlib\/&/g' mfiledep.mk
sed -i '/^ \\/d' mfiledep.mk

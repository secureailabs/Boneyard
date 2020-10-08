#! /bin/bash

ls *.cpp >tmp1
sed -i 's/^/\$\(MODULESRCDIR\)\/matplotlib\/extern\/agg24-svn\/src\/ctrl\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
gcc -M -I./ -I../../Include -I../../include -I/home/slowloris/sgxsdk/sgxsdk/include -DPy_BUILD_CORE *.cpp > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MODULESRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.cpp/\$\(MODULESRCDIR\)\/matplotlib\/extern\/agg24-svn\/src\/ctrl\/\1\.cpp/g' filedep.mk
sed -i 's/.*\.o/\$\(MODULESRCDIR\)\/matplotlib\/extern\/agg24-svn\/src\/ctrl\/&/g' filedep.mk


#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(FT2SRCDIR\)\/raster\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include -I/home/accuser/sgxsdk/include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(FT2SRCDIR\)\/raster\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(FT2SRCDIR\)\/raster\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(FT2SRCDIR\)\/raster\/&/g' filedep.mk


#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(FT2SRCDIR\)\/bdf\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(FT2SRCDIR\)\/bdf\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(FT2SRCDIR\)\/bdf\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(FT2SRCDIR\)\/bdf\/&/g' filedep.mk


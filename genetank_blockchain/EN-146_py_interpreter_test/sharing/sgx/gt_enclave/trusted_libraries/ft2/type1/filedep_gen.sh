#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(FT2SRCDIR\)\/type1\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(FT2SRCDIR\)\/type1\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(FT2SRCDIR\)\/type1\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(FT2SRCDIR\)\/type1\/&/g' filedep.mk


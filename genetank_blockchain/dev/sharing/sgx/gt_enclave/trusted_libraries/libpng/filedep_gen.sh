#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(PNGSRCDIR\)\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(PNGSRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(PNGSRCDIR\)\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(PNGSRCDIR\)\/&/g' filedep.mk


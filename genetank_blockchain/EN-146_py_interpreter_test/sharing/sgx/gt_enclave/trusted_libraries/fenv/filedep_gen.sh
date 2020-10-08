#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(FENVSRCDIR\)\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I../include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(FENVSRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(FENVSRCDIR\)\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(FENVSRCDIR\)\/&/g' filedep.mk
sed -i '/^ \\/d' filedep.mk


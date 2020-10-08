#! /bin/bash

ls *.c >tmp
sed -i 's/^/\$\(LIBFFISRCDIR\)\//g' tmp
sed -i 's/$/ \\/g' tmp
cat tmp > files.mk
gcc -M -I. -I../include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(LIBFFISRCDIR\)\/\.\./g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(LIBFFISRCDIR\)\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(LIBFFISRCDIR\)\/&/g' filedep.mk

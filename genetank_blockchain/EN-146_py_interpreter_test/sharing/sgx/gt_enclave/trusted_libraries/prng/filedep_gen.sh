#! /bin/bash

ls *.c >tmp1
sed -i 's/^/\$\(PRNGSRCDIR\)\//g' tmp1
sed -i 's/$/ \\/g' tmp1
cat tmp1 > files.mk
rm tmp1
gcc -M -I./ -I/home/accuser/sgxsdk/include *.c > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ \\//g' filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(PRNGSRCDIR\)\//g' filedep.mk
sed -i 's/ \(.*\)\.c/\$\(PRNGSRCDIR\)\/\1\.c/g' filedep.mk
sed -i 's/.*\.o/\$\(PRNGSRCDIR\)\/&/g' filedep.mk


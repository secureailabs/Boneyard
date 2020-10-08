g++ -MT 'rtemp' -M -m64 -O0 -g -nostdinc -fvisibility=hidden -fpie -ffunction-sections -fdata-sections -fstack-protector-strong -fPIC -I/home/accuser/sgxsdk/include -I/home/accuser/sgxsdk/include/tlibc -I../../../gt_app/include -I/home/accuser/sgxsdk/include/libcxx -Iinclude -I/home/accuser/sgxsdk/include/tlibc -I/home/accuser/sgxsdk/include/libcxx -std=c++11 -nostdinc++ -I../include -DHAVE_CONFIG_H *.c | sed 's/rtemp: \(.*\)\.c/\1.o:&/g' | sed 's/rtemp://g' > filedep.mk
sed -i 's/\busr[^ ]*//g'  filedep.mk
sed -i 's/\/ //g' filedep.mk
sed -i '/^ *$/d' filedep.mk
sed -i 's/\.\./\$\(MAINSRCDIR\)\/\.\./g' filedep.mk
sed -i 's/.*\.o/\$\(R_BUILD\)\/\$\(MAINSRCDIR\)\/&/g' filedep.mk
sed -i '/^ \\/d' filedep.mk


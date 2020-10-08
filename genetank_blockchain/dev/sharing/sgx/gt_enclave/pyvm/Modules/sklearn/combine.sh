#!/bin/bash
for f in $(find ./ -name 'files.mk'); do
    cat $f >> "outfiles.mk"
done

for f in $(find ./ -name 'filedep.mk'); do
    cat $f >> "outdep.mk"
done

for f in $(find ./ -name 'filecxx.mk'); do
    cat $f >> "outfilecxx.mk"
done

for f in $(find ./ -name 'filedepcxx.mk'); do
    cat $f >> "outdepcxx.mk"
done

#!/bin/bash
for f in $(find ./ -name 'files.mk'); do
    cat $f >> "outfiles.mk"
done

for f in $(find ./ -name 'filedep.mk'); do
    cat $f >> "outdep.mk"
done

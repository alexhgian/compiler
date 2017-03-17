#! /bin/bash
# clear


cd .tmp
for TEST in $@
do
    echo $TEST
    ../../p4exe < $TEST.glsl > $TEST.bc
    ../../gli $TEST.bc > $TEST.out
done
cd ../

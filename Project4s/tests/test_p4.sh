#! /bin/bash
# clear

cd $1

echo ""
echo "=============== Running p4exe ======================"
echo ""
../../p4exe < $2.glsl > $2.bc
llvm-dis $2.bc
cat $2.ll

echo ""
echo "=============== p4exe output ======================"
echo ""

../../gli $2.bc

cd ..

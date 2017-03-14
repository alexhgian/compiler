#! /bin/bash
# clear

echo ""
echo "=============== Running p4exe ======================"
echo ""
../p4exe < $1.glsl > $1.bc
llvm-dis $1.bc
cat $1.ll

echo ""
echo "=============== p4exe output ======================"
echo ""

../gli $1.bc
echo ""
echo "=============== Running glc ========================"
echo ""

../glc < $1.glsl > $1.bc

echo ""
echo "=============== gli output ========================="
echo ""

../gli $1.bc

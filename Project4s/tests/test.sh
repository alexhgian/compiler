#! /bin/bash
# clear

cd $1

echo ""
echo "=============== Running p4exe ======================"
echo ""
../../p4exe < $2.glsl > $2.bc
llvm-dis $2.bc
cat $2.ll
cat $2.ll > $2_p4.log

echo ""
echo "=============== p4exe output ======================"
echo ""

../../gli $2.bc
echo ""
echo "=============== Running glc ========================"
echo ""

../../glc < $2.glsl > $2.bc
llvm-dis $2.bc
cat $2.ll
cat $2.ll > $2_glc.log

echo ""
echo "=============== gli output ========================="
echo ""

../../gli $2.bc

echo ""
echo "=============== diff ========================="
echo ""

diff $2_p4.log $2_glc.log

rm $2_p4.log
rm $2_glc.log
cd ..

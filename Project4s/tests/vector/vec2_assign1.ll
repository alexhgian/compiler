; ModuleID = 'vec2_assign1.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @main() {
entry:
  %v2 = alloca <2 x float>
  br label %next

next:                                             ; preds = %entry
  %0 = load <2 x float>* %v2
  %1 = insertelement <2 x float> %0, float 0x400F333340000000, i32 0
  store volatile <2 x float> %1, <2 x float>* %v2
  %2 = load <2 x float>* %v2
  %3 = insertelement <2 x float> %2, float 1.000000e+00, i32 1
  store volatile <2 x float> %3, <2 x float>* %v2
  %4 = load <2 x float>* %v2
  %5 = extractelement <2 x float> %4, i32 0
  ret float %5
}

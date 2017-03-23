; ModuleID = 'vec2_test.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v = external global <2 x float>

define float @v2test(float %x) {
entry:
  %arg0 = alloca float
  %v2 = alloca <2 x float>
  br label %next

next:                                             ; preds = %entry
  store float %x, float* %arg0
  %0 = load <2 x float>* @v
  %1 = load float* %arg0
  %2 = insertelement <2 x float> undef, float %1, i32 0
  %3 = insertelement <2 x float> %2, float %1, i32 1
  %4 = fmul <2 x float> %0, %3
  store <2 x float> %4, <2 x float>* %v2
  %5 = load <2 x float>* %v2
  %6 = extractelement <2 x float> %5, i32 0
  %7 = load <2 x float>* %v2
  %8 = extractelement <2 x float> %7, i32 1
  %9 = fadd float %6, %8
  ret float %9
}

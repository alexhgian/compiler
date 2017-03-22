; ModuleID = 'vector_shuffle.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@v2 = external global <2 x float>

define float @foo(float %f) {
entry:
  %arg0 = alloca float
  %t = alloca float
  br label %next

next:                                             ; preds = %entry
  store float %f, float* %arg0
  %0 = load <2 x float>* @v2
  %1 = shufflevector <2 x float> %0, <2 x float> %0, <2 x i32> <i32 1, i32 0>
  %2 = extractelement <2 x float> %1, i32 0
  %3 = load float* %arg0
  %4 = fadd float %2, %3
  store float %4, float* %t
  %5 = load float* %t
  ret float %5
}

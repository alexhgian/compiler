; ModuleID = 'unary.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@a = external global <2 x float>

define float @unary(float %f) {
entry:
  %arg0 = alloca float
  %t = alloca float
  br label %next

next:                                             ; preds = %entry
  store float %f, float* %arg0
  %0 = load float* %arg0
  %1 = fadd float %0, 1.000000e+00
  store volatile float %1, float* %arg0
  store float %1, float* %t
  %2 = load float* %t
  %3 = fsub float %2, 1.000000e+00
  store volatile float %3, float* %t
  %4 = load <2 x float>* @a
  %5 = insertelement <2 x float> %4, float %3, i32 0
  store volatile <2 x float> %5, <2 x float>* @a
  %6 = load float* %t
  %7 = fadd float %6, 1.000000e+00
  store volatile float %7, float* %t
  %8 = load <2 x float>* @a
  %9 = insertelement <2 x float> %8, float %7, i32 1
  store volatile <2 x float> %9, <2 x float>* @a
  %10 = load <2 x float>* @a
  %11 = extractelement <2 x float> %10, i32 0
  %12 = load <2 x float>* @a
  %13 = extractelement <2 x float> %12, i32 1
  %14 = fadd float %11, %13
  %15 = load float* %arg0
  %16 = fsub float %15, 1.000000e+00
  store volatile float %16, float* %arg0
  %17 = fadd float %14, %15
  ret float %17
}

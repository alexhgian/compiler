; ModuleID = 'vect_assign_partial.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@a = external global <2 x float>

define float @assignvec() {
entry:
  %v4 = alloca <4 x float>
  br label %next

next:                                             ; preds = %entry
  %0 = load <2 x float>* @a
  %1 = load <4 x float>* %v4
  %2 = extractelement <2 x float> %0, i32 0
  %3 = insertelement <4 x float> %1, float %2, i32 0
  %4 = extractelement <2 x float> %0, i32 1
  %5 = insertelement <4 x float> %3, float %4, i32 1
  store volatile <4 x float> %5, <4 x float>* %v4
  %6 = load <4 x float>* %v4
  %7 = insertelement <4 x float> %6, float 5.000000e-01, i32 3
  store volatile <4 x float> %7, <4 x float>* %v4
  %8 = load <4 x float>* %v4
  %9 = insertelement <4 x float> %8, float 5.000000e-01, i32 2
  store volatile <4 x float> %9, <4 x float>* %v4
  %10 = load <4 x float>* %v4
  %11 = extractelement <4 x float> %10, i32 0
  %12 = load <4 x float>* %v4
  %13 = extractelement <4 x float> %12, i32 1
  %14 = fadd float %11, %13
  %15 = load <4 x float>* %v4
  %16 = extractelement <4 x float> %15, i32 2
  %17 = fadd float %14, %16
  %18 = load <4 x float>* %v4
  %19 = extractelement <4 x float> %18, i32 3
  %20 = fadd float %17, %19
  ret float %20
}

; ModuleID = 'assign_mult.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @main() {
entry:
  %x = alloca float
  br label %next

next:                                             ; preds = %entry
  store float 3.000000e+00, float* %x
  %0 = load float* %x
  %1 = fmul float %0, 3.000000e+00
  store float %1, float* %x
  %2 = load float* %x
  %3 = fdiv float %2, 2.000000e+00
  store float %3, float* %x
  %4 = load float* %x
  ret float %4
}

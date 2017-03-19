; ModuleID = 'call.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @f(float %x, float %y, float %z) {
entry:
  %arg0 = alloca float
  %arg1 = alloca float
  %arg2 = alloca float
  br label %next

next:                                             ; preds = %entry
  store float %x, float* %arg0
  store float %y, float* %arg1
  store float %z, float* %arg2
  %0 = load float* %arg0
  %1 = load float* %arg1
  %2 = fadd float %0, %1
  %3 = load float* %arg2
  %4 = fadd float %2, %3
  ret float %4
}

define float @main() {
entry:
  %x = alloca float
  br label %next

next:                                             ; preds = %entry
  store float 6.000000e+00, float* %x
  %0 = load float* %x
  %1 = call float @f(float 2.000000e+00, float 0x3FF99999A0000000, float %0)
  ret float %1
}

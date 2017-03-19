; ModuleID = 'assign.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @main() {
entry:
  %y = alloca float
  %x = alloca [10 x float]
  br label %next

next:                                             ; preds = %entry
  store float 0x400F333340000000, float* %y
  %0 = getelementptr [10 x float]* %x, i32 0, i32 2
  store float 0x400F333340000000, float* %0
  %1 = load float* %y
  %2 = getelementptr [10 x float]* %x, i32 0, i32 2
  %3 = load float* %2
  %4 = fadd float %1, %3
  ret float %4
}

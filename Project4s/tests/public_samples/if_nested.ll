; ModuleID = 'if_nested.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @ifnested(float %arg) {
entry:
  %arg0 = alloca float
  %f = alloca float
  br label %next

next:                                             ; preds = %entry
  store float %arg, float* %arg0
  %0 = load float* %arg0
  %1 = fcmp ogt float %0, 1.000000e+00
  br i1 %1, label %ThenBB, label %ElseBB

ThenBB:                                           ; preds = %next
  %2 = fcmp olt float 1.000000e+00, 5.000000e-01
  br i1 %2, label %ThenBB3, label %ElseBB2

ThenBB3:                                          ; preds = %ThenBB
  store float 1.000000e+00, float* %f
  br label %footerBB1

ElseBB2:                                          ; preds = %ThenBB
  store float 2.000000e+00, float* %f
  br label %footerBB1

footerBB1:                                        ; preds = %ElseBB2, %ThenBB3
  br label %footerBB

ElseBB:                                           ; preds = %next
  %3 = load float* %arg0
  store float %3, float* %f
  br label %footerBB

footerBB:                                         ; preds = %ElseBB, %footerBB1
  %4 = load float* %f
  ret float %4
}

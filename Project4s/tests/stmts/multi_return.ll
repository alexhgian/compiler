; ModuleID = 'multi_return.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @foo(float %a) {
entry:
  %arg0 = alloca float
  br label %next

next:                                             ; preds = %entry
  store float %a, float* %arg0
  %0 = load float* %arg0
  %1 = fadd float 0x4000CCCCC0000000, %0
  ret float %1
}

define float @bar(i32 %a) {
entry:
  %arg0 = alloca i32
  %t = alloca float
  br label %next

next:                                             ; preds = %entry
  store i32 %a, i32* %arg0
  %0 = load i32* %arg0
  %1 = icmp sgt i32 %0, 1
  br i1 %1, label %ThenBB, label %ElseBB

ThenBB:                                           ; preds = %next
  store float 0x4008CCCCC0000000, float* %t
  br label %footerBB

ElseBB:                                           ; preds = %next
  store float 2.500000e-01, float* %t
  br label %footerBB

footerBB:                                         ; preds = %ElseBB, %ThenBB
  %2 = load float* %t
  ret float %2
}
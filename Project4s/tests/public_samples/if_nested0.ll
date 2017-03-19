; ModuleID = 'if_nested0.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @ifnested() {
entry:
  %x = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 1, i32* %x
  %0 = load i32* %x
  %1 = icmp eq i32 %0, 1
  br i1 %1, label %ThenBB, label %footerBB

ThenBB:                                           ; preds = %next
  %2 = load i32* %x
  %3 = add i32 %2, 1
  store volatile i32 %3, i32* %x
  %4 = load i32* %x
  %5 = icmp eq i32 %4, 2
  br i1 %5, label %ThenBB2, label %footerBB1

ThenBB2:                                          ; preds = %ThenBB
  %6 = load i32* %x
  %7 = add i32 %6, 1
  store volatile i32 %7, i32* %x
  br label %footerBB1

footerBB1:                                        ; preds = %ThenBB2, %ThenBB
  br label %footerBB

footerBB:                                         ; preds = %footerBB1, %next
  %8 = load i32* %x
  ret i32 %8
}

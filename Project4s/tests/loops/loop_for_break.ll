; ModuleID = 'loop_for_break.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @main() {
entry:
  %sum = alloca i32
  %i = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 2, i32* %sum
  store i32 0, i32* %i
  br label %headerBB

headerBB:                                         ; preds = %stepBB, %next
  %0 = load i32* %i
  %1 = icmp slt i32 %0, 5
  br i1 %1, label %bodyBB, label %footerBB

bodyBB:                                           ; preds = %headerBB
  %2 = load i32* %sum
  %3 = add i32 %2, 1
  store volatile i32 %3, i32* %sum
  %4 = load i32* %i
  %5 = icmp sgt i32 %4, 1
  br i1 %5, label %ThenBB, label %footerBB1

ThenBB:                                           ; preds = %bodyBB
  br label %footerBB

footerBB1:                                        ; preds = %bodyBB
  br label %stepBB

stepBB:                                           ; preds = %footerBB1
  %6 = load i32* %i
  %7 = add i32 %6, 1
  store volatile i32 %7, i32* %i
  br label %headerBB

footerBB:                                         ; preds = %ThenBB, %headerBB
  %8 = load i32* %sum
  ret i32 %8
}

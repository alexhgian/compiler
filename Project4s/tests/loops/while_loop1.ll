; ModuleID = 'while_loop1.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @whiletest(i32 %b) {
entry:
  %arg0 = alloca i32
  %i = alloca i32
  %sum = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 %b, i32* %arg0
  store i32 0, i32* %i
  store i32 0, i32* %sum
  br label %headerBB

headerBB:                                         ; preds = %bodyBB, %next
  %0 = load i32* %i
  %1 = load i32* %arg0
  %2 = icmp slt i32 %0, %1
  br i1 %2, label %bodyBB, label %footerBB

bodyBB:                                           ; preds = %headerBB
  %3 = load i32* %sum
  %4 = load i32* %i
  %5 = add i32 %3, %4
  store i32 %5, i32* %sum
  %6 = load i32* %i
  %7 = add i32 %6, 1
  store i32 %7, i32* %i
  br label %headerBB

footerBB:                                         ; preds = %headerBB
  %8 = load i32* %sum
  ret i32 %8
}

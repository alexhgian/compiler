; ModuleID = 'array1.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @main(i32 %i) {
entry:
  %arg0 = alloca i32
  %a = alloca [10 x i32]
  %x = alloca float
  br label %next

next:                                             ; preds = %entry
  store i32 %i, i32* %arg0
  store i32 0, i32* %arg0
  br label %headerBB

headerBB:                                         ; preds = %stepBB, %next
  %0 = load i32* %arg0
  %1 = icmp slt i32 %0, 10
  br i1 %1, label %bodyBB, label %footerBB

bodyBB:                                           ; preds = %headerBB
  %2 = load i32* %arg0
  %3 = load i32* %arg0
  %4 = getelementptr [10 x i32]* %a, i32 0, i32 %3
  store i32 %2, i32* %4
  br label %stepBB

stepBB:                                           ; preds = %bodyBB
  %5 = load i32* %arg0
  %6 = add i32 %5, 1
  store volatile i32 %6, i32* %arg0
  br label %headerBB

footerBB:                                         ; preds = %headerBB
  %7 = getelementptr [10 x i32]* %a, i32 0, i32 6
  %8 = load i32* %7
  ret i32 %8
}

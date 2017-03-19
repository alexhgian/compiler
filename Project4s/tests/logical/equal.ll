; ModuleID = 'equal.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32
  %y = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 1, i32* %x
  store i32 2, i32* %y
  %0 = load i32* %x
  %1 = load i32* %y
  %2 = icmp eq i32 %0, %1
  br i1 %2, label %ThenBB, label %ElseBB

ThenBB:                                           ; preds = %next
  ret i32 1

ElseBB:                                           ; preds = %next
  ret i32 2

footerBB:                                         ; No predecessors!
  ret i32 0
}

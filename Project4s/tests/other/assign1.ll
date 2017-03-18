; ModuleID = 'assign1.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @main(i32 %x) {
entry:
  %arg0 = alloca i32
  %y = alloca i32
  %z = alloca i32
  %w = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 %x, i32* %arg0
  store i32 1, i32* %w
  store i32 2, i32* %y
  %0 = load i32* %y
  %1 = load i32* %w
  %2 = add i32 %0, %1
  store i32 %2, i32* %arg0
  store i32 %2, i32* %z
  %3 = load i32* %z
  ret i32 %3
}

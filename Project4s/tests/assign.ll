; ModuleID = 'assign.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@x = external global i32

define i32 @main(i32 %f) {
entry:
  %arg0 = alloca i32
  %y = alloca i32
  %z = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 %f, i32* %arg0
  %0 = load i32* @x
  %1 = load i32* %arg0
  %2 = add i32 %0, %1
  store i32 %2, i32* %z
  store i32 %2, i32* %y
  %3 = load i32* %y
  %4 = load i32* %z
  %5 = add i32 %3, %4
  ret i32 %5
}

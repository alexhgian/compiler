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
  %0 = getelementptr [10 x i32]* %a, i32 0, i32 6
  %1 = load i32* %0
  ret i32 %1
}

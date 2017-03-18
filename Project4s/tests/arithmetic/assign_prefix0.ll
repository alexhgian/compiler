; ModuleID = 'assign_prefix0.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32
  br label %next

next:                                             ; preds = %entry
  store i32 0, i32* %x
  %0 = load i32* %x
  %1 = add i32 %0, 1
  store volatile i32 %1, i32* %x
  %2 = mul i32 %1, 6
  %3 = sdiv i32 1, 4
  %4 = load i32* %x
  %5 = sub i32 %4, 1
  store volatile i32 %5, i32* %x
  %6 = mul i32 %3, %5
  %7 = add i32 %2, %6
  store i32 %7, i32* %x
  %8 = load i32* %x
  ret i32 %8
}

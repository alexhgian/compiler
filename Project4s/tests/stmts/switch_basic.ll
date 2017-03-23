; ModuleID = 'switch_basic.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

define float @main() {
entry:
  %f = alloca float
  %a = alloca i32
  br label %next

next:                                             ; preds = %entry
  store float 0.000000e+00, float* %f
  store i32 1, i32* %a
  %0 = load i32* %a
  switch i32 %0, label %switchDef [
    i32 0, label %switchCase0
    i32 1, label %switchCase1
    i32 2, label %switchCase2
  ]

switchCase0:                                      ; preds = %next
  br label %switchCase1

switchCase1:                                      ; preds = %switchCase0, %next
  br label %switchCase2

switchCase2:                                      ; preds = %switchCase1, %next
  br label %switchDef

switchDef:                                        ; preds = %switchCase2, %next
  br label %switchExit

switchExit:                                       ; preds = %switchDef
  %1 = load float* %f
  ret float %1
}

; ModuleID = 'vec2_to_vec2.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@vecA = external global <2 x float>

define float @main() {
entry:
  %vecB = alloca <4 x float>
  br label %next

next:                                             ; preds = %entry
  %0 = load <2 x float>* @vecA
  %1 = load <4 x float>* %vecB
  %2 = extractelement <2 x float> %0, i32 0
  %3 = insertelement <4 x float> %1, float %2, i32 0
  %4 = extractelement <2 x float> %0, i32 1
  %5 = insertelement <4 x float> %3, float %4, i32 1
  store volatile <4 x float> %5, <4 x float>* %vecB
  ret float 0x3FF19999A0000000
}

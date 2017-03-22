vec2 a;

float assignvec()
{
   vec4 v4;
   v4.xy = a;
   v4.z = v4.w = 0.5;

   return v4.x + v4.y + v4.z + v4.w;
}

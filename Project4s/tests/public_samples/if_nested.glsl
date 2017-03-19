float ifnested(float arg)
{
   float f;
   if ( arg > 1.0 ) {
      if ( 1.0 < 0.5 )
         f = 1.0;
      else
         f = 2.0;
   } else {
      f = arg;
   }
   return f;
}

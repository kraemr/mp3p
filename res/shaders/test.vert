#version 330 core
                  precision highp
              int;/**/float G(vec3 p){
           float s=sin(iTime),c=cos(iTime
         ),n=1.-c,o=.7,k=o*o;vec4 D=mat4(n
       *k+c,n*k,o*s,0,n*k,n*k+c,-o*s,0,-o*s,
     o*s,c,0,0,0,0,0)*vec4(p+vec3(0,0,-4),1);
    return length(vec2(length(D.xz)-1.,D.y))-.4
   ;}void mainImage(out vec4 O,in vec2 I){vec2 R
   =iResolution.xy,u=(I.xy-vec2((R.x-R.y)*.5,0))
  /R.y*45.;/**/vec3 d=       normalize(vec3(floor
  (u)/45.-.5,1)),C =           vec3(0);for(int i=
  0;i<15;++i){C+=d*G           (C);}float D= max(
  dot(normalize((vec3          (G(C+vec3(.1,0,0))
  ,G(C+vec3(0,.1,0))           ,G(C+vec3(0,0,.1))
  )-G(C))/.1),normalize     (-C)),.0); ivec2 b =
   ivec2(floor(fract(u)*7.8))-(1,0);O=vec4(vec3(
   length(C)<9.&&b.x>=0&&b.x<=4&&b.y>=0&&b.y<=6
    &&/*#*/(int[13](0,0x40000,0xc0000,0x810200,
    0x41038104, 0x28000, 0x3428000, 0x1428500,
      0x5c000,0x141c500,0xb9cee73a,0xc2afea86
         ,0xf55b305f)[int(D*13.)]&1<<int(b
           .x*7+(6-b.y)-1))>0?D:0.),1);}
             /*# made by @dkaraush #*/
                 /*# dima.bio #*/
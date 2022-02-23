#ifdef GL_ES
precision mediump float;

#endif

vec2 u_resolution=vec2(600.0,800.0);
//vec2 u_mouse=vec2(500.0,500.0);
uniform vec3 me;
uniform vec4 mouse;

//out vec4 FragColor;
bool isSphereIntersect( in vec3 rayOrigin, in vec3 rayDirection, in vec3 center, in float radius, out float distance1,out float distance2 )
{
    vec3  originCenter = rayOrigin - center;
    float b = dot( originCenter, rayDirection );
    float tangentSQ = dot( originCenter, originCenter ) - radius*radius;
    float h = b*b - tangentSQ;
    if( h<0.0 ||b>0.0) return false; // если нет пересечений или пересечене есть но оно с другой стороны от ориджина
    h = sqrt( h );
    distance1 = -b-h;
    distance2 = -b + h;
    return true;
}
 
void main()
{
vec3 me = me+vec3(500.0,500.0,0.000);
vec3 frgCrd = gl_FragCoord.xyz/500.0-me;
vec3 ecran = normalize(vec3(  (gl_FragCoord.xy/u_resolution-vec2(0.5,0.5)),  0.7+sqrt(16.0-pow((gl_FragCoord.x/500.0-0.5),2.0)+pow(abs(gl_FragCoord.y/500.0-0.5),2.0))/16.0));
/**/ecran = mat3(//матрица поворота
    1,0,0,
    0,mouse.y,-mouse.x,
    0,mouse.x,mouse.y
)*mat3(
    mouse.w,0,mouse.z,
    0,1,0,
    -mouse.z,0,mouse.w
)*ecran;/**/
float distan1;
float distan2;
vec3 sphCen = vec3(450.0,450.0,1000.0);
float radiusSph = 14.9;

if(isSphereIntersect(me,ecran,sphCen,radiusSph,distan1,distan2))
{
    gl_FragColor = vec4(1.0,sin(distan1/5.1),sin(distan2),1.0);
}
else
{
    gl_FragColor = vec4(normalize(vec3(sin(gl_FragCoord.y/500.0),1.0,0.0)),1.0);
}
}// Author:
// Title:


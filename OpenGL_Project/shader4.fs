#version 460 core
#ifdef GL_ES
precision mediump float;

#endif

vec2 u_resolution=vec2(600.0,800.0);
uniform vec3 me1;
uniform vec4 mouse;
uniform ivec2 sizeSph;
uniform sampler2D Spheres;//{radius, x, y, z, r, g, b,0};

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
	if(distance1==0.0||distance2==0.0) return false;
	return true;

}
 
vec3 sphN(vec3 sphCen,vec3 me,vec3 rayDirection,float distance)
{
	return normalize(me+rayDirection*distance-sphCen);
}


void getPosition(out vec3 me,out vec3 lookTo)
{
	me.xyz = me1.xyz;
	lookTo = normalize(vec3((gl_FragCoord.xy/u_resolution-vec2(0.5,0.5)),  0.7+sqrt(16.0-pow((gl_FragCoord.x/500.0-0.5),2.0)+pow(abs(gl_FragCoord.y/500.0-0.5),2.0))/16.0));
	lookTo = mat3(//матрица поворота
	1,0,0,
	0,mouse.y,-mouse.x,
	0,mouse.x,mouse.y
	)*mat3(
	mouse.w,0,mouse.z,
	0,1,0,
	-mouse.z,0,mouse.w
	)*lookTo;
}

bool getSphClr(in vec3 me,in vec3 lookTo,out float len,out vec4 color)
{
	vec4 sphs1,sphs2;
	float distan1,distan2,radiusSph,lenNow = 1000000000000000000000.0;
	len = lenNow;
	vec3 sphCen;
	int iNow = 0;
	int size = sizeSph.x*sizeSph.y;
	if(size==0) return false;
	for (int i = 0;i<size;i=i+2)
	{
		sphCen = sphs1.yzw;
		radiusSph = sphs1.x;
		sphs1 = texelFetch(Spheres,ivec2(i,0),0);
		sphs2 = texelFetch(Spheres,ivec2(i+1,0),0);
		if(!isSphereIntersect(me,lookTo,sphCen,radiusSph,distan1,distan2))
		{
			continue;
		}
		else if(distan1<0||distan2<0)
		{
			lenNow=max(distan1,distan2);
		}
		else
		{
			lenNow = min(distan1,distan2);
		}
		if(lenNow < len)
		{
			len = lenNow;
			iNow = i;
		}
	}
	if (len>10000000.0) return false;
	color = normalize(vec4(texelFetch(Spheres,ivec2(iNow+1,0),0).xyz,1.0));
	return true;
}

void main()
{
vec3 me,ecran;
vec4 clr;
float len;
getPosition(me,ecran);
if(getSphClr(me,ecran,len,clr))
{
	gl_FragColor = clr;
}
else
{
	gl_FragColor = vec4(normalize(vec3(2.0,1.0,0.0)),1.0);
}
}



#version 460 core
//#extension GL_EXT_texture_array : enable
//#extension GL_NV_texture_array : enable

#ifdef GL_ES
precision mediump float;
#endif

layout (location = 105) uniform ivec2 u_resolution;
layout (location = 100) uniform vec3 me1;
layout (location = 101) uniform vec4 mouse;
layout (location = 10) uniform sampler2DArray texture_array;//88ff штук максимум
layout (location = 8)uniform sampler2D Content;
layout (location = 9)uniform isampler2D ContentAbout;
layout (location = 10)uniform isampler2D AboutMaterial;
const float inf = abs(1.0/0.0);
out vec4 out_gl_FragColor;

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
 
bool isBoxIntersect( in vec3 rayOrigin, in vec3 rayDirection,in vec3 boxSize, out vec3 outNormal,out float toNear) 
{

	float toFar;
	vec3 m = 1.0/rayDirection;//непонятная часть это непереводимая игра слов вследствие рефакторинга
	vec3 n = m*rayOrigin;
	vec3 k = abs(m)*boxSize;
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	toNear = max( max( t1.x, t1.y ), t1.z );
	toFar = min( min( t2.x, t2.y ), t2.z );
	if( toNear>toFar || toFar<0.0 || toNear<0.0) return false;
	outNormal = -sign(rayDirection)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
	return true;
}

bool isTriIntersect( in vec3 rayOrigin, in vec3 rayDirection, in vec3 v0, in vec3 v1, in vec3 v2,out float ans,out vec3 normal0 )
{
	if(dot(v0-rayOrigin,rayDirection)<0&&dot(v1-rayOrigin,rayDirection)<0&&dot(v2-rayOrigin,rayDirection)<0)return false;
    vec3 v01 = v1 - v0;
    vec3 v02 = v2 - v0;
    vec3 rayOrigin0 =rayOrigin - v0;
    normal0 = cross( v01, v02 );
    vec3  q = cross( rayOrigin0, rayDirection );
    float d = 1.0/dot( rayDirection,normal0 );
    float u = d*dot( -q, v02 );
    float v = d*dot(  q, v01 );
    float t = d*dot( -normal0, rayOrigin0 );
    if( u<0.0 || u>1.0 || v<0.0 || (u+v)>1.0 ) return false;
	//t=dot(v0-rayOrigin,rayDirection);
	//ans =  vec3( t, u, v );
	ans = t;
    return true;
}
vec3 rotate(vec3 inpt,vec2 rx,vec2 ry,vec2 rz)
{
	inpt = inpt*mat3(
		rx.x,rx.y,0,
		-rx.y,rx.x,0,
		0,0,1
	);
	inpt = inpt*mat3(
		ry.x,0,ry.y,
		0,1,0,
		-ry.y,0,ry.x
	);
	return(inpt*mat3(
		1,0,0,
		0,rz.x,rz.y,
		0,-rz.y,rz.x
	));
}
vec3 unrotate(vec3 inpt,vec2 rx,vec2 ry,vec2 rz)
{
	inpt=inpt*mat3(
		1,0,0,
		0,rz.x,-rz.y,
		0,rz.y,rz.x
	);
	inpt = inpt*mat3(
		ry.x,0,-ry.y,
		0,1,0,
		ry.y,0,ry.x
	);
	return(inpt*mat3(
		rx.x,-rx.y,0,
		rx.y,rx.x,0,
		0,0,1
	));
}
bool getBoxClr1(in vec3 me,in vec3 lookTo,in vec3 boxSize,in vec4 boxRotxy,in vec2 boxRotz, in vec3 boxOrigin, out float len, out vec3 norm)
{
	me = me-boxOrigin;
	me=rotate(me,boxRotxy.xy,boxRotxy.zw,boxRotz);
	lookTo=rotate(lookTo,boxRotxy.xy,boxRotxy.zw,boxRotz);
	bool ans;
	ans = isBoxIntersect(me,lookTo,boxSize,norm,len);
	if(ans==false) return false;
	norm = unrotate(norm,boxRotxy.xy,boxRotxy.zw,boxRotz);
	return true;
}
bool getBoxClr(in vec3 me,in vec3 lookTo,in ivec4 in1, in ivec4 in2,out float len/*, out vec3 norm,out vec4 color*/,out int id)
{
	//return true;
	vec3 norm;
	vec4 sphs1,sphs2,sphs3;
	float lenNow = inf;
	len = lenNow;
	vec3 normAns;
	int iNow = in1.x;
	if(in1.x/4==0) return false;
	for (int i = in2.x/4;i<in2.y/4;i=i+in1.y/4)
	{//sicoss[0], sicoss[1], sicoss[2], sicoss[3], sicoss[4], sicoss[5],sizes[0],sizes[1],sizes[2],position[0],position[1],position[2] 
		sphs1 = texelFetch(Content,ivec2(i,0),0);
		sphs2 = texelFetch(Content,ivec2(i+1,0),0);
		sphs3 = texelFetch(Content,ivec2(i+2,0),0);
		if(!getBoxClr1(me,lookTo,vec3(sphs2.zw,sphs3.x),sphs1,sphs2.xy,sphs3.yzw,lenNow,norm))
			continue;
		else if(len>lenNow)
		{
			len=lenNow;
			iNow=i;
			normAns=norm;
		}
		else
			continue;
	}
	if (len==inf) return false;
	norm=normAns;
	id = (iNow-in2.x/4)*4/in1.y;
	//color = vec4(normalize(texelFetch(Content,ivec2(iNow+0,0),0).xyz),0);
	return true;
}

vec3 sphN(in vec3 sphCen,vec3 me,in vec3 rayDirection,in float distan)
{
	return normalize(me+rayDirection*distan-sphCen);
}
bool getTriColor(in vec3 me,in vec3 lookTo,in ivec4 in1, in ivec4 in2,out float len,out int id)
{
vec4 sphs1,sphs2,sphs3;
	float lenNow = inf;
	vec3 normal;
	len = lenNow;
	int iNow = in1.x;
	if(in1.x/4==0) return false;
	for (int i = in2.x/4;i<in2.y/4;i=i+in1.y/4)
	{
		sphs1 = texelFetch(Content,ivec2(i,0),0);
		sphs2 = texelFetch(Content,ivec2(i+1,0),0);
		sphs3 = texelFetch(Content,ivec2(i+2,0),0);
		if(!isTriIntersect(me,lookTo,sphs1.xyz,vec3(sphs1.w,sphs2.xy),vec3(sphs2.zw,sphs3.x),lenNow,normal))
		{
			continue;
		}
		else if(lenNow < len)
		{
			len = lenNow;
			iNow = i;
		}
	}
	id = (iNow-in2.x/4)*4/in1.y;
	if (len==inf) return false;
//	color = normalize(vec4(texelFetch(Content,ivec2(iNow+1,0),0).xyz,1.0));
//	normal = sphN(texelFetch(Content,ivec2(iNow,0),0).yzw,me,lookTo,len);
	return true;
}

void getPosition(out vec3 me,out vec3 lookTo)
{
	me.xyz = me1.xyz;
	float maxres = max(u_resolution.x,u_resolution.y);
	vec2 nFragCoord = (gl_FragCoord.xy-u_resolution/2)/vec2(maxres/2,maxres/2);
	lookTo = normalize(vec3((nFragCoord),  1/tan(3.1415/6)*(u_resolution.x+u_resolution.y)/(maxres/2*2*2)));//фокусное расстояние  такое что на экране все видно  под углом в х градусов в среднем
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

bool getSphClr(in vec3 me,in vec3 lookTo,in ivec4 in1, in ivec4 in2, out float len,/* out vec4 color,out vec3 normal,*/out int id)
{
	vec4 sphs1,sphs2;
	float distan1,distan2,radiusSph,lenNow = inf;
	len = lenNow;
	vec3 sphCen;
	int iNow = in1.x;
	if(in1.x/4==0) return false;
	for (int i = in2.x/4;i<in2.y/4;i=i+in1.y/4)
	{
		sphs1 = texelFetch(Content,ivec2(i,0),0);
		sphs2 = texelFetch(Content,ivec2(i+1,0),0);
		sphCen = sphs1.yzw;
		radiusSph = sphs1.x;
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
	if (len==inf) return false;
	id = (iNow-in2.x/4)*4/in1.y;
//	color = normalize(vec4(texelFetch(Content,ivec2(iNow+1,0),0).xyz,1.0));
//	normal = sphN(texelFetch(Content,ivec2(iNow,0),0).yzw,me,lookTo,len);
	return true;
}

bool RTX(in vec3 me,in vec3 lookTo,  out vec4 color)
{
	float lengsum = 0;
	for(int i = 0;i<4;++i)
	{
		vec2 leng=vec2(inf,inf);
		ivec2 id,id_T;
		id_T.x=texelFetch(ContentAbout,ivec2(0,0),0).w;
		if(getSphClr(me,lookTo,texelFetch(ContentAbout,ivec2(0,0),0),texelFetch(ContentAbout,ivec2(1,0),0),leng.y,id_T.y)) 
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}

		}
		id_T.x=texelFetch(ContentAbout,ivec2(2,0),0).w;
		if(getBoxClr(me,lookTo,texelFetch(ContentAbout,ivec2(2,0),0),texelFetch(ContentAbout,ivec2(3,0),0),leng.y,id_T.y))
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}

		}
		
		id_T.x=texelFetch(ContentAbout,ivec2(4,0),0).w;
		if(getTriColor(me,lookTo,texelFetch(ContentAbout,ivec2(4,0),0),texelFetch(ContentAbout,ivec2(5,0),0),leng.y,id_T.y))
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}
		}

		//выход после блока должен быть в количестве материала (номер и номер коллекции) и вектор нормали
		ivec2 materialID;
		vec3 norm;
		if(leng.x==inf)
		{
			break;
		}
		else if(id.x==texelFetch(ContentAbout,ivec2(0,0),0).w)//если сфера
		{
		ivec4 in1 = texelFetch(ContentAbout,ivec2(0,0),0);
		ivec4 in2 = texelFetch(ContentAbout,ivec2(0,0),0);
		vec4 sphs1 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4,0),0);
		norm = sphN(sphs1.yzw,me,lookTo,leng.x);
		}
		else if(id.x==texelFetch(ContentAbout,ivec2(2,0),0).w)//если коробка
		{
		ivec4 in1 = texelFetch(ContentAbout,ivec2(2,0),0);
		ivec4 in2 = texelFetch(ContentAbout,ivec2(3,0),0);
		}
		else if(id.x==texelFetch(ContentAbout,ivec2(4,0),0).w)//если сфера
		{
		ivec4 in1 = texelFetch(ContentAbout,ivec2(4,0),0);
		ivec4 in2 = texelFetch(ContentAbout,ivec2(5,0),0);
		}




		lengsum+=leng.x;

	}
	if(lengsum==0)
	{
	return false;
	}


}
void main()
{
	vec3 me,ecran;
	vec4 clr;
	float len;
	vec3 normal;
	getPosition(me,ecran);
	if(RTX(me,ecran,clr))
	{
		out_gl_FragColor = clr;
	}
	else
	{
		vec4 clr2=vec4(0,0,0,0);
		//out_gl_FragColor = vec4(normalize(vec3(2.0,1.0,0.0)),1.0);
		clr2 = texelFetch(Content,ivec2(0,0),0);
		//clr2 = texture2DArray(texture_array, vec3(0,0,0));
		if(clr2 == vec4(0.1,0.2,0.3,0.8))
		clr2 = vec4(1,0,0,1);
		else
		clr2 = clr2;
		//clr2 = vec4(0,1,0,1);
		//if(out_gl_FragColor==vec4(0,0,0,0))
		
		out_gl_FragColor = vec4(normalize(vec3(clr2.xyz)),1.0);
	}
//if(gl_FragCoord.x<u_resolution.x/2)
//out_gl_FragColor = vec4(1,1,0,1);
//else
//out_gl_FragColor = vec4(0,0,0,0);

}

 
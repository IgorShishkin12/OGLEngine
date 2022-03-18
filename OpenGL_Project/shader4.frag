
#version 460 core
//#extension GL_EXT_texture_array : enable
//#extension GL_NV_texture_array : enable

#ifdef GL_ES
precision mediump float;
#endif

layout (location = 105) uniform ivec2 u_resolution;
layout (location = 106) uniform float time;
layout (location = 100) uniform vec3 me1;
layout (location = 101) uniform vec4 mouse;
layout (location = 8)uniform sampler2D Content;
layout (location = 9)uniform isampler2D ContentAbout;
layout (location = 10) uniform sampler2DArray texture_array;//88ff штук максимум
layout (location = 11)uniform isampler2D AboutMaterial;
const float inf = abs(1.0/0.0);
const float epsilon0 = 1e-10;
const float epsilon = 1e-6;
const float epsilon2 = 1e-3;
const float pi = 3.1415926535;///**/8979323846/**/2643383279/**/5028841971/**/6939937510/**/5820974944;
out vec4 out_gl_FragColor;
float seed = 13746.785*gl_FragCoord.y/(gl_FragCoord.x+time)/**length(me1)+time*/;


float random()
{
	//seed =fract(tan(dot(vec2(cos(seed*34),tan(967/seed)), vec2(seed+45,atan(133.2345/seed)))*pow(3.42342,seed)));
	 //seed = fract(cos(1./fract(sin(1./fract(tan(seed))))));
//	 const float max_att = 2;
//	 for(float i = 0.0;i<max_att;i=i+1)
//	 {
//		seed = 1/((max_att+10-i)*fract(seed+i+55));
//	 }
    //seed = fract(sin(dot(vec2(1./seed,seed),vec2(10,0.1)))*144.0);
	seed = fract(sin(dot(vec2(seed/gl_FragCoord),
                         vec2(12.9898,78.233)))*
        43758.5453123);
	return seed;
}
vec2 random2()
{
return vec2(random(),random());
}
vec3 random3()
{return vec3(random(),random(),random());}
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
 
bool isBoxIntersectN( in vec3 rayOrigin, in vec3 rayDirection,in vec3 boxSize, out vec3 outNormal) 
{
	vec3 m = 1.0/rayDirection;//непонятная часть это непереводимая игра слов вследствие рефакторинга
	vec3 n = m*rayOrigin;
	vec3 k = abs(m)*boxSize;
	vec3 t1 = -n - k;
	vec3 t2 = -n + k;
	outNormal = -sign(rayDirection)*step(t1.yzx,t1.xyz)*step(t1.zxy,t1.xyz);
	return true;
}
bool isBoxIntersect( in vec3 rayOrigin, in vec3 rayDirection,in vec3 boxSize, out float toNear) 
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
	return true;
}

bool isTriIntersect( in vec3 rayOrigin, in vec3 rayDirection, in vec3 v0, in vec3 v1, in vec3 v2,out float ans,out vec3 normal0 )
{
	//if(dot(v0-rayOrigin,rayDirection)<0&&dot(v1-rayOrigin,rayDirection)<0&&dot(v2-rayOrigin,rayDirection)<0)return false;
    vec3 v01 = v1 - v0;
    vec3 v02 = v2 - v0;
    vec3 rayOrigin0 = rayOrigin - v0;
    normal0 = cross( v01, v02 );
	//normal0 = normal0*(-sign(dot(normal0,rayDirection)));
    vec3  q = cross( rayOrigin0, rayDirection );
    float d = 1.0/dot( rayDirection,normal0 );
	//if(dot(normal0,rayOrigin0)*d/*/dot(normal0,rayDirection)*/>0)return false;//если луч уходит от треугольника
	//if(abs(d)>10e6)return false;
    float u = d*dot( -q, v02 );
    float v = d*dot(  q, v01 );
    ans = d*dot( -normal0, rayOrigin0 );//можно заменить т на анс
    if( u<0.0 || v<0.0 || (u+v)>1.0 ||ans<0) return false;
	//t=dot(v0-rayOrigin,rayDirection);
	//ans =  vec3( t, u, v );
	//ans = t;
	//if(ans<0) return false;
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
bool getBoxClr1(in vec3 me,in vec3 lookTo,in vec3 boxSize,in vec4 boxRotxy,in vec2 boxRotz, in vec3 boxOrigin, out float len)
{
	me = me-boxOrigin;
	me=rotate(me,boxRotxy.xy,boxRotxy.zw,boxRotz);
	lookTo=rotate(lookTo,boxRotxy.xy,boxRotxy.zw,boxRotz);
	bool ans;
	ans = isBoxIntersect(me,lookTo,boxSize,len);
	if(ans==false) return false;
	//norm = unrotate(norm,boxRotxy.xy,boxRotxy.zw,boxRotz);
	return true;
}
bool getBoxClr(in vec3 me,in vec3 lookTo,in ivec4 in1, in ivec4 in2,out float len/*, out vec3 norm,out vec4 color*/,out int id)
{
	//return true;
	vec3 norm;
	vec4 sphs1,sphs2,sphs3;
	float lenNow = inf;
	len = lenNow;
	//vec3 normAns;
	//int iNow = in1.x;
	int id_T = 0;
	if(in1.x/4==0) return false;
	for (int i = in2.x/4;i<in2.y/4;i=i+in1.y/4,
									id_T++)
	{//sicoss[0], sicoss[1], sicoss[2], sicoss[3], sicoss[4], sicoss[5],sizes[0],sizes[1],sizes[2],position[0],position[1],position[2] 
		sphs1 = texelFetch(Content,ivec2(i,0),0);
		sphs2 = texelFetch(Content,ivec2(i+1,0),0);
		sphs3 = texelFetch(Content,ivec2(i+2,0),0);
		if(!getBoxClr1(me,lookTo,vec3(sphs2.zw,sphs3.x),sphs1,sphs2.xy,sphs3.yzw,lenNow))
			continue;
		else if(len>lenNow)
		{
			len=lenNow;
			id = id_T;
			//normAns=norm; v
		}
		else
			continue;
	}
	if (len==inf) return false;
	//norm=normAns;
	//id = (iNow-in2.x/4)*4/in1.y;
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
	int id_T=0;
	if(in1.x/4==0) return false;
	for (int i = in2.x/4;i<in2.y/4;i=i+in1.y/4,
										id_T++	)
	{
		sphs1 = texelFetch(Content,ivec2(i,0),0);
		sphs2 = texelFetch(Content,ivec2(i+1,0),0);
		sphs3 = texelFetch(Content,ivec2(i+2,0),0);
		if(isTriIntersect(me,lookTo,sphs1.xyz,vec3(sphs1.w,sphs2.xy),vec3(sphs2.zw,sphs3.x),lenNow,normal))
		{
			if(lenNow < len)
			{
				len = lenNow;
				id = id_T;
			}
		}
	}
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
	//int iNow = in1.x;
	if(in1.x/4==0) return false;
	int id_T=0;
	for (int i = in2.x/4;i<in2.y/4; i=i+in1.y/4,
									id_T++		)
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
			id = id_T;
		}
	}
	if (len==inf) return false;
	//id = (iNow-in2.x/4)*4/in1.y;
//	color = normalize(vec4(texelFetch(Content,ivec2(iNow+1,0),0).xyz,1.0));
//	normal = sphN(texelFetch(Content,ivec2(iNow,0),0).yzw,me,lookTo,len);
	return true;
}
float arctan(float a,float b)
{
	float ans = atan(abs(a/b));
	if(a>=0&&b<=0)ans =pi- ans;
	else if(a<=0&&b<=0)ans = ans+pi;
	else if(a<=0&&b>=0)ans = 2*pi-ans;
	return ans;

}
//float arccos(float a,float c)
//{
//	float ans = acos(abs(a/c))
//}
vec2 SphToTex(in vec3 sphO,in vec3 P,in vec3 x,in vec3 y)
{
	P = normalize(P-sphO);
	float x1,y1,z1;
	x1 = dot(normalize(x),P);
	y1 = dot(normalize(y),P);
	z1 = dot(normalize(cross(x,y)),P);//тангенс переодичен с периодом в pi
	//return (vec2(atan(z1/x1),atan(z1/y1))/pi/*теперь от -1 до 1*/+1)/4+vec2((sign(x1)+1)/4,(sign(y1)+1)/4);
	return vec2(arctan(z1,x1),arctan(z1,y1))/(2*pi);
}
vec3 textoPSph(in vec3 sphO,in float radius,in vec3 x,in vec3 y,in vec2 TexC)
{
//	vec2 signs = sign(TexC-vec2(0.5));
//	TexC = TexC-vec2((signs.x+1)/4,(signs.y+1)/4);
//	TexC = (TexC*4-1)*pi;
	float x1,y1,z1;
//	x1 = abs(cos(TexC.x))*signs.x;
//	y1 = abs(cos(TexC.y))*signs.y;
//	z1 = abs(sin(TexC.x))*sign(tan(TexC.x)*signs.x);
//	return normalize((x1*x+y1*y+z1*normalize(cross(x,y))))*radius+sphO;
TexC=TexC*2*pi;
x1 = cos(TexC.x);
y1= cos(TexC.y);
z1= sin(TexC.x);
return sphO+normalize(x*x1+y*y1+cross(x,y)*z1)*radius;


}
vec2 triToTex1(vec3 v0,vec3 v1,vec3 v2,vec3 p,vec3 norm)
{
	//https://habr.com/ru/post/249467/
	v1 = v1-v0;
	v2 = v2-v0;
	p=p-v0;
	vec3 x = normalize(v1);
	vec3 y = normalize(cross(x,cross(x,v2)));
	vec2 v11 = vec2(dot(x,v1),0);
	vec2 v22 = vec2(dot(x,v2),dot(y,v2));
	vec2 p2 = vec2(dot(x,p),dot(y,p));
	vec3 barycentric = cross(vec3(v11.x,v22.x,p2.x),vec3(v11.y,v22.y,p2.y));
	barycentric = barycentric/barycentric.z;
	if(sign(dot(norm,cross(x,y)))<0)
	barycentric=barycentric*(-1.)+1;
	return barycentric.xy;

}
vec3 texToPTri1(vec3 v0,vec3 v1,vec3 v2,vec2 p,out vec3 norm)
{	
	norm = normalize(cross(v1-v0,v2-v0))*(-sign(p.x+p.y-1));
	if(p.x+p.y>1)p=(p-1)*(-1);
	return v0+(v1-v0)*p.x+(v2-v0)*p.y;

}
vec2 triToTex2(vec3 v0,vec3 v1,vec3 v2,vec3 p)
{
	//https://habr.com/ru/post/249467/
	v1 = v1-v0;
	v2 = v2-v0;
	p=p-v0;
	vec3 x = normalize(v1);
	vec3 y = normalize(cross(x,cross(x,v2)));
	vec2 v11 = vec2(dot(x,v1),0);
	vec2 v22 = vec2(dot(x,v2),dot(y,v2));
	vec2 p2 = vec2(dot(x,p),dot(y,p));
	vec3 barycentric = cross(vec3(v11.x,v22.x,p2.x),vec3(v11.y,v22.y,p2.y));
	barycentric = barycentric/barycentric.z;
	barycentric = barycentric*(barycentric.x+barycentric.y)/max(barycentric.x,barycentric.y);
	return barycentric.xy;

}
vec3 texToPTri2(vec3 v0,vec3 v1,vec3 v2,vec2 p)
{
	p = p*max(p.x,p.y)/(p.x+p.y);
	return  v0+(v1-v0)*p.x+(v2-v0)*p.y;
}
vec2 jff(vec2 dd)
{
	vec3 v0 = vec3(23,32,22);
	vec3 v1 = vec3(67,54,34);
	vec3 v2 = vec3(32,567,8);
	vec3 p = texToPTri2(v0,v1,v2,dd);
	return triToTex2(v0,v1,v2,p);
}
void orthogonalVs(in vec3 v1, out vec3 v2, out vec3 v3)
{
	//vec3 randomVec =  normalize(max(vec3(0,0,1)*length(cross(vec3(0,0,1),v1)),vec3(0,1,0)*length(cross(vec3(0,1,0),v1))));//рандомный вектор, неколинеарный с данным
	vec3 randomVec = vec3(2,random2());
	if(length(cross(vec3(0,0,1),v1))<=0)
		randomVec = -1.0/randomVec;
	v2 = normalize(cross(randomVec,v1));
	v3 = normalize(cross(v1,v2));
//	if(dot(v1,v2)>epsilon||dot(v1,v3)>epsilon||dot(v2,v3)>epsilon)
//	{v2 = vec3(0,0,0);
//	v3=vec3(0,0,0);}

}
vec3 ROwVaL(in vec3 v, in float l)//random orthogonal with vector and length
{
	vec3 v1,v2;
	vec2 rand = random2();
	orthogonalVs(v,v1,v2);
	return normalize(v1*rand.x+v2*rand.y)*l;
}
vec3 RVwDPX(in vec3 v, in float x,in float maxTanDiff) //random vector with difference probability x
{
	//max tan diff for max parameter of (delta vector)/lenght(v)
	//функция перевода рандомного числа в угол такая так как https://umath.ru/calc/graph/?&func=acos(x%5E(1/20));
	//cos(acos((fract(random()))^(1/x)))
	// ans = v;
	float maxL = length(v)*	min(tan(acos(		(pow(fract(random()),1.0/x))	)),maxTanDiff);
	if(maxL<epsilon)
	return v;
	return normalize(v + ROwVaL(v,maxL));
}

bool RTX(in vec3 me,in vec3 lookTo,  out vec4 color)
{
	float lengsum = 0;
		int qwerte = 0;
	for(int i = 0;i<2;++i)
	{
		vec2 leng=vec2(inf,inf);
		ivec2 id,id_T;//в переменной id должно находится первым символом номер искомой функции, вторым-последовательный номер в наборе из этих функций где первая-0
		id_T.x=texelFetch(ContentAbout,ivec2(0,0),0).w;
		if(getSphClr(me,lookTo,texelFetch(ContentAbout,ivec2(0,0),0),texelFetch(ContentAbout,ivec2(1,0),0),leng.y,id_T.y)) 
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}

		}
		leng.y = inf;
		id_T.x=texelFetch(ContentAbout,ivec2(2,0),0).w;
		if(getBoxClr(me,lookTo,texelFetch(ContentAbout,ivec2(2,0),0),texelFetch(ContentAbout,ivec2(3,0),0),leng.y,id_T.y))
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y;
			id= id_T;
			}

		}
		leng.y = inf;
		
		id_T.x=texelFetch(ContentAbout,ivec2(4,0),0).w;
		if(getTriColor(me,lookTo,texelFetch(ContentAbout,ivec2(4,0),0),texelFetch(ContentAbout,ivec2(5,0),0),leng.y,id_T.y))
		{
			if(leng.y<leng.x)
			{
			leng.x=leng.y*(1-100*epsilon);//расстояние определяется чуть больше чем есть на самом деле так что вот так вот
			id= id_T;
			}
//			color = normalize(vec4(sin(leng.y),2,2,1));
//			break;
		}

		//выход после блока должен быть в количестве материала (номер и номер коллекции) и вектор нормали
		ivec2 materialID = ivec2(0,0);
		vec3 norm;
		if(leng.x==inf)
		{
		
			break;
		}
		else if(id.x==/*texelFetch(ContentAbout,ivec2(0,0),0).w*/1)//если сфера
		{
			ivec4 in1 = texelFetch(ContentAbout,ivec2(0,0),0);
			ivec4 in2 = texelFetch(ContentAbout,ivec2(1,0),0);
			vec4 sphs1 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4,0),0);
			materialID.x= in2.z;
			norm = sphN(sphs1.yzw,me,lookTo,leng.x);
		}
		else if(id.x==/*texelFetch(ContentAbout,ivec2(2,0),0).w*/2)//если коробка
		{
			ivec4 in1 = texelFetch(ContentAbout,ivec2(2,0),0);
			ivec4 in2 = texelFetch(ContentAbout,ivec2(3,0),0);
			materialID.x= in2.z;
			vec4 sphs1,sphs2,sphs3;
			sphs1 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4,0),0);
			sphs2 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4+1,0),0);
			sphs3 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4+2,0),0);
			
			vec3 me2 = me-sphs3.yzw;
			me2=rotate(me2,sphs1.xy,sphs1.zw,sphs2.xy);
			vec3 lookTo2=rotate(lookTo,sphs1.xy,sphs1.zw,sphs2.xy);
			isBoxIntersectN(me2,lookTo2,vec3(sphs2.zw,sphs3.x),norm);
			norm = unrotate(norm,sphs1.xy,sphs1.zw,sphs2.xy);

		}
		else if(id.x==/*texelFetch(ContentAbout,ivec2(4,0),0).w*/3)//если треугольник
		{
			ivec4 in1 = texelFetch(ContentAbout,ivec2(4,0),0);
			ivec4 in2 = texelFetch(ContentAbout,ivec2(5,0),0);
			materialID.x= in2.z;
			vec4 sphs1,sphs2,sphs3;
			sphs1 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4,0),0);
			sphs2 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4+1,0),0);
			sphs3 = texelFetch(Content,ivec2(id.y*in1.y/4+in2.x/4+2,0),0);
			//float len22 = inf;
			//isTriIntersect(me,lookTo,sphs1.xyz,vec3(sphs1.w,sphs2.xy),vec3(sphs2.zw,sphs3.x),len22/*рандомное значение*/,norm);
			norm = cross(normalize(vec3(sphs1.w,sphs2.xy)-sphs1.xyz),normalize(vec3(sphs2.zw,sphs3.x)-sphs1.xyz));
			  
			//norm = cross(normalize(vec3(1000,1000,1000)-vec3(1000,1000,-1000)),normalize(vec3(1000,-1000,1000 )-vec3(1000,1000,-1000)));
			norm = normalize(norm);
			//norm = vec3(1,0,0);
			if(dot(norm,lookTo)>0)norm = norm * (-1);
			//norm = norm*(-sign(dot(norm,lookTo)));
			qwerte += 1;
		}

		//поиск значения искомого материала
		{
			ivec4 qt= texelFetch(AboutMaterial,ivec2((materialID.x+id.y*2)/4,0),0);
			if((materialID.x/2+id.y)%2==1)
			{
				materialID=qt.zw;
			}
			else
				materialID=qt.xy;
		}
		//вытаскивание номера в последовательности по номеру функции
		{
			for(int i = 0; i<100;i+=2)
			{
				if(texelFetch(ContentAbout,ivec2(i,0),0).w==materialID.x)
				{
					materialID.x=i;
					break;
				}
			}

		}
		//materialID сейчас представлят указатель* на описание класса в ContentAbout и номер в этом классе в текстуре content
		vec4 color_t;
		if(texelFetch(ContentAbout,ivec2(materialID.x,0),0).w==100)
		{
			ivec4 in_T = texelFetch(ContentAbout,ivec2(materialID.x,0),0);
			ivec4 in_T2 = texelFetch(ContentAbout,ivec2(materialID.x+1,0),0);
			vec4 data1 = texelFetch(Content,ivec2(   (in_T2.x+materialID.y*in_T.y)/4   ,0),0);
			vec4 data2 = texelFetch(Content,ivec2(   (in_T2.x+materialID.y*in_T.y)/4+1   ,0),0);
			color_t = vec4(   normalize(data1.xyz),1   );
			norm = RVwDPX(norm,data2.x,data2.y);
		}
		else
			color_t = normalize(vec4(0,0,1,1));

		{
			color = color+color_t*pow(0.5,i);
		}
		{
			//отражение
			me = me + lookTo*leng.x;
			vec3 dontlookto = lookTo;
			lookTo = reflect(lookTo,norm);
			//lookTo = normalize(lookTo);
//			if(qwerte == 3)
//			{
//			if(i>2)
//			{
//			color = vec4(0.5,0.5,0.5,1);
//			return true;
//			}
//			}

			//освещение
			//мы смотрим на сколько вектор нормали конечный отличается от угла падения света и умножаем свет на это число
			float isLig = -dot(lookTo,normalize(vec3(0,cos(time/1e10),sin(time/1e10))));
			isLig = max(isLig,0.3);
			color = color*(isLig);
			
			//color = vec4(norm,1);
		}
		lengsum+=leng.x;
//		color = vec4(norm,1);
//		float qwerty = time/8e9;
//		vec2 qw = vec2(sin(qwerty),cos(qwerty));
		//if(length(norm)<epsilon)color = vec4(normalize(vec3(qw.x,qw.y,1)),1);
//		if(length(norm-vec3(qw.x,0,qw.y))<1)color = vec4(1,2,3,1);
//		else color = vec4(3,2,1,1);
//		if(length(norm-vec3(qw,0))<0.1)color = vec4(1,qw,1);
//		if(length(norm-vec3(-1,0,0))<0.1)color = vec4(1,0,0,1);
//		else color = vec4(0,1,0,1);
//		if(length(norm.xz-vec3(-1,0,0).xz)<0.1)color = vec4(0,1,0,1);
//		else color = vec4(0,1,0,1);
//		if(length(norm-vec3(-1,0,0))<0.1)color = vec4(0,0,0,1);
//		break;
//		else color = vec4(0,1,0,1);
//if(time>8e15)color = vec4(0,0,1,1);
//else color = vec4(1,0,0,1);
//color = vec4(normalize(color.xyz),1);
//vec3 a,b;
//a = random()*vec3(0,0,1)+random()*vec3(1,0,0);
//b = random()*vec3(0,0,1)+random()*vec3(1,0,0);
//vec3 dfdt = normalize(cross(a,b));
//dfdt = dfdt*sign(dfdt.y);
//if(length(dfdt-vec3(0,1,0))>0)color = vec4(1,1,1,1);
//		break;
//
	}
	if(lengsum==0)
	{
	return false;
	}
	return true;


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
//		vec4 clr2=vec4(0,0,0,0);
//		//out_gl_FragColor = vec4(normalize(vec3(2.0,1.0,0.0)),1.0);
//		clr2 = texelFetch(Content,ivec2(0,0),0);
//		//clr2 = texture2DArray(texture_array, vec3(0,0,0));
//		if(clr2 == vec4(0.1,0.2,0.3,0.8))
//		clr2 = vec4(1,0,0,1);
//		else
//		clr2 = clr2;
//		//clr2 = vec4(0,1,0,1);
//		//if(out_gl_FragColor==vec4(0,0,0,0))
		
		out_gl_FragColor = vec4(normalize(vec3(1,1,1)),1.0);
		
	}
	
//vec3 a,b;
//a = random()*vec3(0,0,1)+random()*vec3(1,0,0);
//float seedd_t = seed;
//b = random()*vec3(0,0,1)+random()*vec3(1,0,0);
//if(seed==seedd_t)out_gl_FragColor = vec4(1,1,1,1);
//vec3 dfdt = normalize(cross(vec3(400,7,60)-vec3(6,7.001,8),vec3(3000,7,5000)-vec3(6,7.001,8)));
//vec3 dfdt = normalize(cross(a,b));
//dfdt = dfdt*sign(dfdt.y);
//if(length(dfdt-vec3(0,1,0))<0.01)out_gl_FragColor = vec4(1,1,1,1);
//	for(int i = 0; i < 100000;++i)
//	{
//	if(round(i-epsilon)!=i)
//	{
//	out_gl_FragColor = vec4(0,0,1,1);
//	break;
//	}}
	//if(round(0.4)==0)out_gl_FragColor=vec4(0,0,1,1);
	//out_gl_FragColor = vec4(jff(out_gl_FragColor.xy),out_gl_FragColor.zw);
	//if((jff(normalize(out_gl_FragColor.xy+epsilon))-normalize(out_gl_FragColor.xy+epsilon)).x>0.0000000001)out_gl_FragColor = vec4(0,0,0,1);
//if(gl_FragCoord.x<u_resolution.x/2)
//out_gl_FragColor = vec4(1,1,0,1);
//else
//out_gl_FragColor = vec4(0,0,0,0);

}

 
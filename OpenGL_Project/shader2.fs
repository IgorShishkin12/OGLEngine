#version 460 core

#define FAR_DISTANCE 1000000.0
#define SPHERE_COUNT 3
#define BOX_COUNT 8
out vec4 OutColor;

struct Material
{

    vec3 emmitance;
    vec3 reflectance;
    float roughness;
    float opacity;
    //vec3 color=opacity;
    //float howMuchZerkalnyy = reflectance;
};

struct Box
{
    Material material;
    vec3 halfSize;
    mat3 rotation;
    vec3 position;
};

struct Sphere
{
    Material material;
    vec3 position;
    float radius;
};

vec3 outColor2;

uniform vec2 uViewportSize;
uniform vec3 uPosition;
uniform vec3 uDirection;
uniform vec3 uUp;
uniform float uFOV;
uniform float uTime;
uniform int uSamples;

#define PI 3.1415926535
#define HALF_PI (PI / 2.0)
#define FAR_DISTANCE 1000000.0

#define MAX_DEPTH 8
#define SPHERE_COUNT 3
#define BOX_COUNT 8
#define N_IN 0.99
#define N_OUT 1.0

Sphere spheres[SPHERE_COUNT];
Box boxes[BOX_COUNT];

void InitializeScene()
{
    spheres[0].position = vec3(2.5, 1.5, -1.5);
    spheres[1].position = vec3(-2.5, 2.5, -1.0);
    spheres[2].position = vec3(0.5, -4.0, 3.0);
    spheres[0].radius = 1.5;
    spheres[1].radius = 1.0;
    spheres[2].radius = 1.0;
    spheres[0].material.roughness = 1.0;
    spheres[1].material.roughness = 0.8;
    spheres[2].material.roughness = 1.0;
    spheres[0].material.opacity = 0.0;
    spheres[1].material.opacity = 0.0;
    spheres[2].material.opacity = 0.8;
    spheres[0].material.reflectance = vec3(1.0, 0.0, 0.0);
    spheres[1].material.reflectance = vec3(1.0, 0.4, 0.0);
    spheres[2].material.reflectance = vec3(1.0, 1.0, 1.0);
    spheres[0].material.emmitance = vec3(0.0);
    spheres[1].material.emmitance = vec3(0.0);
    spheres[2].material.emmitance = vec3(0.0);

    // up
    boxes[0].material.roughness = 0.0;
    boxes[0].material.emmitance = vec3(0.0);
    boxes[0].material.reflectance = vec3(1.0, 1.0, 1.0);
    boxes[0].halfSize = vec3(5.0, 0.5, 5.0);
    boxes[0].position = vec3(0.0, 5.5, 0.0);
    boxes[0].rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // down
    boxes[1].material.roughness = 0.3;
    boxes[1].material.opacity = 0.0;
    boxes[1].material.emmitance = vec3(0.0);
    boxes[1].material.reflectance = vec3(1.0, 1.0, 1.0);
    boxes[1].halfSize = vec3(5.0, 0.5, 5.0);
    boxes[1].position = vec3(0.0, -5.5, 0.0);
    boxes[1].rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // right
    boxes[2].material.roughness = 0.0;
    boxes[2].material.opacity = 0.0;
    boxes[2].material.emmitance = vec3(0.0);
    boxes[2].material.reflectance = vec3(0.0, 1.0, 0.0);
    boxes[2].halfSize = vec3(5.0, 0.5, 5.0);
    boxes[2].position = vec3(5.5, 0.0, 0.0);
    boxes[2].rotation = mat3(
        0.0, 1.0, 0.0,
        -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0
    );

    // left
    boxes[3].material.roughness = 0.0;
    boxes[3].material.opacity = 0.0;
    boxes[3].material.emmitance = vec3(0.0);
    boxes[3].material.reflectance = vec3(1.0, 0.0, 0.0);
    boxes[3].halfSize = vec3(5.0, 0.5, 5.0);
    boxes[3].position = vec3(-5.5, 0.0, 0.0);
    boxes[3].rotation = mat3(
        0.0, 1.0, 0.0,
        -1.0, 0.0, 0.0,
        0.0, 0.0, 1.0
    );

    // back
    boxes[4].material.roughness = 0.0;
    boxes[4].material.opacity = 0.0;
    boxes[4].material.emmitance = vec3(0.0);
    boxes[4].material.reflectance = vec3(1.0, 1.0, 1.0);
    boxes[4].halfSize = vec3(5.0, 0.5, 5.0);
    boxes[4].position = vec3(0.0, 0.0, -5.5);
    boxes[4].rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0
    );

    // light source
    boxes[5].material.roughness = 0.0;
    boxes[5].material.opacity = 0.0;
    boxes[5].material.emmitance = vec3(6.0);
    boxes[5].material.reflectance = vec3(1.0);
    boxes[5].halfSize = vec3(2.5, 0.2, 2.5);
    boxes[5].position = vec3(0.0, 4.8, 0.0);
    boxes[5].rotation = mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    // boxes
    boxes[6].material.roughness = 0.0;
    boxes[6].material.opacity = 0.0;
    boxes[6].material.emmitance = vec3(0.0);
    boxes[6].material.reflectance = vec3(1.0);
    boxes[6].halfSize = vec3(1.5, 3.0, 1.5);
    boxes[6].position = vec3(-2.0, -2.0, -0.0);
    boxes[6].rotation = mat3(
        0.7, 0.0, 0.7,
        0.0, 1.0, 0.0,
        -0.7, 0.0, 0.7
    );
    // boxes
    boxes[7].material.roughness = 0.0;
    boxes[7].material.opacity = 0.0;
    boxes[7].material.emmitance = vec3(0.0);
    boxes[7].material.reflectance = vec3(1.0);
    boxes[7].halfSize = vec3(1.0, 1.5, 1.0);
    boxes[7].position = vec3(2.5, -3.5, -0.0);
    boxes[7].rotation = mat3(
        0.7, 0.0, 0.7,
        0.0, 1.0, 0.0,
        -0.7, 0.0, 0.7
    );
}

bool IntersectRaySphere(vec3 origin, vec3 direction, Sphere sphere, out float fraction, out vec3 normal)
{
    vec3 L = origin - sphere.position;
    float a = dot(direction, direction);
    float b = 2.0 * dot(L, direction);
    float c = dot(L, L) - sphere.radius * sphere.radius;
    float D = b * b - 4 * a * c;

    if (D < 0.0) return false;

    float r1 = (-b - sqrt(D)) / (2.0 * a);
    float r2 = (-b + sqrt(D)) / (2.0 * a);

    if (r1 > 0.0)
        fraction = r1;
    else if (r2 > 0.0)
        fraction = r2;
    else
        return false;

    normal = normalize(direction * fraction + L);

    return true;
}

float IntersectRayBox(vec3 origin, vec3 direction, Box box, out float fraction, out vec3 normal,out bool ans)
{
    vec3 rd = box.rotation * direction;
    vec3 ro = box.rotation * (origin - box.position);

    vec3 m = vec3(1.0) / rd;

    vec3 s = vec3((rd.x < 0.0) ? 1.0 : -1.0,
        (rd.y < 0.0) ? 1.0 : -1.0,
        (rd.z < 0.0) ? 1.0 : -1.0);
    vec3 t1 = m * (-ro + s * box.halfSize);
    vec3 t2 = m * (-ro - s * box.halfSize);

    float tN = max(max(t1.x, t1.y), t1.z);
    float tF = min(min(t2.x, t2.y), t2.z);
    //outColor2 = t1;

    if (tN > tF || tF < 0.0) {ans = false;return 0.0f;}

    mat3 txi = transpose(box.rotation);

    if (t1.x > t1.y && t1.x > t1.z)
        normal = txi[0] * s.x;
    else if (t1.y > t1.z)
        normal = txi[1] * s.y;
    else
        normal = txi[2] * s.z;
    ans = true;
 /*   if(tN==FAR_DISTANCE)
    {
        ans = false;
        fraction = tN;
        return tN;
    }/**/
 /*       if(tN>=FAR_DISTANCE)
    {
        ans = false;
        fraction = tN;
        return tN;
    }/**/
 /*       if(tN>=FAR_DISTANCE)
    {
        ans = false;
        fraction = tN;
        return tN;
    }/**/
    
    fraction = tN;
    
    return 0.0f;
    //if(fraction = tN)

}



bool CastRay(vec3 rayOrigin, vec3 rayDirection, out float fraction, out vec3 normal, out Material material)
{
    float minDistance = FAR_DISTANCE;

    for (int i = 0; i < SPHERE_COUNT; ++i)
    {
        float D;
        vec3 N;
        if (IntersectRaySphere(rayOrigin, rayDirection, spheres[i], D, N))
        {
            if(D < minDistance)
            {
                minDistance = D;
                normal = N;
                material = spheres[i].material;
                return true;//************
            }
                //return true;//************
        }
    }

    for (int i = 0; i < BOX_COUNT; ++i)
    {
        float D;
        vec3 N;
        bool ans;
        if (IntersectRayBox(rayOrigin, rayDirection, boxes[i], D, N,ans)<minDistance&&ans)
        {
            if(D < minDistance&&ans)
            {
                minDistance = D;
                normal = N;
                material = boxes[i].material;
                //outColor2 += vec3(0.0f,sin(minDistance),0.0f);
                return true;//*****************
            }
                fraction = minDistance;
                //outColor2 += vec3(sin(minDistance),1.0f,0.0f);
                return true;//*****************
        }
                //outColor2 += vec3(sin(minDistance),1.0f,0.0f);
    }

    fraction = minDistance;
    return minDistance != FAR_DISTANCE;
}


void main()
{
vec3 origin1=vec3(0.1f,0.1f,0.1f);
vec3 rdirect= normalize(vec3(gl_FragCoord.xy,0.01f));
float len_now = 0;
vec3 normal1;
float distance;
Material out_mat;
float distance_sum = 0.0f;
outColor2 = vec3(1.0f,0.0f,0.0f);
float sum_zerkalnyy = 1.0f;
for (int i = 0; i<1;++i)
{   bool flag1 = false;
    if (CastRay(origin1,rdirect,distance,normal1,out_mat))
    {
        origin1=origin1+rdirect*distance/len_now;
        rdirect=reflect(rdirect,normalize(normal1));
        len_now = length(rdirect-origin1);
        distance_sum+=distance;
        //outColor2 = vec3(sin(distance_sum),0.1f,0.1f)/**(1.0f-distance_sum/FAR_DISTANCE)*/;
        flag1 = true;
        
    }
    if (flag1||true)
    {
        outColor2 =  sin(origin1);

    }
}
OutColor =vec4(normalize(outColor2),1.0f);
}

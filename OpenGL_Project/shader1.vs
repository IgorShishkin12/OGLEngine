#version 460 core

#define FAR_DISTANCE 1000000.0
#define SPHERE_COUNT 3
#define BOX_COUNT 8

struct Box
{
    vec3 halfSize;
    mat3 rotation;
    vec3 position;
};

struct Sphere
{
    vec3 position;
    float radius;
};
struct Plane
{
    
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

bool IntersectRayBox(vec3 origin, vec3 direction, Box box, out float fraction, out vec3 normal)
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

    if (tN > tF || tF < 0.0) return false;

    mat3 txi = transpose(box.rotation);

    if (t1.x > t1.y && t1.x > t1.z)
        normal = txi[0] * s.x;
    else if (t1.y > t1.z)
        normal = txi[1] * s.y;
    else
        normal = txi[2] * s.z;

    fraction = tN;

    return true;
}



bool CastRay(vec3 rayOrigin, vec3 rayDirection, out float fraction, out vec3 normal, out Material material)
{
    float minDistance = FAR_DISTANCE;

    for (int i = 0; i < SPHERE_COUNT; i++)
    {
        float D;
        vec3 N;
        if (IntersectRaySphere(rayOrigin, rayDirection, spheres[i], D, N) && D < minDistance)
        {
            minDistance = D;
            normal = N;
            material = spheres[i].material;
        }
    }

    for (int i = 0; i < BOX_COUNT; i++)
    {
        float D;
        vec3 N;
        if (IntersectRayBox(rayOrigin, rayDirection, boxes[i], D, N) && D < minDistance)
        {
            minDistance = D;
            normal = N;
            material = boxes[i].material;
        }
    }

    fraction = minDistance;
    return minDistance != FAR_DISTANCE;
}

void main()
{
Sphere spheres[SPHERE_COUNT];
Box boxes[BOX_COUNT];
vec3 rayOrigin = vec3(0.0f,0.0f,0.0f);
}
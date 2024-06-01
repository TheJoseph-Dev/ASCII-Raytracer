#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Vector.h"

float time = 0.0f;

#define MAX_D 100.0
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

float iSphere(Ray ray, vec3 pos, float radius)
{
	//sphere at origin has equation |xyz| = r
	//sp |xyz|^2 = r^2.
	//Since |xyz| = ro + t*rd (where t is the parameter to move along the ray),
	//we have ro^2 + 2*ro*rd*t + t^2 - r2. This is a quadratic equation, so:
	vec3 oc = vSum(ray.origin, vMulf(pos, -1)); //distance ray origin - sphere center

	float b = dot(oc, ray.dir);
	float c = dot(oc, oc) - radius * radius; //sph.w is radius
	float h = b*b - c; //Commonly known as delta. The term a is 1 so is not included.

    const float missHit = -1.0;
	float t = (h < 0.0) ? missHit : -b - sqrt(h); // We use -sqrt because we want the CLOSEST distance

	return t;
}

float iBox(Ray r, vec3 boxSize, vec3* outNormal ) 
{   
    vec3 m = { 1.0 / r.dir.x, 1.0 / r.dir.y, 1.0 / r.dir.z };
	vec3 n = vMul(m, r.origin);
	vec3 k = vMul(vAbs(m), boxSize);
	vec3 t1 = vMulf(vSum(n, k), -1);
	vec3 t2 = vSum(vMulf(n, -1), k);
	float tN = MAX(MAX(t1.x, t1.y), t1.z);
	float tF = MIN(MIN(t2.x, t2.y), t2.z);
    
    vec3 yzx = { t1.y, t1.z, t1.x };
	vec3 zxy = { t1.z, t1.x, t1.y };
    *outNormal = normalize( vMul( vMul( vMulf(vSign(r.dir), -1), vStep(yzx, t1) ), vStep(zxy, t1) ) );
    
	return (tN > tF || tF < 0.0) ?
        -1.0 :        // no hit
        MIN(tN, tF);
}

typedef struct ScenePayload {
    vec3 objPos;
    vec3 objNormal;
    float d;
} ScenePayload;

int miss(float hit) { return hit <= 0.0; }

ScenePayload world(Ray r) {
    float d = MAX_D;
    vec3 closestHit;
    vec3 normal;

    {
        vec3 p = { 0.0, 0.0, 0.5 };
        float s = iSphere(r, p, 0.5);
        if (!miss(s)) d = MIN( d, s );
        if(d == s) {
            closestHit = p;
            normal = normalize(vSum(vSum(r.origin, vMulf(p, -1)), vMulf(r.dir, d)));
        }
    }

    {
        vec3 p = { cos(time*2), 0.4, sin(time*2) };
        float s = iSphere(r, p, 0.15);
        if(!miss(s)) d = MIN( d, s );
        if(d == s) { 
            closestHit = p;
            normal = normalize(vSum(vSum(r.origin, vMulf(p, -1)), vMulf(r.dir, d)));
        }
    }

    {
        vec3 n;
        vec3 p = { -2.0, -0.5, 1.0 };
        r.origin = vSum(r.origin, vMulf(p, -1));
        float b = iBox(r, vMulf(vOne(), 0.3), &n);
        if (!miss(b)) d = MIN( d, b );
        if(d == b) { 
            closestHit = p;
            normal = n;
        }
    }

    ScenePayload sp = { closestHit, normal, d };
    return sp;
}

float render(Ray r) {
    ScenePayload sp = world(r);
    if (sp.d >= MAX_D) return 0.0f;

    vec3 normals = sp.objNormal;

    vec3 lightPos = { 1.0, 1.0, -1.0 }; //{ cos(time), 1.0, sin(time) };
    vec3 lightDir = normalize(vSum(lightPos, vMulf(sp.objPos, -1)));
    float shadedScene = dot(normals, lightDir);
    //shadedScene/=2.0; // Remap normals from [-1 1] to [0 1]

    float scene = MIN(shadedScene, 1.0);
    return scene;
}

#endif
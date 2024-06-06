#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Vector.h"

float time = 0.0f;

#define MAX_D 100.0
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define SPHERE_TYPE 0
#define BOX_TYPE 1
typedef struct SceneObject {
    unsigned int type;
    vec3 pos;
    vec3 normal;
    vec3 rotation;
    float size;
} SceneObject;

typedef struct ScenePayload {
    SceneObject hitObj;
    float d;
} ScenePayload;

typedef struct Scene {
    const SceneObject* objects;
    unsigned int objectsCount;
} Scene;

float iSphere(Ray ray, SceneObject* sph)
{
	//sphere at origin has equation |xyz| = r
	//sp |xyz|^2 = r^2.
	//Since |xyz| = ro + t*rd (where t is the parameter to move along the ray),
	//we have ro^2 + 2*ro*rd*t + t^2 - r2. This is a quadratic equation, so:
	vec3 oc = vSum(ray.origin, vMulf(sph->pos, -1)); //distance ray origin - sphere center

	float b = dot(oc, ray.dir);
	float c = dot(oc, oc) - sph->size * sph->size; //sph.w is radius
	float h = b*b - c; //Commonly known as delta. The term a is 1 so is not included.

    const float missHit = -1.0;
	float t = (h < 0.0) ? missHit : -b - sqrt(h); // We use -sqrt because we want the CLOSEST distance
    
    sph->normal = normalize(vSum(vSum(ray.origin, vMulf(sph->pos, -1)), vMulf(ray.dir, t)));

	return t;
}

float iBox(Ray r, SceneObject* box) 
{   
    r.origin = vSum(r.origin, vMulf(box->pos, -1));
    rotate(&r.origin, box->rotation);
    rotate(&r.dir, box->rotation);
    vec3 m = { 1.0 / r.dir.x, 1.0 / r.dir.y, 1.0 / r.dir.z };
	vec3 n = vMul(m, r.origin);
	vec3 k = vMul(vAbs(m), vMulf(vOne(), box->size) );
	vec3 t1 = vMulf(vSum(n, k), -1);
	vec3 t2 = vSum(vMulf(n, -1), k);
	float tN = MAX(MAX(t1.x, t1.y), t1.z);
	float tF = MIN(MIN(t2.x, t2.y), t2.z);
    
    vec3 yzx = { t1.y, t1.z, t1.x };
	vec3 zxy = { t1.z, t1.x, t1.y };
    box->normal = normalize( vMul( vMul( vMulf(vSign(r.dir), -1), vStep(yzx, t1) ), vStep(zxy, t1) ) );

	return (tN > tF || tF < 0.0) ?
        -1.0 :        // no hit
        MIN(tN, tF);
}


int miss(float hit) { return hit <= 0.0; }

ScenePayload world(Ray r, const SceneObject* sObjects, unsigned int objsCount) {
    float d = MAX_D;
    SceneObject hitObj;

    for(int i = 0; i < objsCount; i++) {
        SceneObject sObj = sObjects[i];
        float s;
        switch (sObj.type) {
            case SPHERE_TYPE:
            
            s = iSphere(r, &sObj);
            if (!miss(s)) d = MIN( d, s );
            if(d == s) hitObj = sObj;
            
            break;
            case BOX_TYPE:

            s = iBox(r, &sObj);
            if (!miss(s)) d = MIN( d, s );
            if(d == s) hitObj = sObj;
             break;
        }
    }

    ScenePayload sp = { hitObj, d };
    return sp;
}

#define len(arr, type) sizeof(arr)/sizeof(type)
float render(Ray r, const Scene* scene) {

    ScenePayload sp = world(r, scene->objects, scene->objectsCount);
    if (sp.d >= MAX_D) return 0.0f;

    vec3 normals = sp.hitObj.normal;

    vec3 lightPos = { 1.0, 1.0, -1.0 }; //{ cos(time), 1.0, sin(time) };
    lightPos = vSum(lightPos, vMulf(sp.hitObj.pos, -1)); // Offset By Pos
    rotate(&lightPos, sp.hitObj.rotation); // Rotate
    vec3 lightDir = normalize(lightPos);
    float shadedScene = dot(normals, lightDir);
    //shadedScene/=2.0; // Remap normals from [-1 1] to [0 1]

    float tracedScene = MIN(shadedScene, 1.0);
    return tracedScene;
}

#endif
#ifndef RAYMARCHER_H
#define RAYMARCHER_H

#include "Vector.h"

float time = 0.0f;

#define MAX_D 100.0
#define MAX_ITER 50
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, lower, upper) (MIN((upper), MAX((x), (lower))))

float sphereSDF(vec3 p, float r) { return length(p)-r; }

float map(vec3 r) {
    float d = MAX_D;
    
    {
        vec3 p = { r.x-0.0, r.y-0.0, r.z-0.5 };
        float s = sphereSDF(p, 0.5);
        d = MIN( d, s );
    }

    {
        vec3 p = { r.x-cos(time*2), r.y-0.4, r.z-sin(time*2)/2 };
        float s = sphereSDF(p, 0.15);
        d = MIN( d, s );
    }

    return d;
}

vec3 GetNormal(vec3 p)
{
    float d0 = map(p);
    const vec3 ep1 = { p.x-.0001, p.y, p.z };
    const vec3 ep2 = { p.x, p.y-.0001, p.z };
    const vec3 ep3 = { p.x, p.y, p.z-.0001 };
    vec3 d1 = {
        map(ep1),
        map(ep2),
        map(ep3) };
    return normalize(vSumf(vMul(d1, -1), d0));
}

vec3 raymarch(Ray r) {

    float d = 0.0;
    vec3 n;
    for(int i = 0; i < MAX_ITER; i++) {
        n = GetNormal(vSum(r.origin, vMul(r.dir, d)));
        d += map(vSum(r.origin, vMul(r.dir, d)));
        if (d >= MAX_D || d <= 0.01) break;
    }

    if (d >= MAX_D) { n.x = 0; n.y = -1.0; n.z = 0; } 
    return n;
}

#endif
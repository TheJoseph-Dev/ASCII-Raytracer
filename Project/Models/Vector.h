#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
int sign(float x) {
    return (x > 0) - (x < 0);
}

int step(float edge, float x) {
    return x >= edge;
}

typedef struct vec3 {
    float x, y, z;
} vec3;

vec3 vOne() {
    vec3 v = { 1.0, 1.0, 1.0 };
    return v;
}

vec3 vZero() {
    vec3 v = { 0.0, 0.0, 0.0 };
    return v;
}

typedef struct Ray {
    vec3 origin;
    vec3 dir;
} Ray;

vec3 vSum(vec3 v1, vec3 v2) { vec3 v = { v1.x+v2.x, v1.y+v2.y, v1.z+v2.z }; return v; }
vec3 vSumf(vec3 v1, float f) { vec3 v = { v1.x+f, v1.y+f, v1.z+f }; return v; }
vec3 vMulf(vec3 v1, float f) { vec3 v = { v1.x*f, v1.y*f, v1.z*f }; return v; }
vec3 vMul(vec3 v1, vec3 v2) { vec3 v = { v1.x*v2.x, v1.y*v2.y, v1.z*v2.z }; return v; }
vec3 vAbs(vec3 v1) { vec3 v = { abs(v1.x), abs(v1.y), abs(v1.z) }; return v; }
vec3 vSign(vec3 v1) { vec3 v = { sign(v1.x), sign(v1.y), sign(v1.z) }; return v; }
vec3 vStep(vec3 v1, vec3 v2) { vec3 v = { step(v1.x, v2.x), step(v1.y, v2.y), step(v1.z, v2.z) }; return v; }


float dot(vec3 v1, vec3 v2) { return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z; }

float length(vec3 v) {
    return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 normalize(vec3 v) {
    return vMulf(v, 1.0/length(v));
}

void yRotate(vec3* v, float angle) {
    vec3 vv = *v;
    (*v).x = vv.x*cos(angle) - vv.z*sin(angle);
    (*v).z = vv.x*sin(angle) + vv.z*cos(angle);
}

void rotate(vec3* v, const vec3 euler) {
    vec3 vv = *v;
    (*v).y = vv.y*cos(euler.x) - vv.z*sin(euler.x);
    (*v).z = vv.y*sin(euler.x) + vv.z*cos(euler.x);
    
    vv = *v;
    (*v).x = vv.x*cos(euler.y) - vv.z*sin(euler.y);
    (*v).z = vv.x*sin(euler.y) + vv.z*cos(euler.y);

    vv = *v;
    (*v).x = vv.x*cos(euler.z) - vv.y*sin(euler.z);
    (*v).y = vv.x*sin(euler.z) + vv.y*cos(euler.z);
}

#endif
#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <conio.h>
#include "Vector.h"

#define DEG2RAD 3.14/180
#define RAD2DEG 180/3.14

#define KEY_FORWARD 'w'
#define KEY_LEFT 'a'
#define KEY_BACK 's'
#define KEY_RIGHT 'd'
#define KEY_UP ' '
#define KEY_DOWN 'z'

void updatePosition(vec3* p, float speed) {
    if(kbhit()){
        char c = getch();
        vec3 dir = { 0.0, 0.0, 0.0 };
        const vec3 fDir = {0.0, 0.0, 1.0};
        const vec3 lDir = {-1.0, 0.0, 0.0};
        const vec3 bDir = {0.0, 0.0, -1.0};
        const vec3 rDir = {1.0, 0.0, 0.0};
        const vec3 uDir = {0.0, 1.0, 0.0};
        const vec3 dDir = {0.0, -1.0, 0.0};

        if(c == KEY_FORWARD) dir = vSum(dir, fDir);
        if(c == KEY_LEFT)    dir = vSum(dir, lDir);
        if(c == KEY_BACK)    dir = vSum(dir, bDir);
        if(c == KEY_RIGHT)   dir = vSum(dir, rDir);
        if(c == KEY_UP)      dir = vSum(dir, uDir);
        if(c == KEY_DOWN)    dir = vSum(dir, dDir);
        *p = vSum(*p, vMulf(dir, speed));
    }
}

#define KEY_RLEFT 'o'
#define KEY_RRIGHT 'p'

void updateRotation(float* axisR, float angle) {
    if(kbhit()){
        char c = getch();
        if(c == KEY_RLEFT) *axisR += angle;
        if(c == KEY_RRIGHT) *axisR += -angle;
    }
}

#endif
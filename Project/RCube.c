#include <stdio.h>
#include <stdlib.h>
#include "Models/Camera.h"
#include "Models/Renderer.h"
//#include "Raymarcher.h"
#include "Models/Raytracer.h"
#include "Models/InputHandler.h"


Camera camera;
#define FRAMEHOLD_TIME 10

void start();
void loop();
void clear();
void draw();
void drawPixel();
void frameHolder();

int main() {

    start();
    loop();

    return 0;
}

void start() {
    setupCanvas();
    const vec3 startPos = { -1.0, 1.0, -3.0 }; //{ 0.0, 0.0, -1.0 }; 
    camera.pos = startPos;
    camera.yRot = 0.0f;
    system(""); // Enable ANSI Terminal Cmpatibility
}

void loop() {
    while(1) {
        clear();
        time += 0.1;
        draw();
        frameHolder(FRAMEHOLD_TIME);
        updatePosition(&camera.pos, 0.25);
        updateRotation(&camera.yRot,  15.0 * DEG2RAD);
    }
}

void clear() {
    printf("\x1b[2j\x1b[H");
}


void draw() {

    for(size_t i = WINDOW_HEIGHT-1; i > 0; i--) {
        for(size_t j = 0; j < WINDOW_WIDTH; j++) {
            
            vec3 uv = { (float)j/WINDOW_WIDTH * 2.0 - 1.0, (float)i/WINDOW_HEIGHT * 2.0 - 1.0, 0 };
            float aspect = WINDOW_WIDTH/WINDOW_HEIGHT;
            uv.x *= aspect/2.0;
            uv.x /= 2.0; // FOV
            uv.y /= 2.0; // FOV
            uv.z = 1.0;

            vec3 dir = normalize(uv);
            yRotate(&dir, camera.yRot);
            Ray ray = { camera.pos, dir };

            SceneObject objs[] = {
                { BOX_TYPE, { 0.0, 0.0, 1.0 }, vZero(), 0.5 }
            };

            Scene scene = { objs, len(objs, SceneObject) };

            float renderedScene = render(ray, &scene);
            canvas[i][j] = getPixel(renderedScene);
            drawPixel(i, j);
        }
        putchar('\n');
    }

    printf("Pos: (%.2f %.2f %.2f) - Angle: %.2f\n", camera.pos.x, camera.pos.y, camera.pos.z, camera.yRot * RAD2DEG);
}

void drawPixel(size_t i, size_t j) {
    putchar(canvas[i][j]);
}

void frameHolder(int time) {
    _sleep(time);
}
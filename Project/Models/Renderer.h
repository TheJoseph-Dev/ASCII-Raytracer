#ifndef RENDERER_H
#define RENDERER_H

#define WINDOW_WIDTH 188
#define WINDOW_HEIGHT 46

// Pixels
#define PIXELS 92
const char pixels[] = " `.-':_,^=;><+!rc*/z?sLTv)J7(|Fi{C}fI31tlu[neoZ5Yxjya]2ESwqkP6h9d4VpOGbUAKXHm8RD#$B0MNWQ%%&@";
float pixelsBrightness[PIXELS] = { 0.05, 0.08, 0.085, 0.09, 0.1227, 0.1403, 0.1559, 0.185, 0.2183, 0.2417, 0.2571, 0.2852, 0.2902, 0.2919, 0.3099, 0.3192, 0.3232, 0.3294, 0.3384, 0.3609, 0.3619, 0.3667, 0.3737, 0.3747, 0.3838, 0.3921, 0.396, 0.3984, 0.3993, 0.4075, 0.4091, 0.4101, 0.42, 0.423, 0.4247, 0.4274, 0.4293, 0.4328, 0.4382, 0.4385, 0.442, 0.4473, 0.4477, 0.4503, 0.4562, 0.458, 0.461, 0.4638, 0.4667, 0.4686, 0.4693, 0.4703, 0.4833, 0.4881, 0.4944, 0.4953, 0.4992, 0.5509, 0.5567, 0.5569, 0.5591, 0.5602, 0.5602, 0.565, 0.5776, 0.5777, 0.5818, 0.587, 0.5972, 0.5999, 0.6043, 0.6049, 0.6093, 0.6099, 0.6465, 0.6561, 0.6595, 0.6631, 0.6714, 0.6759, 0.6809, 0.6816, 0.6925, 0.7039, 0.7086, 0.7235, 0.7302, 0.7332, 0.7602, 0.7834, 0.8037, 1.01 };

char** canvas;

void setupCanvas() {
    // Setup Canvas
    canvas = (char**)malloc(sizeof(char*) * WINDOW_HEIGHT);
    for (size_t i = 0; i < WINDOW_HEIGHT; i++)
        canvas[i] = (char*)malloc(sizeof(char) * WINDOW_WIDTH);

    for(int i = 0; i < WINDOW_HEIGHT; i++) 
        for(int j = 0; j < WINDOW_WIDTH; j++)
            canvas[i][j] = pixels[0];
}

char getPixel(float color) {
    for(size_t i = 0; i < PIXELS; i++)
        if(color < pixelsBrightness[i]) return pixels[i];
}

void drawPixel(size_t i, size_t j) {
    putchar(canvas[i][j]);
}

void clear() {
    printf("\x1b[2j\x1b[H");
}

#endif
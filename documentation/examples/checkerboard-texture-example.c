#include <stdio.h>
#include <stdint.h>
#include "bmp.h"

static void create_checkerboard_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, 
                                        unsigned int squares_x, unsigned int squares_y, 
                                        color_32bpp_t color1, color_32bpp_t color2);

int main() {
    bmp_t bmp;
    unsigned int width = 128, height = 128;

    bmp_init_data(&bmp, width, height, 32);
    color_32bpp_t* colors = (color_32bpp_t*)bmp.data;

    color_32bpp_t magenta = {255, 0, 255, 255};
    color_32bpp_t black = {0, 0, 0, 255};       

    create_checkerboard_texture(colors, width, height, 8, 8, magenta, black);

    bmp_save(&bmp, "checkerboard_texture.bmp");
    bmp_destroy(&bmp);
    
    return 0;
}

static void create_checkerboard_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, 
                                        unsigned int squares_x, unsigned int squares_y, 
                                        color_32bpp_t color1, color_32bpp_t color2) {
    unsigned int square_width = width / squares_x;
    unsigned int square_height = height / squares_y;

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned int offset = (y * width) + x;
            unsigned int square_x = x / square_width;
            unsigned int square_y = y / square_height;
            
            // Determine the color based on the square's position
            color_32bpp_t current_color = ((square_x + square_y) % 2 == 0) ? color1 : color2;

            pixels[offset] = current_color;
        }
    }
}
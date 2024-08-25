#include <stdio.h>
#include <stdint.h>
#include "bmp.h"


static void create_brick_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, 
                                 unsigned int horiz_bricks, unsigned int vert_bricks, 
                                 color_32bpp_t brick_color, color_32bpp_t mortar_color);

int main() {
    bmp_t bmp;
    unsigned int width = 128, height = 128;

    bmp_init_data(&bmp, width, height, 32);
    color_32bpp_t* colors = (color_32bpp_t*)bmp.data;

    color_32bpp_t brick_color = {48, 84, 201, 255};
    color_32bpp_t mortar_color = {164, 177, 219, 255};

    create_brick_texture(colors, width, height, 8, 16, brick_color, mortar_color);

    bmp_save(&bmp, "brick_texture.bmp");
    bmp_destroy(&bmp);
    
    return 0;
}

static void create_brick_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, 
                                 unsigned int horiz_bricks, unsigned int vert_bricks, 
                                 color_32bpp_t brick_color, color_32bpp_t mortar_color) {
    unsigned int h_brick_size = width / horiz_bricks;
    unsigned int v_brick_size = height / vert_bricks;

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            unsigned int offset = (y * width) + x;
            color_32bpp_t cur_color;

            if (y % v_brick_size == 0) {
                cur_color = mortar_color;
            } else if ((y / v_brick_size) % 2 == 0) {
                cur_color = (x % h_brick_size == 0) ? mortar_color : brick_color;
            } else {
                cur_color = ((x + (h_brick_size / 2)) % h_brick_size == 0) ? mortar_color : brick_color;
            }

            pixels[offset] = cur_color;
        }
    }
}
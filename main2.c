// main.c
#include <stdio.h> 
#include <stdint.h> 
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"
#include <inttypes.h>

static void create_checkerboard_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, unsigned int horiz_sections, unsigned int vert_sections, color_32bpp_t color_1, color_32bpp_t color_2);
static void create_brick_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, unsigned int horiz_bricks, unsigned int vert_bricks, color_32bpp_t brick_color, color_32bpp_t mortar_color);

int main(int argc, char *argv[]) {
    bmp_t bmp;

    printf("Initializing bmp data\n");
    bmp_init_data(&bmp, 1024, 1024, 32);

    

    color_32bpp_t* colors;
    colors = (color_32bpp_t*)bmp.data;

    color_32bpp_t color1 = {
        .r = 20,
        .g = 100,
        .b = 90,
        .a = 255
    };

    color_32bpp_t color2 = {
        .r = 255,
        .g = 120,
        .b = 40,
        .a = 255
    };

    printf("Creating brick texture\n");
    create_brick_texture(colors, 1024, 1024, 222, 400, color1, color2);

    bmp_save(&bmp, "photo.bmp");
    bmp_destroy(&bmp);  
    
    
    return 0;
}


static void create_checkerboard_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, unsigned int horiz_sections, unsigned int vert_sections, color_32bpp_t color_1, color_32bpp_t color_2) {
  unsigned int offset;
  unsigned int h_sec_size = width / horiz_sections;
  unsigned int v_sec_size = height / vert_sections;
  color_32bpp_t cur_color;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      offset = (y * width) + x;
      cur_color = ((x / h_sec_size) % 2 == 0) ?
        (((y / v_sec_size) % 2 == 0) ? color_1 : color_2)
        : (((y / v_sec_size) % 2 == 0) ? color_2 : color_1);
      pixels[offset].r = cur_color.r;
      pixels[offset].g = cur_color.g;
      pixels[offset].b = cur_color.b;
      pixels[offset].a = cur_color.a;
    }
  }
}

static void create_brick_texture(color_32bpp_t* pixels, unsigned int width, unsigned int height, unsigned int horiz_bricks, unsigned int vert_bricks, color_32bpp_t brick_color, color_32bpp_t mortar_color) {
  unsigned int offset;
  unsigned int h_brick_size = width / horiz_bricks;
  unsigned int v_brick_size = height / vert_bricks;
  color_32bpp_t cur_color;
  for (unsigned int y = 0; y < height; y++) {
    for (unsigned int x = 0; x < width; x++) {
      offset = (y * width) + x;
      if (y % v_brick_size == 0)
        cur_color = mortar_color;
      else {
        if ((y / v_brick_size) % 2 == 0)
          cur_color = (x % h_brick_size) == 0 ? mortar_color : brick_color;
        else
          cur_color = ((x + (h_brick_size / 2)) % h_brick_size) == 0 ? mortar_color : brick_color;
      }
      pixels[offset].r = cur_color.r;
      pixels[offset].g = cur_color.g;
      pixels[offset].b = cur_color.b;
      pixels[offset].a = cur_color.a;
    }
  }
}


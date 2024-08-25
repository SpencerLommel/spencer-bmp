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
    if (argc < 2)
    {
        printf("Please specify a filepath\n");
        return 1;
    }

    bmp_t bmp;

    bmp_init(&bmp);

    bmp_error_t bmp_status = bmp_openfile(&bmp, argv[1]);
    if (bmp_status != NO_ERROR)
    {
        printf("Corrupted file\n");
        printf("Error: %s\n", bmp_error_strings[bmp_status]);

        return 1;
    }

    color_32bpp_t* colors;
    colors = (color_32bpp_t*)bmp.data;

    for (uint32_t y=0; y < bmp.height; y++)
    {
        for (uint32_t x=0; x < bmp.width; x++)
        {
            uint32_t offset = (y*bmp.width) + x;
            color_32bpp_t color = colors[offset];
            printf("Data: %d %d %d %d\n", color.r, color.g, color.b, color.a);

        }  
    }

    // uint32_t x, y;
    // uint8_t r, g, b, a;
    // printf("Enter coordinates of pixel you want to edit: \n");
    // scanf("%" SCNu32 " %" SCNu32, &x, &y);

    // printf("Enter rgba with spaces (0-255): \n");
    // scanf("%hhu %hhu %hhu %hhu", &r, &g, &b, &a);

    // printf("X: %" PRIu32 " Y: %" PRIu32 "\n", x, y);
    // printf("R: %u G: %u B: %u, A: %u\n", r, g, b, a);


    // uint32_t offset;
    // offset = (bmp.width * y) + x;
    // colors[offset].r = r;
    // colors[offset].b = b;
    // colors[offset].g = g;
    // colors[offset].a = a;    

    color_32bpp_t color1 = {
        .r = 240,
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

    create_brick_texture(colors, 128, 128, 4, 8, color1, color2);

    
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


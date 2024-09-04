// bmp.h
#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_16bpp_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color_24bpp_t;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} color_32bpp_t;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint16_t color_depth;
    void* data; // Cast into color_16/24/32bpp_t
} bmp_t;

typedef enum {
    NO_ERROR = 0,
    INVALID_DEPTH,
    NULL_PTR_ERROR,
    INVALID_FILE,
    INVALID_SIZE,
    INVALID_INFOHEADER,
    INVALID_COMPRESSION,
    DATA_EXISTS,
    OUT_OF_MEMORY

} bmp_error_t;

typedef enum {
    LEFT = 0,
    RIGHT
} bmp_rotation_direction_t;

extern const char* bmp_error_strings[];

bmp_error_t bmp_init(bmp_t* bmp);
bmp_error_t bmp_init_data(bmp_t* bmp, uint16_t width, uint16_t height, uint8_t color_depth);
bmp_error_t bmp_rotate(bmp_t* bmp, bmp_rotation_direction_t direction);
bmp_error_t bmp_destroy(bmp_t* bmp);
bmp_error_t bmp_openfile(bmp_t* bmp, const char* filepath);
bmp_error_t bmp_save(bmp_t* bmp, const char* filepath);


#endif  // BMP_H

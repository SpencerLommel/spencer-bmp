// bmp.c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bmp.h"

typedef struct {
    // uint16_t signature;
    uint32_t filesize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t xpixelsPerM;
    uint32_t ypixelsPerM;
    uint32_t colorsUsed;
    uint32_t imporantColors;


} bmp_header_t;

static uint32_t change_endian_32(uint32_t num);
static uint16_t change_endian_16(uint16_t num);
static void print_header_hex(bmp_header_t* bmp_header);
static void print_header(bmp_header_t* bmp_header);

const char* bmp_error_strings[] = {
    "NO_ERROR",
    "INVALID_DEPTH",
    "NULL_PTR_ERROR",
    "INVALID_FILE",
    "INVALID_SIZE",
    "INVALID_INFOHEADER",
    "INVALID_COMPRESSION",
    "DATA_EXISTS",
    "OUT_OF_MEMORY"
};

const char* bmp_rotation_directions[] = {
    "LEFT",
    "RIGHT"
};

typedef enum {
    BPP_16 = 16,
    BPP_24 = 24,
    BPP_32 = 32
} color_depth_t;

typedef enum {
    RED = 0,
    GREEN,
    BLUE
} color_table_t;

bmp_error_t bmp_init(bmp_t* bmp) {
    bmp->width = 0;
    bmp->height = 0;
    bmp->color_depth = 0;
    bmp->data = NULL;

    return NO_ERROR;
}

bmp_error_t bmp_init_data(bmp_t* bmp, uint16_t width, uint16_t height, uint8_t color_depth) {

    bmp->width = width;
    bmp->height = height;
    bmp->color_depth = color_depth;
    bmp->data = malloc(width * height * (color_depth / 8));

    if (bmp->data == NULL) {
        return OUT_OF_MEMORY;
    }
    return NO_ERROR;
}


bmp_error_t bmp_destroy(bmp_t* bmp) {
    if (bmp->data != NULL)
    {
        free(bmp->data);
        return NO_ERROR;
    }

    return NULL_PTR_ERROR;
}

bmp_error_t bmp_rotate(bmp_t *bmp, bmp_rotation_direction_t direction) {
    if (bmp->data == NULL) {
        return NULL_PTR_ERROR;
    }

    // Calculate the new width and height after rotation
    uint32_t new_width = (direction == RIGHT) ? bmp->height : bmp->width;
    uint32_t new_height = (direction == RIGHT) ? bmp->width : bmp->height;

    // Allocate memory for the new image data
    uint8_t *new_data = malloc(new_width * new_height * (bmp->color_depth / 8));
    if (new_data == NULL) {
        return OUT_OF_MEMORY;
    }

    // Rotate the image data
    for (uint32_t y = 0; y < bmp->height; y++) {
        for (uint32_t x = 0; x < bmp->width; x++) {
            uint32_t new_x, new_y;
            if (direction == LEFT) {
                new_x = bmp->height - y - 1;
                new_y = x;
            } else {
                new_x = y;
                new_y = bmp->width - x - 1;
            }

            uint32_t new_offset = (new_y * new_width + new_x) * (bmp->color_depth / 8);
            uint32_t old_offset = (y * bmp->width + x) * (bmp->color_depth / 8);

            memcpy(&new_data[new_offset], (uint8_t*)bmp->data + old_offset, bmp->color_depth / 8);
        }
    }

    // Update the BMP structure with the new data
    free(bmp->data);
    bmp->data = new_data;
    bmp->width = new_width;
    bmp->height = new_height;

    return NO_ERROR;
}

bmp_error_t bmp_flip(bmp_t *bmp) {
    if (bmp->data == NULL) {
        return NULL_PTR_ERROR;
    }

    uint32_t bytes_per_pixel = bmp->color_depth / 8;
    uint32_t row_size = bmp->width * bytes_per_pixel;
    uint8_t *new_data = malloc(bmp->width * bmp->height * bytes_per_pixel);

    if (new_data == NULL) {
        return OUT_OF_MEMORY;
    }

    for (uint32_t y = 0; y < bmp->height; y++) {
        uint32_t src_offset = y * row_size;
        uint32_t dst_offset = (bmp->height - 1 - y) * row_size;
        memcpy(new_data + dst_offset, bmp->data + src_offset, row_size);
    }

    free(bmp->data);
    bmp->data = new_data;

    return NO_ERROR;
}


bmp_error_t bmp_openfile(bmp_t* bmp, const char* filepath) {
    FILE* fptr;
    fptr = fopen(filepath, "rb");

    if (fptr == NULL) {
        return INVALID_FILE;
    }

    bmp_header_t bmp_header;
    bmp_header.imageSize = 0;

    size_t filesize;
    // Go to the end of the file
    fseek(fptr, 0, SEEK_END);
    // Get the current file offset
    filesize = ftell(fptr);
    // Go back to the beginning of the file
    fseek(fptr, 0, SEEK_SET);

    uint16_t signature;

    fread(&signature, sizeof(uint16_t), 1, fptr);

    if (fread(&bmp_header, sizeof(bmp_header_t), 1, fptr) < 1)
    {
        fclose(fptr);
        return INVALID_SIZE;
    }

    for (unsigned int i = 0; i < sizeof(bmp_header_t); i++) {
        printf("%02X ",((uint8_t*)&bmp_header)[i]);

        }
        printf("\n");


    fseek(fptr, bmp_header.size + 14, SEEK_SET);

    // bmp_header.signature = change_endian_16(bmp_header.signature);
    // signature = change_endian_16(signature);
    // bmp_header.filesize = change_endian_32(bmp_header.filesize);
    // bmp_header.reserved = change_endian_32(bmp_header.reserved);
    // bmp_header.dataOffset = change_endian_32(bmp_header.dataOffset);
    // bmp_header.size = change_endian_32(bmp_header.size);
    // bmp_header.width = change_endian_32(bmp_header.width);
    // bmp_header.height = change_endian_32(bmp_header.height);
    // bmp_header.planes = change_endian_16(bmp_header.planes);
    // bmp_header.bpp = change_endian_16(bmp_header.bpp);
    // bmp_header.compression = change_endian_32(bmp_header.compression);
    // bmp_header.imageSize = change_endian_32(bmp_header.imageSize);
    // bmp_header.xpixelsPerM = change_endian_32(bmp_header.xpixelsPerM);
    // bmp_header.ypixelsPerM = change_endian_32(bmp_header.ypixelsPerM);
    // bmp_header.colorsUsed = change_endian_32(bmp_header.colorsUsed);
    // bmp_header.imporantColors = change_endian_32(bmp_header.imporantColors);


    bmp->width = bmp_header.width;
    bmp->height = bmp_header.height;
    switch (bmp_header.bpp)
    {
        case BPP_16: {
            bmp->color_depth = 16;
        } break;

        case BPP_24: {
            bmp->color_depth = 24;
        } break;

        case BPP_32: {
            bmp->color_depth = 32;
        } break;

        default: {
            fclose(fptr);
            return INVALID_DEPTH;
        }
    }

    if (!(0 < bmp_header.width && 5000 > bmp_header.width && 0 < bmp_header.height && 5000 > bmp_header.height))
    {
        fclose(fptr);
        return INVALID_SIZE;
    }

    // if (bmp_header.size != 40)
    // {
    //     fclose(fptr);
    //     return INVALID_INFOHEADER;
    // }

    // if (bmp_header.compression != 0)
    // {
    //     fclose(fptr);
    //     return INVALID_COMPRESSION;
    // }

    bmp->data = malloc(bmp_header.imageSize);

    if (fread(bmp->data, bmp_header.imageSize, 1, fptr) < 1)
    {
        fclose(fptr);
        free(bmp->data);
        bmp->data = NULL;
        return INVALID_FILE;
    }

    fclose(fptr);
    return NO_ERROR;


}

bmp_error_t bmp_save(bmp_t* bmp, const char* filepath) {
    if (bmp->data == NULL) {
        return NULL_PTR_ERROR;
    }

    bmp_header_t bmp_header;


    switch (bmp->color_depth)
    {
    case 16:
        bmp_header.bpp = BPP_16;
        break;
    case 24:
        bmp_header.bpp = BPP_24;
        break;
    case 32:
        bmp_header.bpp = BPP_32;
        break;
    default:
        return INVALID_DEPTH;
        break;
    }

    bmp_header.size = 40;
    bmp_header.width = bmp->width;
    bmp_header.height = bmp->height;
    bmp_header.imageSize = (bmp->width * bmp->height * (bmp->color_depth / 8));
    bmp_header.filesize = (2 + bmp_header.size + bmp_header.imageSize);
    bmp_header.xpixelsPerM = 69420;
    bmp_header.ypixelsPerM = 69420;
    bmp_header.colorsUsed = 69420;
    bmp_header.imporantColors = 0;
    bmp_header.planes = 1;
    bmp_header.dataOffset = 2 + sizeof(bmp_header_t);
    bmp_header.reserved = 0;
    bmp_header.compression = 0;

    FILE* fptr;
    fptr = fopen(filepath, "wb");

    if (fptr == NULL)
    {
        return INVALID_FILE;
    }

    uint16_t signature = 0x4d42;

    fwrite(&signature, sizeof(uint16_t), 1, fptr);
    fwrite(&bmp_header, sizeof(bmp_header_t), 1, fptr);
    fwrite(bmp->data, bmp_header.imageSize, 1, fptr);
    fclose(fptr);

    return NO_ERROR;


}


// Required static functions
static uint32_t change_endian_32(uint32_t num) {
    return ((num & 0xFF000000) >> 24) |
           ((num & 0x00FF0000) >> 8)  |
           ((num & 0x0000FF00) << 8)  |
           ((num & 0x000000FF) << 24);
}

static uint16_t change_endian_16(uint16_t num) {
    return ((num & 0xFF00) >> 8) |
           ((num & 0x00FF) << 8);
}


// Development static functions
// These can be removed for production releases
static void print_header_hex(bmp_header_t* bmp_header) {
    // printf("Signature: %04x\n", bmp_header->signature);
    printf("FileSize: %08x\n", bmp_header->filesize);
    printf("Reserved: %08x\n", bmp_header->reserved);
    printf("Data Offset: %08x\n", bmp_header->dataOffset);
    printf("Size: %08x\n", bmp_header->size);
    printf("Width: %08x\n", bmp_header->width);
    printf("Height: %08x\n", bmp_header->height);
    printf("Planes: %04x\n", bmp_header->planes);
    printf("BPP: %04x\n", bmp_header->bpp);
    printf("Compression: %08x\n", bmp_header->compression);
    printf("Image Size: %08x\n", bmp_header->imageSize);
    printf("X Pixels Per M: %08x\n", bmp_header->xpixelsPerM);
    printf("Y Pixels Per M: %08x\n", bmp_header->ypixelsPerM);
    printf("Colors Used: %08x\n", bmp_header->colorsUsed);
    printf("Important Colors: %08x\n", bmp_header->imporantColors);

}

static void print_header(bmp_header_t* bmp_header) {
    // printf("Signature: %04x\n", bmp_header->signature);
    printf("FileSize: %u\n", bmp_header->filesize);
    printf("Reserved: %u\n", bmp_header->reserved);
    printf("Data Offset: %u\n", bmp_header->dataOffset);
    printf("Size: %u\n", bmp_header->size);
    printf("Width: %u\n", bmp_header->width);
    printf("Height: %u\n", bmp_header->height);
    printf("Planes: %u\n", bmp_header->planes);
    printf("BPP: %u\n", bmp_header->bpp);
    printf("Compression: %u\n", bmp_header->compression);
    printf("Image Size: %u\n", bmp_header->imageSize);
    printf("X Pixels Per M: %u\n", bmp_header->xpixelsPerM);
    printf("Y Pixels Per M: %u\n", bmp_header->ypixelsPerM);
    printf("Colors Used: %u\n", bmp_header->colorsUsed);
    printf("Important Colors: %u\n", bmp_header->imporantColors);

}

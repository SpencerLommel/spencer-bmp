#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bmp.h"
#include <inttypes.h>

int main() {
    bmp_t bmp;
    bmp_init(&bmp);

    bmp_error_t bmp_status = bmp_openfile(&bmp, "documentation/images/brick_texture.bmp");
    if (bmp_status != NO_ERROR) {
        printf("Invalid file\n");
        printf("Error: %s\n", bmp_error_strings[bmp_status]);

        return 1;
    }

    bmp_rotate(&bmp, RIGHT);

    bmp_save(&bmp, "rotated_image.bmp");
}

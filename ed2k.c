/*
 * Copyright (c) 2024 Ennea
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "md4.h"

int main(void) {
    uint8_t i;
    uint32_t block_bytes_read = 0;
    uint32_t total_blocks_read = 0;
    uint64_t total_bytes_read = 0;
    size_t bytes_read;
    char buf[4096];
    unsigned char md[16] = {0};
    MD4_CTX ctx1, ctx2;

    MD4_Init(&ctx1);
    MD4_Init(&ctx2);

    FILE *fp = stdin;
    while (true) {
        bytes_read = fread(buf, 1, 4096, fp);
        if (ferror(fp)) {
            printf("Error reading stdin\n");
            return 1;
        }

        MD4_Update(&ctx1, buf, bytes_read);
        block_bytes_read += bytes_read;
        total_bytes_read += bytes_read;
        if (block_bytes_read == 9728000) {
            block_bytes_read = 0;
            total_blocks_read++;
            MD4_Final(md, &ctx1);
            MD4_Update(&ctx2, md, 16);
            MD4_Init(&ctx1);
        }

        if (feof(fp)) {
            break;
        }
    }

    if (total_blocks_read == 0) {
        MD4_Final(md, &ctx1);
    } else {
        if (block_bytes_read != 0) {
            MD4_Final(md, &ctx1);
            MD4_Update(&ctx2, md, 16);
        }
        MD4_Final(md, &ctx2);
    }
    printf("%ld ", total_bytes_read);
    for (i = 0; i < 16; i++) {
        printf("%02x", md[i]);
    }
}

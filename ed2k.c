#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "md4.h"

int main() {
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
    // read 9728000 byte chunks; that's 2375 * 4096 block reads
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
        MD4_Final(md, &ctx1);
        MD4_Update(&ctx2, md, 16);
        MD4_Final(md, &ctx2);
    }
    printf("%ld ", total_bytes_read);
    for (i = 0; i < 16; i++) {
        printf("%02x", md[i]);
    }
}

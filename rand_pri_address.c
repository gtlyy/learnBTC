// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/stat.h>
// #include <unistd.h>

#include "mybtc.h"

int main()
{
    unsigned char pri_key[LENGHT_PRIVATE_HEX+1];
    unsigned char address[LENGHT_ADDRESS_BASE58_COMPRESS+1];
    int seed;
    for (int i=0; i<10; i++) {
        seed = get_seed_random();
        srand(seed);            
        random_btc_private_key(pri_key);
        pri_key[LENGHT_PRIVATE_HEX] = '\0';
        printf("%s ", pri_key);

        // 密钥 -> 地址            
        private_to_address_malloc(address, LENGHT_PUBLIC_CHAR_COMPRESS, pri_key);
        address[LENGHT_ADDRESS_BASE58_COMPRESS] = '\0';
        printf("%s\n", address);

        // 顺便 crack:
        if (strcmp("16HUcQitGbVcFtXCkV6W9VkSEYfJiaZW1f", address) == 0) {
            printf("Done: \n key=%s, addr=%s\n", pri_key, address);
            FILE *file = fopen("ok.txt", "w");
            if (file != NULL) {
              fprintf(file, "Done: \n key=%s, addr=%s\n", pri_key, address);
              fclose(file);
            } else {
              printf("无法打开文件\n");
            }
            return 0;
        }

    }

    return 0;
}

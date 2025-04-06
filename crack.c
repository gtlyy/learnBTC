#include "mybtc.h"

int main()
{
    // ===================================================================== crack start:
    unsigned char pri_key[LENGHT_PRIVATE_HEX+1];
    unsigned char address[LENGHT_ADDRESS_BASE58_COMPRESS+1];
    int seed ,i;
    for (long int j=0; j<1000; j++) {
            // 随机密钥
            seed = get_seed_random();
            srand(seed);            
            random_btc_private_key(pri_key);
            pri_key[LENGHT_PRIVATE_HEX] = '\0';
            // printf("%s\n", pri_key);

            // 密钥 -> 地址            
            private_to_address_malloc(address, LENGHT_PUBLIC_CHAR_COMPRESS, pri_key);
            address[LENGHT_ADDRESS_BASE58_COMPRESS] = '\0';

            // crack:
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

/*
4.82BTC 
1Hrbgj881yoMYYvNvPkgGgEx6Kw8JxirLK
10BTC 
16HUcQitGbVcFtXCkV6W9VkSEYfJiaZW1f
*/

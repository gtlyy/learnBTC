// #include <stdio.h>
// #include <time.h>
// #include <stdlib.h>


// #include <fcntl.h>
// #include <unistd.h>

#include "mybtc.h"

int main()
{
    unsigned char pri_key[LENGHT_PRIVATE_HEX+1];

    // 获取随机种子
    int seed = get_seed_random();
    srand(seed);

    // 随机生成btc密钥
    random_btc_private_key(pri_key);
    pri_key[LENGHT_PRIVATE_HEX]='\0';
    printf("%s", pri_key);

    return 0;
}

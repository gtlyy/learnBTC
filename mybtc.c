#include "mybtc.h"

// 获取随机种子
unsigned int get_seed_time()
{
	return (unsigned int) time(0);
}

// 获取真随机种子
unsigned int get_seed_random()
{
	int fd_random=open("/dev/random", O_RDONLY);
    int n;
	read(fd_random,&n,sizeof(n));
	close(fd_random);
	return (unsigned int)abs(n); 
}

// 获取ns级别的时间
unsigned long get_time_ns()
{
    struct timespec ts;
	#ifndef CLOCK_REALTIME
	#define CLOCK_REALTIME 0
	#endif
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec*1000000000 + ts.tv_nsec;
}	

// 随机生成 int*
void random_ints(int *out, int n, int min, int max)
{
	unsigned int s = get_seed_random();
	// printf("%d\n", s);
	srand(s);
	for (int i=0; i<n; i++)
		out[i] = rand() % (max - min + 1) + min;
}

// 随机生成 char*
void random_chars(unsigned char *out, int n, unsigned char *words)
{
	int lenght = strlen(words);
	unsigned int seed = get_seed_random();
	srand(seed);
	for (int i=0; i<n; i++) {
		out[i] = words[rand() % lenght];
	}
}

// 随机生成btc密钥
void random_btc_private_key(unsigned char *out) 
{
    // unsigned char* r = (unsigned char *)malloc(LENGHT_PRIVATE_HEX + 1);
    char words[] = "0123456789abcdef";
    random_chars(out, LENGHT_PRIVATE_HEX, words);
    // r[LENGHT_PRIVATE_HEX] = '\0';
    // return r;
}

// 冒泡算法
void bubbleSort(int *in_out, int n)
{
	int t = 0;
	for (int i=1; i<n; i++)
		for (int j=0; j<n-1; j++)
			if (in_out[j] >= in_out[j+1]) {
				t = in_out[j];
				in_out[j] = in_out[j+1];
				in_out[j+1] = t;
			}
}

// 真随机生成 char*
unsigned char* generate_password(int n)
{
	int t, i;
	unsigned char pwd_words[] = "0123456789abcdef";
	unsigned char* r = (unsigned char *)malloc(n +1);
	for (i=0; i<n; i++) {
		srand(get_seed_random());
		t = rand() % 16;
		*(r + i) = pwd_words[t];
	}
	*(r + i) = '\0';
	return r;
}

void char2hex(unsigned char* out, unsigned char c)
{
    unsigned int i = c;
	sprintf(out, "%02X", i);
}

void char2bit(unsigned char* out, unsigned char c)
{
	unsigned char t;
	for (int i=0; i<=7; i++) {
		t = (c >> (7-i)) & (unsigned char)1;
		sprintf(&out[i], "%c", t+'0');
	}
}

void chars_to_hex(unsigned char* out, int lenght, unsigned char* in)
{
    int i;
    for (i=0; i<lenght; i++) {
        sprintf((unsigned char*)(out+i*2), "%02X", in[i]);
    }
}

// 2Hex -> 1char
void hex_to_chars(unsigned char* out, int lenght, unsigned char* in)
{
	int a10, a;
	if (lenght==0 || (lenght%2)!=0) {
		return;
	}
	for(int i=0; i<lenght-1; i=i+2) {
		// 第一位
		if(in[i]>='0' && in[i] <='9') 
			a10 = in[i] - '0';
		else
			a10 = toupper(in[i]) - 'A' + 10;
		// 第二位
		if(in[i+1]>='0' && in[i+1] <='9') 
			a = in[i+1] - '0';
		else
			a = toupper(in[i+1]) - 'A' + 10;
		out[i/2] = 16 * a10 + a;
	}
}

void sha256(unsigned char* out, int lenght, unsigned char* in_char)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, in_char, lenght);
    SHA256_Final(hash, &sha256);
    chars_to_hex(out, LENGHT_SHA256_CHAR, hash);
}

void ripemd160(unsigned char* out, int lenght, unsigned char* in_char)
{
  RIPEMD160_CTX c;
  unsigned char md[RIPEMD160_DIGEST_LENGTH];
  RIPEMD160_Init(&c);
  RIPEMD160_Update(&c, in_char, lenght);
  RIPEMD160_Final(md, &c);
  chars_to_hex(out, 20, md);
}

void print_hex_dec_char(unsigned char* in, int lenght)
{
    // printf("in: %s\nlen: %d\n", in, strlen(in));
    for (int i=0; i<lenght; i++) {
        printf("%c", in[i]);
    }
    printf("\n");
    for (int i=0; i<lenght; i++) {
        printf("%d: %02X-%d-%c\t", i, in[i], in[i], in[i]);
    }
	printf("\n");
}

unsigned char *en_base58(unsigned char *input, int lenght)  // 编码
{
    static char *nb58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
	// size_t len = strlen(input);
    size_t len = lenght;
    size_t rlen = (len / 2 + 1) * 3;
    unsigned char *ret = malloc(rlen + len);
    unsigned char *src = ret + rlen;
    unsigned char *rptr = ret + rlen;
    unsigned char *ptr, *e = src + len - 1;
    size_t i;
    memcpy(src, input, len);
    while(src <= e)
    {
        if(*src)
        {
            unsigned char rest = 0;
            ptr = src;
            while(ptr <= e)
            {
                unsigned int c = rest * 256;
                rest = (c + *ptr) % 58;
                *ptr = (c + *ptr) / 58;
                ptr++;
            }
            --rptr;
            *rptr = nb58[rest];
        }
        else
        {
            src++;
        }
    }
    for(i = 0; i < ret + rlen - rptr; i++)
        ret[i] = rptr[i];
    ret[i] = 0;
    return ret;
}

char *de_base58(unsigned char *src)  // 解码
{
	static char b58n[] =
	{
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8, -1, -1, -1, -1, -1, -1,
	    -1,  9, 10, 11, 12, 13, 14, 15, 16, -1, 17, 18, 19, 20, 21, -1,
	    22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, -1, -1, -1, -1,
	    -1, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, -1, 44, 45, 46,
	    47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};
    size_t len = strlen(src);
    size_t rlen = (len / 4 + 1) * 3;
    unsigned char *ret = malloc(rlen);
    unsigned char *rptr = ret + rlen;
    size_t i;
    unsigned char *ptr;
    for(i = 0; i < len; i++)
    {
        char rest = b58n[src[i]];
        if(rest < 0)
        {
            free(ret);
            return NULL;
        }
        for(ptr = ret + rlen - 1; ptr >= rptr; ptr--)
        {
            unsigned int c = rest + *ptr * 58;
            *ptr = c % 256;
            rest = c / 256;
        }
        if(rest > 0)
        {
            rptr--;
            if(rptr < ret) 
			{
				free(ret);
	            return NULL;	
			}
            *rptr = rest;
        }
    }
    for(i = 0; i < ret + rlen - rptr; i++)
        ret[i] = rptr[i];
    ret[i] = 0;
    return ret;
}

/*
out: HEX格式的公钥地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
*/
void private_to_pub(unsigned char* out, int len, unsigned char* in)
{
    unsigned char private_key_chars[LENGHT_PRIVATE_CHAR+1];
    hex_to_chars(private_key_chars, LENGHT_PRIVATE_HEX, in);
    private_key_chars[LENGHT_PRIVATE_CHAR] = '\0';

    secp256k1_context *ctx = NULL;
    secp256k1_pubkey pubkey;
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY|SECP256K1_CONTEXT_SIGN);
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key_chars) != 1) {
        printf("Call secp256k1_ec_pubkey_create() error.");
        return;
    } 
    
    size_t publen = len;    
    unsigned char public_key_chars[publen+1];
    if (publen == LENGHT_PUBLIC_CHAR) {
        secp256k1_ec_pubkey_serialize(ctx, public_key_chars, &publen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
    } else if (publen == LENGHT_PUBLIC_CHAR_COMPRESS) {
        secp256k1_ec_pubkey_serialize(ctx, public_key_chars, &publen, &pubkey, SECP256K1_EC_COMPRESSED);
    }
    public_key_chars[publen] = '\0';

    chars_to_hex(out, publen, public_key_chars);
}

/*
out: 传统的btc地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
*/
void private_to_address(unsigned char* out, int len, unsigned char* in)
{
    // // 生成密钥
    // unsigned char* private_key = "18e14a7b6a307f426a94f8114701e7c8e774e7f9a47e2c2035db29a206321725";
    // if (argc >=2 && strlen(argv[1]) == LENGHT_PRIVATE_HEX) {
    //     private_key = argv[1];
    // }    
    unsigned char *private_key = in;
    // printf("private_key: %s\t%d\n", private_key, strlen(private_key));
    unsigned char private_key_chars[LENGHT_PRIVATE_CHAR+1];  // hex -> char: 4A -> J, 7B -> {
    hex_to_chars(private_key_chars, LENGHT_PRIVATE_HEX, private_key);
    private_key_chars[LENGHT_PRIVATE_CHAR] = '\0';

    // 密钥 -> 公钥
    secp256k1_context *ctx = NULL;
    secp256k1_pubkey pubkey;
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY|SECP256K1_CONTEXT_SIGN);
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key_chars) != 1) {
        printf("Call secp256k1_ec_pubkey_create() error. In private_to_address().");
    } 

    // 非压缩公钥  or 压缩公钥：
    // size_t publen = LENGHT_PUBLIC_CHAR;             // 非压缩
    // size_t publen = LENGHT_PUBLIC_CHAR_COMPRESS;    // 压缩
    size_t publen = len;
    int flag_compress = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
    unsigned char public_key_chars[publen+1];
    secp256k1_ec_pubkey_serialize(ctx, public_key_chars, &publen, &pubkey, flag_compress);
    public_key_chars[publen] = '\0';
    unsigned char public_key[publen*2+1];
    chars_to_hex(public_key, publen, public_key_chars);
    public_key[2*publen] = '\0';
    // printf("public_key: %s\t%d\n", public_key, strlen(public_key));

    // 公钥 -> sha256
    unsigned char pub_to_sha256[LENGHT_SHA256_HEX+1];
    sha256(pub_to_sha256, publen, public_key_chars);
    pub_to_sha256[LENGHT_SHA256_HEX] = '\0';
    // printf("pub to sha256: %s\t%d\n", pub_to_sha256, strlen(pub_to_sha256));

    // sha256 -> ripemd160
    unsigned char pub_to_sha256_char[32+1];
    unsigned char rip[40+1];
    hex_to_chars(pub_to_sha256_char, 64, pub_to_sha256);
    pub_to_sha256_char[32] = '\0';
    // print_hex_dec_char(pub_to_sha256_char, 32);
    ripemd160(rip, 32, pub_to_sha256_char);
    rip[40] = '\0';
    // printf("to ripemd160: %s\t%d\n", rip, strlen(rip));

    // 加入版本号0x00
    unsigned char ver_rip[42+1];
    strncpy(ver_rip, "00", 2);
    strncpy(ver_rip+2, rip, 40);
    ver_rip[42] = '\0';
    // printf("add '0x00': %s\t%d\n", ver_rip, strlen(ver_rip));

    // to sha256
    unsigned char ver_to_sha256[64+1];
    unsigned char ver_rip_char[21+1];
    hex_to_chars(ver_rip_char, 42, ver_rip);
    ver_rip_char[21] = '\0';
    // print_hex_dec_char(ver_rip_char, 21);
    sha256(ver_to_sha256, 21, ver_rip_char);
    ver_to_sha256[64] = '\0';
    // printf("to sha256: %s\t%d\n", ver_to_sha256, strlen(ver_to_sha256));

    // to sha256 
    unsigned char sha256_sha256[LENGHT_SHA256_HEX+1];
    unsigned char ver_to_sha256_char[LENGHT_SHA256_CHAR];
    hex_to_chars(ver_to_sha256_char, LENGHT_SHA256_HEX, ver_to_sha256);
    sha256(sha256_sha256, LENGHT_SHA256_CHAR, ver_to_sha256_char);
    sha256_sha256[LENGHT_SHA256_HEX] = '\0';
    // printf("sha256_sha256: %s\t%d\n", sha256_sha256, strlen(sha256_sha256));

    // to address_hex
    unsigned char address_hex[LENGHT_ADDRESS_HEX+1];
    strncpy(address_hex, ver_rip, LENGHT_VER_RIP_HEX);
    strncpy(address_hex+LENGHT_VER_RIP_HEX, sha256_sha256, LENGHT_ADDRESS_HEX-LENGHT_VER_RIP_HEX);
    address_hex[LENGHT_ADDRESS_HEX] = '\0';
    // printf("address_hex: %s\t%d\n", address_hex, strlen(address_hex));

    // to address
    unsigned char address_char[LENGHT_ADDRESS_CHAR];
    hex_to_chars(address_char, LENGHT_ADDRESS_HEX, address_hex);
    unsigned char* b58 = en_base58(address_char, LENGHT_ADDRESS_CHAR);

    int len_address_base58 = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? LENGHT_ADDRESS_BASE58_COMPRESS : LENGHT_ADDRESS_BASE58;
    unsigned char address_base58[len_address_base58+1];
	memset(address_base58, '1', len_address_base58-strlen(b58));
	strncpy(address_base58+len_address_base58-strlen(b58), b58, strlen(b58));
	address_base58[len_address_base58] = '\0';
    // printf("address: %s\t%d\n", address_base58, strlen(address_base58));

    strncpy(out, address_base58, LENGHT_ADDRESS_BASE58_COMPRESS+1);
}

/*
out: 传统的btc地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
malloc版
*/
void private_to_address_malloc(unsigned char* out, int len, unsigned char* in)
{
    unsigned char *private_key = in;
    // unsigned char private_key_chars[LENGHT_PRIVATE_CHAR+1];
    unsigned char *private_key_chars = malloc(LENGHT_PRIVATE_CHAR+1);
    hex_to_chars(private_key_chars, LENGHT_PRIVATE_HEX, private_key);
    private_key_chars[LENGHT_PRIVATE_CHAR] = '\0';

    // 密钥 -> 公钥
    secp256k1_context *ctx = NULL;
    secp256k1_pubkey pubkey;
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY|SECP256K1_CONTEXT_SIGN);
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key_chars) != 1) {
        printf("Call secp256k1_ec_pubkey_create() error. In private_to_address().");
    }

    // 非压缩公钥  or 压缩公钥：
    // size_t publen = LENGHT_PUBLIC_CHAR;             // 非压缩
    // size_t publen = LENGHT_PUBLIC_CHAR_COMPRESS;    // 压缩
    size_t publen = len;
    int flag_compress = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
    // unsigned char public_key_chars[publen+1];
    unsigned char *public_key_chars = malloc(publen+1);
    secp256k1_ec_pubkey_serialize(ctx, public_key_chars, &publen, &pubkey, flag_compress);
    public_key_chars[publen] = '\0';
    // unsigned char public_key[publen*2+1];
    unsigned char *public_key = malloc(publen*2+1);
    chars_to_hex(public_key, publen, public_key_chars);
    public_key[2*publen] = '\0';
    // printf("public_key: %s\t%d\n", public_key, strlen(public_key));

    // 公钥 -> sha256
    // unsigned char pub_to_sha256[LENGHT_SHA256_HEX+1];
    unsigned char *pub_to_sha256 = malloc(LENGHT_SHA256_HEX+1);
    sha256(pub_to_sha256, publen, public_key_chars);
    pub_to_sha256[LENGHT_SHA256_HEX] = '\0';
    // printf("pub to sha256: %s\t%d\n", pub_to_sha256, strlen(pub_to_sha256));

    // sha256 -> ripemd160
    // unsigned char pub_to_sha256_char[32+1];
    // unsigned char rip[40+1];
    unsigned char *pub_to_sha256_char = malloc(32+1);
    unsigned char *rip = malloc(40+1);
    hex_to_chars(pub_to_sha256_char, 64, pub_to_sha256);
    pub_to_sha256_char[32] = '\0';
    // print_hex_dec_char(pub_to_sha256_char, 32);
    ripemd160(rip, 32, pub_to_sha256_char);
    rip[40] = '\0';
    // printf("to ripemd160: %s\t%d\n", rip, strlen(rip));

    // 加入版本号0x00
    // unsigned char ver_rip[42+1];
    unsigned char *ver_rip = malloc(42+1);
    strncpy(ver_rip, "00", 2);
    strncpy(ver_rip+2, rip, 40);
    ver_rip[42] = '\0';
    // printf("add '0x00': %s\t%d\n", ver_rip, strlen(ver_rip));

    // to sha256
    // unsigned char ver_to_sha256[64+1];
    // unsigned char ver_rip_char[21+1];
    unsigned char *ver_to_sha256 = malloc(64+1);
    unsigned char *ver_rip_char = malloc(21+1);
    hex_to_chars(ver_rip_char, 42, ver_rip);
    ver_rip_char[21] = '\0';
    // print_hex_dec_char(ver_rip_char, 21);
    sha256(ver_to_sha256, 21, ver_rip_char);
    ver_to_sha256[64] = '\0';
    // printf("to sha256: %s\t%d\n", ver_to_sha256, strlen(ver_to_sha256));

    // to sha256 
    // unsigned char sha256_sha256[LENGHT_SHA256_HEX+1];
    // unsigned char ver_to_sha256_char[LENGHT_SHA256_CHAR];
    unsigned char *sha256_sha256 = malloc(LENGHT_SHA256_HEX+1);
    unsigned char *ver_to_sha256_char = malloc(LENGHT_SHA256_CHAR);
    hex_to_chars(ver_to_sha256_char, LENGHT_SHA256_HEX, ver_to_sha256);
    sha256(sha256_sha256, LENGHT_SHA256_CHAR, ver_to_sha256_char);
    sha256_sha256[LENGHT_SHA256_HEX] = '\0';
    // printf("sha256_sha256: %s\t%d\n", sha256_sha256, strlen(sha256_sha256));

    // to address_hex
    // unsigned char address_hex[LENGHT_ADDRESS_HEX+1];
    unsigned char *address_hex = malloc(LENGHT_ADDRESS_HEX+1);
    strncpy(address_hex, ver_rip, LENGHT_VER_RIP_HEX);
    strncpy(address_hex+LENGHT_VER_RIP_HEX, sha256_sha256, LENGHT_ADDRESS_HEX-LENGHT_VER_RIP_HEX);
    address_hex[LENGHT_ADDRESS_HEX] = '\0';
    // printf("address_hex: %s\t%d\n", address_hex, strlen(address_hex));

    // to address
    // unsigned char address_char[LENGHT_ADDRESS_CHAR];
    unsigned char *address_char = malloc(LENGHT_ADDRESS_CHAR);
    hex_to_chars(address_char, LENGHT_ADDRESS_HEX, address_hex);
    unsigned char *b58 = en_base58(address_char, LENGHT_ADDRESS_CHAR);

    int len_address_base58 = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? LENGHT_ADDRESS_BASE58_COMPRESS : LENGHT_ADDRESS_BASE58;
    // unsigned char address_base58[len_address_base58+1];
    unsigned char *address_base58 = malloc(len_address_base58+1);
	memset(address_base58, '1', len_address_base58-strlen(b58));
	strncpy(address_base58+len_address_base58-strlen(b58), b58, strlen(b58));
	address_base58[len_address_base58] = '\0';
    // printf("address: %s\t%d\n", address_base58, strlen(address_base58));

    strncpy(out, address_base58, LENGHT_ADDRESS_BASE58_COMPRESS+1);

    free(private_key_chars);
    free(public_key_chars);
    free(public_key);
    free(pub_to_sha256);
    free(pub_to_sha256_char);
    free(rip);
    free(ver_rip);
    free(ver_to_sha256);
    free(ver_rip_char);
    free(sha256_sha256);
    free(ver_to_sha256_char);
    free(address_hex);
    free(address_char);
    free(address_base58);
    free(b58);
}
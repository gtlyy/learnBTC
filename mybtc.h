#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <openssl/ripemd.h>

#include <secp256k1.h>

#define LENGHT_PRIVATE_HEX 64                                       // 私钥长度（HEX格式）
#define LENGHT_PRIVATE_CHAR 32                                      // 私钥长度（CHAR格式）
// Uncompress
#define LENGHT_PUBLIC_HEX 130
#define LENGHT_PUBLIC_CHAR 65
// Compress
#define LENGHT_PUBLIC_HEX_COMPRESS 66
#define LENGHT_PUBLIC_CHAR_COMPRESS 33
#define LENGHT_SHA256_HEX 64
#define LENGHT_SHA256_CHAR 32
#define LENGHT_RIPEMD160_HEX 40
#define LENGHT_RIPEMD160_CHAR 20
#define LENGHT_VER_RIP_HEX 42
#define LENGHT_VER_RIP_CHAR 21
#define LENGHT_ADDRESS_HEX 50
#define LENGHT_ADDRESS_CHAR 25
#define LENGHT_ADDRESS_BASE58 33 
#define LENGHT_ADDRESS_BASE58_COMPRESS 34

// 打印 16进制 - 10进制 - char字符
void print_hex_dec_char(unsigned char* in, int lenght);

// 获取随机种子
unsigned int get_seed_time();
// 获取真随机种子
unsigned int get_seed_random();

// 生成随机整形数组
void random_ints(int *out, int n, int min, int max);
// 生成随机字符数组
void random_chars(unsigned char* out, int n, unsigned char* words);
// 生成随机密钥
void random_btc_private_key(unsigned char *out);     

// 字符 to Hex
void char2hex(unsigned char* out, unsigned char c);
// 字符 to Bit
void char2bit(unsigned char* out, unsigned char c);
// 字符串 to HEX格式字符串，即 1c to 2Hex
void chars_to_hex(unsigned char* out, int lenght, unsigned char* in);
// HEX格式字符串 to 字符串，即 2Hex to 1c
void hex_to_chars(unsigned char* out, int lenght, unsigned char* in);

void sha256(unsigned char* out, int lenght, unsigned char* in);
void ripemd160(unsigned char* out, int lenght, unsigned char* in);
unsigned char *en_base58(unsigned char *input, int lenght);

//unsigned long get_time_ns();
// void bubbleSort(int *in_out, int n);


/*
out: HEX格式的公钥地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
*/
void private_to_pub(unsigned char* out, int len, unsigned char* in);

/*
out: 传统的btc地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
*/
void private_to_address(unsigned char* out, int len, unsigned char* in);


/*
out: 传统的btc地址
len: 公钥的字节数 非压缩65 压缩33
in:  HEX格式的私钥地址
malloc()版
*/
void private_to_address_malloc(unsigned char* out, int len, unsigned char* in);
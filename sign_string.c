#include "mybtc.h"
#include <assert.h>

int main()
{
    // 生成密钥
    unsigned char* private_key = "18e14a7b6a307f426a94f8114701e7c8e774e7f9a47e2c2035db29a206321725";
    // printf("private_key: %s\t%d\n", private_key, strlen(private_key));

    unsigned char private_key_chars[LENGHT_PRIVATE_CHAR+1];  // hex -> char: 4A -> J, 7B -> {
    hex_to_chars(private_key_chars, LENGHT_PRIVATE_HEX, private_key);
    private_key_chars[LENGHT_PRIVATE_CHAR] = '\0';

    // 密钥 -> 公钥
    secp256k1_context *ctx = NULL;
    secp256k1_pubkey pubkey;
    ctx = secp256k1_context_create(SECP256K1_CONTEXT_VERIFY|SECP256K1_CONTEXT_SIGN);
    if (secp256k1_ec_pubkey_create(ctx, &pubkey, private_key_chars) != 1) {
        printf("Call secp256k1_ec_pubkey_create() error.");
        return 1;
    } 

    // 随机一个字符串
    int len1 = 10;
    unsigned char string_char[len1+1];
    random_chars(string_char, len1, "0123456789abcdefghijklmnopqrstuvwxyz");
    string_char[len1] = '\0';
    printf("string: %s\n", string_char);

    // 计算这个字符串的 sha256 hash
    unsigned char hash_char[LENGHT_SHA256_CHAR+1];
    unsigned char hash_hex[LENGHT_SHA256_HEX+1];
    SHA256(string_char, len1, hash_char);
    hash_char[LENGHT_SHA256_CHAR] = '\0';
    chars_to_hex(hash_hex, LENGHT_SHA256_CHAR, hash_char);
    hash_hex[LENGHT_SHA256_HEX] = '\0';
    printf("hash_hex: %s\n", hash_hex);

    // 用私钥对上面这个 hash 进行签名
    secp256k1_ecdsa_signature sig;
    int ret = secp256k1_ecdsa_sign(ctx, &sig, hash_char, private_key_chars, NULL, NULL);
    assert(ret == 1);   // 返回1才ok
    printf("sign ok.\n");

    // 用公钥验证签名是否正确
    ret = secp256k1_ecdsa_verify(ctx, &sig, hash_char, &pubkey);
    assert(ret == 1);
    printf("verify ok.\n");

    return 0;
}
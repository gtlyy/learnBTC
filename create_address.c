#include "mybtc.h"

int main(int argc, char *argv[])
{
    // 生成密钥
    unsigned char* private_key = "18e14a7b6a307f426a94f8114701e7c8e774e7f9a47e2c2035db29a206321725";
    
    // unsigned char* private_key = (unsigned char *)malloc(LENGHT_PRIVATE_HEX + 1);
    // random_btc_private_key(private_key) ;
    // private_key[LENGHT_PRIVATE_HEX] = '\0';

    if (argc >=2 && strlen(argv[1]) == LENGHT_PRIVATE_HEX) {
        private_key = argv[1];
    }    
    printf("private_key: %s\t%d\n", private_key, strlen(private_key));

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

    // 非压缩公钥  or 压缩公钥：
    size_t publen = LENGHT_PUBLIC_CHAR;             // 非压缩
    // size_t publen = LENGHT_PUBLIC_CHAR_COMPRESS;    // 压缩
    int flag_compress = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? SECP256K1_EC_COMPRESSED : SECP256K1_EC_UNCOMPRESSED;
    unsigned char public_key_chars[publen+1];
    secp256k1_ec_pubkey_serialize(ctx, public_key_chars, &publen, &pubkey, flag_compress);
    public_key_chars[publen] = '\0';
    unsigned char public_key[publen*2+1];
    chars_to_hex(public_key, publen, public_key_chars);
    public_key[2*publen] = '\0';
    printf("public_key: %s\t%d\n", public_key, strlen(public_key));

    // 公钥 -> sha256
    unsigned char pub_to_sha256[LENGHT_SHA256_HEX+1];
    sha256(pub_to_sha256, publen, public_key_chars);
    pub_to_sha256[LENGHT_SHA256_HEX] = '\0';
    printf("pub to sha256: %s\t%d\n", pub_to_sha256, strlen(pub_to_sha256));

    // sha256 -> ripemd160
    unsigned char pub_to_sha256_char[32+1];
    unsigned char rip[40+1];
    hex_to_chars(pub_to_sha256_char, 64, pub_to_sha256);
    pub_to_sha256_char[32] = '\0';
    // print_hex_dec_char(pub_to_sha256_char, 32);
    ripemd160(rip, 32, pub_to_sha256_char);
    rip[40] = '\0';
    printf("to ripemd160: %s\t%d\n", rip, strlen(rip));

    // 加入版本号0x00
    unsigned char ver_rip[42+1];
    strncpy(ver_rip, "00", 2);
    strncpy(ver_rip+2, rip, 40);
    ver_rip[42] = '\0';
    printf("add '0x00': %s\t%d\n", ver_rip, strlen(ver_rip));

    // to sha256
    unsigned char ver_to_sha256[64+1];
    unsigned char ver_rip_char[21+1];
    hex_to_chars(ver_rip_char, 42, ver_rip);
    ver_rip_char[21] = '\0';
    // print_hex_dec_char(ver_rip_char, 21);
    sha256(ver_to_sha256, 21, ver_rip_char);
    ver_to_sha256[64] = '\0';
    printf("to sha256: %s\t%d\n", ver_to_sha256, strlen(ver_to_sha256));

    // to sha256 
    unsigned char sha256_sha256[LENGHT_SHA256_HEX+1];
    unsigned char ver_to_sha256_char[LENGHT_SHA256_CHAR];
    hex_to_chars(ver_to_sha256_char, LENGHT_SHA256_HEX, ver_to_sha256);
    sha256(sha256_sha256, LENGHT_SHA256_CHAR, ver_to_sha256_char);
    sha256_sha256[LENGHT_SHA256_HEX] = '\0';
    printf("sha256_sha256: %s\t%d\n", sha256_sha256, strlen(sha256_sha256));

    // to address_hex
    unsigned char address_hex[LENGHT_ADDRESS_HEX+1];
    strncpy(address_hex, ver_rip, LENGHT_VER_RIP_HEX);
    strncpy(address_hex+LENGHT_VER_RIP_HEX, sha256_sha256, LENGHT_ADDRESS_HEX-LENGHT_VER_RIP_HEX);
    address_hex[LENGHT_ADDRESS_HEX] = '\0';
    printf("address_hex: %s\t%d\n", address_hex, strlen(address_hex));

    // to address
    unsigned char address_char[LENGHT_ADDRESS_CHAR];
    hex_to_chars(address_char, LENGHT_ADDRESS_HEX, address_hex);
    unsigned char* b58 = en_base58(address_char, LENGHT_ADDRESS_CHAR);

    int len_address_base58 = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? LENGHT_ADDRESS_BASE58_COMPRESS : LENGHT_ADDRESS_BASE58;
    unsigned char address_base58[len_address_base58+1];
	memset(address_base58, '1', len_address_base58-strlen(b58));
	strncpy(address_base58+len_address_base58-strlen(b58), b58, strlen(b58));
	address_base58[len_address_base58] = '\0';
    printf("address: %s\t%d\n", address_base58, strlen(address_base58));

    return 0;
}

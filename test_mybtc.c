#include <assert.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Automated.h>

#include "mybtc.h"

// 测试套件的进场、退场函数
int suite_success_init(void) 
{
	printf("In suite_success_init()\n");
	return 0;
}
int suite_success_clean(void) 
{
	printf("In suite_success_clean()\n");
	return 0;
}

// 测试函数 random_chars
void test_random_chars(void)
{
	int n = 10;
	char words[] = "0123456789ABCDEF";
	char out[n+1];
	random_chars(out, n, words);
	out[n] = '\0';
	CU_ASSERT(strlen(out) == n);		//检查长度
}

void test_random_chars_len(void)
{
	unsigned char words[] = "0123456789ABCDEF";
	for (int i=1; i<=100; i++) {
		unsigned char a[i+1];
		random_chars(a, i, words);
		a[i] = '\0';
		CU_ASSERT(strlen(a) == i);		//检查长度
	}
}

void test_random_chars_strchr(void)
{
	unsigned char words[] = "0123456789ABCDEF";
	for (int i=1; i<=100; i++) {
		unsigned char a[i+1];
		random_chars(a, i, words);
		a[i] = '\0';
		for (int j=0; j<i; j++) {
			CU_ASSERT(strchr(words, a[j]) != 0);
		}
	}
}

// 测试函数 char2hex
void test_char2hex(void)
{
	unsigned char a[] = "12345abcdeABCDEOPQ";
	unsigned char* h[] = {"31", "32", "33", "34", "35", "61", "62", "63", "64", "65", "41", "42", "43", "44", "45", "4F", "50", "51"};
	for (int i=0; i<strlen(a); i++) {
		unsigned char b[2+1];
		char2hex(b, a[i]);
		b[2] = '\0';
		CU_ASSERT(strcmp(b, h[i]) == 0);
	}
}

// 测试函数 char2bit
void test_char2bit(void)
{
	unsigned char a[] = "12345abcdeABCDEOPQ";
	unsigned char* h[] = {"00110001", "00110010", "00110011", "00110100", "00110101", "01100001", "01100010", "01100011", "01100100", "01100101", "01000001", "01000010", "01000011", "01000100", "01000101", "01001111", "01010000", "01010001"};
	for (int i=0; i<strlen(a); i++) {
		unsigned char b[8+1];
		char2bit(b, a[i]);
		b[8] = '\0';
		CU_ASSERT(strcmp(b, h[i]) == 0);
	}
}

// 测试函数 chars_to_hex
void test_chars_to_hex(void)
{
	char* a[] = {"30E786", "CC89C1", "N0LM8J", "49Q4MBL", "49Q4MBLBO"};
	char* h[] = {"333045373836", "434338394331", "4E304C4D384A", "343951344D424C", "343951344D424C424F"};
	// 下面涉及指针数组的长度计算
	for (int i=0; i<sizeof(a)/sizeof(char*); i++) {
		int lenght = strlen(a[i]);
		char b[2*lenght+1];
		chars_to_hex(b, lenght, a[i]);
		b[2*lenght] = '\0';
		CU_ASSERT(strcmp(b, h[i]) == 0);
	}
}

// 测试函数 hex_to_chars
void test_hex_to_chars(void)
{
	int lenght;
	char* out[] = {"30E786", "CC89C1", "N0LM8J", "49Q4MBL", "49Q4MBLBO"};
	char* in[] = {"333045373836", "434338394331", "4E304C4D384A", "343951344D424C", "343951344D424C424F"};
	// 下面涉及指针数组的长度计算
	for (int i=0; i<sizeof(in)/sizeof(char*); i++) {
	// for (int i=0; i<1; i++) {
		lenght = strlen(in[i]);
		char c[lenght/2 + 1];
		hex_to_chars(c, lenght, in[i]);
		c[lenght/2] = '\0';
		CU_ASSERT(strcasecmp(c, out[i]) == 0);
	}
}

// 测试 sha256
void test_sha256()
{
	int lenght;
	unsigned char* in_char[] = {"hello", "world", "sldfjsldf_2937"};
	unsigned char* out[] = {"2cf24dba5fb0a30e26e83b2ac5b9e29e1b161e5c1fa7425e73043362938b9824", 
							"486ea46224d1bb4fb680f34f7c9ad96a8f24ec88be73ea8e5a6c65260e9cb8a7", 
							"c56efc8d1aeb49a2e910eb4b9d5a5a2cc734bbe8a5bfc356210e6597f39d8049"};
	for (int i=0; i<sizeof(in_char)/sizeof(char*); i++) {
		lenght = strlen(in_char[i]);
		unsigned char sha[65];
		sha256(sha, lenght, in_char[i]);
		sha[64] = '\0';
		CU_ASSERT(strcasecmp(sha, out[i]) == 0);
	}
}

// 测试 ripemd160
void test_ripemd160()
{
	int lenght;
	unsigned char* in_char[] = {"hello world.", "haha_jklml_77", "sldfjl_83746"};
	unsigned char* out[] = {"f97442952c8a5d33fded032b83458d030a017e2d", 
							"1cea883662cc549d9b67e630c09fa0589820e07b", 
							"76868d906111fd1de9a8e5b32392b8778f13ce7a"};
	for (int i=0; i<sizeof(in_char)/sizeof(char*); i++) {
		lenght = strlen(in_char[i]);
		unsigned char md[41];
		ripemd160(md, lenght, in_char[i]);
		md[40] = '\0';
		CU_ASSERT(strcasecmp(md, out[i]) == 0);
	}
}

// 测试 base58
void test_en_base58()
{
	int lenght;
	unsigned char* in_char[] = {"helloabc", "world_2876_oo", "000111_skdjfl_098"};
	unsigned char* out[] = {"JTmsyNwduE2", 
							"Awz8bhfeJjV97gZX5Y", 
							"TGKrhGFz2pRRVWirurMy8hD"};
	for (int i=0; i<sizeof(in_char)/sizeof(char*); i++) {
		lenght = strlen(in_char[i]);
		unsigned char* en = en_base58(in_char[i], lenght);
		CU_ASSERT(strcasecmp(en, out[i]) == 0);
	}

	// for real test:
	unsigned char address_hex[] = "00010966776006953D5567439E5E39F86A0D273BEED61967F6";
	unsigned char address_char[LENGHT_ADDRESS_CHAR+1];
	unsigned char address_right[] = "16UwLL9Risc3QfPqBUvKofHmBQ7wMtjvM";
	hex_to_chars(address_char, LENGHT_ADDRESS_HEX, address_hex);
	address_char[LENGHT_ADDRESS_CHAR] = '\0';
	// print_hex_dec_char(address_char, LENGHT_ADDRESS_HEX_CHAR);
	unsigned char *a = en_base58(address_char, LENGHT_ADDRESS_CHAR);
	// print_hex_dec_char(a, strlen(a));

	unsigned char address[LENGHT_ADDRESS_BASE58+1];
	memset(address, '1', LENGHT_ADDRESS_BASE58-strlen(a));
	strncpy(address+LENGHT_ADDRESS_BASE58-strlen(a), a, strlen(a));
	address[LENGHT_ADDRESS_BASE58] = '\0';
	// print_hex_dec_char(address, LENGHT_ADDRESS_BASE58);

	CU_ASSERT(strcasecmp(address, address_right) == 0);
}

// 测试如何生成btc地址：=========================================================================== Start:
// 测试第二步：private -> pub
void test_private_to_pub()
{
	unsigned char* private_key = "18e14a7b6a307f426a94f8114701e7c8e774e7f9a47e2c2035db29a206321725";
	unsigned char* out = "0450863AD64A87AE8A2FE83C1AF1A8403CB53F53E486D8511DAD8A04887E5B23522CD470243453A299FA9E77237716103ABC11A1DF38855ED6F2EE187E9C582BA6";
	unsigned char* out2 = "0250863AD64A87AE8A2FE83C1AF1A8403CB53F53E486D8511DAD8A04887E5B2352";

	// 非压缩公钥
	int len_pubkey = LENGHT_PUBLIC_CHAR;
    unsigned char pub_hex[len_pubkey*2+1];
    private_to_pub(pub_hex, len_pubkey, private_key);
    pub_hex[2*len_pubkey] = '\0';
	CU_ASSERT(strcasecmp(pub_hex, out) == 0);

	// 压缩公钥
	len_pubkey = LENGHT_PUBLIC_CHAR_COMPRESS;
	unsigned char pub_hex2[len_pubkey*2+1];
    private_to_pub(pub_hex2, len_pubkey, private_key);
    pub_hex2[2*len_pubkey] = '\0';
	CU_ASSERT(strcasecmp(pub_hex2, out2) == 0);
}

// 测试第三步：pub -> sha256
void test_pub_sha256()
{
	unsigned char in_hex[] = "0450863AD64A87AE8A2FE83C1AF1A8403CB53F53E486D8511DAD8A04887E5B23522CD470243453A299FA9E77237716103ABC11A1DF38855ED6F2EE187E9C582BA6";
	unsigned char out_hex_rigth[] = "600FFE422B4E00731A59557A5CCA46CC183944191006324A447BDB2D98D4B408";
	unsigned char in_char[LENGHT_PUBLIC_CHAR+1];
	hex_to_chars(in_char, LENGHT_PUBLIC_HEX, in_hex);
	in_char[LENGHT_PUBLIC_CHAR] = '\0';
	
	unsigned char out_hex[LENGHT_SHA256_HEX+1];
	sha256(out_hex, LENGHT_PUBLIC_CHAR, in_char);
	out_hex[LENGHT_SHA256_HEX] = '\0';	

	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}

// 测试第四步： sha256 -> ripemd160
void test_sha256_ripemd160() {
	unsigned char in_hex[] = "600FFE422B4E00731A59557A5CCA46CC183944191006324A447BDB2D98D4B408";
	unsigned char out_hex_rigth[] = "010966776006953D5567439E5E39F86A0D273BEE";

    unsigned char in_char[LENGHT_SHA256_CHAR+1];
	hex_to_chars(in_char, LENGHT_SHA256_HEX, in_hex);
	in_char[LENGHT_SHA256_CHAR] = '\0';

    unsigned char out_hex[LENGHT_RIPEMD160_HEX+1];    
    ripemd160(out_hex, LENGHT_SHA256_CHAR, in_char);
    out_hex[LENGHT_RIPEMD160_HEX] = '\0';

	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}	

// 测试第五步：加入版本号0x00
void test_ripemd160_add_version() {
	unsigned char in_hex[] = "010966776006953D5567439E5E39F86A0D273BEE";
	unsigned char out_hex_rigth[] = "00010966776006953D5567439E5E39F86A0D273BEE";

	unsigned char out_hex[LENGHT_VER_RIP_HEX+1];
	strncpy(out_hex, "00", 2);
	strncpy(out_hex+2, in_hex, LENGHT_RIPEMD160_HEX);
	out_hex[LENGHT_VER_RIP_HEX] = '\0';
	
	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}

// 测试第六步：上一步 -> sha256
void test_version_sha256() {
	unsigned char in_hex[] = "00010966776006953D5567439E5E39F86A0D273BEE";
	unsigned char out_hex_rigth[] = "445C7A8007A93D8733188288BB320A8FE2DEBD2AE1B47F0F50BC10BAE845C094";	

	unsigned char in_char[LENGHT_VER_RIP_CHAR+1];
	hex_to_chars(in_char, LENGHT_VER_RIP_HEX, in_hex);
	in_char[LENGHT_VER_RIP_CHAR] = '\0';

	unsigned char out_hex[LENGHT_SHA256_HEX+1];
	sha256(out_hex, LENGHT_VER_RIP_CHAR, in_char);
	out_hex[LENGHT_SHA256_HEX] = '\0';
	
	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}

// 测试第七步：再次sha256，即上一步 -> sha256
void test_sha256_sha256() {
	unsigned char in_hex[] = "445C7A8007A93D8733188288BB320A8FE2DEBD2AE1B47F0F50BC10BAE845C094";
	unsigned char out_hex_rigth[] = "D61967F63C7DD183914A4AE452C9F6AD5D462CE3D277798075B107615C1A8A30";	

	unsigned char in_char[LENGHT_SHA256_CHAR+1];
	hex_to_chars(in_char, LENGHT_SHA256_HEX, in_hex);
	in_char[LENGHT_SHA256_CHAR] = '\0';

	unsigned char out_hex[LENGHT_SHA256_HEX+1];
	sha256(out_hex, LENGHT_SHA256_CHAR, in_char);
	out_hex[LENGHT_SHA256_HEX] = '\0';
	
	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}

// 测试第八步：hex形态的btc地址
void test_address_hex() {
	unsigned char in_hex[] = "D61967F63C7DD183914A4AE452C9F6AD5D462CE3D277798075B107615C1A8A30";
	unsigned char in_hex_step5[] = "00010966776006953D5567439E5E39F86A0D273BEE";
	unsigned char out_hex_rigth[] = "00010966776006953D5567439E5E39F86A0D273BEED61967F6";	

    // to address_hex
    unsigned char out_hex[LENGHT_ADDRESS_HEX+1];
    strncpy(out_hex, in_hex_step5, LENGHT_VER_RIP_HEX);
    strncpy(out_hex+LENGHT_VER_RIP_HEX, in_hex, LENGHT_ADDRESS_HEX-LENGHT_VER_RIP_HEX);
    out_hex[LENGHT_ADDRESS_HEX] = '\0';
    
	CU_ASSERT(strcasecmp(out_hex, out_hex_rigth) == 0);
}

// 测试第九步：最后一步：常见的比特币地址形态
void test_address_base58() {
	unsigned char in_hex[] = "00010966776006953D5567439E5E39F86A0D273BEED61967F6";
	unsigned char out_hex_rigth[] = "16UwLL9Risc3QfPqBUvKofHmBQ7wMtjvM";	
	int publen = LENGHT_PUBLIC_CHAR; // 非压缩公钥

	unsigned char address_char[LENGHT_ADDRESS_CHAR];
    hex_to_chars(address_char, LENGHT_ADDRESS_HEX, in_hex);
    unsigned char* b58 = en_base58(address_char, LENGHT_ADDRESS_CHAR);

    int len_address_base58 = (publen == LENGHT_PUBLIC_CHAR_COMPRESS) ? LENGHT_ADDRESS_BASE58_COMPRESS : LENGHT_ADDRESS_BASE58;
    unsigned char address_base58[len_address_base58+1];
	memset(address_base58, '1', len_address_base58-strlen(b58));
	strncpy(address_base58+len_address_base58-strlen(b58), b58, strlen(b58));
	address_base58[len_address_base58] = '\0';

    CU_ASSERT(strcasecmp(address_base58, out_hex_rigth) == 0);
}
// 测试如何生成btc地址：=========================================================================== End.


int main(int argc, char *argv[])
{
	// 初始化测试系统
	if (CU_initialize_registry() != CUE_SUCCESS) {
		fprintf(stderr, "call CU_initialize_registry() error.");
		exit(EXIT_FAILURE);
	}

	// 测试方法2  ok
	CU_TestInfo test_array1[] = {
		{"test_random_chars", test_random_chars},					// 测试函数 random_chars
		{"test_random_chars_len", test_random_chars_len},			// 测试函数 random_chars
		{"test_random_chars_strchr", test_random_chars_strchr},		// 测试函数 random_chars
		{"test_sha256", test_sha256},
		{"test_ripemd160", test_ripemd160},
		{"test_en_base58", test_en_base58},
		{"test_char2bit", test_char2bit},
		{"test_char2hex", test_char2hex},
		{"test_chars_to_hex", test_chars_to_hex},
		{"test_hex_to_chars", test_hex_to_chars},
		{ "test_private_to_pub", test_private_to_pub },					// 第2步
		{ "test_pub_sha256", test_pub_sha256 }, 						// 第3步
		{"test_sha256_ripemd160", test_sha256_ripemd160},				// 第4步
		{"test_ripemd160_add_version", test_ripemd160_add_version},		// 第5步
		{"test_version_sha256", test_version_sha256},					// 第6步
		{"test_sha256_sha256", test_sha256_sha256},						// 第7步
		{"test_address_hex", test_address_hex},							// 第8步
		{"test_address_base58", test_address_base58},					// 第9步
		CU_TEST_INFO_NULL,
	};

	CU_SuiteInfo suites[] = {
		{"suitename1", suite_success_init, suite_success_clean, NULL, NULL, test_array1 },
		CU_SUITE_INFO_NULL,
	};

	// 注册测试系统
	CU_register_suites(suites);

	// 启动测试
	CU_basic_run_tests();

	// 清理测试系统
	CU_cleanup_registry();	

	return 0;
}


/* 测试方法1：ok
	// 创建测试套件
	CU_pSuite s = CU_add_suite("suite_maxi", suite_success_init, suite_success_clean);
	if (s == NULL) {
		fprintf(stderr, "call CU_add_suite() error.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 将测试函数加入测试套件中
	CU_pTest t = CU_add_test(s, "test_random_chars", test_random_chars);
	if (t == NULL) {
		fprintf(stderr, "call CU_add_test().", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}
	CU_pTest t2 = CU_add_test(s, "test_random_chars_len", test_random_chars_len);
	if (t2 == NULL) {
		fprintf(stderr, "call CU_add_test() 2.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}
	CU_pTest t3 = CU_add_test(s, "test_random_chars_strchr", test_random_chars_strchr);
	if (t3 == NULL) {
		fprintf(stderr, "call CU_add_test() 3.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 测试 char2hex
	CU_pTest t4 = CU_add_test(s, "test_char2hex", test_char2hex);
	if (t4 == NULL) {
		fprintf(stderr, "call CU_add_test() 4.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 测试 char2bit
	CU_pTest t5 = CU_add_test(s, "test_char2bit", test_char2bit);
	if (t5 == NULL) {
		fprintf(stderr, "call CU_add_test() 5.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 测试 test_char_to_hex
	CU_pTest t6 = CU_add_test(s, "tests_char_to_hex", test_chars_to_hex);
	if (t6 == NULL) {
		fprintf(stderr, "call CU_add_test() 6.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}	

	// 测试 test_hex_to_chars
	CU_pTest t7 = CU_add_test(s, "test_hex_to_chars", test_hex_to_chars);
	if (t7 == NULL) {
		fprintf(stderr, "call CU_add_test() 7.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}	
	
	// 测试 test_sha256
	CU_pTest t8 = CU_add_test(s, "test_sha256", test_sha256);
	if (t8 == NULL) {
		fprintf(stderr, "call CU_add_test() 8.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}	

	// 测试 test_ripemd160
	CU_pTest t9 = CU_add_test(s, "test_ripemd160", test_ripemd160);
	if (t9 == NULL) {
		fprintf(stderr, "call CU_add_test() 9.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 测试 test_en_base58
	CU_pTest t10 = CU_add_test(s, "test_en_base58", test_en_base58);
	if (t10 == NULL) {
		fprintf(stderr, "call CU_add_test() 10.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}

	// 测试 test_private_to_pub
	CU_pTest t11 = CU_add_test(s, "test_private_to_pub", test_private_to_pub);
	if (t11 == NULL) {
		fprintf(stderr, "call CU_add_test() 11.", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}
*/
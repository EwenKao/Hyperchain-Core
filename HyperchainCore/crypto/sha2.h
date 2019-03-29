﻿/*Copyright 2016-2019 hyperchain.net (Hyperchain)

Distributed under the MIT software license, see the accompanying
file COPYING or https://opensource.org/licenses/MIT.

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#ifndef _SHA2_H_
#define _SHA2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <openssl/evp.h>


int EncodeBase58(const unsigned char *bytes, int len, unsigned char result[]);
int DecodeBase58(const unsigned char *str, int len, unsigned char *result);

enum class DT :char
{
	md4 = 0,
	md5,
	sha1,
	sha256,
	sha384,
	sha512
};

template<DT arg>
struct Digest
{
	Digest() { init(); }
	~Digest() { EVP_MD_CTX_cleanup(&_mdctx); }

	void AddData(const char *data) 
	{
		size_t len = strlen(data);
		AddData(data, len);
	}

	void AddData(const void *data, size_t len)
	{
		int ret = EVP_DigestUpdate(&_mdctx, data, len);
		if (ret != 1) {
			std::cout << "Error occurs in EVP_DigestUpdate " << __FUNCTION__ << " " << ret;
		}
	}

	std::string getDigest()
	{
		unsigned int md_len = 0;
		int ret = EVP_DigestFinal_ex(&_mdctx, _md_value, &md_len);
		if (ret != 1) {
			std::cout << "Error occurs in EVP_DigestFinal_ex" << __FUNCTION__ << " " << ret;
		}
		return std::string((char*)_md_value, md_len);
	}

	std::string getDigestBase58()
	{
		unsigned int md_len = 0;
		int retdigest = EVP_DigestFinal_ex(&_mdctx, _md_value, &md_len);
		if (retdigest != 1) {
			std::cout << "Error occurs in EVP_DigestFinal_ex" << __FUNCTION__ << " " << retdigest;
		}

		unsigned char ret[256] = {0};

		int len = EncodeBase58(_md_value,md_len,ret);

		return std::string((char*)ret, len);
	}
private:
	void init()
	{
		char algorithm[][16] = {
			"md4","md5","sha1","sha256","sha384","sha512"
		};
		OpenSSL_add_all_digests();
		_md = EVP_get_digestbyname(algorithm[(int)arg]);

		EVP_MD_CTX_init(&_mdctx);
		int ret = EVP_DigestInit_ex(&_mdctx, _md, NULL);
		if (ret != 1) {
			std::cout << "Error occurs in EVP_DigestInit_ex" << __FUNCTION__ << " " << ret;
		}
	}
private:
	const EVP_MD* _md;
	EVP_MD_CTX _mdctx;
	unsigned char _md_value[EVP_MAX_MD_SIZE];
};


void GetSHA256(unsigned char* hash_sha256, const char* str, size_t length);
void displayHash(unsigned char hash[32]);


#endif
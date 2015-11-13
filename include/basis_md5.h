// 2015-11-10
// basis_md5.h
// 
// MD5֧����

#ifndef _BASIS_MD5_H_
#define _BASIS_MD5_H_

#include "basis_define.h"

namespace basis
{
////////////////////////////////////////////////////////////////////////////////
// class MD5
////////////////////////////////////////////////////////////////////////////////
class MD5 
{
public:
	MD5();

	void update(const void *input, uint32 length);
	void update(const string &str);
	
	const uint8* digest();
	string toString();
	void reset();

private:
	void update(const uint8 *input, uint32 length);
	void final();
	void transform(const uint8 block[64]);
	void encode(const uint32* input, uint8* output, uint32 length);
	void decode(const uint8 *input, uint32 *output, uint32 length);
	string bytesToHexString(const uint8 *input, uint32 length);

private:
	uint32 m_state[4];	// state (ABCD) 
	uint32 m_count[2];	// number of bits, modulo 2^64 (low-order word first) 
	uint8 m_buffer[64];	// input buffer 
	uint8 m_digest[16];	// message digest 
};

}using namespace basis;

#endif
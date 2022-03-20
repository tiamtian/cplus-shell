// FunCli.h
#include <iostream>
class AES
{
public:
    AES(char key[]);
    virtual ~AES();

    unsigned char *Cipher(unsigned char *input);
    unsigned char *InvCipher(unsigned char *input);

    void *Cipher(void *input, int length = 0);
    void *InvCipher(void *input, int length);

    void PrintData(unsigned char *buffer, int length);
    void Exec(std::string m_chat);

    char temp[4096];
    
private:
    unsigned char Sbox[256];
    unsigned char InvSbox[256];
    unsigned char w[11][4][4];

    void KeyExpansion(char key[], unsigned char w[][4][4]);
	unsigned char FFmul(unsigned char a, unsigned char b);

	void SubBytes(unsigned char state[][4]);
	void ShiftRows(unsigned char state[][4]);
	void MixColumns(unsigned char state[][4]);
	void AddRoundKey(unsigned char state[][4], unsigned char k[][4]);

	void InvSubBytes(unsigned char state[][4]);
	void InvShiftRows(unsigned char state[][4]);
	void InvMixColumns(unsigned char state[][4]);
};

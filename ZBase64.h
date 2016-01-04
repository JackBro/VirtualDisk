#pragma once

#include <string>

class ZBase64
{
public:
	ZBase64();

	//���� DataByte ��������ݳ���,���ֽ�Ϊ��λ
	static std::string saveData(char const* bytes_to_encode,unsigned int in_len);

	//����  OutByte ��������ݳ���,���ֽ�Ϊ��λ,�벻Ҫͨ������ֵ���� ������ݵĳ���
	static std::string parseData(std::string const& encoded_string);

	static inline bool isData(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}
	
private:
	char* m_Base64_Table;

};
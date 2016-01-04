#pragma once

#include <vector>
#include "Node.h"
#include "Subject.h"

class File : public Node, public Subject
{
public:
	File();
	File(std::string name);

	void copy(File* file);  //��������� copyʹ��
	void write(File* file); //����ļ�copy��һ���ļ���ʹ�ã��ϲ���

	const std::vector<char>& getContent();
	void setContent(std::vector<char> content); //����������·����copyʹ��

	int getSize();

	virtual void deleteNode(bool recursion);

	virtual void show(bool isFolder);

private:
	File(const File&);
	File& operator=(const File&);

private:
	std::vector<char> _content;
};
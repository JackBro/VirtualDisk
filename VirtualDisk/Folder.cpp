#include "Folder.h"
#include "SymbolicLink.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

Folder::Folder():
Node()
{
	char str[20];
	sprintf(str, "new folder%d", _tag++);
	string name(str);

	setName(str);

	_wildCardStr = "*";
}

Folder::Folder(string name):
Node(name)
{
	_wildCardStr = "*";
}

void Folder::addSubFile(Node* node)
{
	//��node����Ϊδ��ӵ��ļ����еĽڵ㣬������ܻ����δ֪�Ĵ���
	_subFile.push_back(node);
}

void Folder::removeSubFile(Node* node)
{
	vector<Node*>::iterator iter = find(_subFile.begin(), _subFile.end(), node);
	if (iter == _subFile.end())
	{
		cout << "���ļ����ļ�����ɾ��" << endl;
	}
	else
	{
		_subFile.erase(iter);
	}
}

void Folder::deleteNode(bool recursion)
{
	vector<Node*>::iterator iter = _subFile.begin();
	if (recursion)
	{
		for (iter; iter != _subFile.end();)
		{
			Folder* folder = dynamic_cast<Folder*>(*iter);
			if (folder == nullptr)
			{
				if (matchWildCard(_wildCardStr.c_str(), (*iter)->getName().c_str()))
				{
					SymbolicLink* link = dynamic_cast<SymbolicLink*>(*iter);
					if (nullptr != link && 2 == link->getType())
					{
						//continue;
					}
					else
					{
						Node* node = (*iter);
						iter = _subFile.erase(iter);
						node->deleteNode(recursion);

						continue;
					}
				}
			}
			else
			{
				folder->setWildCardStr(_wildCardStr);
				folder->deleteNode(recursion);
			}
			iter++;
		}
	}
	else
	{
		for (iter; iter != _subFile.end();)
		{
			if (matchWildCard(_wildCardStr.c_str(), (*iter)->getName().c_str()))
			{
				if (dynamic_cast<Folder*>(*iter) == nullptr)
				{
					(*iter)->deleteNode(recursion);
					iter = _subFile.erase(iter);
					continue;
				}
			}

			++iter;
		}
	}

	setWildCardStr("*");
}

void Folder::show(bool isFolder)
{
	Date date = getDate();
	cout << date._year << "/" << date._month << "/" << date._day << "  " << setw(2) <<date._hour << ":" << setw(2) << date._minute << "    " 
		<< "<DIR>" << "            " << getName() << endl;
}

Node* Folder::containNode( std::string nodeName )
{
	for (int i = 0; i < _subFile.size(); ++i)
	{
		if (_subFile[i]->getName().compare(nodeName) == 0)
		{
			return _subFile[i];
		}
	}

	return nullptr;
}

void Folder::showSubFile(bool recursion, bool isFolder)
{
	//cout << "  " << getCurPath() << "��Ŀ¼" << endl;
	
	if (_wildCardStr.compare("*") != 0) //ͨ���ʱ ������Ч
	{
		recursion = false;
		//isFolder = false;
	}

	bool flag = false;
	for (int i = 0; i < _subFile.size(); ++i)
	{
		if (matchWildCard(_wildCardStr.c_str(), _subFile[i]->getName().c_str()))
		{
			flag = true;
			_subFile[i]->show(isFolder);
		}
	}
	
	if (recursion)
	{
		for (int i = 0; i < _subFile.size(); ++i)
		{
			if (matchWildCard(_wildCardStr.c_str(), _subFile[i]->getName().c_str()))
			{
				Folder* folder = dynamic_cast<Folder*>(_subFile[i]);
				if (nullptr != folder)
				{
					cout << "  " << folder->getCurPath() << "��Ŀ¼" << endl;
				}
				
				_subFile[i]->showSubFile(recursion, isFolder);
			}
		}
	}

	if (!flag)
	{
		cout << "�Ҳ����ļ�" << endl;
	}

	setWildCardStr("*"); //ͨ������ ������Ϊԭֵ
}

std::string Folder::getCurPath()
{
	string str;
	Node* folder;

	folder = this;
	if (this->getParent() == nullptr)
	{
		str.insert(0,"\\");
	}

	while (nullptr != folder->getParent())
	{
		str.insert(0, "\\" + folder->getName());
		folder = folder->getParent();
	}
	str.insert(0, folder->getName());

	return str;
}

void Folder::setWildCardStr(std::string str)
{
	_wildCardStr = str;
}

bool Folder::matchWildCard(const char *pat, const char *str)
{
	const char *s = NULL;
	const char *p = NULL;
	bool star = false;
	bool bBreak = false;
	do
	{
		bBreak = false;
		for (s = str, p = pat; *s; ++s, ++p)
		{
			switch (*p)
			{
			case '?':
				break;
			case '*':
				star = true; //����*ƥ���
				str = s;
				pat = p;
				if (!*++pat)
					return true;
				bBreak = true; //�˳�ѭ��
				break;
			default:
				if (*s != *p)
				{
					if (!star)
						return false;
					str++;
					bBreak = true;
				}
				break;
			}
			if (bBreak) //�˳�ѭ�� ���¿�ʼѭ��
				break;
		}
		if (bBreak == false)
		{
			if (*p == '*')
				++p;
			return (!*p);
		}
	} while (true);
}

std::vector<Node*> Folder::matchNode()
{
	vector<Node*> nodeVec;
	for (int i = 0; i < _subFile.size(); ++i)
	{
		if (matchWildCard(_wildCardStr.c_str(), _subFile[i]->getName().c_str()))
		{
			nodeVec.push_back(_subFile[i]);
		}
	}

	setWildCardStr("*");
	return nodeVec;
}

void Folder::deleteFolder()
{
	vector<Node*>::iterator iter = _subFile.begin();
	for (iter; iter != _subFile.end();)
	{
		Folder* folder = dynamic_cast<Folder*>(*iter);
		if (nullptr != folder)
		{
			iter = _subFile.erase(iter);
			folder->deleteFolder();
		}
		else
		{
			Node* node = *iter;
			iter = _subFile.erase(iter);
			node->deleteNode(false);
		}
	}

	notify();
	delete this;
}

bool Folder::isEmpty()
{
	if (0 != _subFile.size())
	{
		return false;
	}

	return true;
}

std::string Folder::getWildCard()
{
	return _wildCardStr;
}

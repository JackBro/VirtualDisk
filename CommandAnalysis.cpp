#include "CommandAnalysis.h"
#include "Folder.h"

#include <algorithm>

using namespace std;

CommandAnalysis::CommandAnalysis()
{
	_cmdType = -1;
}

CommandAnalysis::~CommandAnalysis()
{

}

void CommandAnalysis::splitCmd(string cmdStr)
{
	_cmdElementVec.clear();

	if (count(cmdStr.begin(), cmdStr.end(), '\"') % 2 != 0)  //���ű���Ϊż��
	{
		return;
	}

	CommandElement element;
	string str;

	int i = 0, j = 0;
	while (i < cmdStr.size() && ' ' == cmdStr[i++])
	{
		j++;
	}
	cmdStr.erase(0, j);  //�Ƴ�����ǰ��Ŀո�

	//���������
	i = 0;
	while(i < cmdStr.size() && ' ' != cmdStr[i] && '.' != cmdStr[i])
	{
		str.push_back(cmdStr[i]);
		i++;
	}
	element.setElementType(CMDTYPE);
	element.setElementStr(str);
	_cmdElementVec.push_back(element);

	//�������
	if (i < cmdStr.size() && ' ' == cmdStr[i])
	{
		i++;
	}
	while (i < cmdStr.size())
	{
		str.clear();
		element.setIsWildCard(false);

		while (i < cmdStr.size() && ' ' != cmdStr[i])
		{
			if ('\"' == cmdStr[i])//˫����
			{
				++i;
				while (i < cmdStr.size() && '\"' != cmdStr[i])
				{
					str.push_back(cmdStr[i]);
					++i;
				}
				++i;
			}
			else
			{
				str.push_back(cmdStr[i]);
				i++;
			}
		}
		if (0 != str.size())
		{
			element.setElementStr(str);
			if (str.compare("/ad") == 0 || str.compare("/s") == 0 || str.compare("/y") == 0)
			{
				element.setElementType(PARATYPE);
			}
			else
			{
				element.setElementType(PATHTYPE);
			}

			if (str.find('*') != string::npos || str.find('?') != string::npos)
			{
				element.setIsWildCard(true);
			}

			_cmdElementVec.push_back(element);
		}

		i++;
	}
}

Folder* CommandAnalysis::splitPath(string path, Folder* rootFolder, Folder* curFolder)
{
	_fileNameVec.clear();

	string name;
	int i = 0;

	//����·��
	if (path.find("c:") != string::npos || path.find("C:") != string::npos)
	{
		i = 2;
		_folder = rootFolder;
	}
	else
	{
		_folder = curFolder;
	}

	while (i < path.size())
	{
		if (path[i] == '\\' || path[i] == '/')
		{
			i++;
		}

		name.clear();
		while (i < path.size() && ('\\' != path[i] && '/' != path[i]))
		{
			name.push_back(path[i]);
			i++;
		}

		if (name.size() != 0)
		{
			_fileNameVec.push_back(name);
		}
	}

	return _folder;
}

std::vector<Node*> CommandAnalysis::pathAnalysis(Folder* rootFolder, Folder* curFolder)
{
	vector<Node*> folderVec;
	Node* node;

	if (_cmdElementVec.size() == 0)//�޲��� ��ʾΪ��ǰ·����
	{
		folderVec.push_back(curFolder);
		return folderVec;
	}

	for (int i = 0; i < _cmdElementVec.size(); ++i)
	{
		if (_cmdElementVec[i].getElementType() == PATHTYPE)
		{
			splitPath(_cmdElementVec[i].getElementStr(), rootFolder, curFolder);
			if (_cmdElementVec[i].getIsWildCard())
			{
				for (int j = 0; j < _fileNameVec.size(); ++j)
				{
					if ((_fileNameVec[j].find('*') != string::npos || _fileNameVec[j].find('?') != string::npos ) && j != _fileNameVec.size() - 1 && _fileNameVec.size() > 1)
					{
						cout << "ϵͳ�Ҳ������ļ�" << endl;
						return folderVec; //ֱ�ӷ���
					}
				}

				pathMatch();
				if (_fileNameVec.size() == 1)
				{
					folderVec.push_back(_folder);
					_folder->setWildCardStr(_fileNameVec[_fileNameVec.size() - 1]);
				}
			}
			else
			{
				Node* node = pathMatch();
				if (nullptr != node)
				{
					folderVec.push_back(node);
				}
				else
				{
					if (_cmdType != 3)
					{
						cout << "ϵͳ�Ҳ������ļ�" << endl;
					}
				}
			}
		}
	}

	return folderVec;
}

int CommandAnalysis::cmdTypeAnalysis(string cmdStr)
{
	splitCmd(cmdStr);
	if (_cmdElementVec.size() > 0)
	{
		string str = _cmdElementVec[0].getElementStr();
		if (0 == str.compare("dir"))
		{
			_cmdType = 0;
		}
		else if (0 == str.compare("cd"))
		{
			//cd
			_cmdType = 1;
		}
		else if (0 == str.compare("md"))
		{
			//md
			_cmdType = 2;
		}
		else if (0 == str.compare("copy"))
		{
			//copy
			_cmdType = 3;
		}
		else if (0 == str.compare("del"))
		{
			//del
			_cmdType = 4;
		}
		else if (0 == str.compare("rd"))
		{
			//rd
			_cmdType = 5;
		}
		else if (0 == str.compare("ren"))
		{
			//ren
			_cmdType = 6;
		}
		else if (0 == str.compare("move"))
		{
			//move
			_cmdType = 7;
		}
		else if (0 == str.compare("load"))
		{
			//load
			_cmdType = 8;
		}
		else if (0 == str.compare("mklink"))
		{
			//mklink
			_cmdType = 9;
		}
		else if (0 == str.compare("save"))
		{
			//save
			_cmdType = 10;
		}
		else if(0 == str.compare("exit"))
		{
			//exit
			_cmdType = 11;
		}
		else
		{
			_cmdType = -1;
		}

		_cmdElementVec.erase(_cmdElementVec.begin());
	}
	else
	{
		_cmdType = -1;
	}

	return _cmdType;
}

vector<string> CommandAnalysis::paraAnalysis()
{
	vector<std::string> vec;
	string str;

	vector<CommandElement>::iterator iter = _cmdElementVec.begin();
	for (iter; iter != _cmdElementVec.end();)
	{
		if(iter->getElementType() == PARATYPE)
		{
			str = iter->getElementStr();
			vec.push_back(str);

			iter = _cmdElementVec.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return vec;
}

Node* CommandAnalysis::pathMatch()
{
	Node* node;

	if (_fileNameVec.size() == 0) //c:
	{
		return _folder;
	}

	vector<string>::iterator iter = _fileNameVec.begin();
	for (iter; iter != _fileNameVec.end();)
	{
		if ((*iter).compare("..") == 0)
		{
			Folder* folder = dynamic_cast<Folder*>(_folder->getParent());
			if (nullptr != folder)
			{
				_folder = folder;
			}

			iter = _fileNameVec.erase(iter);
			if (0 == _fileNameVec.size())
			{
				return _folder;
			}
			else
			{
				continue;
			}
		}

		node = _folder->containNode(*iter);
		if (iter == --_fileNameVec.end() && node != nullptr) //�����������һ���ַ���ʱ���������ļ������ļ��� ֻҪ��һ���ļ������� �ͷ���true
		{
			return node;//node ��ʾ·�������һ���ڵ�
		}

		Folder* folder = dynamic_cast<Folder*>(node);
		if (folder == nullptr)
		{
			break;
		}

		_folder = folder;  //_folder��ʾ��������ʱ�� ǰһ���ļ��У��ɹ�ʱ����ʾ �����ڶ����ļ��� ��ʧ��ʱ����ʾ������ڵ��ǰһ���ļ��У�
		iter = _fileNameVec.erase(iter);
	}

	return nullptr;
}

vector<CommandElement>& CommandAnalysis::getCmdElementVec()
{
	return _cmdElementVec;
}

std::vector<std::string> CommandAnalysis::getFileNameVec()
{
	return _fileNameVec;
}

Folder* CommandAnalysis::getLastFolder()
{
	return _folder;
}

#include "CdCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

CdCommand::CdCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{
	_cmdAnalysis = cmdAnalysis;
}

CdCommand::~CdCommand()
{

}

void CdCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElementVec = _cmdAnalysis->getCmdElementVec();
	if (1 < cmdElementVec.size())
	{
		cout << "ϵͳ�Ҳ���ָ��·����" << endl;
	}
	else if (cmdElementVec.size() == 0) //�޲�����ʾ��ǰ·��
	{
		cout << _curFolder->getCurPath() << endl;
	}
	else if (cmdElementVec[0].getElementStr().compare("/") == 0 || cmdElementVec[0].getElementStr().compare("\\") == 0) //���ظ�·��
	{
		while(_curFolder->getParent() != nullptr)
		{
			_curFolder = dynamic_cast<Folder*>(_curFolder->getParent());
		}
	}
	else if (cmdElementVec[0].getElementStr().compare("..") == 0)
	{
		if (_curFolder->getParent() != nullptr)
		{
			_curFolder = dynamic_cast<Folder*>(_curFolder->getParent());
		}
	}
	else
	{
		vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
		if (nodeVec.size() != 1)  //cd ֻ����һ������
		{
			//cout << "ϵͳ�Ҳ������ļ���" << endl;
			return;
		}

		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		if (nullptr == folder)
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[0]);
			if (nullptr != link && 2 == link->getType())
			{
				Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
				while (nullptr == temp)
				{
					SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
					temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
					link = linkTemp;
				}

				_curFolder = temp;
			}
			else
			{
				cout << "��Ч�Ĳ���" << endl;
			}
			return;
		}

		if (cmdElementVec[0].getIsWildCard()) //�Ƿ���Ҫͨ��
		{
			vector<Node*>vec = folder->matchNode();  //�������з���ͨ���node�ڵ�
			for (int i = 0; i < vec.size(); ++i)
			{
				Folder* folder = dynamic_cast<Folder*>(vec[i]);
				if (nullptr != folder)
				{
					_curFolder = folder;  //�ҵ�һ����ok
					return;
				}
			}

			cout << "ϵͳ�Ҳ���ָ��·��" << endl;
		}
		else
		{
			_curFolder = folder;
		}

	}
}

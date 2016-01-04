#include "DelCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

DelCommand::DelCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

DelCommand::~DelCommand()
{

}

void DelCommand::execute( std::string str )
{
	_cmdStr = str;
	_tag = 0;  //���ݹ�ɾ��

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() == 0)
	{
		cout << "��Ч������" << endl;
		return;
	}

	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (strVec.size() > 1 || (strVec.size() == 1 && strVec[0].compare("/s") != 0))//��������1�����߲���Ϊ/y
	{
		cout << "��Ч�Ĳ���" << endl;
		return;
	}
	else if (strVec.size() != 0)
	{
		_tag = 1;   //�ݹ�ɾ��
	}

	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	for (int i = 0; i < nodeVec.size(); ++i)
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[i]);
		if (nullptr == folder)//����Ҫɾ������ļ�
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[i]);
			if (nullptr != link && link->getType() == 2)
			{
				Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
				while (nullptr == temp)
				{
					SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
					temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
					link = linkTemp;
				}

				deleteFolder(temp);
				continue;
			}

			deleteNode(nodeVec[i]);
		}
		else  //ɾ���ļ����µ��ļ�
		{
			if (folder->getWildCard().compare("*") != 0)
			{
				vector<Node*>nodes = folder->matchNode();
				for (int j = 0; j < nodes.size(); ++j)
				{
					Folder* temp = dynamic_cast<Folder*>(nodes[j]);
					if (nullptr == temp)
					{
						SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodes[j]);
						if (nullptr != link && link->getType() == 2)
						{
							continue;
						}
		
						deleteNode(nodes[j]);
					}
				}

				folder->setWildCardStr("*");
			}
			else
			{
				deleteFolder(folder);
			}
		}
	}
}

void DelCommand::deleteNode( Node* file )
{
	Folder* parentFolder = dynamic_cast<Folder*>(file->getParent());
	parentFolder->setWildCardStr(file->getName());
	parentFolder->deleteNode(_tag);
}

void DelCommand::deleteFolder( Folder* folder )
{
	string ch;
	cout << folder->getCurPath() << "\\*���Ƿ�ȷ��(y/n)? ";
	getline(cin, ch);

	if (ch.compare("y") == 0)
	{
		folder->deleteNode(_tag);
	}
}




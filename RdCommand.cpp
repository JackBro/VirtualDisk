#include "RdCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

RdCommand::RdCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

RdCommand::~RdCommand()
{

}

void RdCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElementVec = _cmdAnalysis->getCmdElementVec();
	if (0 == cmdElementVec.size())
	{
		cout << "��Ч������" << endl;
		return;
	}

	int tag = 0; //ֻɾ����Ŀ¼
	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (strVec.size() > 1 || (strVec.size() == 1 && strVec[0].compare("/s") != 0))//��������1�����߲���Ϊ/y
	{
		cout << "��Ч�Ĳ���" << endl;
		return;
	}
	else if (strVec.size() != 0)
	{
		tag = 1;   //ɾ��Ŀ¼�µ������ļ�
	}

	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	for (int i = 0; i < nodeVec.size(); ++i)
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[i]);
		if (folder == nullptr)
		{
			SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[i]);
			if (nullptr != link && link->getType() == 2)  //ֱ���Ƴ�
			{
				Folder* temp = dynamic_cast<Folder*>(link->getParent());
				temp->removeSubFile(link);
				link->deleteNode(false);
			}
			else
			{
				cout << "ϵͳ�Ҳ������ļ�" << endl;
			}

			continue;
		}
		else
		{
			if (cmdElementVec[i].getIsWildCard())
			{
				cout << "ϵͳ�Ҳ������ļ�" << endl;
				folder->setWildCardStr("*");
				continue;
			}

			if (!folder->isEmpty() && 0 == tag)
			{
				cout << "Ŀ¼���ǿյ�" << endl;
				folder->setWildCardStr("*");
				return;
			}
			//ֱ��ɾ��
			Folder* parent = dynamic_cast<Folder*>(folder->getParent());
			if (nullptr == parent)
			{
				cout << "����ɾ���̷�" << endl;
			}
			else
			{
				parent->removeSubFile(folder);
			}

			folder->deleteFolder();
		}
	}
}

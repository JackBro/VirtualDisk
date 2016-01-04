#include "MoveCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

using namespace std;

MoveCommand::MoveCommand( CommandAnalysis* cmdAnalysis ):
Command(cmdAnalysis)
{

}

MoveCommand::~MoveCommand()
{

}

void MoveCommand::execute( std::string str )
{
	_cmdStr = str;

	_tag = 0;//ͬ���ļ����ļ��в�����
	vector<string> strVec = _cmdAnalysis->paraAnalysis();
	if (2 <= strVec.size() || 1 == strVec.size() && strVec[0].compare("/y") != 0)
	{
		cout << "�����ʽ����ȷ" << endl;
		return;
	}
	else if (1 == strVec.size() && strVec[0].compare("/y") == 0)
	{
		_tag = 1; //ͬ���ļ�����
	}

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (2 != cmdElement.size() || cmdElement[1].getIsWildCard())
	{
		cout << "�����ʽ����ȷ" << endl;
		return;
	}

	vector<Node*> dealNodeVec;
	vector<Node*> nodeVec = _cmdAnalysis->pathAnalysis(_rootFolder, _curFolder);
	if (2 != nodeVec.size())
	{
		return;
	}

	if (cmdElement[0].getIsWildCard())
	{
		Folder* folder = dynamic_cast<Folder*>(nodeVec[0]);
		dealNodeVec = folder->matchNode();
	}
	else
	{
		dealNodeVec.push_back(nodeVec[0]);
	}

	Folder* tarFolder = dynamic_cast<Folder*>(nodeVec[1]);
	if (dealNodeVec.size() == 0)
	{
		cout << "�Ҳ����ļ�" << endl;
		return;
	}
	else if (nullptr == tarFolder && dealNodeVec.size() > 1)  //����ļ������ƶ��������ļ���
	{
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[1]);
		if (link != nullptr && 2 == link->getType())
		{
			Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
				temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
				link = linkTemp;
			}
			
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], temp, true);
			}
			return;
		}

		cout << "�޷�������ļ��ƶ��������ļ���" << endl;
		return;
	}
	else if (nullptr == tarFolder && dealNodeVec.size() == 1)  //�����ļ��ƶ��������ļ��� ���� ����Դ�ļ���
	{
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(nodeVec[1]);
		if (link != nullptr && 2 == link->getType())
		{
			Folder* temp = dynamic_cast<Folder*>(link->getLinkNode());
			while (nullptr == temp)
			{
				SymbolicLink* linkTemp = dynamic_cast<SymbolicLink*>(link->getLinkNode());
				temp = dynamic_cast<Folder*>(linkTemp->getLinkNode());
				link = linkTemp;
			}

			if (cmdElement[0].getIsWildCard())
			{
				moveNode(dealNodeVec[0], temp, true);
			}
			else
			{
				moveNode(dealNodeVec[0], temp, false);
			}
		}
		else if (link == nullptr)
		{
			Folder* temp = dynamic_cast<Folder*>(nodeVec[1]->getParent());
			if (cmdElement[0].getIsWildCard())
			{
				moveNode(dealNodeVec[0], temp, true);
			}
			else
			{
				moveNode(dealNodeVec[0], temp, false);
			}
		}
		else
		{
			cout << "�޷��ƶ����ļ�" << endl;
		}		
	}
	else if(nullptr != tarFolder)  //�ƶ����ļ����У�ֻ��֧���ļ�ͨ����� �ļ���ͨ�����֧�֣�
	{
		if (cmdElement[0].getIsWildCard())
		{
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], tarFolder, true);
			}
		}
		else
		{
			for (int i = 0; i < dealNodeVec.size(); ++i)
			{
				moveNode(dealNodeVec[i], tarFolder, false);
			}
		}
	}
}

void MoveCommand::moveNode( Node* node, Folder* folder, bool isWildCard )
{
	if (true == isWildCard)
	{
		if (dynamic_cast<Folder*>(node) != nullptr)
		{
			return;
		}
		SymbolicLink* link = dynamic_cast<SymbolicLink*>(node);
		if (nullptr != link && 2 == link->getType())
		{
			return;
		}
	}
	
	Node* temp = folder->containNode(node->getName());
	if (nullptr == temp)
	{
		node->setParent(folder);
	}
	else
	{
		if (_tag == 0)
		{
			cout << "�����ƶ����ļ�" << endl;
			return;
		}

		folder->setWildCardStr(node->getName());
		folder->deleteNode(false);

		node->setParent(folder);
	}
}

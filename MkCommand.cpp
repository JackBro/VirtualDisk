#include "MkCommand.h"
#include "CommandAnalysis.h"
#include "Folder.h"
#include "SymbolicLink.h"

#include "windows.h"

#include <string>

using namespace std;

MkCommand::MkCommand(CommandAnalysis* cmdAnalysis):
Command(cmdAnalysis)
{

}

MkCommand::~MkCommand()
{

}

void MkCommand::execute( std::string str )
{
	_cmdStr = str;

	vector<CommandElement>& cmdElement = _cmdAnalysis->getCmdElementVec();
	if (cmdElement.size() != 2)
	{
		cout << "�����﷨����ȷ" << endl;
		return;
	}

	Folder* symStartFolder = _cmdAnalysis->splitPath(cmdElement[0].getElementStr(), _rootFolder, _curFolder);
	Node* node = _cmdAnalysis->pathMatch();
	Folder* parentFolder = _cmdAnalysis->getLastFolder();
	vector<string> symPathVec = _cmdAnalysis->getFileNameVec();
	if (nullptr != node)
	{
		cout << "���ļ��Ѿ�����ʱ���޷��������ļ�" << endl;
		return;
	}
	else
	{
		if (1 != symPathVec.size())
		{
			cout << "ϵͳ�Ҳ���·��" << endl;
			return;
		}
	}

	string path;
	Folder* srcStartFolder = _cmdAnalysis->splitPath(cmdElement[1].getElementStr(), _rootFolder, _curFolder);
	vector<string> srcPathVec = _cmdAnalysis->getFileNameVec();
	Node* srcNode = _cmdAnalysis->pathMatch();
	if (nullptr == srcNode)
	{
		cout << "Դ�ļ�������" << endl;
		return;
	}
	
	if (srcStartFolder->getName().compare("c:") == 0)
	{
		path = cmdElement[1].getElementStr();
	}
	else
	{
		path = srcStartFolder->getCurPath();
		for (int i = 0; i < srcPathVec.size(); ++i)
		{
			if (i != 0)
			{
				srcPathVec[i].insert(0, "\\");
			}
			path.append(srcPathVec[i]);
		}
	}
	
	SymbolicLink* link = new SymbolicLink(symPathVec[0], path, srcNode);

	SYSTEMTIME st = {0};
	GetLocalTime(&st);

	Date date(st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
	link->setDate(date);
	link->setParent(parentFolder);

	Subject* subject = dynamic_cast<Subject*>(srcNode);
	subject->addObserver(link);
}

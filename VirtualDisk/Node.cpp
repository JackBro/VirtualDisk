#include "Node.h"
#include "Folder.h"
#include "File.h"
#include "Subject.h"
#include <iostream>

using namespace std;

int Node::_tag = 0;

Node::Node()
{
	_parent = nullptr;
}

Node::Node(string name)
{
	setName(name);
	_parent = nullptr;
}

Node::~Node()
{

}

void Node::setDate( Date date )
{
	_date = date;
}

Date Node::getDate()
{
	return _date;
}

void Node::setName( std::string name )
{
	if (_name.compare(name) == 0)
	{
		return;
	}
	_name = name;
	
	Subject* subject = dynamic_cast<Subject*>(this);
	if (nullptr != subject)
	{
		subject->notify();
	}
}

std::string Node::getName()
{
	return _name;
}

void Node::setParent( Node* node )
{
	if (node == _parent)
	{
		return;
	}

	Folder* folder = dynamic_cast<Folder*>(node);
	if (folder == nullptr)
	{
		//cout << "ֻ���ļ��в�������ļ�" << endl;
		_parent = nullptr;

		return;
	}

	if (_parent) //��ֻ֤��һ���ļ����ܹ������ļ�
	{
		_parent->removeSubFile(this);
	}

	folder->addSubFile(this);
	_parent = node;

	Subject* subject = dynamic_cast<Subject*>(this);
	if (nullptr != subject)
	{
		subject->notify();
	}
}

Node* Node::getParent()
{
	return _parent;
}

void Node::addSubFile( Node* node )
{
	cout << "���ļ��У�����������ļ�" << endl;
}

void Node::removeSubFile( Node* node )
{
	cout << "���ļ��У�����ɾ�����ļ�" << endl;
}

void Node::show(bool isFolder)
{
	//cout << _date._year + "//" +_date._month + "//" + _date._day + "  " + _date._hour + ":" + _date._minute + "    " +	
}

Node* Node::containNode( string nodeName )
{
	return nullptr;
}

void Node::showSubFile(bool recursion, bool isFolder)
{
	//this->show(isFolder);
}

void Node::write( File* file )
{
	cout << "�ܾ�����" << endl;
}

void Node::copy( File* file )
{
	cout << "�ܾ�����" << endl;
}




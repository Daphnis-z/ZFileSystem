#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "ZFileSystem.h"
using namespace std;

void CreatAccount(ZFileSystem& zfs)
{
	string name,
		password,
		group;
	cout << "\nName£º";
	cin >> name;
	cout << "Password£º";
	cin >> password;
	cout << "Group£º";
	cin >> group;
	zfs.AddUser(name, password, group);
}
bool Login(ZFileSystem& zfs)
{
	string name,
		password;
	cout << "\nName£º";
	cin >> name;
	cout << "Password£º";
	cin >> password;
	return zfs.Login(name, password);
}
void Menu1(ZFileSystem& zfs)
{
	cout << "-----------------1.Create Account-------------" << endl;
	cout << "-----------------2.Login-----------------------" << endl;
	cout << "-----------------3.Exit-------------------------" << endl;
	int cmd;
LOGIN:	cout << "CMD£º";
	cin >> cmd;
	switch (cmd)
	{
	case 1:
		CreatAccount(zfs);
		break;
	case 2:
		if (!Login(zfs))
			goto LOGIN;
		break;
	case 3:
		break;
	default:
		break;
	}
}

void cmd_create(ZFileSystem& zfs,string fileName)
{
	string rwr;
	cout << "RWR£º";
	cin >> rwr;
	zfs.CreateTextFile(fileName, rwr);

	string content;
	cout << "Content£º";
	cin >> content;
	zfs.AddTextIntoTextFile(fileName, content);
}
void cmd_cdir(ZFileSystem& zfs, string dicName)
{
	string rwr;
	cout << "RWR£º";
	cin >> rwr;
	zfs.CreateZDirectory(dicName, rwr);
}
void cmd_read(ZFileSystem& zfs, string fileName)
{
	zfs.OpenTextFile(fileName);
}
void cmd_edit(ZFileSystem& zfs, string fileName)
{
	zfs.OpenTextFile(fileName);
	string content;
	cout << "Content£º";
	cin >> content;
	zfs.AddTextIntoTextFile(fileName, content);
}
void cmd_cd(ZFileSystem& zfs,string path)
{
	zfs.JumpToDirectory(path);
}
void cmd_attr(ZFileSystem& zfs,string fileName)
{
	zfs.ShowDirectoryAttributes(fileName);
}
void cmd_del(ZFileSystem& zfs, string fileName)
{
	zfs.RemoveZDirectory(fileName);
}
void cmd_dir(ZFileSystem& zfs,string path)
{
	zfs.ShowDirectory(path);
}

void Menu2(ZFileSystem& zfs)
{
	string cmd, info;
	while (true)
	{
		cout << "CMD£º";
		cin >> cmd;
		if (cmd == "exit")
			break;
		else if (cmd == "format")
		{
			zfs.FormatSystem();
			break;
		}

		cin >> info;
		if (cmd == "create")
			cmd_create(zfs, info);
		else if (cmd == "cdir")
			cmd_cdir(zfs, info);
		else if (cmd == "read")
			cmd_read(zfs, info);
		else if (cmd == "edit")
			cmd_edit(zfs, info);
		else if (cmd == "cd")
			cmd_cd(zfs, info);
		else if (cmd == "attr")
			cmd_attr(zfs, info);
		else if (cmd == "del")
			cmd_del(zfs, info);
		else if (cmd == "dir")
			cmd_dir(zfs, info);
	}
}

void main()
{
	ZFileSystem zfs;
	Menu1(zfs);
	Menu2(zfs);
}


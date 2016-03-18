#pragma once
#include<string>
#include <iostream>
#include "ZFileManager.h"
#include "UserManager.h"

class ZFileSystem
{
public:
	ZFileSystem()
	{
	}

	//��ʽ��ϵͳ
	void FormatSystem()
	{
		FManager.FormatVirtualDisk();
		UserManager u;
	}

	//****************************************************************************
	//�˻�����
	bool Login(std::string userName, std::string password)
	{
		if (UManager.Login(userName, password))
		{
			FManager.SetUserDic(userName);
			std::cout << userName<<" login success ..\n" << std::endl;
			return true;
		}
		std::cout << "��¼ʧ��.. �û��������������" << std::endl;
		return false;
	}
	void AddUser(std::string userName, std::string password, std::string group)
	{
		if (UManager.AddUser(userName, password, group))
		{
			CreateZDirectory(userName, "110");
			FManager.SetCurDic(userName);
			std::cout << "�½��˻��ɹ�.." << std::endl;
		}
		else
			std::cout << "�½��˻�ʧ��.. �û����Ѵ��ڣ�" << std::endl;
	}
	void RemoveUser(std::string userName)
	{
		if (UManager.RemoveUser(userName))
			std::cout << "���˻���ɾ��.." << std::endl;
		else
			std::cout << "�˻�ɾ��ʧ��.. ���˻������ڣ���ǰ�û�Ȩ�޲��㣿" << std::endl;
	}
	//****************************************************************************

	//****************************************************************************
	//�ļ��в���
	//�½��ļ���
	void CreateZDirectory(std::string dicName,std::string rwr)
	{
		if (FManager.CreateZDirectory(dicName, UManager.CurUser.UserName, UManager.CurUser.Group, rwr, "100"))
			std::cout << "directory create success .." << std::endl;
		else
			std::cout << "\n�ļ��д���ʧ��.. �ļ������Ѵ��ڣ�" << std::endl;
	}

	//�������ļ���
	//�Ȳ�ʵ��
	bool RenameZDirectory(std::string newName)
	{
		return true;
	}

	//ɾ����ǰ�ļ��е����ļ���
	void RemoveZDirectory(std::string dicName)
	{
		if (FManager.RemoverDirectoryFromCurDic(dicName))
			std::cout << "remove success .." << std::endl;
		else
			std::cout << "ɾ��ʧ��.. ���ƴ���" << std::endl;
	}

	//��ʾ�ļ�����������ļ�
	void ShowDirectory(std::string path)
	{
		std::list<std::string> files;
		FManager.GetAllFiles(path, files);
		if (files.size()==0)
		{
			std::cout << "�ļ���·��������ļ���Ϊ��.." << std::endl;
			return;
		}
		for each (std::string file in files)
		{
			std::cout << file <<std::endl;
		}
	}

	//��ʾ��ǰ�ļ���ĳ�����ļ�������
	void ShowDirectoryAttributes(std::string fileName)
	{
		std::list<std::string> attrs;
		FManager.GetZFileAttributes(fileName,attrs);
		if (attrs.size()==0)
		{
			std::cout << "�޴��ļ��л��ļ�.." << std::endl;
			return;
		}

		std::cout<<fileName<<" attributes as follow .." << std::endl;
		for each (std::string attr in attrs)
		{
			std::cout << attr << std::endl;
		}
	}

	//��ת��ĳ���ļ���
	void JumpToDirectory(std::string path)
	{
		if (FManager.SetCurDic(path))
			std::cout << path + '>' << std::endl;
		else
			std::cout << "�ļ�����תʧ��.. ·������"<<std::endl;
	}
	//****************************************************************************

	//****************************************************************************
	//�ı��ļ�����
	void CreateTextFile(std::string fileName, std::string rwr)
	{
		if (FManager.CreateTextFile(fileName, UManager.CurUser.UserName, UManager.CurUser.Group, rwr, "001"))
			std::cout << "file create success .." << std::endl;
		else
			std::cout << "\n�ļ�����ʧ��.. �ļ����Ѵ��ڣ�" << std::endl;
	}
	void AddTextIntoTextFile(std::string fileName,std::string content)
	{
		if (FManager.AddContentToTextFile(fileName, content))
			std::cout << "content save success .." << std::endl;
		else
			std::cout << "�ļ�����ʧ��.. ·������" << std::endl;
	}

	//�򿪵�ǰ�ļ����µ�ĳ���ı��ļ�
	void OpenTextFile(std::string fileName)
	{
		std::string content;
		if (FManager.ReadTextFile(fileName, content))
			std::cout << fileName << " has opened��\n" << content << std::endl;
		else
			std::cout << "�ļ���ʧ��.. �ļ������ڣ�" << std::endl;
	}

	//�ڵ�ǰ�ļ����²��Ҳ�ɾ��ָ���ı��ļ�
	void RemovTextFile(std::string fileName)
	{
		if (FManager.RemoverDirectoryFromCurDic(fileName))
			std::cout << "ɾ���ɹ�.." << std::endl;
		else
			std::cout << "ɾ��ʧ��.. ��ǰ�ļ����²����ڴ��ļ���" << std::endl;
	}
	//****************************************************************************


private:
	UserManager UManager;
	ZFileManager FManager;
};
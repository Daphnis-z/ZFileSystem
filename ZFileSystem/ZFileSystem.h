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

	//格式化系统
	void FormatSystem()
	{
		FManager.FormatVirtualDisk();
		UserManager u;
	}

	//****************************************************************************
	//账户操作
	bool Login(std::string userName, std::string password)
	{
		if (UManager.Login(userName, password))
		{
			FManager.SetUserDic(userName);
			std::cout << userName<<" login success ..\n" << std::endl;
			return true;
		}
		std::cout << "登录失败.. 用户名错误？密码错误？" << std::endl;
		return false;
	}
	void AddUser(std::string userName, std::string password, std::string group)
	{
		if (UManager.AddUser(userName, password, group))
		{
			CreateZDirectory(userName, "110");
			FManager.SetCurDic(userName);
			std::cout << "新建账户成功.." << std::endl;
		}
		else
			std::cout << "新建账户失败.. 用户名已存在？" << std::endl;
	}
	void RemoveUser(std::string userName)
	{
		if (UManager.RemoveUser(userName))
			std::cout << "该账户已删除.." << std::endl;
		else
			std::cout << "账户删除失败.. 该账户不存在？当前用户权限不足？" << std::endl;
	}
	//****************************************************************************

	//****************************************************************************
	//文件夹操作
	//新建文件夹
	void CreateZDirectory(std::string dicName,std::string rwr)
	{
		if (FManager.CreateZDirectory(dicName, UManager.CurUser.UserName, UManager.CurUser.Group, rwr, "100"))
			std::cout << "directory create success .." << std::endl;
		else
			std::cout << "\n文件夹创建失败.. 文件夹名已存在？" << std::endl;
	}

	//重命名文件夹
	//先不实现
	bool RenameZDirectory(std::string newName)
	{
		return true;
	}

	//删除当前文件夹的子文件夹
	void RemoveZDirectory(std::string dicName)
	{
		if (FManager.RemoverDirectoryFromCurDic(dicName))
			std::cout << "remove success .." << std::endl;
		else
			std::cout << "删除失败.. 名称错误？" << std::endl;
	}

	//显示文件夹里的所有文件
	void ShowDirectory(std::string path)
	{
		std::list<std::string> files;
		FManager.GetAllFiles(path, files);
		if (files.size()==0)
		{
			std::cout << "文件夹路径错误或文件夹为空.." << std::endl;
			return;
		}
		for each (std::string file in files)
		{
			std::cout << file <<std::endl;
		}
	}

	//显示当前文件夹某个子文件夹属性
	void ShowDirectoryAttributes(std::string fileName)
	{
		std::list<std::string> attrs;
		FManager.GetZFileAttributes(fileName,attrs);
		if (attrs.size()==0)
		{
			std::cout << "无此文件夹或文件.." << std::endl;
			return;
		}

		std::cout<<fileName<<" attributes as follow .." << std::endl;
		for each (std::string attr in attrs)
		{
			std::cout << attr << std::endl;
		}
	}

	//跳转到某个文件夹
	void JumpToDirectory(std::string path)
	{
		if (FManager.SetCurDic(path))
			std::cout << path + '>' << std::endl;
		else
			std::cout << "文件夹跳转失败.. 路径错误？"<<std::endl;
	}
	//****************************************************************************

	//****************************************************************************
	//文本文件操作
	void CreateTextFile(std::string fileName, std::string rwr)
	{
		if (FManager.CreateTextFile(fileName, UManager.CurUser.UserName, UManager.CurUser.Group, rwr, "001"))
			std::cout << "file create success .." << std::endl;
		else
			std::cout << "\n文件创建失败.. 文件名已存在？" << std::endl;
	}
	void AddTextIntoTextFile(std::string fileName,std::string content)
	{
		if (FManager.AddContentToTextFile(fileName, content))
			std::cout << "content save success .." << std::endl;
		else
			std::cout << "文件保存失败.. 路径错误？" << std::endl;
	}

	//打开当前文件夹下的某个文本文件
	void OpenTextFile(std::string fileName)
	{
		std::string content;
		if (FManager.ReadTextFile(fileName, content))
			std::cout << fileName << " has opened：\n" << content << std::endl;
		else
			std::cout << "文件打开失败.. 文件不存在？" << std::endl;
	}

	//在当前文件夹下查找并删除指定文本文件
	void RemovTextFile(std::string fileName)
	{
		if (FManager.RemoverDirectoryFromCurDic(fileName))
			std::cout << "删除成功.." << std::endl;
		else
			std::cout << "删除失败.. 当前文件夹下不存在此文件？" << std::endl;
	}
	//****************************************************************************


private:
	UserManager UManager;
	ZFileManager FManager;
};
#pragma once
#include <string>
#include <map>
#include "BlockAddressManager.h"
#include "VirtualDiskManager.h"

struct User
{
	std::string UserName,
		Password,
		Group;//�û�������
};
class UserManager
{
public:
	bool IsLogined = false;
	User CurUser;

	UserManager()
	{
		ReadUserInfo();

		if (Users.size() == 0)
			InitializeUserInfo();
	}

	bool Login(std::string userName, std::string password)
	{
		if (Users.find(userName) != Users.end() && Users[userName].Password == password)
		{
			CurUser = Users[userName];
			IsLogined = true;
		}
		return IsLogined;
	}

	bool AddUser(std::string userName, std::string password, std::string group)
	{
		if (CurUser.Group == USER||Users.find(userName) != Users.end())
			return false;
		User u;
		u.UserName = userName;
		u.Password = password;
		u.Group = group;
		Users[userName] = u;

		WriteUserInfo();
		return true;
	}
	bool RemoveUser(std::string userName)
	{
		if (CurUser.Group == USER || Users.find(userName) == Users.end())
			return false;
		Users.erase(Users.find(userName));
		WriteUserInfo();
		return true;
	}

	//��ʼ���û���Ϣ
	void InitializeUserInfo()
	{
		CurUser.UserName = "User1";
		CurUser.Password = "123";
		CurUser.Group = ADMIN;
		Users[CurUser.UserName] = CurUser;
		WriteUserInfo();
	}

private:
	const std::string ADMIN = "admin",//����Ա��
		USER = "user";//�û���
	std::map<std::string, User> Users;
	BlockAddressManager BAManager;
	VirtualDiskManager VDManager;

	//��ȡ�����ϵ��û���Ϣ
	void ReadUserInfo()
	{
		long userNum = VDManager.ReadALong(BAManager.B2_S_Addr);
		std::list<std::string> vals;
		VDManager.ReadSomeString(vals, 3*userNum, BAManager.B2_S_Addr + 4);
		for (auto ite = vals.begin(); ite != vals.end();++ite)
		{
			User u;
			u.UserName = *ite;
			++ite;
			u.Password = *ite;
			++ite;
			u.Group = *ite;
			Users[u.UserName] = u;
		}
	}

	//ˢ�´����ϵ��û���Ϣ
	void WriteUserInfo()
	{
		VDManager.WriteALong(Users.size(), BAManager.B2_S_Addr);
		std::list<std::string> vals;
		for (auto ite = Users.begin(); ite != Users.end();++ite)
		{
			vals.push_back(ite->second.UserName);
			vals.push_back(ite->second.Password);
			vals.push_back(ite->second.Group);
		}
		VDManager.WriteSomeString(vals, BAManager.B2_S_Addr + 4);
	}
};
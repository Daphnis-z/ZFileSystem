#pragma once
#include <list>
#include "VirtualDiskManager.h"

//负责维护各区域地址信息（B1）
class BlockAddressManager
{
public:
	//****************************************************************************
	//公共成员域

	//各区域地址信息存储区域
	const long B1_S_Addr = 0;

	//账户信息区域(B1_1)
	const long B2_S_Addr = 8*1024,//开始地址
		B2_E_Addr =B2_S_Addr+8*1024-1;//结束地址
	long	B2_AE_Addr = B2_S_Addr;//有效区域结束地址

	//文件管理区域(B1_2)
	const long B31_S_Addr = B2_E_Addr+1,//文件头区磁盘碎片管理
		B32_S_Addr = B31_S_Addr+8*1024;//文件数据区磁盘碎片管理

	//文件头区域(B1_3)
	const long B33_S_Addr=B32_S_Addr+8*1024,
		B33_E_Addr=B33_S_Addr+1600*1024-1;
	long	B33_AE_Addr=B33_S_Addr;

	//文件数据区(B1_4)
	const long B34_S_Addr = B33_E_Addr + 1,
		B34_E_Addr = B34_S_Addr + 4000 * 1024 - 1;
	long	B34_AE_Addr=B34_S_Addr;


	//****************************************************************************
	//构造函数
	BlockAddressManager()
	{
		if (CheckIsAddrInfoInitialized())
			Refresh_AES();
		else
			InitializeAllAddrInfo();
	}

	//****************************************************************************
	//功能函数

	//判断地址信息是否已初始化好
	bool CheckIsAddrInfoInitialized()
	{
		long n = VDManager.ReadALong(B2_AE_B1);
		if (n > 0)
			return true;
		return false;
	}

	//初始化地址信息
	void InitializeAllAddrInfo()
	{
		std::list<long> vals = {
			B2_S_Addr, B2_E_Addr, B2_AE_Addr,
			B31_S_Addr, B32_S_Addr,
			B33_S_Addr, B33_E_Addr, B33_AE_Addr,
			B34_S_Addr, B34_E_Addr, B34_AE_Addr
		};
		VDManager.WriteSomeLong(vals, B1_S_Addr);
	}

	//刷新地址信息
	void Refresh_B2AE(long ae)
	{
		B2_AE_Addr = ae;
		VDManager.WriteALong(ae,B2_AE_B1);
	}
	void Refresh_B33AE(long ae)
	{
		B33_AE_Addr = ae;
		VDManager.WriteALong(ae, B33_AE_B1);
	}
	void Refresh_B34AE(long ae)
	{
		B34_AE_Addr = ae;
		VDManager.WriteALong(ae, B34_AE_B1);
	}

private:
	VirtualDiskManager VDManager;

	//各块需更改部分在B1中的起始地址
	const long B2_AE_B1 = 8,
		B33_AE_B1 = 28,
		B34_AE_B1 = 40;

	//刷新各块中的可变部分地址
	void Refresh_AES()
	{
		B2_AE_Addr = VDManager.ReadALong(B2_AE_B1);
		B33_AE_Addr = VDManager.ReadALong(B33_AE_B1);
		B34_AE_Addr = VDManager.ReadALong(B34_AE_B1);
	}

};
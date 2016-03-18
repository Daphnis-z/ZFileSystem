#pragma once
#include <string>
#include <list>
#include <deque>
#include <sstream>
#include <stdlib.h>
#include<stdio.h>
#include "VirtualDiskManager.h"
#include "BlockAddressManager.h"

struct ZFile 
{
	std::string FileName,//128B
		Creator,//64B
		Group,//64B
		DST,//是否为目录、系统文件、文本文件，4B
		RWR;//文件读写运行权限,4B
	long Size=0,//4B
		Addr=0,//4B
		ChildFileActSize=0;//4B
	std::list<ZFile> ChildFile;//子文件列表,文件夹特有
	std::string Content;//文件特有
};

class ZFileManager
{
public:
	//****************************************************************************
	//构造函数
	ZFileManager()
	{
		if (!CheckFileAreaInitialized())
			InitializeFileArea();
		else
		{
			ReadRootDirectory();
			CurZFile = RootZFile;
			UserZFile = RootZFile;
		}
	}
	//****************************************************************************


	//****************************************************************************
	//文件区域初始化相关

	//检查文件区域是否已初始化好
	bool CheckFileAreaInitialized()
	{
		if (VDManager.ReadAString(BAManager.B33_S_Addr) == "MFD")
			return true;
		return false;
	}

	//初始化文件区域
	void InitializeFileArea()
	{
		BAManager.Refresh_B33AE(BAManager.B33_S_Addr-1);
		CreateZDirectory("MFD", "Daphnis", "SuperManager", "000", "100");
		CreateZDirectory("User1", "Daphnis", "User", "110", "100");
		RootZFile = CurZFile;
		UserZFile = CurZFile;
// 		CurZFile = *RootZFile.ChildFile.begin();
	}

	//格式化整个虚拟磁盘
	void FormatVirtualDisk()
	{
		VirtualDiskManager v;
		BlockAddressManager b;
		ZFileManager();
	}
	//****************************************************************************

	//设置用户文件夹
	bool SetUserDic(std::string userName)
	{
		for each (ZFile zf in RootZFile.ChildFile)
		{
			ReadDirectoryInfo(zf);
			if (zf.FileName == userName)
			{
				UserZFile = zf;
				CurZFile = UserZFile;
				return true;
			}
		}
		return false;
	}

	//设置当前文件夹
	bool SetCurDic(std::string path)
	{
		std::list<std::string> dics;
		SplitPath(path, dics);
		
		ZFile zFile = UserZFile.FileName==RootZFile.FileName? CurZFile:UserZFile;
		if (GetZFile(zFile, dics))
		{
			CurZFile = zFile;
			return true;
		}
		return false;
	}

	//删除当前文件夹下的某个子文件
	bool RemoverDirectoryFromCurDic(std::string name)
	{
		GetChileZFiles(name, CurZFile);
		for (auto ite = CurZFile.ChildFile.begin(); ite != CurZFile.ChildFile.end();++ite)
		{
			if (ite->FileName==name)
			{
				CurZFile.ChildFile.erase(ite);
				CurZFile.ChildFileActSize -= AddrLen;

				std::list<long> vals = { CurZFile.ChildFileActSize };
				for each (ZFile z in CurZFile.ChildFile)
				{
					vals.push_back(z.Addr);
				}
				VDManager.WriteSomeLong(vals, CurZFile.Addr + DIC_OTHER_SIZE - AddrLen);
				return true;
			}
		}
		return false;
	}


	//****************************************************************************
	//写文件夹信息

	//创建文件夹
	bool CreateZDirectory(std::string dicName, std::string creater, std::string group, std::string rwr, std::string dst)
	{
		ZFile dic;
		dic.FileName = dicName;
		dic.Creator = creater;
		dic.Group = group;
		dic.RWR = rwr;
		dic.DST = dst;
		dic.Size = dicName.size() + creater.size() + group.size() + RWR_Len + DST_Len;
		long addr = AllocationMemoryForDic();
		if (addr<0)
			return false;
		dic.Addr = addr;

		WriteDirectoryInfo(dic);
		if (CurZFile.FileName!="")
			AddFileToCurDic(dic);
		else
		{
			RootZFile = dic;
			CurZFile = RootZFile;
		}
		return true;
	}

	//将文件夹信息写入磁盘
	void WriteDirectoryInfo(ZFile& dic)
	{
		int offset = dic.Addr;
		VDManager.WriteAString(dic.FileName,offset);
		offset += FileNameLen;
		VDManager.WriteAString(dic.Creator, offset);
		offset += CreatorLen;
		VDManager.WriteAString(dic.Group, offset);
		offset += GroupLen;

		std::list<std::string> vals = {dic.DST,dic.RWR};
		VDManager.WriteSomeString(vals, offset);
		offset += DST_Len + RWR_Len;

		std::list<long> ll = {dic.Size,dic.Addr};
		VDManager.WriteSomeLong(ll, offset);
	}

	//添加文件到当前文件夹
	void AddFileToCurDic(ZFile& zFile)
	{
		CurZFile.ChildFile.push_back(zFile);
		CurZFile.ChildFileActSize += AddrLen;

		std::list<long> vals = { CurZFile.ChildFileActSize };
		for each (auto a in CurZFile.ChildFile)
		{
			vals.push_back(a.Addr);
		}
		VDManager.WriteSomeLong(vals, CurZFile.Addr + DIC_OTHER_SIZE - ChildFileActSizeLen);
	}


	//****************************************************************************
	//读文件夹

	//获取当前文件夹的所有子文件‘ZFile’对象
	void GetChileZFiles(std::string fileName, ZFile& zFile)
	{
		for (auto ite = zFile.ChildFile.begin(); ite != zFile.ChildFile.end();++ite)
		{
			ReadDirectoryInfo(*ite);
		}
	}

	//将指定文件夹下所有的文件夹及文件放入files中
	//path,eg:"file\\hey\\hello"
	void GetAllFiles(std::string path, std::list<std::string>& files)
	{
		std::list<std::string> dics;
		SplitPath(path, dics);

		bool isFind = true;
		ZFile zFile;
		if (dics.back()==CurZFile.FileName)
			zFile = CurZFile;
		else
		{
			zFile = UserZFile;
			isFind = GetZFile(zFile, dics);
		}
		if (isFind)
		{
			for each (ZFile zf in zFile.ChildFile)
			{
				ReadDirectoryInfo(zf);
				files.push_back(zf.FileName+"\taddr："+LongToString(zf.Addr)+"\tSize（B）："+LongToString(zf.Size));
			}
		}
	}

	//根据路径链表获取指定‘ZFile对象 ’
	bool GetZFile(ZFile& zFile, std::list<std::string>& dics)
	{
		for each (auto str in dics)
		{
			bool canSea = false;
			if (str == zFile.FileName)
				continue;
			else
			{
				for each (ZFile zf in zFile.ChildFile)
				{
					ReadDirectoryInfo(zf);
					if (str == zf.FileName)
					{
						zFile = zf;
						canSea = true;
						break;
					}
				}
				if (!canSea)
					return false;
			}
		}
		return true;
	}

	//根据路径获取指定‘ZFile对象 ’
	bool GetZFileByPath(ZFile& zFile, std::string path)
	{
		std::list<std::string> dics;
		SplitPath(path, dics);
		if (GetZFile(zFile, dics))
			return true;
		return false;
	}

	//根据文件名在指定文件夹中获取子文件'ZFile对象'
	bool GetChildZFile(std::string fileName, ZFile& fzFile, ZFile& czFile)
	{
		bool isFind = false;
		for each (ZFile zf in fzFile.ChildFile)
		{
			ReadDirectoryInfo(zf);
			if (zf.FileName == fileName)
			{
				czFile = zf;
				isFind = true;
				break;
			}
		}
		return isFind;
	}

	//获取文件所有属性
	void GetZFileAttributes(std::string fileName, std::list<std::string>& attributes)
	{
		ZFile zFile;
		if (GetChildZFile(fileName,CurZFile,zFile))
		{
			std::string s = "";
			attributes.push_back(s + "Name：" + fileName);
			attributes.push_back(s+"IsSystemFile：" + zFile.DST[1]);
			attributes.push_back(s+"IsDirectory：" + zFile.DST[0]);
			attributes.push_back(s+"IsTextFile：" + zFile.DST[2]);
			attributes.push_back(s+"Creator：" + zFile.Creator);
			attributes.push_back(s+"Group：" + zFile.Group);
			attributes.push_back(s+"Size（B）："+LongToString(zFile.Size));
		}
	}

	//从文件读取根目录信息
	void ReadRootDirectory()
	{
		RootZFile.Addr = BAManager.B33_S_Addr;
		ReadDirectoryInfo(RootZFile);
	}

	//读取单个文件夹信息
	void ReadDirectoryInfo(ZFile& zFile)
	{
		long offset = zFile.Addr;
		zFile.FileName = VDManager.ReadAString(offset);
		offset += FileNameLen;
		zFile.Creator = VDManager.ReadAString(offset);
		offset += CreatorLen;
		zFile.Group = VDManager.ReadAString(offset);
		offset += GroupLen;

		std::list<std::string> vals;
		VDManager.ReadSomeString(vals,2,offset);
		auto ite = vals.begin();
		zFile.DST = *ite;
		++ite;
		zFile.RWR = *ite;
		offset += DST_Len + RWR_Len;

		std::list<long> ll;
		VDManager.ReadSomeLong(ll, 3, offset);
		auto itl = ll.begin();
		zFile.Size = *itl;
		++itl;
		++itl;
		zFile.ChildFileActSize = *itl;
		offset += SizeLen + AddrLen + ChildFileActSizeLen;

		if (zFile.ChildFileActSize>0)
		{
			std::list<long> sll;
			VDManager.ReadSomeLong(sll, zFile.ChildFileActSize / AddrLen, offset);
			for each (auto a in sll)
			{
				ZFile z;
				z.Addr = a;
				zFile.ChildFile.push_back(z);
			}
		}
	}

	//****************************************************************************
	//文本文件操作
	bool CreateTextFile(std::string fileName,std::string creater, std::string group, std::string rwr, std::string dst)
	{
		ZFile dic;
		dic.FileName = fileName;
		dic.Creator = creater;
		dic.Group = group;
		dic.RWR = rwr;
		dic.DST = dst;
		dic.Size = fileName.size() + creater.size() + group.size() + RWR_Len + DST_Len;
		long addr = AllocationMemoryForFile();
		if (addr < 0)
			return false;
		dic.Addr = addr;

		WriteFileInfo(dic);
		AddFileToCurDic(dic);
		return true;
	}
	void WriteFileInfo(ZFile& dic)
	{
		int offset = dic.Addr;
		VDManager.WriteAString(dic.FileName, offset);
		offset += FileNameLen;
		VDManager.WriteAString(dic.Creator, offset);
		offset += CreatorLen;
		VDManager.WriteAString(dic.Group, offset);
		offset += GroupLen;

		std::list<std::string> vals = { dic.DST, dic.RWR };
		VDManager.WriteSomeString(vals, offset);
		offset += DST_Len + RWR_Len;

		std::list<long> ll = { dic.Size, dic.Addr };
		VDManager.WriteSomeLong(ll, offset);
	}
	bool AddContentToTextFile(std::string path,std::string content)
	{
		ZFile zFile = CurZFile;
		if (GetZFileByPath(zFile, path))
		{
			content = VDManager.ReadAString(zFile.Addr + DIC_OTHER_SIZE)+content;
			VDManager.WriteAString(content, zFile.Addr + DIC_OTHER_SIZE);
			return true;
		}
		return false;
	}
	
	//读取当前文件夹下的某个文本文件
	bool ReadTextFile(std::string fileName,std::string& content)
	{
		ZFile zFile;
		if (GetChildZFile(fileName,CurZFile,zFile))
		{
			content = VDManager.ReadAString(zFile.Addr + DIC_OTHER_SIZE);
			return true;
		}
		return false;
	}

	//****************************************************************************


	//****************************************************************************
	//工具函数

	//为文件夹分配内存
	long AllocationMemoryForDic()
	{
		long addr = -1;
		if (DicEmptyBlock.size() > 0)
		{
			addr = DicEmptyBlock.front();
			DicEmptyBlock.pop_front();
		}
		else if (BAManager.B33_AE_Addr + BASE_MEM_BLOCK<BAManager.B33_E_Addr)
		{
			addr = BAManager.B33_AE_Addr + 1;
			BAManager.Refresh_B33AE(BAManager.B33_AE_Addr + BASE_MEM_BLOCK);
		}
		return addr;
	}

	//为文件分配内存
	long AllocationMemoryForFile()
	{
		long addr = -1;
		if (BAManager.B34_AE_Addr + BASE_MEM_BLOCK < BAManager.B34_E_Addr)
		{
			addr = BAManager.B34_AE_Addr + 1;
			BAManager.Refresh_B34AE(BAManager.B34_AE_Addr + BASE_MEM_BLOCK);
		}
		return addr;
	}

	std::string LongToString(long val)
	{
		std::stringstream ss;
		ss << val;
		std::string s = ss.str();
		s.resize(8);
		return s;
	}

	//拆分路径
	void SplitPath(std::string path, std::list<std::string>& dics)
	{
		int i1 = 0, i2 = 0;
		while ((i2 = path.find('\\', i2)) < path.length())
		{
			dics.push_back(path.substr(i1, i2 - i1));
			i1 = ++i2;
		}
		dics.push_back(path.substr(path.find_last_of('\\') + 1));
	}

private:
	//各属性字节大小
	int	FileNameLen=128,//128B
		CreatorLen=64,//64B
		GroupLen=64,//64B
		DST_Len=4,//是否为目录、系统文件、文本文件
		RWR_Len=4,//文件读写运行权限
		SizeLen=4,	
		AddrLen=4,
		ChildFileActSizeLen=4 ;

	//各内存块大小
	const long BASE_MEM_BLOCK=8*1024,
		DIC_OTHER_SIZE=276,
		DIC_BLOCK_TOTAL_SIZE = 8000 * 1024;

	VirtualDiskManager VDManager;
	BlockAddressManager BAManager;

	std::string SuperBlockName;
	ZFile	RootZFile,//根目录
		UserZFile,//当前用户目录
		CurZFile;//当前目录
	std::deque<long> DicEmptyBlock,
		FileEmptyBlock;
};
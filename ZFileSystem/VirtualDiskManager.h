#pragma once
#include <fstream>
#include<stdio.h>
#include<string>
#include <windows.h>
#include <list>


class VirtualDiskManager
{
public:
	bool IsInitialized = false;//虚拟磁盘是否已初始化

	VirtualDiskManager()
	{
		IsInitialized = CheckVirtualDiskIsInitialized();

		//自动初始化虚拟磁盘
		if (!IsInitialized)
			InitializeVirtualDisk();
	}

	//初始化虚拟磁盘
	void InitializeVirtualDisk()
	{
		std::ofstream out(VDFN);
		std::string dicInfoBlock;
		dicInfoBlock.resize(VD_SIZE);
		out << dicInfoBlock;
		out.close();
	}

	//检查虚拟磁盘是否已初始化
	bool CheckVirtualDiskIsInitialized()
	{
		return FileExists(VDFN);
	}

	//判断文件是否存在
	bool FileExists(const char* fileName)
	{
		WIN32_FIND_DATAA wfd;
		HANDLE a = FindFirstFileA(fileName, &wfd);
		if (a != INVALID_HANDLE_VALUE)
		{
			FindClose(a);
			return true;
		}
		return false;
	}

	//****************************************************************************
	//读写磁盘

	//读写单个ASCII字符
	void WriteAChar(char val, long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp, offset, SEEK_SET);
		fputc(val, fp);
		fclose(fp);
	}
	char ReadAChar(long offset)
	{
		FILE* fp = fopen(VDFN, "r");
		fseek(fp, offset, SEEK_SET);
		char val = fgetc(fp);
		fclose(fp);
		return val;
	}

	//读写单个ASCII字符串
	void WriteAString(std::string val, long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp,offset,SEEK_SET);
		fputs(val.data(), fp);
		fputc('\0', fp);
		fclose(fp);
	}
	std::string ReadAString(long offset)
	{
		FILE* fp = fopen(VDFN, "r");
		fseek(fp,offset,SEEK_SET);
		std::string str =ReadAStringFromStream(fp);
		fclose(fp);
		return str;
	}

	//读写多个ASCII字符串
	void WriteSomeString(std::list<std::string>& vals,long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp, offset, SEEK_SET);
		for each (auto str in vals)
		{
			fputs(str.data(), fp);
			fputc('\0', fp);
		}
		fclose(fp);
	}
	void ReadSomeString(std::list<std::string>& vals, int cnt, long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp, offset, SEEK_SET);
		for (int i = 0; i < cnt;++i)
		{
			vals.push_back(ReadAStringFromStream(fp));
		}
		fclose(fp);
	}

	//读写单个‘long’
	void WriteALong(long val, long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp,offset,SEEK_SET);
		putw(val, fp);
		fclose(fp);
	}
	long ReadALong(long offset)
	{
		FILE* fp = fopen(VDFN, "r");
		fseek(fp, offset, SEEK_SET);
		long val = getw(fp);
		fclose(fp);
		return val;
	}

	//读写多个‘long’
	void WriteSomeLong(std::list<long>& vals, long offset)
	{
		FILE* fp = fopen(VDFN, "rb+");
		fseek(fp, offset, SEEK_SET);
		for each (auto val in vals)
		{
			putw(val, fp);
		}
		fclose(fp);
	}
	void ReadSomeLong(std::list<long>& vals,int cnt, long offset)
	{
		FILE* fp = fopen(VDFN, "r");
		fseek(fp, offset, SEEK_SET);
		for (int i = 0; i < cnt;++i)
		{
			vals.push_back(getw(fp));
		}
		fclose(fp);
	}

private:
	const char* VDFN = "ZFileSystem.zfs";//虚拟磁盘文件名称
	const long VD_SIZE = 10000 * 1024;//虚拟磁盘容量

	//从文件流中读取一个字符串
	std::string ReadAStringFromStream(FILE* _File)
	{
		std::string str = "";
		char c = fgetc(_File);
		while (c != '\0')
		{
			str += c;
			c = fgetc(_File);
		}
		return str;
	}
};
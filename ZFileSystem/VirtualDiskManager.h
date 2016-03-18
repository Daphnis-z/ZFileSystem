#pragma once
#include <fstream>
#include<stdio.h>
#include<string>
#include <windows.h>
#include <list>


class VirtualDiskManager
{
public:
	bool IsInitialized = false;//��������Ƿ��ѳ�ʼ��

	VirtualDiskManager()
	{
		IsInitialized = CheckVirtualDiskIsInitialized();

		//�Զ���ʼ���������
		if (!IsInitialized)
			InitializeVirtualDisk();
	}

	//��ʼ���������
	void InitializeVirtualDisk()
	{
		std::ofstream out(VDFN);
		std::string dicInfoBlock;
		dicInfoBlock.resize(VD_SIZE);
		out << dicInfoBlock;
		out.close();
	}

	//�����������Ƿ��ѳ�ʼ��
	bool CheckVirtualDiskIsInitialized()
	{
		return FileExists(VDFN);
	}

	//�ж��ļ��Ƿ����
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
	//��д����

	//��д����ASCII�ַ�
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

	//��д����ASCII�ַ���
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

	//��д���ASCII�ַ���
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

	//��д������long��
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

	//��д�����long��
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
	const char* VDFN = "ZFileSystem.zfs";//��������ļ�����
	const long VD_SIZE = 10000 * 1024;//�����������

	//���ļ����ж�ȡһ���ַ���
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
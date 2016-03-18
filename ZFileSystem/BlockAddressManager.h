#pragma once
#include <list>
#include "VirtualDiskManager.h"

//����ά���������ַ��Ϣ��B1��
class BlockAddressManager
{
public:
	//****************************************************************************
	//������Ա��

	//�������ַ��Ϣ�洢����
	const long B1_S_Addr = 0;

	//�˻���Ϣ����(B1_1)
	const long B2_S_Addr = 8*1024,//��ʼ��ַ
		B2_E_Addr =B2_S_Addr+8*1024-1;//������ַ
	long	B2_AE_Addr = B2_S_Addr;//��Ч���������ַ

	//�ļ���������(B1_2)
	const long B31_S_Addr = B2_E_Addr+1,//�ļ�ͷ��������Ƭ����
		B32_S_Addr = B31_S_Addr+8*1024;//�ļ�������������Ƭ����

	//�ļ�ͷ����(B1_3)
	const long B33_S_Addr=B32_S_Addr+8*1024,
		B33_E_Addr=B33_S_Addr+1600*1024-1;
	long	B33_AE_Addr=B33_S_Addr;

	//�ļ�������(B1_4)
	const long B34_S_Addr = B33_E_Addr + 1,
		B34_E_Addr = B34_S_Addr + 4000 * 1024 - 1;
	long	B34_AE_Addr=B34_S_Addr;


	//****************************************************************************
	//���캯��
	BlockAddressManager()
	{
		if (CheckIsAddrInfoInitialized())
			Refresh_AES();
		else
			InitializeAllAddrInfo();
	}

	//****************************************************************************
	//���ܺ���

	//�жϵ�ַ��Ϣ�Ƿ��ѳ�ʼ����
	bool CheckIsAddrInfoInitialized()
	{
		long n = VDManager.ReadALong(B2_AE_B1);
		if (n > 0)
			return true;
		return false;
	}

	//��ʼ����ַ��Ϣ
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

	//ˢ�µ�ַ��Ϣ
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

	//��������Ĳ�����B1�е���ʼ��ַ
	const long B2_AE_B1 = 8,
		B33_AE_B1 = 28,
		B34_AE_B1 = 40;

	//ˢ�¸����еĿɱ䲿�ֵ�ַ
	void Refresh_AES()
	{
		B2_AE_Addr = VDManager.ReadALong(B2_AE_B1);
		B33_AE_Addr = VDManager.ReadALong(B33_AE_B1);
		B34_AE_Addr = VDManager.ReadALong(B34_AE_B1);
	}

};
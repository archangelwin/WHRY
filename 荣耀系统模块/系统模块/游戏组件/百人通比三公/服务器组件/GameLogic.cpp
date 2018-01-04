#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////

//�˿�����
const BYTE CGameLogic::m_cbCardListData[CARD_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K
};

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameLogic::CGameLogic()
{
}

//��������
CGameLogic::~CGameLogic()
{
}

//�����˿�
void CGameLogic::RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	//����׼��
	BYTE cbCardData[CountArray(m_cbCardListData)];
	CopyMemory(cbCardData,m_cbCardListData,sizeof(m_cbCardListData));

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}


void CGameLogic::RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount)
{
	BYTE *cbCardData=new BYTE[cbBufferCount];
	CopyMemory(cbCardData,cbCardBuffer,sizeof(BYTE)*cbBufferCount);

	//�����˿�
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(cbBufferCount-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[cbBufferCount-cbRandCount];
	} while (cbRandCount<cbBufferCount);
	delete[] cbCardData;
}

int CGameLogic::RetType(int itype)
{
	itype = itype%10;
	return itype;

}
//��ȡ����
BYTE CGameLogic::GetCardType(const BYTE cbCardData[], BYTE cbCardCount)
{
	//�Ϸ��ж�
	ASSERT(3 == cbCardCount);
	if (3 != cbCardCount) return CT_ERROR;

	//�����˿�
	BYTE cbCardDataSort[CARD_COUNT];
	CopyMemory(cbCardDataSort, cbCardData, sizeof(BYTE)*cbCardCount);
	SortCardList(cbCardDataSort, cbCardCount, ST_VALUE);

	if (cbCardCount != 3)
		return CT_ERROR;

	if (IsSanGong(cbCardDataSort, cbCardCount))
		return CT_SanGong;

	return CT_SanPai;
}

//��С�Ƚ�
/*
cbNextCardData>cbFirstCardData  ����1
cbNextCardData<cbFirstCardData  ����-1
cbNextCardData==cbFirstCardData ����0
*/
int CGameLogic::CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount, const BYTE cbNextCardData[], BYTE cbNextCardCount)
{
	//�Ϸ��ж�
	ASSERT(3 == cbFirstCardCount && 3 == cbNextCardCount);
	if (!(3 == cbFirstCardCount && 3 == cbNextCardCount)) return 0;


	//�����˿�
	BYTE cbFirstCardDataTemp[3];
	CopyMemory(cbFirstCardDataTemp, cbFirstCardData, sizeof(cbFirstCardDataTemp));
	SortCardList(cbFirstCardDataTemp, cbFirstCardCount, ST_VALUE);

	BYTE cbNextCardDataTemp[3];
	CopyMemory(cbNextCardDataTemp, cbNextCardData, sizeof(cbNextCardDataTemp));
	SortCardList(cbNextCardDataTemp, cbNextCardCount, ST_VALUE);

	//��ȡ����
	BYTE cbFirstCardType = GetCardType(cbFirstCardDataTemp, cbFirstCardCount);
	BYTE cbNextCardType = GetCardType(cbNextCardDataTemp, cbNextCardCount);

	//���ͱȽ�
	if (cbFirstCardType != cbNextCardType)
	{
		if (cbFirstCardType > cbNextCardType) 
			return 1;
		else 
			return -1;
	}

	//���������ж�
	if (CT_SanPai != cbFirstCardType && cbFirstCardType == cbNextCardType)
	{
		BYTE cbFirstValue[3] = { GetCardValue(cbFirstCardDataTemp[0]), GetCardValue(cbFirstCardDataTemp[1]), GetCardValue(cbFirstCardDataTemp[2]) };
		BYTE cbNextValue[3] = { GetCardValue(cbNextCardDataTemp[0]), GetCardValue(cbNextCardDataTemp[1]), GetCardValue(cbNextCardDataTemp[2]) };
		BYTE cbCount = 0;
		for (BYTE i = 0; i<3; i++)
		{
			if (cbFirstValue[i] > cbNextValue[i])
				return 1;
			else
				cbCount++;
		}
		if (cbCount == 3)	//���Ź��� �Ƶ���ͬ  ������ƵĻ�ɫ
		{
			if (GetCardColor(cbNextCardDataTemp[0]) > GetCardColor(cbFirstCardDataTemp[0]))
				return 1;
		}
		return -1;
	}

	//��ȡ����
	BYTE cbFirstPip = GetCardListPip(cbFirstCardDataTemp, cbFirstCardCount);
	BYTE cbNextPip = GetCardListPip(cbNextCardDataTemp, cbNextCardCount);

	//�����Ƚ�
	if (cbFirstPip != cbNextPip)
	{
		if (cbFirstPip > cbNextPip) return 1;
		else return -1;
	}

	//ͬ���ж� �ȱȽϹ���
	if (cbFirstPip == cbNextPip)
	{
		BYTE a, b;
		a = GetCardValue(cbNextCardDataTemp[0]);
		b = GetCardValue(cbFirstCardDataTemp[0]);
		if (GetCardValue(cbNextCardDataTemp[0]) < GetCardValue(cbFirstCardDataTemp[0])) //�ȹ��Ƶ�����С
			return 1;
		else if (GetCardValue(cbNextCardDataTemp[0]) == GetCardValue(cbFirstCardDataTemp[0])) //���Ƶ�����ͬ�Ȼ�ɫ
		{
			if (GetCardColor(cbNextCardDataTemp[0]) < GetCardColor(cbFirstCardDataTemp[0]))
				return 1;
			else
				return -1;
		}
		else
			return -1;
	}
	return -1;
}

//��ȡ�Ƶ�
BYTE CGameLogic::GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount)
{
	//��������
	BYTE cbPipCount = 0;

	//��ȡ�Ƶ�
	BYTE cbCardValue = 0;
	for (BYTE i = 0; i < cbCardCount; i++)
	{
		cbCardValue = GetCardValue(cbCardData[i]);
		if (cbCardValue > 10)
			continue;
		cbPipCount += cbCardValue;
	}

	return (cbPipCount % 10);
}

BYTE CGameLogic::GetCardNewValue(BYTE cbCardData)
{
	//�˿�����
	BYTE cbCardColor=GetCardColor(cbCardData);
	BYTE cbCardValue=GetCardValue(cbCardData);

	//ת����ֵ
	if (cbCardColor==0x04) return cbCardValue+13+2;
	return cbCardValue;

}
//�߼���С
BYTE CGameLogic::GetCardLogicValue(BYTE cbCardData)
{
	BYTE cbValue=GetCardValue(cbCardData);

	//��ȡ��ɫ
	BYTE cbColor=GetCardColor(cbCardData);

	if(cbValue>10)
	{
		cbValue = 10;

	}
	if(cbColor==0x4)
	{
		return 11;
	}
	return cbValue;
}

//�����˿�
void CGameLogic::SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType)
{
	//��Ŀ����
	if (cbCardCount==0) return;

	//ת����ֵ
	BYTE cbSortValue[CARD_COUNT];
	if (ST_VALUE==cbSortType)
	{
		for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardValue(cbCardData[i]);	
	}
	else 
	{
		if(cbSortType==ST_NEW)
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardNewValue(cbCardData[i]);	

		}else
		{
			for (BYTE i=0;i<cbCardCount;i++) cbSortValue[i]=GetCardLogicValue(cbCardData[i]);	
		}

	}


	//�������
	bool bSorted=true;
	BYTE cbThreeCount,cbLast=cbCardCount-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<cbLast;i++)
		{
			if ((cbSortValue[i]<cbSortValue[i+1])||
				((cbSortValue[i]==cbSortValue[i+1])&&(cbCardData[i]<cbCardData[i+1])))
			{
				//����λ��
				cbThreeCount=cbCardData[i];
				cbCardData[i]=cbCardData[i+1];
				cbCardData[i+1]=cbThreeCount;
				cbThreeCount=cbSortValue[i];
				cbSortValue[i]=cbSortValue[i+1];
				cbSortValue[i+1]=cbThreeCount;
				bSorted=false;
			}	
		}
		cbLast--;
	} while(bSorted==false);

	return;
}


//�Ƿ�������
bool CGameLogic::IsSanGong(const BYTE cbCardBuffer[], const BYTE cbBufferCount)
{
	if (cbBufferCount != 3)
		return false;

	for (BYTE i = 0; i < cbBufferCount; i++)
	{
		if (GetCardValue(cbCardBuffer[i]) < 0x0B)
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////


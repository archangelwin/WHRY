#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����

enum emCardType
{
	CT_ERROR			=		0,								//��������
	CT_SanPai			=		1,								//ɢ������
	CT_SanGong			=		2,								//��������
};

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_NEW					    3									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					52									//�˿���Ŀ
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼�
class CGameLogic
{
	//��������
public:
	static const BYTE				m_cbCardListData[CARD_COUNT];		//�˿˶���

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//���ƺ���
public:
	//�����˿�
	void RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void RandCardListEx(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//�����˿�
	void SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	int RetType(int itype);

	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);

	//�Ƿ�������
	bool IsSanGong(const BYTE cbCardBuffer[], const BYTE cbBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif

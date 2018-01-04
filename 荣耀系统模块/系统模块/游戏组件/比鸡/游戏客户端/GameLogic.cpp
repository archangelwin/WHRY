#include "StdAfx.h"
#include "GameLogic.h"

//////////////////////////////////////////////////////////////////////////
const BYTE	CGameLogic::m_bCardListData[52]=
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
	ZeroMemory(btCardSpecialData,sizeof(btCardSpecialData));
}

//��������
CGameLogic::~CGameLogic()
{
}

//��ȡ����
BYTE CGameLogic::GetCardType(BYTE bCardData[], BYTE bCardCount,BYTE btSpecialCard[])
{
	//����У��
	ASSERT(bCardCount==9) ;
	if (bCardCount != 9)
	{
		return CT_INVALID;
	}

	tagAnalyseData AnalyseData;
	memset(&AnalyseData, 0, sizeof(tagAnalyseData));
	AnalyseCard(bCardData, bCardCount, AnalyseData);


	// ȫͬ��
	if (AnalyseData.bStraight)
	{
		return CT_ALL_TONGHUA;
	}

	//ȫ����
	if (AnalyseData.bThreeCount == 3)
	{
		return CT_ALL_SHANTIAO;
	}

	// ͬɫ
	int nCountBlack = 0;
	int nCountRed = 0;
	for (int i = 0; i < 9; i++)
	{
		if (GetCardColor(bCardData[i]) == 0x00 || GetCardColor(bCardData[i]) == 0x02)
		{
			nCountBlack++;
		}
		else
		{
			nCountRed++;
		}
	}

	if (nCountBlack == 9 || nCountRed == 9)
	{
		return CT_ALL_TONGSE;
	}

	return CT_INVALID;
}

//�����˿�
void CGameLogic::SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType)
{
	ASSERT(bCardCount>=1 && bCardCount<=13) ;
	if(bCardCount<1 || bCardCount>13) return ;

	//ת����ֵ
	BYTE bLogicVolue[13];
	for (BYTE i=0;i<bCardCount;i++)	bLogicVolue[i]=GetCardLogicValue(bCardData[i]);	

	if(enDescend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bLogicVolue[i]<bLogicVolue[i+1])||
					((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]<bCardData[i+1])))
				{
					//����λ��
					bTempData=bCardData[i];
					bCardData[i]=bCardData[i+1];
					bCardData[i+1]=bTempData;
					bTempData=bLogicVolue[i];
					bLogicVolue[i]=bLogicVolue[i+1];
					bLogicVolue[i+1]=bTempData;
					bSorted=false;
				}	
			}
			bLast--;
		} while(bSorted==false);
	}
	else if(enAscend==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bLogicVolue[i]>bLogicVolue[i+1])||
					((bLogicVolue[i]==bLogicVolue[i+1])&&(bCardData[i]>bCardData[i+1])))
				{
					//����λ��
					bTempData=bCardData[i];
					bCardData[i]=bCardData[i+1];
					bCardData[i+1]=bTempData;
					bTempData=bLogicVolue[i];
					bLogicVolue[i]=bLogicVolue[i+1];
					bLogicVolue[i+1]=bTempData;
					bSorted=false;
				}	
			}
			bLast--;
		} while(bSorted==false);
	}
	else if(enColor==SortCardType)
	{
		//�������
		bool bSorted=true;
		BYTE bTempData,bLast=bCardCount-1;
		BYTE m_bCardCount=1;
		BYTE bColor[13];
		for (BYTE i=0;i<bCardCount;i++)	bColor[i]=GetCardColor(bCardData[i]);	
		do
		{
			bSorted=true;
			for (BYTE i=0;i<bLast;i++)
			{
				if ((bColor[i]<bColor[i+1])||
					((bColor[i]==bColor[i+1])&&(GetCardLogicValue(bCardData[i])<GetCardLogicValue(bCardData[i+1]))))
				{
					//����λ��
					bTempData=bCardData[i];
					bCardData[i]=bCardData[i+1];
					bCardData[i+1]=bTempData;
					bTempData=bColor[i];
					bColor[i]=bColor[i+1];
					bColor[i+1]=bTempData;
					bSorted=false;
				}	
			}
			bLast--;
		} while(bSorted==false);
	}
	return;
}

//�����˿�
void CGameLogic::RandCardList(BYTE bCardBuffer[], BYTE bBufferCount)
{
	//����׼��
	BYTE bCardData[sizeof(m_bCardListData)];
	CopyMemory(bCardData,m_bCardListData,sizeof(m_bCardListData));
	//CopyMemory(bCardBuffer,m_bCardListData,sizeof(m_bCardListData));

	static LONG dwRandCount=0L;
	srand((unsigned)time(NULL)+dwRandCount++);
	//�����˿�
	BYTE bRandCount=0,bPosition=0;
	do
	{
		bPosition=rand()%(bBufferCount-bRandCount);
		bCardBuffer[bRandCount++]=bCardData[bPosition];
		bCardData[bPosition]=bCardData[bBufferCount-bRandCount];
	} while (bRandCount<bBufferCount);

	return;
}

// �����˿�2
void CGameLogic::RandCardListEx(BYTE bCardBuffer[], BYTE bBufferCount)
{
	static LONG dwRandCount = 0L;
	srand((unsigned)time(NULL) + dwRandCount++);

	BYTE bTmp;
	int nIndex = 0;
	for (int i = 0; i < bBufferCount; i++)
	{
		nIndex = rand() % (bBufferCount - 1) + i;
		if (nIndex != i)
		{
			bTmp = bCardBuffer[i];
			bCardBuffer[i] = bCardBuffer[nIndex];
			bCardBuffer[nIndex] = bTmp;
		}
	}
}

void CGameLogic::RandCardListEx2(BYTE *pCard, BYTE bBufferCount)
{
	static LONG dwRandCount = 0L;
	srand((unsigned)time(NULL) + dwRandCount++);

	BYTE bTmp;
	int nIndex = 0;
	for (int i = 0; i < bBufferCount; i++)
	{
		nIndex = rand() % (bBufferCount - 1);
		if (nIndex != i)
		{
			bTmp = *(pCard + i);
			*(pCard + i) = *(pCard + nIndex);
			*(pCard + nIndex) = bTmp;
		}
	}
}

//ɾ���˿�
bool CGameLogic::RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount)
{
	ASSERT(bRemoveCount <= bCardCount);

	//�������
	BYTE bDeleteCount = 0;

	BYTE * pByte = new BYTE[bCardCount];
	CopyMemory(pByte, bCardData, sizeof(bCardData[0]) * bCardCount);

	//�����˿�
	for (BYTE i = 0; i < bRemoveCount; i++)
	{
		for (BYTE j = 0; j < bCardCount; j++)
		{
			if (bRemoveCard[i] == *(pByte + j))
			{
				bDeleteCount++;
				*(pByte + j) = 0;
				break;
			}
		}
	}
	
// 	if (bDeleteCount != bRemoveCount)
// 	{
// 		delete[]pByte;
// 		pByte = NULL;
// 
// 		return false;
// 	}


	//�����˿�
	BYTE bCardPos = 0;
	memset(bCardData, 0, bCardCount);
	for (BYTE i = 0; i < bCardCount; i++)
	{
		if (*(pByte + i) != 0)
		{
			bCardData[bCardPos] = *(pByte + i);
			bCardPos++;
		}
	}

	delete []pByte;
	pByte = NULL;

	return true;
}

//�߼���ֵ
BYTE CGameLogic::GetCardLogicValue(BYTE bCardData)
{
	//�˿�����
	BYTE bCardValue=GetCardValue(bCardData); 

	//ת����ֵ
	return (bCardValue==1)?(bCardValue+13):bCardValue; 
}

/*
*	�����˿˵ĵ��ţ�һ�ԡ���������Ŀ������¼��ÿ�������˿˵ĵ�һ���ƣ�Ҳ���������ƣ�λ���Ա�Ƚϴ�С��ͬʱ�ж�ͬһ��ɫ�Ƿ�������
*/
//�����˿�
/*
void CGameLogic::AnalyseCard(const BYTE bCardData[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
//������֤
ASSERT(bCardCount>=1 && bCardCount<=13) ;
if(bCardCount<1 || bCardCount>13) return ;

memset(&AnalyseData , 0 , sizeof(AnalyseData)) ;
BYTE bCardValue = GetCardValue(bCardData[0]) ;
BYTE bCardColor = GetCardColor(bCardData[0]) ;

BYTE bSameValueCount = 1 ,
bSameColorCount = 1 ,
bFirstCardIndex = 0 ;	//��¼�±�

for(BYTE i=1 ; i<bCardCount ; ++i)
{
if(GetCardValue(bCardData[i])==bCardValue) ++bSameValueCount ;

if(GetCardValue(bCardData[i])!=bCardValue || i==bCardCount-1)
{
switch(bSameValueCount) 
{
case 1:
AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
AnalyseData.bOneCount++ ;
break;

case 2:
AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
AnalyseData.bTwoCount++ ;
break;

case 3:
AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
AnalyseData.bThreeCount++ ;
break;

case 4:
AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
AnalyseData.bFourCount++ ;
break;

case 5:
AnalyseData.bFiveCount++ ;
break;

default:
AfxMessageBox("������") ;
return;
}

if(i==bCardCount-1) 
{
AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
AnalyseData.bOneCount++ ;
}

bCardValue = GetCardValue(bCardData[i]) ;
bSameValueCount = 1 ;
bFirstCardIndex = i ;
}

if(GetCardColor(bCardData[i])!=bCardColor) bSameColorCount = 1 ;
else									   ++bSameColorCount ;
}

//�Ƿ�ͬ��
AnalyseData.bStraight = (5==bSameColorCount) ? true : false ;
}*/


//�����˿�
void CGameLogic::AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) 
{
	BYTE bCardData[9] ;
	CopyMemory(bCardData , bCardDataList , bCardCount) ;
	SortCardList(bCardData , bCardCount , enDescend) ;

	BYTE bSameCount = 1 ,
		bCardValueTemp=0,
		bSameColorCount = 1 ,
		bFirstCardIndex = 0 ;

	BYTE bLogicValue=GetCardLogicValue(bCardData[0]);
	BYTE bCardColor = GetCardColor(bCardData[0]) ;

	ASSERT(3==bCardCount || 9==bCardCount) ;

	//���ý��
	memset(&AnalyseData,0,sizeof(AnalyseData));

	//�˿˷���
	for (BYTE i=1;i<bCardCount;i++)
	{
		//��ȡ�˿�
		bCardValueTemp=GetCardLogicValue(bCardData[i]);
		if (bCardValueTemp==bLogicValue) bSameCount++;

		//������
		if ((bCardValueTemp!=bLogicValue)||(i==(bCardCount-1)))
		{
			switch (bSameCount)
			{
			case 1:		//һ��
				break; 
			case 2:		//����
				{
					AnalyseData.bTwoFirst[AnalyseData.bTwoCount]	 = bFirstCardIndex ;
					AnalyseData.bTwoCount++ ;
					break;
				}
			case 3:		//����
				{
					AnalyseData.bThreeFirst[AnalyseData.bThreeCount] = bFirstCardIndex ;
					AnalyseData.bThreeCount++ ;
					break;
				}
			case 4:		//����
				{
					AnalyseData.bFourFirst[AnalyseData.bFourCount]   = bFirstCardIndex ;
					AnalyseData.bFourCount++ ;
					break;
				}
			default:
				CString str ;
				//str.Format("AnalyseCard�������˿ˣ�: %d" , bSameCount) ;
				// #ifdef _DEBUG
				// 				AfxMessageBox(str) ;
				// #endif				
				break;
			}
		}

		//���ñ���
		if (bCardValueTemp!=bLogicValue)
		{
			if(bSameCount==1)
			{
				if(i!=bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
				}
				else
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= bFirstCardIndex ;
					AnalyseData.bOneCount++ ;
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;				
				}
			}
			else
			{
				if(i==bCardCount-1)
				{
					AnalyseData.bOneFirst[AnalyseData.bOneCount]	= i ;
					AnalyseData.bOneCount++ ;
				}
			}
			bSameCount=1;
			bLogicValue=bCardValueTemp;
			bFirstCardIndex = i ;

		}
		if(GetCardColor(bCardData[i])!=bCardColor) bSameColorCount = 1 ;
		else									   ++bSameColorCount ;
	}

	//�Ƿ�ͬ��
	AnalyseData.bStraight = (bCardCount==bSameColorCount) ? true : false ;

	return;
}

//�����˿�
void CGameLogic::AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult)
{
	//���ý��
	ZeroMemory(&AnalyseResult,sizeof(AnalyseResult));

	//�˿˷���
	for (BYTE i=0;i<cbCardCount;i++)
	{
		//��������
		BYTE cbSameCount=1,cbCardValueTemp=0;
		BYTE cbLogicValue=GetCardLogicValue(cbCardData[i]);

		//����ͬ��
		for (BYTE j=i+1;j<cbCardCount;j++)
		{
			//��ȡ�˿�
			if (GetCardLogicValue(cbCardData[j])!=cbLogicValue) break;

			//���ñ���
			cbSameCount++;
		}

		//���ý��
		switch (cbSameCount)
		{
		case 1:		//����
			{
				BYTE cbIndex=AnalyseResult.cbSignedCount++;
				AnalyseResult.cbSignedCardData[cbIndex*cbSameCount]=cbCardData[i];
				break;
			}
		case 2:		//����
			{
				BYTE cbIndex=AnalyseResult.cbDoubleCount++;
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbDoubleCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				break;
			}
		case 3:		//����
			{
				BYTE cbIndex=AnalyseResult.cbThreeCount++;
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbThreeCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				break;
			}
		case 4:		//����
			{
				BYTE cbIndex=AnalyseResult.cbFourCount++;
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount]=cbCardData[i];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+1]=cbCardData[i+1];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+2]=cbCardData[i+2];
				AnalyseResult.cbFourCardData[cbIndex*cbSameCount+3]=cbCardData[i+3];
				break;
			}
		}

		//��������
		i+=cbSameCount-1;
	}

	return;
}

tagAnalyseType CGameLogic::GetType( BYTE bCardData[], BYTE bCardCount )
{
	tagAnalyseType Type;
	ZeroMemory(&Type,sizeof(Type));
	if(bCardCount==0)
	{
		ZeroMemory(&Type,sizeof(Type));
		return Type;
	}
	//�����˿�
	BYTE CardData[9] ;
	CopyMemory(CardData , bCardData , bCardCount) ;
	SortCardList(CardData , bCardCount , enDescend) ;


	BYTE Index[9]={0};
	BYTE Number=0;
	BYTE SameValueCount=1;

	BYTE Num[8]={0};
	//�ж���ֵ��ͬ����
	BYTE bLogicValue=GetCardLogicValue(CardData[0]);
	Index[Number++]=0;
	for(BYTE i=1;i<bCardCount;i++)
	{
		if(bLogicValue==GetCardLogicValue(CardData[i]))
		{
			SameValueCount++;
			Index[Number++]=i;
		}
		if(bLogicValue!=GetCardLogicValue(CardData[i])||i==bCardCount-1)
		{
			if(SameValueCount==1)
			{

			}
			else if(SameValueCount==2)
			{
				Type.bOnePare=true;
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-2];
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-1];
				Type.btOnePare++;
				//printf("%d,%d  ",Index[0],Index[1]);
			}
			else if(SameValueCount==3)
			{
				Type.bOnePare=true;
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-3];
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-2];
				//printf("%d,%d  ",Index[0],Index[1]);
				Type.bThreeSame=true;
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-3];
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-2];
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-1];
				Type.btThreeSame++;
			}
			else if(SameValueCount==4)
			{
				Type.bOnePare=true;
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-4];
				Type.cbOnePare[Num[0]++]=Index[SameValueCount-3];
				//printf("%d,%d  ",Index[0],Index[1]);
				Type.bThreeSame=true;
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-4];
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-3];
				Type.cbThreeSame[Num[2]++]=Index[SameValueCount-2];
				Type.bFourSame=true;
				Type.cbFourSame[Num[6]++]=Index[SameValueCount-4];
				Type.cbFourSame[Num[6]++]=Index[SameValueCount-3];
				Type.cbFourSame[Num[6]++]=Index[SameValueCount-2];
				Type.cbFourSame[Num[6]++]=Index[SameValueCount-1];
				Type.btFourSame++;
			}
			else
			{

			}
			Number=0; 
			ZeroMemory(Index,sizeof(Index));
			Index[Number++]=i;
			SameValueCount=1;
			bLogicValue=GetCardLogicValue(CardData[i]);
		}

	}
	//�ж�����   
	BYTE OnePareCount=Num[0]/2;
	BYTE ThreeSameCount=Num[2]/3;
	if(OnePareCount>=2)
	{
		Type.bTwoPare=true;
		for(BYTE i=0;i<OnePareCount;i++)
		{
			for(BYTE j=i+1;j<OnePareCount;j++)
			{
				Type.cbTwoPare[Num[1]++]=Type.cbOnePare[i*2];
				Type.cbTwoPare[Num[1]++]=Type.cbOnePare[i*2+1];
				Type.cbTwoPare[Num[1]++]=Type.cbOnePare[j*2];
				Type.cbTwoPare[Num[1]++]=Type.cbOnePare[j*2+1];
				Type.btTwoPare++;
			}
		}
	}
	//�жϺ�«
	if(OnePareCount>0&&ThreeSameCount>0)
	{
		for(BYTE i=0;i<ThreeSameCount;i++)
		{
			for(BYTE j=0;j<OnePareCount;j++)
			{
				if(GetCardLogicValue(Type.cbThreeSame[i*3])==GetCardLogicValue(Type.cbOnePare[j*2]))
				{
					continue;
				}
				Type.bGourd=true;
				Type.cbGourd[Num[5]++]=Type.cbThreeSame[i*3];
				Type.cbGourd[Num[5]++]=Type.cbThreeSame[i*3+1];
				Type.cbGourd[Num[5]++]=Type.cbThreeSame[i*3+2];
				Type.cbGourd[Num[5]++]=Type.cbOnePare[j*2];
				Type.cbGourd[Num[5]++]=Type.cbOnePare[j*2+1];
				Type.btGourd++;
			}
		}
	}
	//�ж�˳�Ӽ�ͬ��˳
	Number=0;
	ZeroMemory(Index,sizeof(Index));
	BYTE Straight=1;
	BYTE bStraight=GetCardLogicValue(CardData[0]);
	Index[Number++]=0;
	if(bStraight!=14)
	{
		for(BYTE i=1;i<bCardCount;i++)
		{
			if(bStraight==GetCardLogicValue(CardData[i])+1)
			{
				Straight++;
				Index[Number++]=i;
				bStraight=GetCardLogicValue(CardData[i]);
			}
			if(bStraight>GetCardLogicValue(CardData[i])+1||i==bCardCount-1)
			{
				if(Straight>=5)
				{
					Type.bStraight=true;
					for(BYTE j=0;j<Straight;j++)
					{
						if(Straight-j>=5)
						{
							Type.cbStraight[Num[3]++]=Index[j];
							Type.cbStraight[Num[3]++]=Index[j+1];
							Type.cbStraight[Num[3]++]=Index[j+2];
							Type.cbStraight[Num[3]++]=Index[j+3];
							Type.cbStraight[Num[3]++]=Index[j+4];
							Type.btStraight++;
							//���������ҵ���˳��5��������һ����ֵ��ͬ���ƣ������һ��˳��
							for(int k=j;k<j+5;k++)
							{
								for(int m=0;m<bCardCount;m++)
								{
									if(GetCardLogicValue(CardData[Index[k]])==GetCardLogicValue(CardData[m])&&GetCardColor(CardData[Index[k]])!=GetCardColor(CardData[m]))
									{
										for(int n=j;n<j+5;n++)
										{
											if(n==k)
											{
                                                Type.cbStraight[Num[3]++]=m;
											}
											else
											{
												Type.cbStraight[Num[3]++]=Index[n];
											}
										}
                                        Type.btStraight++;
									}
								}
							}
							

							// 							if(GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+1]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+2]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+3]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+4]]))
							// 							{
							// 								Type.bStraightFlush=true;
							// 								Type.cbStraightFlush[Num[7]++]=Index[j];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+1];
							//                                 Type.cbStraightFlush[Num[7]++]=Index[j+2];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+3];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+4];
							// 							}

						}
						else
						{
							break;
						}
					}

				}
				if(bCardCount-i<5)
				{
					break;
				}
				bStraight=GetCardLogicValue(CardData[i]);
				Straight=1;
				Number=0;
				ZeroMemory(Index,sizeof(Index));
				Index[Number++]=i;
			}
		}

	}
	if(bStraight==14)
	{
		for(BYTE i=1;i<bCardCount;i++)
		{
			if(bStraight==GetCardLogicValue(CardData[i])+1)
			{
				Straight++;
				Index[Number++]=i;
				bStraight=GetCardLogicValue(CardData[i]);
			}
			if(bStraight>GetCardLogicValue(CardData[i])+1||i==bCardCount-1)
			{
				if(Straight>=5)
				{
					Type.bStraight=true;
					for(BYTE j=0;j<Straight;j++)
					{
						if(Straight-j>=5)
						{
							Type.cbStraight[Num[3]++]=Index[j];
							Type.cbStraight[Num[3]++]=Index[j+1];
							Type.cbStraight[Num[3]++]=Index[j+2];
							Type.cbStraight[Num[3]++]=Index[j+3];
							Type.cbStraight[Num[3]++]=Index[j+4];
							Type.btStraight++;
							//���������ҵ���˳��5��������һ����ֵ��ͬ���ƣ������һ��˳��
							for(int k=j;k<j+5;k++)
							{
								for(int m=0;m<bCardCount;m++)
								{
									if(GetCardLogicValue(CardData[Index[k]])==GetCardLogicValue(CardData[m])&&GetCardColor(CardData[Index[k]])!=GetCardColor(CardData[m]))
									{
										for(int n=j;n<j+5;n++)
										{
											if(n==k)
											{
												Type.cbStraight[Num[3]++]=m;
											}
											else
											{
												Type.cbStraight[Num[3]++]=Index[n];
											}
										}
										Type.btStraight++;
									}
								}
							}
							// 							if(GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+1]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+2]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+3]])&&
							// 								GetCardColor(CardData[Index[j]])==GetCardColor(CardData[Index[j+4]]))
							// 							{
							// 								Type.bStraightFlush=true;
							// 								Type.cbStraightFlush[Num[7]++]=Index[j];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+1];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+2];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+3];
							// 								Type.cbStraightFlush[Num[7]++]=Index[j+4];
							// 							}
						}
						else
						{
							break;
						}
					}
				}
				if(bCardCount-i<5)
				{
					break;
				}
				bStraight=GetCardLogicValue(CardData[i]);
				Straight=1;
				Number=0;
				ZeroMemory(Index,sizeof(Index));
				Index[Number++]=i;
			}
		}
		if(GetCardLogicValue(CardData[bCardCount-1])==2)
		{
			Number=0;
			BYTE BackA=1;
			BYTE FrontA=1;
			bStraight=GetCardLogicValue(CardData[0]);
			ZeroMemory(Index,sizeof(Index));
			Index[Number++]=0;
			bStraight=GetCardLogicValue(CardData[bCardCount-1]);
			Index[Number++]=bCardCount-1;
			for(int i=bCardCount-2;i>=0;i--)
			{
				if(bStraight==GetCardLogicValue(CardData[i])-1)
				{
					FrontA++;
					Index[Number++]=i;
					bStraight=GetCardLogicValue(CardData[i]);
				}
			}
			if(FrontA+BackA>=5)
			{
				Type.bStraight=true;
				for(BYTE i=BackA;i>0;i--)
				{
					for(BYTE j=1;j<=FrontA;j++)
					{
						if(i+j==5)
						{
							for(BYTE k=0;k<i;k++)
							{
								Type.cbStraight[Num[3]++]=Index[k];
							}
							for(BYTE k=0;k<j;k++)
							{
								Type.cbStraight[Num[3]++]=Index[k+BackA];
							}
							break;
						}
					}
				}
				//                 for(BYTE i=0;i<FrontA+BackA;i++)
				// 				{
				// 					if(FrontA+BackA-i>=5)
				// 					{
				// 						if(GetCardColor(CardData[Index[i]])==GetCardColor(CardData[Index[i+1]])&&
				// 							GetCardColor(CardData[Index[i]])==GetCardColor(CardData[Index[i+2]])&&
				// 							GetCardColor(CardData[Index[i]])==GetCardColor(CardData[Index[i+3]])&&
				// 							GetCardColor(CardData[Index[i]])==GetCardColor(CardData[Index[i+4]]))
				// 						{
				// 							Type.bStraightFlush=true;
				// 							Type.cbStraightFlush[Num[7]++]=Index[i];
				// 							Type.cbStraightFlush[Num[7]++]=Index[i+1];
				// 							Type.cbStraightFlush[Num[7]++]=Index[i+2];
				// 							Type.cbStraightFlush[Num[7]++]=Index[i+3];
				// 							Type.cbStraightFlush[Num[7]++]=Index[i+4];
				// 						}
				// 					}
				// 				}
			}
		}  

	}
	//�ж�ͬ����ͬ��˳
	Number=0;
	ZeroMemory(Index,sizeof(Index));
	SortCardList(CardData , bCardCount , enColor) ;
	BYTE cbCardData[13];
	CopyMemory(cbCardData , bCardData , bCardCount) ;
	SortCardList(cbCardData , bCardCount , enDescend) ;
	BYTE SameColorCount=1;
	BYTE bCardColor = GetCardColor(CardData[0]) ;
	Index[Number++]=0;
	for(BYTE i=1;i<bCardCount;i++)
	{
		if(bCardColor==GetCardColor(CardData[i]))
		{
			SameColorCount++;
			Index[Number++]=i;
		}
		if(bCardColor!=GetCardColor(CardData[i])||i==bCardCount-1)
		{
			if(SameColorCount>=5)
			{
				Type.bFlush=true;

				for(BYTE j=0;j<SameColorCount;j++)
				{
					for(BYTE k=0;k<bCardCount;k++)
					{
						if(GetCardLogicValue(CardData[Index[j]])==GetCardLogicValue(cbCardData[k])
							&&GetCardColor(CardData[Index[j]])==GetCardColor(cbCardData[k]))
						{
							Index[j]=k;
							break;
						}
					}
				} 
				BYTE SaveIndex=0;
				for(BYTE j=0;j<SameColorCount;j++)
				{
					for(BYTE k=j+1;k<SameColorCount;k++)
					{
						if(Index[j]>Index[k])
						{
							SaveIndex=Index[j];
							Index[j]=Index[k];
							Index[k]=SaveIndex;
						}
					}
				}
				for(BYTE j=0;j<SameColorCount;j++)
				{
					if(SameColorCount-j>=5)
					{
						Type.cbFlush[Num[4]++]=Index[j];
						Type.cbFlush[Num[4]++]=Index[j+1];
						Type.cbFlush[Num[4]++]=Index[j+2];
						Type.cbFlush[Num[4]++]=Index[j+3];
						Type.cbFlush[Num[4]++]=Index[j+4];
						Type.btFlush++;
						if(GetCardLogicValue(cbCardData[Index[j]])==14)
						{
							if(GetCardLogicValue(cbCardData[Index[j+1]])==5&&GetCardLogicValue(cbCardData[Index[j+2]])==4&&
								GetCardLogicValue(cbCardData[Index[j+3]])==3&&GetCardLogicValue(cbCardData[Index[j+4]])==2)
							{
								Type.bStraightFlush=true;
								Type.cbStraightFlush[Num[7]++]=Index[j];
								Type.cbStraightFlush[Num[7]++]=Index[j+1];
								Type.cbStraightFlush[Num[7]++]=Index[j+2];
								Type.cbStraightFlush[Num[7]++]=Index[j+3];
								Type.cbStraightFlush[Num[7]++]=Index[j+4];
								Type.btStraightFlush++;
							}
                            
						}
						if(GetCardLogicValue(cbCardData[Index[j]])==GetCardLogicValue(cbCardData[Index[j+1]])+1&&
							GetCardLogicValue(cbCardData[Index[j]])==GetCardLogicValue(cbCardData[Index[j+2]])+2&&
							GetCardLogicValue(cbCardData[Index[j]])==GetCardLogicValue(cbCardData[Index[j+3]])+3&&
							GetCardLogicValue(cbCardData[Index[j]])==GetCardLogicValue(cbCardData[Index[j+4]])+4)
						{
							Type.bStraightFlush=true;
							Type.cbStraightFlush[Num[7]++]=Index[j];
							Type.cbStraightFlush[Num[7]++]=Index[j+1];
							Type.cbStraightFlush[Num[7]++]=Index[j+2];
							Type.cbStraightFlush[Num[7]++]=Index[j+3];
							Type.cbStraightFlush[Num[7]++]=Index[j+4];
							Type.btStraightFlush++;
						}

					}
					else
					{
						break;
					}
				}
			}
			if(bCardCount-i<5)
			{
				break;
			}
			Number=0;
			ZeroMemory(Index,sizeof(Index));
			SameColorCount=1;
			Index[Number++]=i;
			bCardColor=GetCardColor(CardData[i]);
		}
	}
	//printf("guo2");
	return Type;
}

bool CGameLogic::AppendCard( const BYTE bAppendCard[],BYTE bAppendCount,BYTE bCardData[],BYTE &bCardCount )
{
	ASSERT(bAppendCount+bCardCount<=13);
	for(int i=0;i<bAppendCount;i++)
	{
		bCardData[bCardCount+i]=bAppendCard[i];
	}
	bCardCount+=bAppendCount;
	return true;
}

BYTE CGameLogic::GetCardCount( BYTE bCardData[] )
{
	BYTE Number=0;
	if(0==GetCardLogicValue(bCardData[1])&&0==GetCardLogicValue(bCardData[2]))
	{
		return 0;
	}
	else
	{
		for(BYTE i=0;i<5;i++)
		{
			if(GetCardLogicValue(bCardData[i])>0)
			{
				Number++;
			}
			else
			{
				break;
			}
		}
	}
	return Number;
}


BYTE CGameLogic::ThreeDunAllShuiShu( BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[] )
{
	BYTE AllShuiShu=0;
	tagAnalyseType tagCardType1;
	tagAnalyseType tagCardType2;
	tagAnalyseType tagCardType3;
	ZeroMemory(&tagCardType1,sizeof(tagCardType1));
	ZeroMemory(&tagCardType2,sizeof(tagCardType2));
	ZeroMemory(&tagCardType3,sizeof(tagCardType3));
	tagCardType1=GetType(btFrontCard,3);
	tagCardType2=GetType(btMidCard,5);
	tagCardType3=GetType(btBackCard,5);
	if(tagCardType1.bThreeSame)
	{
		AllShuiShu+=3;
	}
	else 
	{
		AllShuiShu+=1;
	}

	if(tagCardType2.bStraightFlush)
	{
		AllShuiShu+=10;
	}
	else if(tagCardType2.bFourSame)
	{
		AllShuiShu+=8;
	}
	else if(tagCardType2.bGourd)
	{
		AllShuiShu+=2;
	}
	else 
	{
		AllShuiShu+=1;
	}

	if(tagCardType3.bStraightFlush)
	{
		AllShuiShu+=5;
	}
	else if(tagCardType3.bFourSame)
	{
		AllShuiShu+=4;
	}
	else 
	{
		AllShuiShu+=1;
	}
	return AllShuiShu;
}
/*
bool CGameLogic::IsBiggerThanFront( BYTE btFrontFrontCard[],BYTE btFrontMidCard[],BYTE btFrontBackCard[],BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[])
{
	int btFrontShuiShu=ThreeDunAllShuiShu(btFrontFrontCard,btFrontMidCard,btFrontBackCard);
	int btShuiShu=ThreeDunAllShuiShu(btFrontCard,btMidCard,btBackCard);
	tagAnalyseType tagFrontFrontCardType;
	tagAnalyseType tagFrontMidCardType;
	tagAnalyseType tagFrontBackCardType;
	tagAnalyseType tagFrontCardType;
	tagAnalyseType tagMidCardType;
	tagAnalyseType tagBackCardType;
	ZeroMemory(&tagFrontFrontCardType,sizeof(tagFrontFrontCardType));
	ZeroMemory(&tagFrontMidCardType,sizeof(tagFrontMidCardType));
	ZeroMemory(&tagFrontBackCardType,sizeof(tagFrontBackCardType));
	ZeroMemory(&tagFrontCardType,sizeof(tagFrontCardType));
	ZeroMemory(&tagMidCardType,sizeof(tagMidCardType));  
	ZeroMemory(&tagBackCardType,sizeof(tagBackCardType));
	tagFrontFrontCardType=GetType(btFrontFrontCard,3);
	tagFrontMidCardType=GetType(btFrontMidCard,5);
	tagFrontBackCardType=GetType(btFrontBackCard,5);
	tagFrontCardType=GetType(btFrontCard,3);
	tagMidCardType=GetType(btMidCard,5);
	tagBackCardType=GetType(btBackCard,5);
	int btFrontWin=0;
	int btWin=0;
	int btCompare=0;
    
	int btFrontCanWin=0;
	int btCanWin=0;
	if(IsSameCardData(btFrontFrontCard,btFrontCard,3,3)==false)
	{
		if(!(tagFrontFrontCardType.bThreeSame&&tagFrontCardType.bThreeSame))
		{
			if(CompareCard(btFrontFrontCard,btFrontCard,3,3,true))
			{
                
				btCompare++;
			}
			else
			{
				btCompare--;
			}
		}
	}
	
	if(IsSameCardData(btFrontMidCard,btMidCard,5,5)==false)
	{
		if(!(tagFrontMidCardType.bThreeSame&&tagMidCardType.bThreeSame))
		{
			if(CompareCard(btFrontMidCard,btMidCard,5,5,true))
			{
				btCompare++;
			}
			else
			{
				btCompare--;
			}
		}
		else 
		{
			BYTE btSpecialCard[13];
			ZeroMemory(btSpecialCard,sizeof(btSpecialCard));
            if(GetCardType(btMidCard,5,btSpecialCard)>GetCardType(btFrontMidCard,5,btSpecialCard))
			{
               btCompare++;
			}
			else if(GetCardType(btMidCard,5,btSpecialCard)<GetCardType(btFrontMidCard,5,btSpecialCard))
			{
               btCompare--;
			}
		}
	}

	if(IsSameCardData(btFrontBackCard,btBackCard,5,5)==false)
	{
		if(!(tagFrontBackCardType.bThreeSame&&tagBackCardType.bThreeSame))
		{
			if(CompareCard(btFrontBackCard,btBackCard,5,5,true))
			{
				btCompare++;
			}
			else
			{
				btCompare--;
			}
		}
		else 
		{
			BYTE btSpecialCard[13];
			ZeroMemory(btSpecialCard,sizeof(btSpecialCard));
			if(GetCardType(btBackCard,5,btSpecialCard)>GetCardType(btFrontBackCard,5,btSpecialCard))
			{
				btCompare++;
			}
			else if(GetCardType(btBackCard,5,btSpecialCard)<GetCardType(btFrontBackCard,5,btSpecialCard))
			{
				btCompare--;
			}
		}
	}
	//ǰ�����
	if(tagFrontFrontCardType.bThreeSame)
	{
		btFrontWin+=1;
		btFrontCanWin+=3;
	}
	else if(tagFrontFrontCardType.bOnePare)
	{
		btFrontWin+=1;
		btFrontCanWin+=1;
	}
	else
	{
		if(GetCardLogicValue(btFrontFrontCard[0])==14)
		{
            btFrontWin+=0;
			btFrontCanWin+=0;
		}
		else
		{
			btFrontWin+=-1;
			btFrontCanWin+=-1;
		}
		
	}
    //�ڶ���
	if(tagFrontMidCardType.bStraightFlush)
	{
		btFrontWin+=1;
		btFrontCanWin+=10;
	}
	else if(tagFrontMidCardType.bFourSame)
	{
		btFrontWin+=1;
		btFrontCanWin+=8;
	}
	else if(tagFrontMidCardType.bGourd)
	{
		btFrontWin+=1;
		btFrontCanWin+=5;
	}
	else if(tagFrontMidCardType.bFlush)
	{
		btFrontWin+=1;
		btFrontCanWin+=4;
	}
	else if(tagFrontMidCardType.bStraight)
	{
		btFrontWin+=1;
		btFrontCanWin+=3;
	}
	else if(tagFrontMidCardType.bThreeSame)
	{
		btFrontWin+=1;
		btFrontCanWin+=2;
	}
	else if(tagFrontMidCardType.bTwoPare)
	{
		if(GetCardLogicValue(btFrontMidCard[tagFrontMidCardType.cbTwoPare[0]])<6)
		{
			btFrontWin+=0;
			btFrontCanWin+=0;
		}
		else 
		{
			btFrontWin+=1;
			btFrontCanWin+=1;
		}
	}
	else if(tagFrontMidCardType.bOnePare)
	{
		if(GetCardLogicValue(btFrontMidCard[tagFrontMidCardType.cbOnePare[0]])<12)
		{
			btFrontWin+=-1;
			btFrontCanWin+=-1;
		}
		else 
		{
			btFrontWin+=0;
			btFrontCanWin+=0;
		}
	}
	else
	{
		btFrontWin+=-1;
		btFrontCanWin+=-2;
	}
    //������
	if(tagFrontBackCardType.bStraightFlush)
	{
		btFrontWin+=1;
		btFrontCanWin+=5;
	}
	else if(tagFrontBackCardType.bFourSame)
	{
		btFrontWin+=1;
		btFrontCanWin+=4;
	}
	else if(tagFrontBackCardType.bGourd)
	{
		btFrontWin+=1;
		btFrontCanWin+=3;
	}
	else if(tagFrontBackCardType.bFlush)
	{
		if(GetCardLogicValue(btFrontBackCard[tagFrontBackCardType.cbFlush[0]])<11)
		{
			btFrontWin+=0;
            btFrontCanWin+=1;
		}
		else
		{
			btFrontWin+=1;
			btFrontCanWin+=2;
		}
	}
	else if(tagFrontBackCardType.bStraight)
	{
		if(GetCardLogicValue(btFrontBackCard[tagFrontBackCardType.cbStraight[0]])<11)
		{
			btFrontWin+=-1;
			btFrontCanWin+=-1;
		}
		else
		{
			btFrontWin+=0;
			btFrontCanWin+=0;
		}
	}
	else
	{
		if(tagFrontBackCardType.bThreeSame)
		{
           btFrontCanWin+=-2;
		}
		else if(tagFrontBackCardType.bTwoPare)
		{
			btFrontCanWin+=-3;
		}
		else if(tagFrontBackCardType.bOnePare)
		{
			btFrontCanWin+=-4;
		}
		else 
		{
			btFrontCanWin+=-5;
		}
		btFrontWin+=-1;
	}
	//���ڵ���
	if(tagFrontCardType.bThreeSame)
	{
		btWin+=1;
		btCanWin+=3;
	}
	else if(tagFrontCardType.bOnePare)
	{
		btWin+=1;
		btCanWin+=1;
	}
	else 
	{
		if(GetCardLogicValue(btFrontCard[0])==14)
		{
			btWin+=0;
			btCanWin+=0;
		}
		else
		{
			btWin+=-1;
			btCanWin+=-1;
		}
	}
    //�ڶ���
	if(tagMidCardType.bStraightFlush)
	{
		btWin+=1;
		btCanWin+=10;
	}
	else if(tagMidCardType.bFourSame)
	{
		btWin+=1;
		btCanWin+=8;
	}
	else if(tagMidCardType.bGourd)
	{
		btWin+=1;
		btCanWin+=5;
	}
	else if(tagMidCardType.bFlush)
	{
		btWin+=1;
		btCanWin+=4;
	}
	else if(tagMidCardType.bStraight)
	{
		btWin+=1;
		btCanWin+=3;
	}
	else if(tagMidCardType.bThreeSame)
	{
		btWin+=1;
		btCanWin+=2;
	}
	else if(tagMidCardType.bTwoPare)
	{
		if(GetCardLogicValue(btMidCard[tagMidCardType.cbTwoPare[0]])<6)
		{
			btWin+=0;
			btCanWin+=0;
		}
		else 
		{
			btWin+=1;
			btCanWin+=1;
		}
	}
	else if(tagMidCardType.bOnePare)
	{
		if(GetCardLogicValue(btMidCard[tagMidCardType.cbOnePare[0]])<12)
		{
			btWin+=-1;
			btCanWin+=-1;
		}
		else 
		{
			btWin+=0;
			btCanWin+=0;
		}
	}
	else
	{
		btWin+=-1;
		btCanWin+=-2;
	}
    //������
	if(tagBackCardType.bStraightFlush)
	{
		btWin+=1;
		btCanWin+=5;
	}
	else if(tagBackCardType.bFourSame)
	{
		btWin+=1;
		btCanWin+=4;
	}
	else if(tagBackCardType.bGourd)
	{
		btWin+=1;
		btCanWin+=3;
	}
	else if(tagBackCardType.bFlush)
	{
		if(GetCardLogicValue(btBackCard[tagBackCardType.cbFlush[0]])<11)
		{
			btWin+=0;
			btCanWin+=1;
		}
		else
		{
			btWin+=1;
			btCanWin+=2;
		}
	}
	else if(tagBackCardType.bStraight)
	{
		if(GetCardLogicValue(btBackCard[tagBackCardType.cbStraight[0]])<11)
		{
			btWin+=-1;
			btCanWin+=-1;
		}
		else
		{
			btWin+=0;
			btCanWin+=0;
		}
	}
	else
	{
		if(tagBackCardType.bThreeSame)
		{
			btCanWin+=-2;
		}
		else if(tagBackCardType.bTwoPare)
		{
			btCanWin+=-3;
		}
		else if(tagBackCardType.bOnePare)
		{
			btCanWin+=-4;
		}
		else 
		{
			btCanWin+=-5;
		}
		btWin+=-1;
	}

	if(btShuiShu==btFrontShuiShu)
	{
		if(btWin>btFrontWin)
		{
			return true;
		}
		else if(btWin==btFrontWin)
		{
			if(btCompare>=1)
			{
				return true;
			}
			else
			{
				if(btCanWin>btFrontCanWin)
				{
					return true;
				}
				else 
				{
					return false;
				}
			}
		}
		else 
		{
			return false;
		}
	}
	else if(btShuiShu==btFrontShuiShu+1)
	{
		if(btFrontWin-btWin>=4)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if(btShuiShu+1==btFrontShuiShu)
	{
		if(btWin-btFrontWin>=4)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if(btShuiShu>btFrontShuiShu)
	{
		return true;
	}
	else if(btShuiShu<btFrontShuiShu)
	{
		return false;
	}
	return false;
}

*/

bool CGameLogic::IsSameCardData( BYTE btFirstCard[],BYTE btNextCard[],BYTE btFirstCount,BYTE btNextCount)
{
   if(btNextCount!=btFirstCount)
   {
	   return false;
   }
   for(int i=0;i<btFirstCount;i++)
   {
      if(btFirstCard[i]!=btNextCard[i])
	  {
         return false;
	  }
   }
   return true;
}

BYTE CGameLogic::GetCardType(BYTE cbCardData[], BYTE cbCardCount)
{
	ASSERT(cbCardCount == TYPE_MAX_COUNT);

	if (cbCardCount == TYPE_MAX_COUNT)
	{
		//��������
		bool cbSameColor = true, bLineCard = true;
		BYTE cbFirstColor = GetCardColor(cbCardData[0]);
		BYTE cbFirstValue = GetCardLogicValue(cbCardData[0]);

		//���η���
		for (BYTE i = 1; i < cbCardCount; i++)
		{
			//���ݷ���
			if (GetCardColor(cbCardData[i]) != cbFirstColor)
			{
				cbSameColor = false;
			}
			if (cbFirstValue != (GetCardLogicValue(cbCardData[i]) + i))
			{
				bLineCard = false;
			}

			//�����ж�
			if ((cbSameColor == false) && (bLineCard == false))
			{
				break;
			}
		}

		//����A32
		if (!bLineCard)
		{
			bool bOne = false, bTwo = false, bThree = false;
			for (BYTE i = 0; i < TYPE_MAX_COUNT; i++)
			{
				if (GetCardValue(cbCardData[i]) == 1)
				{
					bOne = true;
				}
				else if (GetCardValue(cbCardData[i]) == 2)
				{
					bTwo = true;
				}
				else if (GetCardValue(cbCardData[i]) == 3)
				{
					bThree = true;
				}
			}
			if (bOne && bTwo && bThree)
			{
				bLineCard = true;
			}
		}

		//˳������
		if ((cbSameColor) && (bLineCard))
		{
			return CT_SHUN_JIN;
		}

		//˳������
		if ((!cbSameColor) && (bLineCard))
		{
			return CT_SHUN_ZI;
		}

		//������
		if ((cbSameColor) && (!bLineCard))
		{
			return CT_JIN_HUA;
		}

		//���η���
		bool bDouble = false, bPanther = true;

		//���Ʒ���
		for (BYTE i = 0; i < cbCardCount - 1; i++)
		{
			for (BYTE j = i + 1; j < cbCardCount; j++)
			{
				if (GetCardLogicValue(cbCardData[i]) == GetCardLogicValue(cbCardData[j]))
				{
					bDouble = true;
					break;
				}
			}
			if (bDouble)
			{
				break;
			}
		}

		//����(����)����
		for (BYTE i = 1; i < cbCardCount; i++)
		{
			if (bPanther && cbFirstValue != GetCardLogicValue(cbCardData[i]))
			{
				bPanther = false;
			}
		}

		//���Ӻͱ����ж�
		if (bDouble == true)
		{
			return (bPanther) ? CT_BAO_ZI : CT_DOUBLE;
		}

		//����235
		bool bTwo = false, bThree = false, bFive = false;
		for (BYTE i = 0; i < cbCardCount; i++)
		{
			if (GetCardValue(cbCardData[i]) == 2)
			{
				bTwo = true;
			}
			else if (GetCardValue(cbCardData[i]) == 3)
			{
				bThree = true;
			}
			else if (GetCardValue(cbCardData[i]) == 5)
			{
				bFive = true;
			}
		}
		if (bTwo && bThree && bFive)
		{
			return CT_SPECIAL;
		}
	}

	return CT_SINGLE;
}

BYTE CGameLogic::CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount)
{
	//���ñ���
	BYTE FirstData[TYPE_MAX_COUNT], NextData[TYPE_MAX_COUNT];
	CopyMemory(FirstData, cbFirstData, sizeof(FirstData));
	CopyMemory(NextData, cbNextData, sizeof(NextData));

	//��С����
	SortCardList(FirstData, cbCardCount);
	SortCardList(NextData, cbCardCount);

	//��ȡ����
	BYTE cbNextType = GetCardType(NextData, cbCardCount);
	BYTE cbFirstType = GetCardType(FirstData, cbCardCount);

	//�����������
// 	if ((cbNextType + cbFirstType) == (CT_SPECIAL + CT_BAO_ZI))
// 	{
// 		return (BYTE)(cbFirstType > cbNextType);
// 	}

	//��ԭ��������
	if (cbNextType == CT_SPECIAL)
	{
		cbNextType = CT_SINGLE;
	}
	if (cbFirstType == CT_SPECIAL)
	{
		cbFirstType = CT_SINGLE;
	}

	//�����ж�
	if (cbFirstType != cbNextType)
	{
		return (cbFirstType > cbNextType) ? 1 : 0;
	}

	//������
	switch (cbFirstType)
	{
	case CT_BAO_ZI:     //����
	case CT_SINGLE:     //����
	case CT_JIN_HUA:    //��
	{
							//�Ա���ֵ
							for (BYTE i = 0; i<cbCardCount; i++)
							{
								BYTE cbNextValue = GetCardLogicValue(NextData[i]);
								BYTE cbFirstValue = GetCardLogicValue(FirstData[i]);
								if (cbFirstValue != cbNextValue)
								{
									return (cbFirstValue>cbNextValue) ? 1 : 0;
								}
							}
							return DRAW;
	}
	case CT_SHUN_ZI:    //˳��
	case CT_SHUN_JIN:   //˳�� 432>A32
	{
							BYTE cbNextValue = GetCardLogicValue(NextData[0]);
							BYTE cbFirstValue = GetCardLogicValue(FirstData[0]);

							//����A32
							if (cbNextValue == 14 && GetCardLogicValue(NextData[cbCardCount - 1]) == 2)
							{
								cbNextValue = 3;
							}
							if (cbFirstValue == 14 && GetCardLogicValue(FirstData[cbCardCount - 1]) == 2)
							{
								cbFirstValue = 3;
							}

							//�Ա���ֵ
							if (cbFirstValue != cbNextValue)
							{
								return (cbFirstValue > cbNextValue) ? 1 : 0;
							};
							return DRAW;
	}
	case CT_DOUBLE:     //����
	{
							BYTE cbNextValue = GetCardLogicValue(NextData[0]);
							BYTE cbFirstValue = GetCardLogicValue(FirstData[0]);

							//���Ҷ���/����
							BYTE bNextDouble = 0, bNextSingle = 0;
							BYTE bFirstDouble = 0, bFirstSingle = 0;
							if (cbNextValue == GetCardLogicValue(NextData[1]))
							{
								bNextDouble = cbNextValue;
								bNextSingle = GetCardLogicValue(NextData[cbCardCount - 1]);
							}
							else
							{
								bNextDouble = GetCardLogicValue(NextData[cbCardCount - 1]);
								bNextSingle = cbNextValue;
							}
							if (cbFirstValue == GetCardLogicValue(FirstData[1]))
							{
								bFirstDouble = cbFirstValue;
								bFirstSingle = GetCardLogicValue(FirstData[cbCardCount - 1]);
							}
							else
							{
								bFirstDouble = GetCardLogicValue(FirstData[cbCardCount - 1]);
								bFirstSingle = cbFirstValue;
							}

							if (bNextDouble != bFirstDouble)
							{
								return (bFirstDouble > bNextDouble) ? 1 : 0;
							}
							if (bNextSingle != bFirstSingle)
							{
								return (bFirstSingle > bNextSingle) ? 1 : 0;
							}
							return DRAW;
	}
	}

	return DRAW;
}

BYTE CGameLogic::GetSpecialCardType(BYTE cbFirstData[], BYTE cbMidData[], BYTE cbBackDatd[], int nFirstCount, int nMidCount, int nBackCount, int nType)
{
	//����У��
	ASSERT(nFirstCount == 3 && nMidCount == 3 && nBackCount == 3);
	if (nFirstCount != 3 || nMidCount != 3|| nBackCount != 3)
	{
		return CT_INVALID;
	}

	BYTE bTmpCard[9] = { 0 };
	CopyMemory(bTmpCard, cbFirstData, nFirstCount);
	CopyMemory(bTmpCard + 3, cbMidData, nMidCount);
	CopyMemory(bTmpCard + 6, cbBackDatd, nBackCount);
	SortCardList(bTmpCard, sizeof(bTmpCard));
	tagAnalyseData AnalyseData;
	memset(&AnalyseData, 0, sizeof(tagAnalyseData));
	AnalyseCard(bTmpCard, sizeof(bTmpCard), AnalyseData);

	switch (nType)
	{
	case 1:
	{
			  // ����
			  if (GetCardType(cbFirstData, nFirstCount) == CT_JIN_HUA
				  && GetCardType(cbMidData, nMidCount) == CT_JIN_HUA
				  && GetCardType(cbBackDatd, nBackCount) == CT_JIN_HUA)
			  {
				  return CT_SPECIAL_SANQING;
			  }
			  break;
	}
	case 2:
	{
			  // ȫ��
			  int nCountBlack = 0;
			  for (int i = 0; i < 9; i++)
			  {
				  if (GetCardColor(bTmpCard[i]) == 0x03 || GetCardColor(bTmpCard[i]) == 0x01)
				  {
					  nCountBlack++;
				  }
			  }

			  if (nCountBlack == 9)
			  {
				  return CT_SPECIAL_QUANHEI;
			  }
			  break;

	}
	case 3:
	{
			  // ȫ��
			  int nCountRed = 0;
			  for (int i = 0; i < 9; i++)
			  {
				  if (GetCardColor(bTmpCard[i]) == 0x02 || GetCardColor(bTmpCard[i]) == 0x00)
				  {
					  nCountRed++;
				  }
			  }

			  if (nCountRed == 9)
			  {
				  return CT_SPECIAL_QUANHONG;
			  }
			  break;
	}
	case 4:
	{
			  // ˫˳��
			  int nCount = 0;
			  if (GetCardType(cbFirstData, nFirstCount) == CT_SHUN_JIN)
			  {
				  nCount++;
			  }

			  if (GetCardType(cbMidData, nMidCount) == CT_SHUN_JIN)
			  {
				  nCount++;
			  }

			  if (GetCardType(cbBackDatd, nBackCount) == CT_SHUN_JIN)
			  {
				  nCount++;
			  }

			  if (nCount == 2)
			  {
				  return CT_SPECIAL_SHUANGSHUNQING;
			  }

			  break;
	}	
	case 5:
	{
			  // ˫����
			  if (GetCardType(cbMidData, nMidCount) == CT_BAO_ZI && GetCardType(cbBackDatd, nBackCount))
			  {
				  return CT_SPECIAL_SHUANGSANTIAO;
			  }
			  break;
	}
		
	case 6:
	{
			  // ȫ����
			  if (AnalyseData.bThreeCount == 3)
			  {
				  return CT_SPECIAL_QUANSANTIAO;
			  }
			  break;
	}
		
	case 7:
	{
			  // �ĸ�ͷ
			  if (AnalyseData.bFourCount == 1)
			  {
				  if (GetCardType(cbFirstData, nFirstCount) == CT_BAO_ZI
					  || GetCardType(cbMidData, nMidCount) == CT_BAO_ZI
					  || GetCardType(cbBackDatd, nBackCount) == CT_BAO_ZI)
				  {
					  return CT_SPECIAL_SIGETOU;
				  }
			  }
			  break;
	}
		
	case 8: 
	{
			  // �ĸ�ͷ2
			  if (AnalyseData.bFourCount == 2)
			  {
				  int nCountTmp = 0;
				  if (GetCardType(cbFirstData, nFirstCount) == CT_BAO_ZI)
				  {
					  nCountTmp++;
				  }

				  if (GetCardType(cbMidData, nMidCount) == CT_BAO_ZI)
				  {
					  nCountTmp++;
				  }

				  if (GetCardType(cbBackDatd, nBackCount) == CT_BAO_ZI)
				  {
					  nCountTmp++;
				  }

				  if (nCountTmp == 2)
				  {
					  return CT_SPECIAL_SIGETOU2;
				  }

			  }

			  break;
	}
		
	case 9:
	{
			  // һ����
			  BYTE bFirstCard[3] = { 0 };
			  BYTE bMidCard[3] = { 0 };
			  BYTE bBackCard[3] = { 0 };
			  CopyMemory(bFirstCard, cbFirstData, nFirstCount);
			  CopyMemory(bMidCard, cbMidData, nMidCount);
			  CopyMemory(bBackCard, cbBackDatd, nBackCount);
			  SortCardList(bFirstCard, sizeof(bFirstCard), enAscend);
			  SortCardList(bMidCard, sizeof(bMidCard), enAscend);
			  SortCardList(bBackCard, sizeof(bBackCard), enAscend);
			  // һ����
			  if (GetCardType(cbFirstData, nFirstCount) == CT_SHUN_ZI
				  && GetCardType(cbMidData, nMidCount) == CT_SHUN_ZI
				  && GetCardType(cbBackDatd, nBackCount) == CT_SHUN_ZI)
			  {
				  if (GetCardLogicValue(bBackCard[0]) - GetCardLogicValue(bMidCard[2]) == 1
					  && GetCardLogicValue(bMidCard[0]) - GetCardLogicValue(bFirstCard[2]) == 1)
				  {
					  return CT_SPECIAL_YITIAOLONG;
				  }
			  }

			  break;
	}
	default:
	{
			   break;
	}
	}

	return CT_INVALID;
}

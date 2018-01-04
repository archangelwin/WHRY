#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////
// ��������
#define CT_INVALID			0				  // ��������
#define CT_SINGLE			1                 //��������
#define CT_DOUBLE			2                 //��������
#define CT_SHUN_ZI          3                 //˳������
#define CT_JIN_HUA          4                 //������
#define CT_SHUN_JIN         5                 //˳������
#define CT_BAO_ZI			6                 //��������
#define CT_SPECIAL          7                 //��������
#define TYPE_MAX_COUNT		3
#define DRAW                2                 //�;�����

// ��������
#define CT_ALL_TONGHUA					31								// ȫͬ��
#define CT_ALL_SHANTIAO					32								// ȫ����
#define CT_ALL_TONGSE					33								// ͬɫ

#define CT_SPECIAL_SANQING				34								// ����
#define CT_SPECIAL_QUANHEI				35								// ȫ��
#define CT_SPECIAL_QUANHONG				36								// ȫ��
#define CT_SPECIAL_SHUANGSHUNQING		37								// ˫˳��
#define CT_SPECIAL_SHUANGSANTIAO		38								// ˫����
#define CT_SPECIAL_QUANSANTIAO			39								// ȫ����
#define CT_SPECIAL_SIGETOU				40								// �ĸ�ͷ
#define CT_SPECIAL_SIGETOU2				41								// �ĸ�ͷ
#define CT_SPECIAL_YITIAOLONG			42								// һ����
#define CT_SPECIAL_TONGGUAN				43								// ͨ��
//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0							//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F							//��ֵ����

//�����ṹ
struct tagAnalyseData
{
	BYTE							bOneCount ;								//������Ŀ
	BYTE							bTwoCount ;								//������Ŀ 
	BYTE							bThreeCount ;							//������Ŀ
	BYTE							bFourCount ;							//������Ŀ
	BYTE							bOneFirst[13];							//����λ��
	BYTE							bTwoFirst[13];							//����λ��
	BYTE							bThreeFirst[13];						//����λ��
	BYTE							bFourFirst[13];							//����λ��
	bool							bStraight;								//�Ƿ�ͬ��
};

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbFourCount;						//������Ŀ
	BYTE 							cbThreeCount;						//������Ŀ
	BYTE 							cbDoubleCount;						//������Ŀ
	BYTE							cbSignedCount;						//������Ŀ
	BYTE							cbFourCardData[13];					//�����˿�
	BYTE							cbThreeCardData[13];				//�����˿�
	BYTE							cbDoubleCardData[13];				//�����˿�
	BYTE							cbSignedCardData[13];				//�����˿�
};
//�����ṹ
struct tagAnalyseType
{
	bool                            bOnePare;                           //��һ��
	bool                            bTwoPare;                           //������
	bool                            bThreeSame;                         //������
	bool                            bStraight;                          //��˳��
	bool                            bFlush;                             //��ͬ��
	bool                            bGourd;                             //�к�«
	bool                            bFourSame;                          //����֧
	bool                            bStraightFlush;                     //��ͬ��˳
	BYTE                            cbOnePare[100];                     //һ�Ե����
	BYTE                            cbTwoPare[100];                     //���Ե����
	BYTE                            cbThreeSame[100];                   //���������
	BYTE                            cbStraight[100];                    //˳�ӵ����
	BYTE                            cbFlush[100];                       //ͬ�������
	BYTE                            cbGourd[100];                       //��«�����
	BYTE                            cbFourSame[100];                    //��֧�����
	BYTE                            cbStraightFlush[100];               //ͬ��˳�����
	bool                            bbOnePare[20];                      //����һ�Ա�־����
	bool                            bbTwoPare[20];                      //���ж��Ա�־����
	bool                            bbThreeSame[20];                    //����������־����
	bool                            bbStraight[20];                     //����˳�ӱ�־����
	bool                            bbFlush[20];                        //����ͬ����־����
	bool                            bbGourd[20];                        //���к�«��־����
	bool                            bbFourSame[20];                     //������֧��־����
	bool                            bbStraightFlush[20];                //����ͬ��˳��־����
	BYTE                            btOnePare;                          //һ�Ե����� ����
	BYTE                            btThreeSame;                        //��������   ����

    BYTE                            btTwoPare;                          //���Ե�����
	BYTE                            btStraight;                         //˳�ӵ�����
	BYTE                            btFlush;                            //ͬ��������
	BYTE                            btGourd;                            //��«������
	BYTE                            btFourSame;                         //��֧������
	BYTE                            btStraightFlush;                    //ͬ��˳������ 
};
//////////////////////////////////////////////////////////////////////////
//��������
enum enSortCardType
{
	enDescend ,																//�������� 
	enAscend ,																//��������
	enColor																	//��ɫ����
};


//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����
    
	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();
    BYTE                            btCardSpecialData[9];
	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE cbCardData[], BYTE cbCardCount);
	BYTE CompareCard(BYTE cbFirstData[], BYTE cbNextData[], BYTE cbCardCount);

	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE btSpecialCard[]);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //ʮ������ǰ����λ��ʾ�Ƶ���ֵ
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //ʮ�����ƺ�����λ��ʾ�ƵĻ�ɫ 


	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	void RandCardListEx(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);
	//����˿�
	bool AppendCard(const BYTE bAppendCard[],BYTE bAppendCount,BYTE bCardData[],BYTE &bCardCount);
	//�õ��Ƶ�����
	BYTE GetCardCount(BYTE bCardData[]);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);

	//���8������
	tagAnalyseType GetType(BYTE bCardData[], BYTE bCardCount);
	//�ڲ�����

	//�����˿�
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;

	//�����ܹ���ˮ��
	BYTE ThreeDunAllShuiShu(BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[]);
	//�Ƿ��ǰ��Ĵ�
	//bool IsBiggerThanFront(BYTE btFrontFrontCard[],BYTE btFrontMidCard[],BYTE btFrontBackCard[],BYTE btFrontCard[],BYTE btMidCard[],BYTE btBackCard[]);
	//�������Ƿ�һ��
	bool IsSameCardData(BYTE btFirstCard[],BYTE btNextCard[],BYTE btFirstCount,BYTE btNextCount);
	BYTE GetSpecialCardType(BYTE cbFirstData[], BYTE cbMidData[], BYTE cbBackDatd[], int nFirstCount, int nMidCount, int nBackCount, int nType);
	void RandCardListEx2(BYTE *pCard, BYTE bBufferCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
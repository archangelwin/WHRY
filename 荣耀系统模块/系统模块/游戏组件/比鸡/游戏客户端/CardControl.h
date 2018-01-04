#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

#include "GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//�˿˶���
//	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//���� A - K
//	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//÷�� A - K
//	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//���� A - K
//	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//���� A - K

//ö�ٶ���

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//��Ϣ����
#define	IDM_LEFT_HIT_CARD			(WM_USER+500)						//����˿�
#define	IDM_RIGHT_HIT_CARD			(WM_USER+501)						//�һ��˿�

//��ඨ��
#define DEF_CARD_SHOOT				20									//Ĭ�ϼ��
#define DEF_CARD_V_SPACE			10									//Ĭ�ϼ��
#define DEF_CARD_H_SPACE			10									//Ĭ�ϼ��

//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};

//�ڷŷ�ʽ
enum enCardSide
{
	enRight ,						//�ұ߰ڷ�
	enLeft ,						//��߰ڷ�
	enHorizon,						//ˮƽ�ڷ�
	enError,						//����ڷ�
};

//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							bCardData;							//�˿�����
	bool							bMultiSelect;						//�Ƿ�ס��������ѡ
};

//////////////////////////////////////////////////////////////////////////

//�˿˿ؼ�
class CCardControl  //public CWnd
{
	//���ñ���
protected:
	bool							m_bDisplay;							//��ʾ��־
	bool							m_bHorizontal;						//��ű�־
	bool							m_bPositively;						//��Ӧ��־
	enCardSide						m_enSide;							//�˿�λ��

	bool                            m_bDrawCard;                        //�Ƿ���
	bool                            m_bLButtonUp;                       //�Ƿ�����
    bool                            m_bMoveCard;                        //�Ƿ�����
	//�������
public:
	float							m_dwCardHSpace;						//������
	float							m_dwCardVSpace;						//������
	float							m_dwShootAltitude;					//����߶�
	bool							m_bDrawCardFlash;					///<�Ƿ񲥷�FLASH����
	int								m_nI;								///<���ż�����
	CWHArray<tagCardItem>			m_CardDataItem;						//�˿�����
	CSize							m_PosCard;							//��������
	CSize							m_SendCardFlash;					///<���ƶ����Ŀ�ȸ߶�
	int								m_NumCard;							///<���Ƶ�����
	int								m_nCardNum;							///<�Ƶ��������������
	int								m_nShuffleNum;						///<��ʾϴ�ƶ����ڼ�֡
	bool							m_bAyaka;							//�Ƿ���Ʋʻ�
	BYTE							m_nAyakaFrames;						//�ʻ�֡��
	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ�ã��˿��ƿؼ���������λ��	��
	CPoint                          m_BenchmarkPosOld;                  //ԭʼ��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//���б���
protected:
	DWORD							m_dwCurrentIndex;					//�������

	//�ڲ�����
protected:
	CRgn							m_CardRegion;						//�ؼ�����
	CWnd *							m_pSinkWindow;						//�ص�����
	

	//��Դ����
protected:
	
		
	static bool						m_bLoad;							//���ر�־
	
	static CSize					m_CardSize;							//�˿˴�С
	static CSize                    m_BigCardSize;                      //���ƴ��˿˴�С
    static CSize                    m_User2BigCardSize;                 //�Լ��˿˵Ĵ�С
	static CSize					m_RightCardSize;			        //�˿˴�С
	
	///ZBD

	CPngImage                        m_ISendCard;						//����
	CPngImage                        m_IShuffleCard;					//ϴ��
	CPngImage                        m_ImageCard;						//ͼƬ��Դ
	CPngImage                        m_ImageBigCard;                    //��ͼƬ��Դ
	CPngImage                        m_CardType;						//������
	CPngImage                        m_CardSpecialType;					//����������
	CPngImage                        m_MidThreeDeouble;					//�жպ�«
	CPngImage                        m_Ayaka;							//�ʻ�

	bool                            m_bBigCard;                         //�Ƿ��Ǵ��˿�
	bool                            m_bCompare;                         //�ȷ�
	bool                            m_bDrawType;                       //��������
	int                             m_nTypeX;                          //
	int                             m_nTypeY;                          //
	//int                           m_nCardType;                       //����
 	bool                            m_bMyselfUser;                     //�Ƿ����Լ����
	BYTE                            m_DrawType;                        //�ƻ�������
	bool                            m_bDrawFire;                        //����ǹ
	int                             m_nFireGunStatus;                   //��ǹ12�����



	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//���ú���
public:
	//���ô���
	void SetSinkWindow(CWnd * pSinkWindow);

	//�ؼ�����
public:
	//�����˿�
	void ShootAllCard(bool bShoot);
	//������ʾ
	void SetDisplayFlag(bool bDisplay);
	//���÷���
	void SetDirection(bool bHorizontal);
	//����λ��
	void SetCardSide(enCardSide	enSide) ;
	//������Ӧ
	void SetPositively(bool bPositively);
	//���ü��
	void SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace, DWORD dwShootAltitude);
	//��׼λ��
	void SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	void SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	void SetbBigCard(bool bBigCard);
	void SetbCompare(bool bCompare);
	void SetbMyselfUser(bool bMyselfUser);
	void SetbDrawType(bool bDrawType);
	void SetDrawTypePoint(int nx,int ny);
	void SetDraw();
	bool GetDraw();
	void SetCardType(BYTE CardType);

	void SetbDrawFire(bool bDrawFire);
	void SetFireGunStatus(int nFireGunStatus);
	void SetDrawCard(bool bDrawCard);
	//void DrawCard(CDC *pDC);

	//�˿˿���
public:
	//��ȡ��Ŀ
	DWORD GetCardCount();
	//��ȡ�˿�
	DWORD GetShootCard(BYTE bCardData[], DWORD dwMaxCount);
	//�����˿�
	DWORD SetCardData(const BYTE bCardData[], DWORD dwCardCount);
	//��ȡ�˿�
	DWORD GetCardData(BYTE bCardData[], DWORD dwMaxCount);
	//��ȡ�˿�
	DWORD GetCardData(BYTE bCardData);
	//�����˿�
	bool SetCardItem(BYTE bCardData, DWORD dwIndex);
	//�����˿�
	DWORD SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount);
	//���õ����˿�
	DWORD SetShootCard(const BYTE bCardData[], DWORD dwCardCount);
	//���ûָ��˿�
    DWORD SetRegainCard(DWORD dwCardCount);	

	//�ڲ�����
private:
	//����λ��
	void RectifyControl();
	//�����л�
	DWORD SwitchCardPoint(const CPoint & MousePoint);

	//��Ϣ����
// protected:
// 	//�ػ�����
// 	afx_msg void OnPaint();
// 	//������Ϣ
// 	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
// 	//�����Ϣ
   // void OnRButtonUp(UINT nFlags, CPoint Point);
// 
// 	DECLARE_MESSAGE_MAP()
public:
	int OnLButtonUp(CPoint Point);
	// 	//�����Ϣ
	void OnLButtonDown(CPoint Point);
	// 	//�����Ϣ
	// 	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);
	void OnMouseMove(CPoint point);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString    GetAbsolutePath() 
	{
		CString    sPath;
		GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
		sPath.ReleaseBuffer();
		int    nPos;
		nPos=sPath.ReverseFind('\\');
		sPath=sPath.Left(nPos);
		return    sPath;   
	}

	CString  m_strPath;
	void ShuffleCardFlash(CDC* pDc,int nWidth,int nHeight);  ///<ϴ�ƶ���
	void SendCardFlash0(CDC* pDc,int nWidth,int nHeight);  ///<���ƶ���0
	void SendCardFlash(CDC* pDc,int nWidth,int nHeight);  ///<���ƶ���
	void DrawCard(CDC* pDc);
	void DrawCardFlash(CDC* pDc,int nWidth,int nHeight);
	void LoadCardImage(CPngImage CardImage,CPngImage BigCardImage,CPngImage SendCard,CPngImage ShuffleCard);  
	//DECLARE_MESSAGE_MAP()
	//afx_msg void OnBnClickedOk();
	//afx_msg void OnBnClickedEnableSound();
};
//////////////////////////////////////////////////////////////////////////

#endif
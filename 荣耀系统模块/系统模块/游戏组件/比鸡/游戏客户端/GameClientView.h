#pragma once

#include "Stdafx.h"
#include "CardControl.h"
#include "GameLogic.h"
#include "MulSecCardControl.h"
#include "ScoreView.h"
#include "MysteryDlg.h"
#include "ClientControl.h"

/////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define IDM_START					(WM_APP+100)						//��ʼ��Ϣ
#define IDM_END                     (WM_APP+101)                        //������Ϣ
#define IDM_OK				        (WM_APP+102)						//̯����Ϣ
#define IDM_FRONTCARD				(WM_APP+103)						//����ǰ��  
#define IDM_MIDCARD					(WM_APP+104)						//�����ж�
#define IDM_BACKCARD				(WM_APP+105)						//���ú��
#define IDM_DESCEND_SORT			(WM_APP+106)						//������Ϣ
#define IDM_ASCEND_SORT				(WM_APP+107)						//������Ϣ
#define IDM_COLOR_SORT				(WM_APP+108)						//��ɫ��Ϣ
#define IDM_AUTO_OUT_CARD           (WM_APP+109)                        //�Զ�����
#define IDM_RESTORE_CARD			(WM_APP+110)                        //�ָ��˿�

#define IDM_ONEPARE                 (WM_APP+111)                        //һ��
#define IDM_TWOPARE                 (WM_APP+112)                        //����
#define IDM_THREESAME               (WM_APP+113)                        //����
#define IDM_STRAIGHT                (WM_APP+114)                        //˳��
#define IDM_FLUSH                   (WM_APP+115)                        //ͬ��
#define IDM_GOURD                   (WM_APP+116)                        //��«
#define IDM_FOURSAME                (WM_APP+117)                        //��֧
#define IDM_STRAIGHTFLUSH           (WM_APP+118)                        //ͬ��˳

#define IDM_RECYCLEFRONTCARD        (WM_APP+119)                        //���յ�һ��
#define IDM_RECYCLEMIDCARD          (WM_APP+120)                        //���յڶ���
#define IDM_RECYCLEBACKCARD         (WM_APP+121)                        //���յ�����
#define IDM_RECYCLEALLCARD          (WM_APP+122)                        //����ȫ��
#define IDM_PAIXU					(WM_APP+123)						//����
#define IDM_SUBMIT					(WM_APP+124)                        //ȷ����������Ϣ
#define IDM_CANCEL					(WM_APP+125)                        //ȡ����������Ϣ

////////////////////////////////////////////////////////////////////////////////////////////////

//�˿�����
enum enCardType
{
	enHandCard,						//�����˿�
	enSegmentCard,					//�ֶ��˿�
	enAllSegmentCard,				//���зֶ�
	enCompareCard,					//�Ƚ��˿�
	enBankCard,						//ׯ���˿�
	enErrorType,					//��������
};

//��Ϸ��ͼ
class CGameClientView : public CGameFrameViewGDI
{
	//�������
protected:
	CString							m_strShowCompare;					//��ʾ�Ƚ�
	bool							m_bShowCompare;						//��ʾ�Ƚ�
	enSegmentType					m_enSegmentType ;					//�Ƚ�����
	CPoint							m_UserAddrInfo[GAME_PLAYER];		//��ַ��Ϣ
	bool                            m_bEnd;                             //�Ƿ����
	bool                            m_bStart;                           //�Ƿ�ʼ
	bool                            m_bCompare;                         //�Ƿ�ʼ����
	int                             m_nXPos;                            //
	int                             m_nYPos;                            //
	int                             m_nCardCount;                       //
	BYTE                            m_btCompareCardData[5];             //
	CSize                           m_SBigCardSize;                     //���˿˵Ĵ�С
	bool                            m_bDrawTimer;                       //
	bool                            m_bDrawFrontCardType;               //
	bool                            m_bDrawMidCardType;                 //
	bool                            m_bDrawBackCardType;                //
	int                             m_nFrontCardType;                   //
	int                             m_nMidCardType;                     //
	int                             m_nBackCardType;                    //
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
protected:
	CMD_S_GameEnd                   m_nSGameEnd;                        //
	bool                            m_bDrawPlayer1[5];                  //
	bool                            m_bDrawPlayer2[5];                  //
	bool                            m_bDrawPlayer3[5];                  //
	bool                            m_bDrawPlayer4[5];                  //
	int                             m_nDrawPlayer[GAME_PLAYER];         //
	bool                            m_bPlayer[4];                       //
	int                             m_nCurrentUser;                     //
	LONG                            m_LAllScore[4];                     //�ۼƷ���
	LONG                            m_LAllTax[4];                       //�ۼ�˰��
	bool                            m_bDrawSpecialType[GAME_PLAYER];
	bool                            m_bDrawSpecialCard;                 //�Ƿ��������ͷ���
    bool                            m_bSendCardAction;                  //�Ƿ��Ƿ��ƶ���
	int                             m_nXpoint[4];                       //
	int                             m_nYpoint[4];                       //
	int                             m_nCardSpace[4];                    //
	int                             m_nWinPlayer;                       //��һ�����
	int                             m_nLosePlayer;                      //�������    
	bool                            m_bDrawFire;                        //����ǹ
	int                             m_nFireGunStatus;                   //��ǹ12�����
	bool                            m_bDrawViewCard;                    //
	BYTE                            m_ViewSegmentCard[GAME_PLAYER][3][5];//
	bool                            m_bDrawFireCourse[5]                ;
	int                             m_nDrawFireWhichFrame;              ;//����ǹ��һ֡
	CPoint                          m_UserName[4];                       //
    int                             m_nChairWin;                         //
	int                             m_nChairLoss;                        //
	bool                            m_bDragon[GAME_PLAYER];              //��ˮ
	CPoint							m_HuaXian;

	CPoint							m_TishiFramePoint;					//��������ʾ�׿�λ��
	CPoint                          m_ThanCardPoint[4];                //����λ��
    CPoint                          m_ReadyPoint[4];                     //׼��λ��
	CPoint                          m_BoyPoint[4];                       //��ͷ��λ��
	CPoint                          m_GirlPoint[4];                      //Ůͷ��λ��
	CPoint                          m_FacePoint[4];                      //ͷ��λ��
	CPoint                          m_DragonPoint[GAME_PLAYER];          //��ˮλ��
	CPoint                          m_DrawCardKuang1[GAME_PLAYER];		//�����ƿ�λ��
	CPoint                          m_FourbaggerPoint[GAME_PLAYER];		//�����ƿ�λ��
    
	int                             m_nWidth;                            //��
	int                             m_nHeight;                           //��
	int								m_iPaiKuangZS;						 //�ƿ�����֡��

	bool                            m_bClockPoint;                       //�Ƿ��ʱ�ӻ������½�
	bool                            m_bDrawMyScore;                      //�Ƿ��Լ��ķ���
	bool                            m_bShowCard;                         //�Լ��Ƿ�̯��
	WORD                            m_wMeChairID;                        //�Լ�����ID

	 

	//״̬����
public:
	WORD							m_wAllUser;							//ȫ�ݴ��û�
	CHAR							m_szSegInfo[4][32];					//�ֶ���Ϣ
	TCHAR                           m_szUserNameInfo[4][32];            //�û�����
	LONG                            m_szPassScoreInfo[4];				//��һ�ֳɼ�
	LONG                            m_szTaxInfo[4];						//˰��
	int								m_szbToltalWinDaoShu[GAME_PLAYER];	//ˮ��
	BYTE							m_nFourbaggerFrames;				//ȫ�ݴ�֡��
	bool							m_bFourbagger;						//�Ƿ�ȫ�ݴ�
	bool							m_bShowFourbagger;					//�Ƿ���ʾȫ�ݴ�
	bool							m_bTishi;							//�Ƿ���ʾ������ 
	bool							m_bCompareAllCard;					//�Ƿ�ֱ�ӱ�ȫ����
	bool							m_bDrawCardKuang[GAME_PLAYER];		 //�Ƿ������Ƶ׿�
	bool                            m_bSpecialTypeview[GAME_PLAYER];    
	bool							m_bShowBorder;						 //�Ƿ���ʾ�߿�
	bool                            m_bDrawHandCard;                     //�Ƿ����е���
	bool							m_bDrawShuffle;						///<�Ƿ�ϴ�� 
	bool							m_bOnOffThanCard[GAME_PLAYER];		///<�Ƿ񻭱��� 
	bool							m_bOnOffTishikuang;					///<��ʾ�򿪹�
	bool							m_bOnOffBackAdornment;				///<����װ�� 
	bool                            m_bDrawScore;                       //�Ƿ񻭱Ƚϵķ��� 
	int								m_nShootNum;						///<�����ĸ��û���ǹ
	int								m_nShootState[6][2];				///<��ǹ״̬
	BYTE							m_nSpecialType;						//����������
	SCORE							m_lCellScore;						//��Ԫ�׷�
	//CStringArray					Str;

	//������Դ
public:
	CDFontEx						m_Font;								//����
	
	//�ؼ�����
	//��ť�ؼ�
public:
	CSkinButton						    m_btStart;							//��ʼ��ť
	CSkinButton                         m_btEnd;                            //������ť
	CSkinButton	                        m_btFrontCard;						//ǰ�հ�ť
	CSkinButton							m_btMidCard;						//�жհ�ť
	CSkinButton							m_btBackCard;						//��հ�ť

	CSkinButton							m_btDescend;						//����ť
	CSkinButton							m_btAscend;							//����ť
	CSkinButton							m_btColor;							//��ɫ��ť
	CSkinButton							m_btAutoOutCard;                    //�Զ�����
	CSkinButton						    m_btRestoreCard;					//�ָ��˿�

	CSkinButton                         m_btRecycleCard1;                   //���յ�һ��         
	CSkinButton                         m_btRecycleCard2;                   //���յڶ���
	CSkinButton                         m_btRecycleCard3;                   //���յ�����
	CSkinButton                         m_btRecycleAllCard;                 //����ȫ��
	CSkinButton                         m_btOk;                             //ȷ��

	CSkinButton                         m_btOnePare;                        //һ��
	CSkinButton                         m_btTwoPare;                        //����
	CSkinButton                         m_btThreeSame;                      //����
	CSkinButton                         m_btStraight;                       //˳��
	CSkinButton                         m_btFlush;                          //ͬ��
	CSkinButton                         m_btGourd;                          //��«
	CSkinButton                         m_btFourSame;                       //��֧
	CSkinButton                         m_btStraightFlush;                  //ͬ��˳
	CSkinButton							m_btPaiXu;							//����
	CSkinButton							m_btSubmit;							//������ȷ��
	CSkinButton							m_btCancel;							//������ȡ��
	CButton							m_btOpenAdmin;

	//�˿˿ؼ�
public:
	CCardControl					m_UserSendCard0;		//�û�����
	CCardControl					m_UserSendCard[GAME_PLAYER];		//�û�����
	CCardControl					m_UserShuffleCard[GAME_PLAYER];		//ϴ��
	CCardControl					m_UserCardControl[GAME_PLAYER];		//�����˿�
	CCardControl					m_AllSegCardControl[GAME_PLAYER][3];//�����ֶ�
	CCardControl					m_SegCardControl[GAME_PLAYER][3];	//�ֶ��˿�
	//CCardControl					m_HandSegCardControl[3];			//���Ϸֶ�
	CCardControl					m_CompareCarControl[GAME_PLAYER];	//�Ա��˿�
	CCardControl				    m_OverTimeCardControl[GAME_PLAYER]; //��ʱ�˿�
	CCardControl                    m_CardType[GAME_PLAYER];            //
	CCardControl                    m_DrawGun;                          //


	//λͼ����

public:
	///ZBD
// 	CPngImage                                       m_ImagezsFlower;                   //���ϻ���
// 	CPngImage                                       m_ImagezxFlower;                   //���»���
// 	CPngImage                                       m_ImageysFlower;                   //���ϻ���
// 	CPngImage                                       m_ImageyxFlower;                   //���»���
	CPngImage                                       m_ImageDragon;                     //��ˮ
	CPngImage                                       m_ImageReady;                      //׼����Դ
	CPngImage                                       m_ImageViewFill;                   //�����Դ
	CPngImage									    m_ImageScoreFrame;					//���ְ񱳾�
	CPngImage			                            m_ImageCenter;						//������Դ
	CPngImage										m_ImageClock;						//����
	CPngImage										m_ImageTimer;		                //ʱ��
	CPngImage	                                    m_ITishikuang;                     //��ʾ��
	CPngImage                                       m_ICardFrame;                      //�Ƶ׿� 
	CPngImage                                       m_ImageCardType;					//������
	CPngImage                                       m_CardSpecialType;					//������
	CPngImage                                       m_EndNumSmall;						//��ˮ���֣�С��
	CPngImage                                       m_EndNumBig;						//��ˮ���֣���
	CPngImage										m_ImageGun1;		                //��ǹ1
	CPngImage										m_ImageGun2;		                //��ǹ2
	//CPngImage                                     m_IFire;							//�ӵ�
	CPngImage                                       m_ICrash;							//��ը
	CPngImage										m_ImageCard;						//ͼƬ��Դ
	CPngImage										m_ImageBigCard;                     //��ͼƬ��Դ
	CPngImage                                       m_ISendCard;						//����
	CPngImage                                       m_IShuffleCard ;					//ϴ��
	CPngImage                                       m_IThanCard;						//����
	CPngImage                                       m_SpecCardFlash;					//�������͵׿�
	CPngImage                                       m_ScoreFrame;						//�׷ֿ�
	CPngImage                                       m_TishiFrame;						//��������ʾ��
	CPngImage                                       m_FourbaggerFlash;					//ȫ�ݴ򶯻�
	CPngImage                                       m_Fourbagger;						//ȫ�ݴ�

	CPngImage                                       m_IPlusBigNumber;                  //�ӷִ�����
	CPngImage                                       m_IPlusSmallNumber;                //�ӷ�С����
	CPngImage                                       m_IBigSubtractionSign;             //�����
	CPngImage                                       m_IBigPlusSign;                    //��Ӻ�
	CPngImage                                       m_ISmallSubtractionSign;           //С����
	CPngImage                                       m_ISmallPlusSign;                  //С�Ӻ�

	CPngImage                                       m_IBackAdornment1;					//����װ��ͼ1



	//�ؼ����
public:
	CScoreView						                m_ScoreView;						//������ͼ
	CMysteryDlg										m_MysteryDlg;						//���ƴ���
	CClientControl									m_ClientControl;					//�ͻ��˿���

	//��������
public:
	//���캯��
	CGameClientView();
	//��������
	virtual ~CGameClientView();
public:
	//������ͼ
	void RefreshGameView();
	//�̳к���
private:
	//���ý���
	virtual VOID ResetGameView();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);
	//���ƽ���
	virtual VOID DrawGameView(CDC * pDC, INT nWidth, INT nHeight);
	//WIN7֧��
	virtual bool RealizeWIN7() { return true; }

	//��������
protected:
	//��������
	void DrawTextString(CDC * pDC, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, int nXPos, int nYPos);
	//�滭�ַ�
	void DrawTextString(CDC * pDC, CDFontEx* pFont, LPCTSTR pszString, COLORREF crText, COLORREF crFrame, CRect rcRect, UINT nDrawFormat);

	//���ܺ���
public:
	//�����˿�
	void SetCardData(const WORD wChairID , const BYTE bCardData[] , const BYTE bCardCount , const enCardType bCardType , const BYTE bSeg=255) ;
	//��ʾ�Ƚ�
	void SetShowCompare(bool bShow , const enSegmentType SegmentType=enErr) ;
	//�ֶ���Ϣ
	void SetDrawThanCard(bool bThanCard);
	bool GetDrawThanCard();
	void ShowSegInfo(enSegmentType seg, CHAR const szInfo[32]);	
	void SetEnd(bool bEnd);
	void SetStart(bool bStart);
	void SetCompare(bool bCompare);
	void SetCompareCard(int nXPos,int nYPos,BYTE btCompareCardData[],int nCardCount);
	void SetbDrawTimer(bool bDrawTimer);
	void SetbDrawFrontCardType(bool bDraw,int CardType);
	void SetbDrawMidCardType(bool bDraw,int CardType);
    void SetbDrawBackCardType(bool bDraw,int CardType);
	void SetSGameEnd(CMD_S_GameEnd SGameEnd);
	void SetbDrawPlayer(bool bDrawPlayer1[],bool bDrawPlayer2[],bool bDrawPlayer3[],bool bDrawPlayer4[]);
	void SetnDrawPlayer(int nDrawPlayer[]);
	void SetbPlayer(bool bPlayer[]);
	void SetnCurrentUser(int nCurrentUser);
	int GetViewChairID(int nCurrentUser);
	void GetAllUserScoreInfo(LONG szPassScoreInfo[],LONG szTaxInfo[],int szbToltalWinDaoShu[],bool bUserLeft[]);
	void GetUserNameInfo(TCHAR szUserNameInfo1[],TCHAR szUserNameInfo2[],TCHAR szUserNameInfo3[],TCHAR szUserNameInfo4[]);
    void GetbSpecialType(bool bSpecialType[]);
	void GetbDrawSpecial(bool bDrawSpecialCard);
	void SetbDrawScore(bool bDrawScore);
	void SetbSendAction(bool bSendAction);
	void SetnPoint(int xPoint[],int yPoint[]);
	void SetnCardSpace(int nCardSpace[]);
	void SetCardInfo(int xPoint[],int yPoint[],int nCardSpace[]);
	void SetbDrawFire(bool bDrawFire);
	void SetFireGunStatus(int nFireGunStatus);
	void SetbDrawViewCard(bool bDrawViewCard,BYTE cbSegmentCard11[],BYTE cbSegmentCard12[],BYTE cbSegmentCard13[],BYTE cbSegmentCard21[],
		                  BYTE cbSegmentCard22[],BYTE cbSegmentCard23[],BYTE cbSegmentCard31[],BYTE cbSegmentCard32[],BYTE cbSegmentCard33[],
		                  BYTE cbSegmentCard41[],BYTE cbSegmentCard42[],BYTE cbSegmentCard43[]);
	void SetnDrawFireWhichFrame(int nDrawFireWhichFrame);
	void SetUser2CardSpace(int nUser2CardSpace);
	void GetUserWinOrLoss(int &nWinUser,int &nLossUser);
	void SetDragon(int wViewChairID,bool bDragon);
	void CleanUserScoreInfo(int nUserChairID);
	void SetbClockPoint(bool bClcokPoint);
	void SetbDrawMyScore(bool bDrawMyScore);
	void SetbShowCard(bool bShowCard);
	void SetMeChairID(WORD wMeChairID);
public:
	void ResetViewData(bool IsLookon);
	void HideButton();
	void DrawFrontCardType( CDC* pDC ,int nXPos,int nYPos );
    void DrawMidCardType(CDC* pDC ,int nXPos,int nYPos);
	void DrawBackCardType(CDC* pDC ,int nXPos,int nYPos);
	void DrawCompareScore(CDC* pDC ,int nXPos,int nYPos);
	void DrawUserInfo(CDC* pDC ,int nXPos,int nYPos);
	void DrawFire(CDC* pDC ,int nXPos,int nYPos);
	void DrawReady(CDC* pDC ,int nXPos,int nYPos);
	void DrawThanCard( CDC* pDC,int nXPos,int nYPos); ///<������
	void DrawBackAdornment1( CDC* pDC,int nXPos,int nYPos );///<������װ����1
	
	void DrawUserClock(CDC* pDC ,int nXPos,int nYPos);
	void DrawUserTimer(CDC* pDC , int nXPos, int nYPos, WORD wTime, WORD wTimerArea/* =99 */);

	//��Ϣӳ��
protected:

	//��������
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//��ʼ��ť
	afx_msg void OnStart() ;
	//����ǰ��
	afx_msg void OnFrontCard();
	//�����ж�
	afx_msg void OnMidCard();
	//���ú��
	afx_msg void OnBackCard();
	//ȷ��������
	afx_msg void OnSubmit();
	//ȡ��������
	afx_msg void OnCancel();
	//̯��
	afx_msg void OnShowCard();
	//������1
	afx_msg void OnRecyeleCard1();
	//������2
	afx_msg void OnRecyeleCard2();
	//������3
	afx_msg void OnRecyeleCard3();
	//����ȫ����
	afx_msg void OnRecyeleAllCard();
	//һ��
	afx_msg void OnOnePare();
	//����
	afx_msg void OnTwoPare();
	//����
	afx_msg void OnThreeSame();
	//˳��
	afx_msg void OnStraight();
	//ͬ��
	afx_msg void OnFlush();
	//��«
	afx_msg void OnGord();
	//��֧
	afx_msg void OnFourSamw();
	//ͬ��˳
	afx_msg void OnStraightFlush();
	//����
	afx_msg void OnTaxis();
	//����Ա����
	afx_msg void OpenAdminWnd();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

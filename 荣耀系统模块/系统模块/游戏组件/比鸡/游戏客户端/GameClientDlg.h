#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "GameClientView.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ�Ի���
class CGameClientDlg : public CGameFrameEngine
{
	//�ؼ�����
protected:
	CGameLogic						m_GameLogic;						//��Ϸ�߼�
	CGameClientView					m_GameClientView;					//��Ϸ��ͼ
public:
	int m_nMiddleWidth;
	int m_nMiddleHeight;
	bool m_bOnOffStart;													///<�Ƿ����ƿ�ʼ��Ϸ
	int                             m_nPlayer;                          ///<�������
	bool							m_bOnTimeEvent;						///<�¼�
	bool							m_bSound;							///<�������ֿ���
	//���ñ���
protected:
	bool							m_bBackGroundSound;					//��������
	DWORD							m_dwCardHSpace;						//�˿�����
	int                             m_nCurrentUser;                     //
	CMD_S_GameEnd                   m_ppBuffer;                        //
	CMD_S_SendCard                  m_Buffer;                          //
	WORD                            m_wDataSize;                        //
	BYTE                            m_btCardType[GAME_PLAYER][3];        //����Ƶ�����
	
	int                             m_nWhichCard;                        //�ڼ���
	bool                            m_bPlayer[4];                        //����Ƿ���
	bool                            m_bDraw;                             //
	bool                            m_bSpecialType[GAME_PLAYER];         //�Ƿ�����������
	int                             m_nSpecialType;                      //�������͵ĸ���
	bool                            m_bShowCard;                         //�Ƿ�̯��
	bool                            m_bStrat;                            //��ʼ
	bool                            m_bCompareCard;                      //�Ƿ����
	bool                            m_bDragon[GAME_PLAYER];              //�Ƿ�����
	bool							bFirst;

	


	//��Ϸ����
protected:
	BYTE							m_bHandCardData[HAND_CARD_COUNT];	//�����˿�
	BYTE                            m_bAllHandCardData[GAME_PLAYER][HAND_CARD_COUNT];//������ҵ��˿�
	BYTE                            m_bSpecialCardData[GAME_PLAYER][HAND_CARD_COUNT];//
	BYTE							m_bTmpHandCardData[HAND_CARD_COUNT];//�����˿�
	BYTE							m_bHandCardCount ;					//�˿���Ŀ
	BYTE							m_bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	UINT_PTR						m_nTimer ;							//��ʱ��ID
	LONG							m_wUserToltalChip;					//�ܹ����
	bool							m_bFinishSegment;					//��ɷֶ�
	bool							m_bReady;							//�Ƿ�׼��
	LONG							m_lGameScore[GAME_PLAYER];			//��һ���
	BYTE							m_bOverTime[GAME_PLAYER];			//̯�Ƴ�ʱ

	tagAnalyseType                  m_bType;                            //���ͼ����
	bool                            m_bbThreeCard[3];                   //�������޴���
	
public:
	//CDSBuffer						*m_pSoundBackGround;                                   ///<��������ָ��
	//CGameSounds						*m_pSoundBackGround;
	//CMultiAudioPlay *
	//��������
public:
	//���캯��
	CGameClientDlg();
	//��������
	virtual ~CGameClientDlg();

	//��Ϣ����
protected:
	//�����˿�
	bool OnSubSendCard(const void * pBuffer, WORD wDataSize);
	//��Ϸ����
	bool OnSubGameEnd(const void * pBuffer, WORD wDataSize) ;
	//���÷ֶ�
    bool OnSubSetSegment(const void * pBuffer , WORD wDataSize);
    //���̯��
	bool OnSubShowCard(const void * pBuffer , WORD wDataSize) ;
	//���ǿ��
	bool OnSubPlayerLeft(const void * pBuffer , WORD wDataSize) ;
	//������ʾ
	bool OnControlPrompt(const void * pBuffer, WORD wDataSize);
    
    //��������
protected:
	//�Զ�����
	void AutomaticOutCard() ;
	//�ֶ���Ϣ
	bool ShowSegInfo() ;
	//���ð�ť
	void SetThreeCard();
	//����
	bool ComparePlayerCard(const void * pBuffer, WORD wDataSize);
	BYTE GetCardType(int Player,int WhichCard,bool bFrontCard,bool bMidCard);
	int GetCardType(BYTE BCardType,bool bFrontCard,bool bMidCard);
	void GetSpecialCard(BYTE CardData[],BYTE CardCount);
	void ResetDlgData();

	//����̳�
private:
	//��ʼ����
	virtual bool OnInitGameEngine();
	//���ÿ��
	virtual bool OnResetGameEngine();
	//��Ϸ����
	virtual void OnGameOptionSet();

	//ʱ���¼�
public:
	//ʱ��ɾ��
	virtual bool OnEventGameClockKill(WORD wChairID);
	//ʱ����Ϣ
	virtual bool OnEventGameClockInfo(WORD wChairID, UINT nElapse, WORD wClockID);

	//��Ϸ�¼�
public:
	//�Թ�״̬
	virtual bool OnEventLookonMode(VOID * pData, WORD wDataSize);
	//������Ϣ
	virtual bool OnEventGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize);
	//��Ϸ����
	virtual bool OnEventSceneMessage(BYTE cbGameStatus, bool bLookonUser, VOID * pData, WORD wDataSize);
	//��������
	virtual bool AllowBackGroundSound(bool bAllowSound);

	//�û��¼�
public:
	//�û�����
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem, bool bLookonUser);
	//�û��뿪
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem, bool bLookonUser);

	//��Ϣ����
protected:
	//��ʼ��Ϣ
	LRESULT OnStart(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	LRESULT OnEnd(WPARAM wParam, LPARAM lParam);
	//����˿�
	LRESULT OnLeftHitCard(WPARAM wParam, LPARAM lParam) ;
	//����ǰ��
	LRESULT OnFrontCard(WPARAM wParam, LPARAM lParam) ;
	//�����ж�
	LRESULT OnMidCard(WPARAM wParam, LPARAM lParam) ;
	//���ú��
	LRESULT OnBackCard(WPARAM wParam, LPARAM lParam) ;
	//���̯��
	LRESULT OnShowCard(WPARAM wParam, LPARAM lParam) ;	
	//��������
	LRESULT	OnDescendSort(WPARAM wParam, LPARAM lParam) ;	
	//��������
	LRESULT	OnAscendSort(WPARAM wParam, LPARAM lParam) ;	
	//��ɫ����
	LRESULT	OnColorSort(WPARAM wParam, LPARAM lParam) ;	
	//�Զ�����
	LRESULT OnAutoOutCard(WPARAM wParam, LPARAM lParam);
	//�ָ��˿�
	LRESULT OnRestoreCard(WPARAM wParam, LPARAM lParam) ;
	//һ��
	LRESULT OnOnePareCard(WPARAM wParam, LPARAM lParam );
	//����
	LRESULT OnTwoPareCard(WPARAM wParam, LPARAM lParam );
	//����
	LRESULT OnThreeSameCard(WPARAM wParam, LPARAM lParam );
	//˳��
	LRESULT OnStraightCard(WPARAM wParam, LPARAM lParam );
	//ͬ��
	LRESULT OnFlushCard(WPARAM wParam, LPARAM lParam );
	//��«
	LRESULT OnGourdCard(WPARAM wParam, LPARAM lParam );
	//��֧
	LRESULT OnFourSameCard(WPARAM wParam, LPARAM lParam );
	//ͬ��˳
	LRESULT OnStraightFlushCard(WPARAM wParam, LPARAM lParam );
	//���յ�һ��
	LRESULT OnRecycleFrontCard(WPARAM wParam, LPARAM lParam);
	//���յڶ���
	LRESULT OnRecycleMidCard(WPARAM wParam, LPARAM lParam);
	//���յ�����
	LRESULT OnRecycleBackCard(WPARAM wParam, LPARAM lParam);
	//����ȫ��
	LRESULT OnRecycleAllCard(WPARAM wParam, LPARAM lParam);
	//ȷ��������
	LRESULT OnSubmit(WPARAM wParam, LPARAM lParam);
	//ȡ��������
	LRESULT OnCancel(WPARAM wParam, LPARAM lParam);
	//����Ա����
	LRESULT OnAdminCommand(WPARAM wParam, LPARAM lParam);
   
public:
	afx_msg void OnTimer(UINT nIDEvent);
	//������
	LRESULT OnOperateMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
//////////////////////////////////////////////////////////////////////////

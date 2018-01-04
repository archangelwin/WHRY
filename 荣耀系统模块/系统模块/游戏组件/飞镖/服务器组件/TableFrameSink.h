#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "AfxTempl.h"
#include "GameLogic.h"
#include "HistoryScore.h"

#include "ServerControl.h"
#define MAX_CARD					5
#define MAX_CARDGROUP				5
#define CONTROL_AREA				4
//////////////////////////////////////////////////////////////////////////
//��ʷ��¼
#define MAX_SCORE_HISTORY			16									//��ʷ����
//////////////////////////////////////////////////////////////////////////////////

struct Region
{
	int nIndex;
	SCORE sCoinScore;	
	SCORE sBetScore;	
};


//��Ϸ����
class CTableFrameSink : public ITableFrameSink, public ITableUserAction
{
	//�û���Ϣ
protected:
	LONGLONG						m_lUserStartScore[GAME_PLAYER];		//��ʼ����

	//����ע��
protected:
	LONGLONG						m_lAllJettonScore[AREA_COUNT+1];		//ȫ����ע
	
	//������ע
protected:
	LONGLONG						m_lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];	//������ע
	WORD							m_wSitDownInfo[SEAT_COUNT];					//��λ�Ŷ�Ӧ�����Ӻ�

	//���Ʊ���
protected:						
	bool							m_bRefreshCfg;							//ÿ��ˢ��
	TCHAR							m_szRoomName[32];						//���÷���
	TCHAR							m_szConfigFileName[MAX_PATH];			//�����ļ�
	LONGLONG						m_lAreaLimitScore;						//��������
	LONGLONG						m_lUserLimitScore;						//��������
	int								m_nEndGameMul;							//��ǰ���ưٷֱ�

	//��ׯ����
	LONGLONG						m_lApplyBankerCondition;				//��������
	LONGLONG						m_nBankerTimeLimit;						//��������
	LONGLONG						m_nBankerTimeAdd;						//���Ӵ��� (��Ҵ����������ʱ)
	LONGLONG						m_lExtraBankerScore;					//�������� (���ڴ�ֵʱ������������������ׯ)
	LONGLONG						m_nExtraBankerTime;						//�������

	//������
	LONGLONG						m_lStorageStart;						//�����ֵ
	LONGLONG						m_lStorageCurrent;						//�����ֵ
	LONGLONG						m_nStorageDeduct;						//��ȡ����
	LONGLONG						m_lStorageMax1;							//���ⶥ1
	LONGLONG						m_lStorageMul1;							//ϵͳ��Ǯ����
	LONGLONG						m_lStorageMax2;							//���ⶥ2
	LONGLONG						m_lStorageMul2;							//ϵͳ��Ǯ����
	bool							m_bControl;								//�Ƿ����
	TCHAR							m_szControlName[LEN_NICKNAME];			//��������

	//ʱ�����
	LONGLONG						m_nFreeTime;							//����ʱ��
	LONGLONG						m_nPlaceJettonTime;						//��עʱ��
	LONGLONG						m_nDartTime;							//�����ʱ��
	LONGLONG						m_nGameEndTime;							//����ʱ��

	//����
	tagOdds							m_tOdds;

	//�����˿���
	LONGLONG						m_nMaxChipRobot;						//�����Ŀ (��ע������)
	LONGLONG						m_nChipRobotCount;						//����ͳ�� (��ע������)
	LONGLONG						m_lRobotAreaLimit;						//����ͳ�� (������)
	LONGLONG						m_lRobotAreaScore[AREA_COUNT+1];		//����ͳ�� (������)
	LONGLONG						m_nRobotListMaxCount;					//�������

	WORD							m_wCheatArea;

	//��ҳɼ�
protected:
	LONGLONG						m_lUserWinScore[GAME_PLAYER];			//��ҳɼ�
	LONGLONG						m_lUserReturnScore[GAME_PLAYER];		//������ע
	LONGLONG						m_lUserRevenue[GAME_PLAYER];			//���˰��
	BYTE							m_cbLeftCardCount;						//�˿���Ŀ
	bool							m_bContiueCard;							//��������
	BYTE							m_bcFirstPostCard;						//���°l���c��

	//�˿���Ϣ
protected:
	BYTE							m_cbCardCount[4];						//�˿���Ŀ
    BYTE							m_cbTableCardArray[5][5];				//�����˿�
	BYTE							m_cbTableCard[CARD_COUNT];				//�����˿�
	BYTE							m_cbDartPoint[2];						//���ڵ���
	WORD							m_cbDartFlag[2];						//�����ǣ��Ƿ��ѷ���

	//״̬����
protected:
	DWORD							m_dwJettonTime;							//��ʼʱ��
	bool							m_bExchangeBanker;						//�任ׯ��
	LONGLONG						m_wAddTime;								//������ׯ 

	//ׯ����Ϣ
protected:
	CWHArray<WORD>					m_ApplyUserArray;						//�������
	WORD							m_wCurrentBanker;						//��ǰׯ��
	WORD							m_wBankerTime;							//��ׯ����
	LONGLONG						m_lBankerScore;							//
	LONGLONG						m_lBankerWinScore;						//�ۼƳɼ�
	LONGLONG						m_lBankerCurGameScore;					//��ǰ�ɼ�
	bool							m_bEnableSysBanker;						//ϵͳ��ׯ

	//���Ʊ���
protected:
	BYTE							m_cbWinSideControl;						//������Ӯ
	int								m_nSendCardCount;						//���ʹ���

	//��¼����
protected:
	tagServerGameRecord				m_GameRecordArrary[MAX_SCORE_HISTORY];	//��Ϸ��¼
	int								m_nRecordFirst;							//��ʼ��¼
	int								m_nRecordLast;							//����¼
	CFile							m_FileRecord;							//��¼���
	DWORD							m_dwRecordCount;						//��¼��Ŀ

	BYTE							m_cbWinArea[AREA_COUNT];				//Ӯ������
	Region						    m_rgArea[AREA_COUNT];					//��¼������Ϣ

	//�������
protected:
	CGameLogic						m_GameLogic;							//��Ϸ�߼�
	ITableFrame						* m_pITableFrame;						//��ܽӿ�
	tagGameServiceOption			* m_pGameServiceOption;					//���ò���
	tagGameServiceAttrib *			m_pGameServiceAttrib;					//��Ϸ����

	//�������
protected:
	HINSTANCE						m_hInst;
	IServerControl*					m_pServerContro;

	//���Ա���
protected:
	static const WORD				m_wPlayerCount;							//��Ϸ����

	//��������
public:
	//���캯��
	CTableFrameSink();
	//��������
	virtual ~CTableFrameSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//��λ�ӿ�
	virtual VOID RepositionSink();
	//���ýӿ�
	virtual bool Initialization(IUnknownEx * pIUnknownEx);

	virtual bool SetGameRule(byte *cbRule, int len){ return true; }
	//�����ӿ�
public:
	//���û���
	virtual void SetGameBaseScore(LONG lBaseScore){};
	//��Ϸ�¼�
public:
	//��Ϸ��ʼ
	virtual bool OnEventGameStart();
	//��Ϸ����
	virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//���ͳ���
	virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret);

	//�¼��ӿ�
public:
	//ʱ���¼�
	virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
	//��Ϸ��Ϣ
	virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
	//�����Ϣ
	virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);

	//�û��¼�
public:
	//�û�����
	virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem);
	//�û�����
	virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem) { return true; }
	//�û�����
	virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�����
	virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
	//�û�ͬ��
	virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize) { return true; }
	//��ѯ�ӿ�
public:
	//��ѯ�޶�
	virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem);
	//���ٻ���
	virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem){return 0;}
	//�����¼�
	virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize){return false;}
	//�����¼�
	virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
	//��ѯ�Ƿ�۷����
	virtual bool QueryBuckleServiceCharge(WORD wChairID);	

	//��Ϸ�¼�
protected:
	//��ע�¼�
	bool OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore);
	//�������
	bool OnDart(WORD wChairID);
	//����ׯ��
	bool OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem);
	//ȡ������
	bool OnUserCancelBanker(IServerUserItem *pICancelServerUserItem);
	//����б�
	bool GetAllPlayerList(WORD wChairID);
	//��̨�б�
	bool GetAllBankerList(WORD wChairID);
	//K������
	bool GetAllRecordList(WORD wChairID);
	//����
	bool OnUserSitDown(tagUserInfo userInfo, WORD wSeatID);
	//����
	bool OnUserLeaveSeat(WORD wChairID);
	//��������
private:
	//�����˿�
	bool DispatchTableCard();
	//����ׯ��
	void SendApplyUser( IServerUserItem *pServerUserItem );
	//����ׯ��
	bool ChangeBanker(bool bCancelCurrentBanker);
	//�ֻ��ж�
	void TakeTurns();
	//���ͼ�¼
	void SendGameRecord(IServerUserItem *pIServerUserItem);
	//������Ϣ
	void SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg);
	//��ȡ����
	void ReadConfigInformation();
	//�Ƿ�˥��
	bool NeedDeductStorage();
	//������ע��Ϣ
	void SendUserBetInfo( IServerUserItem *pIServerUserItem );

	//��Ӯ����
protected:
	// ����Ż�
	VOID StorageOptimize();
	// �ж���Ӯ
	void JudgeSystemScore( BYTE bCardData[5][5], LONGLONG& lSystemScore, bool& bAllEat );
	// ��¼����
	void WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString );

	//��ע����
private:
	//�����ע
	LONGLONG GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea);

	//��Ϸͳ��
private:
	//����÷�
	LONGLONG CalculateScore();
	void DeduceWinnerEx(int &nTian, int &nDi, int &nXuan, int &nHuang);
	//�ƶ�Ӯ��
	void DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple );
	int GetMultiple(BYTE bAreaNum);
	void GetWinArea();
	void ControlWinArea();
};


//////////////////////////////////////////////////////////////////////////////////

#endif
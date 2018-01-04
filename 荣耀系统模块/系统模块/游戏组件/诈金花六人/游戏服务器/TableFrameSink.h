#ifndef TABLE_FRAME_SINK_HEAD_FILE
#define TABLE_FRAME_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameLogic.h"
#include "ServerControl.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////


//��Ϸ������
class CTableFrameSink :public ITableFrameSink, public ITableUserAction
{
  //��Ϸ����
protected:
  BYTE              m_bOperaCount;              //��������
  WORD              m_wCurrentUser;             //��ǰ�û�
  WORD              m_wBankerUser;              //ׯ���û�
  WORD              m_wFlashUser[GAME_PLAYER];  //�����û�
  WORD				m_wTurnCount;				//��ע����
  WORD				m_wMinLookTurn;				//��������
  WORD				m_wMinCompareTurn;			//��������
  WORD				m_wMaxOpenTurn;				//�ⶥ����
  SCORE				m_lMaxOpenScore;			//
  BOOL				m_bSpecilBaozi;				//235������
  WORD				m_wMaxPlayerCount;			//�������

  BOOL				m_bCompareSmall;			//��С
  BYTE				m_cbCompareWay;				//���Ʒ�ʽ�ȴ��С
  SCORE				m_lTotlalTableScore;		//����ע��

  //�û�״̬
protected:
  BYTE              m_cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  BYTE              m_cbGiveUpUser[GAME_PLAYER];      //��Ϸ״̬
  LONGLONG          m_lCompareCount;				  //����״̬
  bool              m_bGameEnd;						  //����״̬
  CMD_S_GameEnd     m_StGameEnd;					  //��������
  BYTE              m_cbRealPlayer[GAME_PLAYER];      //�������
  BYTE              m_cbAndroidStatus[GAME_PLAYER];   //����״̬

  //�˿˱���
protected:
  BYTE              m_cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
  CWHArray<WORD>    m_wCompardUser[GAME_PLAYER];			 //�˿�����

  //��ע��Ϣ
protected:
  LONGLONG            m_lTableScore[GAME_PLAYER];       //��ע��Ŀ
  LONGLONG            m_lUserMaxScore[GAME_PLAYER];     //�����ע
  LONGLONG            m_lMaxCellScore;            //��Ԫ����
  LONGLONG            m_lCellScore;             //��Ԫ��ע
  LONGLONG            m_lCurrentTimes;            //��ǰ����
  bool				  m_bMingZhu[GAME_PLAYER];        //������ע

  //�������
protected:
  CGameLogic            m_GameLogic;              //��Ϸ�߼�
  ITableFrame           * m_pITableFrame;           //��ܽӿ�
  const tagGameServiceOption    * m_pGameServiceOption;         //���ò���

  //�������
protected:
  HINSTANCE           m_hControlInst;
  IServerControl*         m_pServerControl;

  //AI����
protected:
  bool              m_bUpdataStorage;           //���¿��

  //���Ա���
protected:
  static const WORD       m_wPlayerCount;             //��Ϸ����
  static const BYTE       m_GameStartMode;            //��ʼģʽ

  //��������
public:
  //���캯��
  CTableFrameSink();
  //��������
  virtual ~CTableFrameSink();

  //�����ӿ�
public:
  //�ͷŶ���
  virtual VOID  Release();
  //�Ƿ���Ч
  virtual bool  IsValid()
  {
    return AfxIsValidAddress(this,sizeof(CTableFrameSink))?true:false;
  }
  //�ӿڲ�ѯ
  virtual void *  QueryInterface(const IID & Guid, DWORD dwQueryVer);

  //����ӿ�
public:
  //��ʼ��
  virtual bool  Initialization(IUnknownEx * pIUnknownEx);
  //��λ����
  virtual void  RepositionSink();
  //��ѯ�ӿ�
public:
  //��ѯ�޶�
  virtual SCORE QueryConsumeQuota(IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //���ٻ���
  virtual SCORE QueryLessEnterScore(WORD wChairID, IServerUserItem * pIServerUserItem)
  {
    return 0;
  }
  //�����¼�
  virtual bool OnDataBaseMessage(WORD wRequestID, VOID * pData, WORD wDataSize)
  {
    return false;
  }
  //�����¼�
  virtual bool OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
  {
    return false;
  }
  //��ѯ�Ƿ�۷����
  virtual bool QueryBuckleServiceCharge(WORD wChairID)
  {
    return true;
  }

  //�����ӿ�
public:
  //���û���
  virtual void SetGameBaseScore(LONG lBaseScore) {};

  //��Ϣ�ӿ�
public:
  //��ʼģʽ
  virtual BYTE GetGameStartMode();
  //��Ϸ״̬
  virtual bool IsUserPlaying(WORD wChairID);

  //��Ϸ�¼�
public:
  //��Ϸ��ʼ
  virtual bool OnEventGameStart();
  //��Ϸ����
  virtual bool OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason);
  //���ͳ���
  virtual bool OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE bGameStatus, bool bSendSecret);

  virtual bool SetGameRule(byte *cbRule, int len){ 
	  PriviteRoomRule *pPriveteRoomRule = (PriviteRoomRule*)cbRule;
	  m_wMaxOpenTurn = pPriveteRoomRule->wMaxOpenTurn;
	  if (m_wMaxOpenTurn == 0)
		  m_wMaxOpenTurn = INVALID_WORD;
	  m_wMinCompareTurn = pPriveteRoomRule->wMinCompareTurn;
	  m_wMinLookTurn = pPriveteRoomRule->wMinLookTurn;
	  m_wMaxPlayerCount = pPriveteRoomRule->wMaxPlayer;
	  m_lCellScore = pPriveteRoomRule->lCellScore;
	  m_lMaxOpenScore = pPriveteRoomRule->lMaxOpenScore;
	  if (m_lMaxOpenScore == 0)
		  m_lMaxOpenScore = 0xFFFFFFFF;


	  return true; 
  }

  //�¼��ӿ�
public:
  //��ʱ���¼�
  virtual bool OnTimerMessage(DWORD wTimerID, WPARAM wBindParam);
  //��Ϸ��Ϣ����
  virtual bool OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);
  //�����Ϣ����
  virtual bool OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem);


  //�û��¼�
public:
  //�û�����
  virtual bool OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserConnect(WORD wChairID,IServerUserItem * pIServerUserItem)
  {
    return true;
  }
  //�û�����
  virtual bool OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�����
  virtual bool OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser);
  //�û�ͬ��
  virtual bool OnActionUserOnReady(WORD wChairID,IServerUserItem * pIServerUserItem, VOID * pData, WORD wDataSize)
  {
    return true;
  }

  //��Ϸ�¼�
protected:
  //�����¼�
  bool OnUserGiveUp(WORD wChairID,bool bExit=false);
  //�����¼�
  bool OnUserLookCard(WORD wChairID);
  //�����¼�
  bool OnUserCompareCard(WORD wFirstChairID,WORD wNextChairID);
  //�����¼�
  bool OnUserOpenCard(WORD wUserID);
  //��ע�¼�
  bool OnUserAddScore(WORD wChairID, LONGLONG lScore, bool bGiveUp, bool bCompareUser);

  //���ܺ���
protected:
  //�˿˷���
  void AnalyseStartCard();
  //�Ƿ�˥��
  bool NeedDeductStorage();
  //���·����û���Ϣ
  void UpdateUserRosterInfo(IServerUserItem *pIServerUserItem, LONGLONG lGameScore = 0LL, USERROSTER userRoster = INVALID_ROSTER, LONGLONG lScoreLimit = 0LL);
  //����û�����
  void RemoveUserRoster();
  //��������˷���
  LONGLONG CalculateAndroidScore(WORD &wWinUser);
};

//////////////////////////////////////////////////////////////////////////

#endif

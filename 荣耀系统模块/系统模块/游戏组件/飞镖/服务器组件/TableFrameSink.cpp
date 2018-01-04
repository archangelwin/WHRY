#include "StdAfx.h"
#include "DlgCustomRule.h"
#include "TableFrameSink.h"
#include <locale>

//////////////////////////////////////////////////////////////////////////////////
//��������
#define SEND_COUNT					100									//���ʹ���

//��������
#define INDEX_PLAYER				0									//�м�����
#define INDEX_BANKER				1									//ׯ������

//ʱ�䶨��
#define IDI_FREE					1									//����ʱ��
#define IDI_PLACE_JETTON			2									//��עʱ��
#define IDI_DART					3									//�����ʱ��
#define IDI_GAME_END				4									//����ʱ��

#define TIME_FREE					5									//����ʱ��
#define TIME_PLACE_JETTON			10									//��עʱ��
#define TIME_DART					10									//�����ʱ��
#define TIME_GAME_END				5									//����ʱ��
//////////////////////////////////////////////////////////////////////////

//��̬����
const WORD			CTableFrameSink::m_wPlayerCount=GAME_PLAYER;				//��Ϸ����

void Debug(char *text,...)
{
	static DWORD num=0;	
	char buf[1024];	
	FILE *fp=NULL;	
	va_list ap;	
	va_start(ap,text);	
	vsprintf(buf,text,ap);	
	va_end(ap);		
	if(num == 0)	
	{		
		fp=fopen("������Ϸ���.log","w");	
	}	
	else	
	{		
		fp=fopen("������Ϸ���.log","a");	
	}	
	if(fp == NULL)	
		return ;	
	num++;	
	SYSTEMTIME time;	
	GetLocalTime(&time);	
	fprintf(fp, "%d:%s - %d/%d/%d %d:%d:%d \n", num, buf, time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);	
	fclose(fp); 
}
//////////////////////////////////////////////////////////////////////////
//���캯��
CTableFrameSink::CTableFrameSink()
{
	//��ʼ����
	ZeroMemory(m_lUserStartScore,sizeof(m_lUserStartScore));

	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//���Ʊ���
	m_cbWinSideControl=0;
	m_nSendCardCount=0;
	m_bControl=false;
	m_wCheatArea = 255;

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//�˿���Ϣ
	ZeroMemory(m_cbCardCount,sizeof(m_cbCardCount));
	ZeroMemory(m_cbTableCardArray,sizeof(m_cbTableCardArray));
	ZeroMemory(m_cbDartPoint, sizeof(m_cbDartPoint));
	memset(m_cbDartFlag, INVALID_CHAIR, sizeof(m_cbDartFlag));
	memset(m_wSitDownInfo, INVALID_CHAIR,sizeof(m_wSitDownInfo));

	//״̬����
	m_dwJettonTime=0L;
	m_bExchangeBanker=true;
	m_wAddTime=0;
	CopyMemory(m_szControlName,TEXT("����"),sizeof(m_szControlName));

	//ׯ����Ϣ
	m_ApplyUserArray.RemoveAll();
	m_wCurrentBanker=INVALID_CHAIR;
	m_wBankerTime=0;
	m_lBankerWinScore=0L;		
	m_lBankerCurGameScore=0L;
	m_bEnableSysBanker=true;
	m_cbLeftCardCount=0;
	m_bContiueCard=false;

	//��¼����
	ZeroMemory(m_GameRecordArrary,sizeof(m_GameRecordArrary));
	m_nRecordFirst=0;
	m_nRecordLast=0;
	m_dwRecordCount=0;
	ZeroMemory(m_rgArea, sizeof(m_rgArea));
	ZeroMemory(&m_tOdds, sizeof(m_tOdds));

	//�����˿���
	m_nChipRobotCount = 0;
	m_nRobotListMaxCount =0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//�������
	m_hInst = NULL;
	m_pServerContro = NULL;
	m_hInst = LoadLibrary(TEXT("DartServerControl.dll"));
	if ( m_hInst )
	{
		typedef void * (*CREATE)(); 
		CREATE ServerControl = (CREATE)GetProcAddress(m_hInst,"CreateServerControl"); 
		if ( ServerControl )
		{
			m_pServerContro = static_cast<IServerControl*>(ServerControl());
		}
	}

	return;
}

//��������
CTableFrameSink::~CTableFrameSink()
{
}

VOID CTableFrameSink::Release()
{
	//if( m_pServerContro )
	//{
	//	delete m_pServerContro;
	//	m_pServerContro = NULL;
	//}

	if( m_hInst )
	{
		FreeLibrary(m_hInst);
		m_hInst = NULL;
	}
	delete this; 
}
//�ӿڲ�ѯ
VOID * CTableFrameSink::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrameSink,Guid,dwQueryVer);
	QUERYINTERFACE(ITableUserAction,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameSink,Guid,dwQueryVer);
	return NULL;
}

//��������
bool CTableFrameSink::Initialization(IUnknownEx * pIUnknownEx)
{
	//��ѯ�ӿ�
	m_pITableFrame=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableFrame);

	//�����ж�
	if (m_pITableFrame==NULL)
	{
		CTraceService::TraceString(TEXT("��Ϸ���� CTableFrameSink ��ѯ ITableFrame �ӿ�ʧ��"),TraceLevel_Exception);
		return false;
	}

	//��ʼģʽ
	m_pITableFrame->SetStartMode(START_MODE_TIME_CONTROL);

	m_pGameServiceOption = m_pITableFrame->GetGameServiceOption();
	m_pGameServiceAttrib = m_pITableFrame->GetGameServiceAttrib();

	ASSERT(m_pGameServiceOption!=NULL);
	ASSERT(m_pGameServiceAttrib!=NULL);
	//�����ļ���
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);
	_sntprintf(m_szConfigFileName,sizeof(m_szConfigFileName),TEXT("%s\\DartConfig.ini"),szPath);

	//��������
	memcpy(m_szRoomName, m_pGameServiceOption->szServerName, sizeof(m_pGameServiceOption->szServerName));

	ReadConfigInformation();

	return true;
}

//��λ����
VOID CTableFrameSink::RepositionSink()
{
	//����ע��
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));

	//������ע
	ZeroMemory(m_lUserJettonScore,sizeof(m_lUserJettonScore));

	//��ҳɼ�	
	ZeroMemory(m_lUserWinScore,sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore,sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue,sizeof(m_lUserRevenue));

	//����Ͷ������
	ZeroMemory(m_cbDartPoint, sizeof(m_cbDartPoint));
	memset(m_cbDartFlag, INVALID_CHAIR, sizeof(m_cbDartFlag));
	m_ApplyUserArray.RemoveAll();

	//�����˿���
	m_nChipRobotCount = 0;
	m_bControl=false;
	m_wCheatArea = 255;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	return;
}

//��ѯ�޶�
SCORE CTableFrameSink::QueryConsumeQuota(IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem->GetUserStatus() == US_PLAYING)
	{
		return 0L;
	}
	else
	{
		return __max(pIServerUserItem->GetUserScore()-m_pGameServiceOption->lMinTableScore, 0L);
	}
}

//�����¼�
bool CTableFrameSink::OnUserScroeNotify(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	//��ׯ�ҷ����ڿ���ʱ��䶯ʱ(��ׯ�ҽ����˴�ȡ��)У��ׯ�ҵ���ׯ����
	if(wChairID == m_wCurrentBanker && m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE)
	{
		ChangeBanker(false);
	}

	return true;
}

//��Ϸ��ʼ
bool CTableFrameSink::OnEventGameStart()
{
	//�����������
	srand(GetTickCount());

	//���Ϳ����Ϣ
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��ȡ�û�
		IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if ( pIServerUserItem == NULL )
			continue;

		if( CUserRight::IsGameCheatUser(m_pITableFrame->GetTableUserItem(i)->GetUserRight()))
		{
			CString strInfo;
			strInfo.Format(TEXT("��ǰ��棺%I64d"), m_lStorageCurrent);

			m_pITableFrame->SendGameMessage(pIServerUserItem,strInfo,SMT_CHAT);
		}	
	}
	
	CString strStorage;
	CTime Time(CTime::GetCurrentTime());
	strStorage.Format(TEXT(" ����: %s | ʱ��: %d-%d-%d %d:%d:%d | ���: %I64d \n"), m_pGameServiceOption->szServerName, Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(), m_lStorageCurrent );
	WriteInfo(TEXT("������Ϸ�����־.log"), strStorage);
	
	//��������
	CMD_S_GameStart GameStart;
	ZeroMemory(&GameStart,sizeof(GameStart));

	//��ȡ����
	memcpy(&GameStart.tOdds, &m_tOdds, sizeof(GameStart.tOdds));

	//��ȡׯ��
	IServerUserItem *pIBankerServerUserItem=NULL;
	if (INVALID_CHAIR!=m_wCurrentBanker) 
	{
		pIBankerServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		m_lBankerScore=pIBankerServerUserItem->GetUserScore();
	}

	//���ñ���
	GameStart.cbTimeLeave=m_nPlaceJettonTime;
	GameStart.wBankerUser=m_wCurrentBanker;
	if (pIBankerServerUserItem!=NULL) GameStart.lBankerScore=m_lBankerScore;
	GameStart.bContiueCard=m_bContiueCard;

	//��ע����������
	int nChipRobotCount = 0;
	for (int i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	GameStart.nChipRobotCount = min(nChipRobotCount, m_nMaxChipRobot);
	
	nChipRobotCount = 0;
	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++) 
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray.GetAt(i));
		if (pIServerUserItem != NULL && pIServerUserItem->IsAndroidUser())
			nChipRobotCount++;
	}

	if(nChipRobotCount > 0)
		GameStart.nAndriodApplyCount=nChipRobotCount-1;

	//�����˿���
	m_nChipRobotCount = 0;
	ZeroMemory(m_lRobotAreaScore, sizeof(m_lRobotAreaScore));

	//�Թ����
	m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	

	//��Ϸ���
	for (WORD wChairID=0; wChairID<GAME_PLAYER; ++wChairID)
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem==NULL) continue;
		int iTimer = 10;

		//���û���
		GameStart.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*iTimer);

		m_pITableFrame->SendTableData(wChairID,SUB_S_GAME_START,&GameStart,sizeof(GameStart));	
	}

	return true;
}

//��Ϸ����
bool CTableFrameSink::OnEventGameConclude(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbReason)
{
	ControlWinArea();

	switch (cbReason)
	{
	case GER_NORMAL:		//�������	
		{
			//��Ϸ����
// 			if( m_pServerContro != NULL && m_pServerContro->NeedControl() )
// 			{
// 				//��ȡ����Ŀ������
// 				m_bControl=true;
// 				m_pServerContro->GetSuitResult(m_cbTableCardArray, m_cbTableCard, m_lAllJettonScore);
// 				m_pServerContro->CompleteControl();
// 			}
// 			else
// 			{	
// 				StorageOptimize();		
// 			}

			//�������
			CalculateScore();
			
			//��������
			m_wBankerTime++;

			//������Ϣ
			CMD_S_GameEnd GameEnd;
			ZeroMemory(&GameEnd,sizeof(GameEnd));
			//�˿���Ϣ
			CopyMemory(GameEnd.cbDartPoint, m_cbDartPoint, sizeof(m_cbDartPoint));
			GameEnd.cbTotalPoint = m_cbDartPoint[0] + m_cbDartPoint[1];

			//���ͻ���
			GameEnd.cbTimeLeave=m_nGameEndTime;	
			for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
				if (pIServerUserItem == NULL)
				{
					continue;
				}

				//���óɼ�
				GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

				//��������
				GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

				//����˰��
				if (m_lUserRevenue[wUserIndex] > 0)
				{
					GameEnd.lRevenue = m_lUserRevenue[wUserIndex];
				}
				else
				{
					GameEnd.lRevenue = 0;
				}

				//������Ϣ					
				m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
			}

			return true;
		}
	case GER_USER_LEAVE:		//�û��뿪
	case GER_NETWORK_ERROR:
		{
			//�м��ж�
			if (m_wCurrentBanker!=wChairID)
			{
				//��������
				LONGLONG lRevenue=0;

				//�Ƿ���
				if (m_pITableFrame->GetGameStatus() == GAME_SCENE_PLACE_JETTON)
				{
					//������ע
					for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
					{
						if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
						{
							CMD_S_PlaceJettonFail PlaceJettonFail;
							ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
							PlaceJettonFail.lJettonArea=nAreaIndex;
							PlaceJettonFail.lPlaceScore=m_lUserJettonScore[nAreaIndex][wChairID];
							PlaceJettonFail.wPlaceUser=wChairID;

							//��Ϸ���
							for (WORD i=0; i<GAME_PLAYER; ++i)
							{
								IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
								if (pIServerUserItem==NULL) 
									continue;

								m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
							}

							m_lAllJettonScore[nAreaIndex] -= m_lUserJettonScore[nAreaIndex][wChairID];
							m_lUserJettonScore[nAreaIndex][wChairID] = 0;
						}
					}
				}
				else
				{
					//д�����
					if (m_lUserWinScore[wChairID]!=0L) 
					{
						tagScoreInfo ScoreInfo[GAME_PLAYER];
						ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
						//�����
						if (!pIServerUserItem->IsAndroidUser())
							m_lStorageCurrent -= (m_lUserWinScore[wChairID]+m_lUserRevenue[wChairID]);						
						ScoreInfo[wChairID].lScore = m_lUserWinScore[wChairID];
						ScoreInfo[wChairID].cbType=m_lUserWinScore[wChairID]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
						ScoreInfo[wChairID].lRevenue = m_lUserRevenue[wChairID];
						m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));
						m_lUserWinScore[wChairID] = 0;
					}
					//�����ע
					for (int nAreaIndex = 1; nAreaIndex <= AREA_COUNT; nAreaIndex++)
					{
						m_lUserJettonScore[nAreaIndex][wChairID] = 0;
					}
				}

				return true;
			}

			//״̬�ж�
			if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_GAME_END)
			{
				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("����ׯ��[ %s ]ǿ�ˣ���Ϸ��ǰ������"),pIServerUserItem->GetNickName());

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	

				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_GAME_END);

				//����ʱ��
				m_dwJettonTime=(DWORD)time(NULL);
				m_pITableFrame->KillGameTimer(IDI_PLACE_JETTON);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, (DWORD)m_nGameEndTime * 1000, 1, 0L);

				//��Ϸ����
				if( m_pServerContro != NULL && m_pServerContro->NeedControl() )
				{
					//��ȡ����Ŀ������
					m_pServerContro->GetSuitResult(m_cbTableCardArray, m_cbTableCard, m_lAllJettonScore);
					m_pServerContro->CompleteControl();
				}

				//�������
				CalculateScore();

				//������Ϣ
				CMD_S_GameEnd GameEnd;
				ZeroMemory(&GameEnd,sizeof(GameEnd));

				//ׯ����Ϣ
				GameEnd.nBankerTime = m_wBankerTime;
				GameEnd.lBankerTotallScore=m_lBankerWinScore;
				GameEnd.lBankerScore=m_lUserWinScore[m_wCurrentBanker];

				//�˿���Ϣ
				memcpy(GameEnd.cbDartPoint, m_cbDartPoint, sizeof(GameEnd.cbDartPoint));
				GameEnd.cbTotalPoint = m_cbDartPoint[0] + m_cbDartPoint[1];

				//���ͻ���
				GameEnd.cbTimeLeave=m_nGameEndTime;	
				for ( WORD wUserIndex = 0; wUserIndex < GAME_PLAYER; ++wUserIndex )
				{
					IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserIndex);
					if ( pIServerUserItem == NULL ) continue;

					//���óɼ�
					GameEnd.lUserScore=m_lUserWinScore[wUserIndex];

					//��������
					GameEnd.lUserReturnScore=m_lUserReturnScore[wUserIndex];

					//����˰��
					if (m_lUserRevenue[wUserIndex]>0) GameEnd.lRevenue=m_lUserRevenue[wUserIndex];
					else if (m_wCurrentBanker!=INVALID_CHAIR) GameEnd.lRevenue=m_lUserRevenue[m_wCurrentBanker];
					else GameEnd.lRevenue=0;

					//������Ϣ					
					m_pITableFrame->SendTableData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
					m_pITableFrame->SendLookonData(wUserIndex,SUB_S_GAME_END,&GameEnd,sizeof(GameEnd));
				}
			}

			//�۳�����
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			ScoreInfo[m_wCurrentBanker].lScore =m_lUserWinScore[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].lRevenue = m_lUserRevenue[m_wCurrentBanker];
			ScoreInfo[m_wCurrentBanker].cbType = (m_lUserWinScore[m_wCurrentBanker]>0?SCORE_TYPE_WIN:SCORE_TYPE_LOSE);
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			//�����
			IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			if (pIServerUserItem != NULL && !pIServerUserItem->IsAndroidUser())
				m_lStorageCurrent -= (m_lUserWinScore[m_wCurrentBanker]+m_lUserRevenue[m_wCurrentBanker]);

			m_lUserWinScore[m_wCurrentBanker] = 0;

			//�л�ׯ��
			ChangeBanker(true);

			return true;
		}
	}
	ASSERT(FALSE);
	return false;
}

//���ͳ���
bool CTableFrameSink::OnEventSendGameScene(WORD wChairID, IServerUserItem * pIServerUserItem, BYTE cbGameStatus, bool bSendSecret)
{
	switch (cbGameStatus)
	{
	case GAME_SCENE_FREE:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);

			//��������
			CMD_S_StatusFree StatusFree;
			ZeroMemory(&StatusFree,sizeof(StatusFree));			

			//����
			CopyMemory(&StatusFree.tOdds, &m_tOdds, sizeof(StatusFree.tOdds));

			//��λ�ϵ����
			CopyMemory(StatusFree.tSeatInfo, m_wSitDownInfo, sizeof(StatusFree.tSeatInfo));

			//������Ϣ
			StatusFree.lApplyBankerCondition = m_lApplyBankerCondition;
			StatusFree.lAreaLimitScore = m_lAreaLimitScore;
			StatusFree.bGenreEducate = m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
			//ׯ����Ϣ
			StatusFree.bEnableSysBanker=m_bEnableSysBanker;
			StatusFree.wBankerUser=m_wCurrentBanker;	
			StatusFree.cbBankerTime=m_wBankerTime;
			StatusFree.lBankerWinScore=m_lBankerWinScore;
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
				StatusFree.lBankerScore=pIServerUserItem->GetUserScore();
			}
			StatusFree.nEndGameMul = m_nEndGameMul;

			//�����Ϣ
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				StatusFree.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*10); 
			}

			//��������
			CopyMemory(StatusFree.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusFree.szGameRoomName));

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			StatusFree.cbTimeLeave=(BYTE)(m_nFreeTime-__min(dwPassTime,(DWORD)m_nFreeTime));
			
			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusFree.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusFree,sizeof(StatusFree));

			//������ʾ
			TCHAR szTipMsg[128];
			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
				m_lAreaLimitScore,m_lUserLimitScore);

			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

			//����������
			//SendApplyUser(pIServerUserItem);
			
			//���¿����Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));
				
				updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				updateStorage.lStorageStart = m_lStorageStart;
				updateStorage.lStorageDeduct = m_nStorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;
			
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}

			return bSuccess;
		}
	case GAME_SCENE_PLACE_JETTON:		//��ע״̬
	case GAME_SCENE_DART:				//�����״̬
	case GAME_SCENE_GAME_END:			//����״̬
		{
			//���ͼ�¼
			SendGameRecord(pIServerUserItem);		

			//��������
			CMD_S_StatusPlay StatusPlay;
			ZeroMemory(&StatusPlay, sizeof(StatusPlay));
			memset(&StatusPlay.tSeatInfo, INVALID_CHAIR, sizeof(StatusPlay.tSeatInfo));

			//����
			CopyMemory(&StatusPlay.tOdds, &m_tOdds, sizeof(StatusPlay.tOdds));

			//��λ�ϵ����
			CopyMemory(StatusPlay.tSeatInfo, m_wSitDownInfo, sizeof(StatusPlay.tSeatInfo));

			//���ڵ���
			CopyMemory(StatusPlay.cbPoint, m_cbDartPoint, sizeof(StatusPlay.cbPoint));

			for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
			{
				if (pIServerUserItem->GetChairID() == m_ApplyUserArray[nUserIdx])
				{
					StatusPlay.bIsBanker = true;
					break;
				}
			}

			//ȫ����ע
			CopyMemory(StatusPlay.lAllJettonScore,m_lAllJettonScore,sizeof(StatusPlay.lAllJettonScore));

			//�����ע
			if (pIServerUserItem->GetUserStatus()!=US_LOOKON)
			{
				for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex)
					StatusPlay.lUserJettonScore[nAreaIndex] = m_lUserJettonScore[nAreaIndex][wChairID];

				//�����ע
				StatusPlay.lUserMaxScore=min(pIServerUserItem->GetUserScore(),m_lUserLimitScore*10);
			}

			//������Ϣ
			StatusPlay.lApplyBankerCondition=m_lApplyBankerCondition;		
			StatusPlay.lAreaLimitScore=m_lAreaLimitScore;		
			StatusPlay.bGenreEducate = m_pGameServiceOption->wServerType&GAME_GENRE_EDUCATE;
			//ׯ����Ϣ
			StatusPlay.bEnableSysBanker=m_bEnableSysBanker;
			StatusPlay.wBankerUser=m_wCurrentBanker;			
			StatusPlay.cbBankerTime=m_wBankerTime;
			StatusPlay.lBankerWinScore=m_lBankerWinScore;	
			if (m_wCurrentBanker!=INVALID_CHAIR)
			{
				StatusPlay.lBankerScore=m_lBankerScore;
			}	
			StatusPlay.nEndGameMul = m_nEndGameMul;

			//ȫ����Ϣ
			DWORD dwPassTime=(DWORD)time(NULL)-m_dwJettonTime;
			LONGLONG	nTotalTime = (cbGameStatus==GAME_SCENE_PLACE_JETTON?m_nPlaceJettonTime:m_nGameEndTime);
			if (cbGameStatus == GAME_SCENE_DART)
			{
				nTotalTime = m_nDartTime;
			}
			StatusPlay.cbTimeLeave=(BYTE)(nTotalTime-__min(dwPassTime,(DWORD)nTotalTime));
			StatusPlay.cbGameStatus=m_pITableFrame->GetGameStatus();			

			//��������
			CopyMemory(StatusPlay.szGameRoomName, m_pGameServiceOption->szServerName, sizeof(StatusPlay.szGameRoomName));

			//�����ж�
			if (cbGameStatus==GAME_SCENE_GAME_END)
			{
				//���óɼ�
				StatusPlay.lEndUserScore=m_lUserWinScore[wChairID];

				//��������
				StatusPlay.lEndUserReturnScore=m_lUserReturnScore[wChairID];

				//����˰��
				if (m_lUserRevenue[wChairID]>0) StatusPlay.lEndRevenue=m_lUserRevenue[wChairID];
				//else if (m_wCurrentBanker!=INVALID_CHAIR) StatusPlay.lEndRevenue=m_lUserRevenue[m_wCurrentBanker];
				else StatusPlay.lEndRevenue=0;

				//ׯ�ҳɼ�
				StatusPlay.lEndBankerScore=m_lBankerCurGameScore;

				//�˿���Ϣ
				CopyMemory(StatusPlay.cbTableCardArray,m_cbTableCardArray,sizeof(m_cbTableCardArray));
			}
			
			//����������
			if(pIServerUserItem->IsAndroidUser())
			{
				tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
				ASSERT(pCustomConfig);

				CopyMemory(&StatusPlay.CustomAndroid, &pCustomConfig->CustomAndroid, sizeof(tagCustomAndroid));
			}

			//���ͳ���
			bool bSuccess = m_pITableFrame->SendGameScene(pIServerUserItem,&StatusPlay,sizeof(StatusPlay));

			//������ʾ
			TCHAR szTipMsg[128];
// 			_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ����Ϊ��%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
// 				m_lAreaLimitScore,m_lUserLimitScore);

//			m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT);

			//����������
			SendApplyUser( pIServerUserItem );
			
			//���¿����Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));
				
				updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				updateStorage.lStorageStart = m_lStorageStart;
				updateStorage.lStorageDeduct = m_nStorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;
			
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}
			
			//���������ע��Ϣ
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()))
			{
				SendUserBetInfo(pIServerUserItem);
			}

			return bSuccess;
		}
	}

	return false;
}

//��ʱ���¼�
bool CTableFrameSink::OnTimerMessage(DWORD wTimerID, WPARAM wBindParam)
{
	switch (wTimerID)
	{
	case IDI_FREE:		//����ʱ��
		{
			//��ʼ��Ϸ
			m_pITableFrame->StartGame();

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_PLACE_JETTON, (DWORD)m_nPlaceJettonTime * 1000, 1, 0L);

			//�ɷ��˿�
			//DispatchTableCard();

			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_PLACE_JETTON);

			return true;
		}
	case IDI_PLACE_JETTON:		//��עʱ��
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_DART);

			//����ʱ��
			m_dwJettonTime = (DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_DART, (DWORD)m_nDartTime * 1000, 1, 0L);

			//������Ϣ
			CMD_S_StartDart startDart;
			ZeroMemory(&startDart, sizeof(startDart));
			startDart.wFirstDartUser = INVALID_CHAIR;
			startDart.wSecDartUser = INVALID_CHAIR;

			startDart.cbTimeLeave = m_nDartTime;

			if (m_ApplyUserArray.GetCount() >= 2)
			{
				startDart.wFirstDartUser = m_ApplyUserArray.GetAt(0);
				startDart.wSecDartUser = m_ApplyUserArray.GetAt(1);
			}
			else if (m_ApplyUserArray.GetCount() == 1)
			{
				startDart.wFirstDartUser = m_ApplyUserArray.GetAt(0);
			}

			/*
			startDart.cbPoint[0] = rand() % 6 + 1;
			startDart.cbPoint[1] = rand() % 6 + 1;
			memcpy(m_cbDartPoint,startDart.cbPoint,sizeof(m_cbDartPoint));
			*/

			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_StartDart, &startDart, sizeof(startDart));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_StartDart, &startDart, sizeof(startDart));

			return true;
		}
	case IDI_DART:		//�����ʱ��
		{
			//״̬�ж�(��ֹǿ���ظ�����)
			if (m_pITableFrame->GetGameStatus() != GAME_SCENE_GAME_END)
			{
				//�����̨�б��б����Ѿ�������ڵ����
				for (int i = 0; i < 2;	i++)
				{
					if (m_cbDartFlag[i] >= 0 && m_cbDartFlag[i] != INVALID_CHAIR)
					{
						for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
						{
							if (m_cbDartFlag[i] == m_ApplyUserArray[nUserIdx])
							{
								m_ApplyUserArray.RemoveAt(nUserIdx);
								break;
							}
						}
					}
					else//���û����ҷ��䣬��ϵͳ�Զ�����
					{
						OnDart(INVALID_CHAIR);
					}
				}

				/*
				//����״̬
				m_pITableFrame->SetGameStatus(GAME_SCENE_GAME_END);
				//������Ϸ
				OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

				//����ʱ��
				m_dwJettonTime = (DWORD)time(NULL);
				m_pITableFrame->SetGameTimer(IDI_GAME_END, (DWORD)m_nGameEndTime * 1000, 1, 0L);
				*/
			}

			return true;
		}
	case IDI_GAME_END:			//������Ϸ
		{
			//д�����
			tagScoreInfo ScoreInfo[GAME_PLAYER];
			ZeroMemory(ScoreInfo,sizeof(ScoreInfo));
			for ( WORD wUserChairID = 0; wUserChairID < GAME_PLAYER; ++wUserChairID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserChairID);				
				if ( pIServerUserItem == NULL||(QueryBuckleServiceCharge(wUserChairID)==false)) continue;				
				
				//ʤ������
				BYTE ScoreKind=(m_lUserWinScore[wUserChairID]>0L)?SCORE_TYPE_WIN:SCORE_TYPE_LOSE;
				ScoreInfo[wUserChairID].lScore = m_lUserWinScore[wUserChairID];
				ScoreInfo[wUserChairID].lRevenue = m_lUserRevenue[wUserChairID];
				ScoreInfo[wUserChairID].cbType = ScoreKind;

				//�����
				if (!pIServerUserItem->IsAndroidUser())
				{
					m_lStorageCurrent -= (m_lUserWinScore[wUserChairID]+m_lUserRevenue[wUserChairID]);					
				}
			}
			
			//д�����
			m_pITableFrame->WriteTableScore(ScoreInfo,CountArray(ScoreInfo));

			LONGLONG lTempDeduct=0;
			lTempDeduct=m_nStorageDeduct;
			bool bDeduct=NeedDeductStorage();
			lTempDeduct=bDeduct?lTempDeduct:0;
			//���˥��
			if (m_lStorageCurrent > 0)
				m_lStorageCurrent = m_lStorageCurrent - m_lStorageCurrent*lTempDeduct/1000;

			//������Ϸ
			m_pITableFrame->ConcludeGame(GAME_SCENE_FREE);

			//�л�ׯ��
			ChangeBanker(false);

			//����ʱ��
			m_dwJettonTime=(DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_FREE, (DWORD)m_nFreeTime * 1000, 1, 0L);

			//������Ϣ
			CMD_S_GameFree GameFree;
			ZeroMemory(&GameFree,sizeof(GameFree));
			GameFree.cbTimeLeave=m_nFreeTime;
			GameFree.lStorageStart=m_lStorageCurrent;
			GameFree.nListUserCount=m_ApplyUserArray.GetCount()-1;
			m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_GAME_FREE,&GameFree,sizeof(GameFree));
			
			//���¿����Ϣ
			for ( WORD wUserID = 0; wUserID < GAME_PLAYER; ++wUserID )
			{
				IServerUserItem *pIServerUserItem = m_pITableFrame->GetTableUserItem(wUserID);
				if ( pIServerUserItem == NULL ) continue;
				if( !CUserRight::IsGameCheatUser( pIServerUserItem->GetUserRight() ) ) continue;

				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));
				
				updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				updateStorage.lStorageStart = m_lStorageStart;
				updateStorage.lStorageDeduct = m_nStorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;
			
				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}

			return true;
		}
	}

	return false;
}

//��Ϸ��Ϣ
bool CTableFrameSink::OnGameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	switch (wSubCmdID)
	{
	case SUB_C_PLACE_JETTON:		//�û���ע
		{
			//Ч������
			ASSERT(wDataSize==sizeof(CMD_C_PlaceJetton));
			if (wDataSize!=sizeof(CMD_C_PlaceJetton)) return false;

			//�û�Ч��
			tagUserInfo * pUserData=pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus!=US_PLAYING) return true;

			//��Ϣ����
			CMD_C_PlaceJetton * pPlaceJetton=(CMD_C_PlaceJetton *)pData;
			return OnUserPlaceJetton(pUserData->wChairID,pPlaceJetton->cbJettonArea,pPlaceJetton->lJettonScore);
		}
	case SUB_C_APPLY_BANKER:		//������ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus == US_LOOKON) return true;

			return OnUserApplyBanker(pIServerUserItem);
		}
	case SUB_C_CANCEL_BANKER:		//ȡ����ׯ
		{
			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus == US_LOOKON) return true;

			return OnUserCancelBanker(pIServerUserItem);
		}
	case SUB_C_DART:
		{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			if (pUserData->cbUserStatus != US_PLAYING) return true;

			for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
			{
				if (nUserIdx > 1)
				{
					break;
				}

				//�Ѿ�������ģ�ͬһ�֣����ܷ���ڶ���
				if (m_cbDartFlag[0] == m_ApplyUserArray[nUserIdx] || m_cbDartFlag[1] == m_ApplyUserArray[nUserIdx])
				{
					continue;
				}

				if (m_ApplyUserArray[nUserIdx] == pUserData->wChairID)
				{
					return OnDart(pUserData->wChairID);
				}
			}
			return true;
		}
	case SUB_C_ALLPLAYER_LIST:
		{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			return GetAllPlayerList(pUserData->wChairID);
		}
	case SUB_C_ALLBANKER_LIST:
		{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			return GetAllBankerList(pUserData->wChairID);
		}
	case SUB_C_ALLRECORD_LIST:
		{
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			return GetAllRecordList(pUserData->wChairID);
		}
	case SUB_C_SITDOWN:
		{
			//Ч������
			ASSERT(wDataSize == sizeof(CMD_C_SitDownOrLeave));
			if (wDataSize != sizeof(CMD_C_SitDownOrLeave)) return false;

			//�û�Ч��
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();

			//��Ϣ����
			CMD_C_SitDownOrLeave * pSeatInfo = (CMD_C_SitDownOrLeave *)pData;
			return OnUserSitDown(*pUserData, pSeatInfo->wSeatID);
		}
	case SUB_C_LEAVESEAT:
		{
			//��Ϣ����
			tagUserInfo * pUserData = pIServerUserItem->GetUserInfo();
			return OnUserLeaveSeat(pUserData->wChairID);
		}
	case SUB_C_AMDIN_COMMAND:
		{
			ASSERT(wDataSize==sizeof(CMD_C_AdminReq));
			if(wDataSize!=sizeof(CMD_C_AdminReq)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			if ( m_pServerContro == NULL)
				return false;
			CopyMemory(m_szControlName,pIServerUserItem->GetNickName(),sizeof(m_szControlName));

			return m_pServerContro->ServerControl(wSubCmdID, pData, wDataSize, pIServerUserItem, m_pITableFrame, m_pGameServiceOption);
		}
	case SUB_C_UPDATE_STORAGE:		//���¿��
		{
			ASSERT(wDataSize==sizeof(CMD_C_UpdateStorage));
			if(wDataSize!=sizeof(CMD_C_UpdateStorage)) return false;

			//Ȩ���ж�
			if(CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())==false)
				return false;

			//��Ϣ����
			CMD_C_UpdateStorage * pUpdateStorage=(CMD_C_UpdateStorage *)pData;
			if (pUpdateStorage->cbReqType==RQ_SET_STORAGE)
			{
				m_nStorageDeduct = pUpdateStorage->lStorageDeduct;
				m_lStorageCurrent = pUpdateStorage->lStorageCurrent;
				m_lStorageMax1 = pUpdateStorage->lStorageMax1;
				m_lStorageMul1 = pUpdateStorage->lStorageMul1;
				m_lStorageMax2 = pUpdateStorage->lStorageMax2;
				m_lStorageMul2 = pUpdateStorage->lStorageMul2;

				//��¼��Ϣ
				CString strControlInfo, str;
				str.Format(TEXT("%s"), TEXT("�޸Ŀ�����ã�"));
				CTime Time(CTime::GetCurrentTime());
				strControlInfo.Format(TEXT("����: %s | ����: %u | ʱ��: %d-%d-%d %d:%d:%d\n�������˺�: %s | ������ID: %u\n%s\r\n"),
					m_pGameServiceOption->szServerName, m_pITableFrame->GetTableID()+1, Time.GetYear(), Time.GetMonth(), Time.GetDay(),
					Time.GetHour(), Time.GetMinute(), Time.GetSecond(), pIServerUserItem->GetNickName(), pIServerUserItem->GetGameID(), str);

				WriteInfo(TEXT("������Ϸ������Ϣ.log"),strControlInfo);
			}
			
			for(WORD wUserID = 0; wUserID < GAME_PLAYER; wUserID++)
			{
				IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(wUserID);
				if ( pIServerUserItemSend == NULL ) continue;
				if( !CUserRight::IsGameCheatUser( pIServerUserItemSend->GetUserRight() ) ) continue;

				if(RQ_REFRESH_STORAGE == pUpdateStorage->cbReqType && pIServerUserItem->GetChairID() != wUserID) continue;

				CMD_S_UpdateStorage updateStorage;
				ZeroMemory(&updateStorage, sizeof(updateStorage));
				
				if(RQ_SET_STORAGE == pUpdateStorage->cbReqType && pIServerUserItem->GetChairID() == wUserID)
				{
					updateStorage.cbReqType = RQ_SET_STORAGE;
				}
				else
				{
					updateStorage.cbReqType = RQ_REFRESH_STORAGE;
				}

				updateStorage.lStorageStart = m_lStorageStart;
				updateStorage.lStorageDeduct = m_nStorageDeduct;
				updateStorage.lStorageCurrent = m_lStorageCurrent;
				updateStorage.lStorageMax1 = m_lStorageMax1;
				updateStorage.lStorageMul1 = m_lStorageMul1;
				updateStorage.lStorageMax2 = m_lStorageMax2;
				updateStorage.lStorageMul2 = m_lStorageMul2;

				m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_UPDATE_STORAGE,&updateStorage,sizeof(updateStorage));
			}

			return true;
		}
	}

	return false;
}

//�����Ϣ
bool CTableFrameSink::OnFrameMessage(WORD wSubCmdID, VOID * pData, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	return false;
}

bool CTableFrameSink::OnActionUserOffLine(WORD wChairID,IServerUserItem * pIServerUserItem)
{
	if(pIServerUserItem == NULL) return false;
	//�л�ׯ��
	if (wChairID==m_wCurrentBanker) ChangeBanker(true);

	//ȡ������
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		if (wChairID!=m_ApplyUserArray[i]) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		//�������
		CMD_S_CancelBanker CancelBanker;
		ZeroMemory(&CancelBanker,sizeof(CancelBanker));

		//���ñ���
		CancelBanker.wCancelUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

		break;
	}

	return true;

}
//�û�����
bool CTableFrameSink::OnActionUserSitDown(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	//��ʼ����
	m_lUserStartScore[wChairID] = pIServerUserItem->GetUserScore();

	//����ʱ��
	if ((bLookonUser==false)&&(m_dwJettonTime==0L))
	{
		m_dwJettonTime=(DWORD)time(NULL);
		m_pITableFrame->SetGameTimer(IDI_FREE, (DWORD)m_nFreeTime * 1000, 1, NULL);
		m_pITableFrame->SetGameStatus(GAME_SCENE_FREE);
	}
	
	//������ʾ
	TCHAR szTipMsg[128];
	_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("��������ׯ��ұ�����ڣ�%I64d,��������Ϊ��%I64d,�������Ϊ��%I64d"),m_lApplyBankerCondition,
		m_lAreaLimitScore,m_lUserLimitScore);
	m_pITableFrame->SendGameMessage(pIServerUserItem,szTipMsg,SMT_CHAT|SMT_EJECT);

	return true;
}

//�û�����
bool CTableFrameSink::OnActionUserStandUp(WORD wChairID,IServerUserItem * pIServerUserItem, bool bLookonUser)
{
	ASSERT(pIServerUserItem !=NULL);

	//��ʼ����
	m_lUserStartScore[wChairID] = 0;

	//��¼�ɼ�
	if (bLookonUser==false)
	{
		//�л�ׯ��
		if (wChairID==m_wCurrentBanker)
		{
			ChangeBanker(true);
			m_bContiueCard=false;
		}

		//ȡ������
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			if (wChairID!=m_ApplyUserArray[i]) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser=wChairID;

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

			break;
		}
	}

	return true;
}


//��ע�¼�
bool CTableFrameSink::OnUserPlaceJetton(WORD wChairID, BYTE cbJettonArea, LONGLONG lJettonScore)
{
	//Ч�����
	ASSERT((cbJettonArea>0 && cbJettonArea<=24)&&(lJettonScore>0L));
	if ((cbJettonArea>24) || (lJettonScore <= 0L) || cbJettonArea<=0)
	{
		return false;
	}
	////Ч��״̬
	//ASSERT(m_pITableFrame->GetGameStatus()==GS_PLACE_JETTON);

	if (m_pITableFrame->GetGameStatus()!=GAME_SCENE_PLACE_JETTON)
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
		return true;
	}
	//ׯ���ж�
// 	if (m_wCurrentBanker==wChairID)
// 	{
// 		return true;
// 	}
// 	if (m_bEnableSysBanker==false && m_wCurrentBanker==INVALID_CHAIR) 
// 	{
// 		return true;
// 	}

	/*ϵͳ��ׯ
	//ׯ�һ���
	LONGLONG lBankerScore = 0;
	if(INVALID_CHAIR != m_wCurrentBanker)
	{
		IServerUserItem * pIServerUserItemBanker = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		lBankerScore = pIServerUserItemBanker->GetUserScore();
	}
	*/
	//��������
	IServerUserItem * pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	LONGLONG lJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lJettonCount += m_lUserJettonScore[nAreaIndex][wChairID];
	LONGLONG lAllJettonCount=0L;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lAllJettonCount += m_lAllJettonScore[nAreaIndex];

	//��һ���
	LONGLONG lUserScore = pIServerUserItem->GetUserScore();

	//�Ϸ�У��
	if (lUserScore < lJettonCount + lJettonScore) return true;
	if (m_lUserLimitScore < lJettonCount + lJettonScore) return true;

	//�ɹ���ʶ
	bool bPlaceJettonSuccess=true;

	//�Ϸ���֤
	if (GetUserMaxJetton(wChairID, cbJettonArea) >= lJettonScore)
	{
		//��������֤
		if(pIServerUserItem->IsAndroidUser())
		{
			//��������
			if (m_lRobotAreaScore[cbJettonArea] + lJettonScore > m_lRobotAreaLimit)
				return true;

			//��Ŀ����
			bool bHaveChip = false;
			for (int i = 0; i <= AREA_COUNT; i++)
			{
				if (m_lUserJettonScore[i+1][wChairID] != 0)
					bHaveChip = true;
			}

			if (!bHaveChip)
			{
				if (m_nChipRobotCount+1 > m_nMaxChipRobot)
				{
					bPlaceJettonSuccess = false;
				}
				else
					m_nChipRobotCount++;
			}

			//ͳ�Ʒ���
			if (bPlaceJettonSuccess)
				m_lRobotAreaScore[cbJettonArea] += lJettonScore;
		}

		if (bPlaceJettonSuccess)
		{
			//������ע
			m_lAllJettonScore[cbJettonArea] += lJettonScore;
			m_lUserJettonScore[cbJettonArea][wChairID] += lJettonScore;			
		}
	}
	else
	{
		bPlaceJettonSuccess=false;
	}

	if (bPlaceJettonSuccess)
	{
		if (CUserRight::IsGameCheatUser(m_pITableFrame->GetTableUserItem(wChairID)->GetUserRight()))
		{
			m_wCheatArea = cbJettonArea;
		}
		for(WORD i=0; i<GAME_PLAYER; i++)
		{
			IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
			if ( pIServerUserItemSend == NULL ) continue;
			
			bool bIsGameCheatUser =  CUserRight::IsGameCheatUser(pIServerUserItemSend->GetUserRight());

			//��������
			CMD_S_PlaceJetton PlaceJetton;
			ZeroMemory(&PlaceJetton,sizeof(PlaceJetton));

			//�������
			PlaceJetton.wChairID=wChairID;
			PlaceJetton.cbJettonArea=cbJettonArea;
			PlaceJetton.lJettonScore=lJettonScore;

			if(bIsGameCheatUser || i == wChairID)
			{
				PlaceJetton.bIsAndroid=pIServerUserItem->IsAndroidUser();
				PlaceJetton.bAndroid=pIServerUserItem->IsAndroidUser()?TRUE:FALSE;
			}
			else
			{
				PlaceJetton.bIsAndroid=true;
				PlaceJetton.bAndroid=true;
			}

			//������Ϣ
			m_pITableFrame->SendTableData(i,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
			m_pITableFrame->SendLookonData(i,SUB_S_PLACE_JETTON,&PlaceJetton,sizeof(PlaceJetton));
		}

		//���������ע��Ϣ
		if(!pIServerUserItem->IsAndroidUser())
		{
			for(WORD i=0; i<GAME_PLAYER; i++)
			{
				IServerUserItem * pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
				if(NULL == pIServerUserItemSend) continue;
				if(!CUserRight::IsGameCheatUser(pIServerUserItemSend->GetUserRight())) continue;

				SendUserBetInfo(pIServerUserItemSend);
			}
		}
		/*ϵͳ��ׯ��������������
		//��ע�ܶ��ׯ����֧������ʱ��ǰ�������
		if(INVALID_CHAIR != m_wCurrentBanker)
		{
			LONGLONG lBankerCanUseScore = lBankerScore*m_nEndGameMul/100;
			if(lBankerCanUseScore <= (lAllJettonCount + lJettonScore + 100)*10)
			{
				if (m_pITableFrame->GetGameStatus() != GAME_SCENE_DART)
				{
					//����״̬
					m_pITableFrame->SetGameStatus(GAME_SCENE_DART);

					//������Ϸ
					//OnEventGameConclude(INVALID_CHAIR,NULL,GER_NORMAL);

					//����ʱ��
					m_dwJettonTime=(DWORD)time(NULL);
					m_pITableFrame->SetGameTimer(IDI_DART, (DWORD)m_nDartTime * 1000, 1, 0L);
				}
			}
		}
		*/
	}
	else
	{
		CMD_S_PlaceJettonFail PlaceJettonFail;
		ZeroMemory(&PlaceJettonFail,sizeof(PlaceJettonFail));
		PlaceJettonFail.lJettonArea=cbJettonArea;
		PlaceJettonFail.lPlaceScore=lJettonScore;
		PlaceJettonFail.wPlaceUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendTableData(wChairID,SUB_S_PLACE_JETTON_FAIL,&PlaceJettonFail,sizeof(PlaceJettonFail));
	}

	return true;
}

//�������
bool CTableFrameSink::OnDart(WORD wChairID)
{
	if (m_cbDartPoint[0] <= 0)
	{
		m_cbDartPoint[0] = rand() % 6 + 1;
		m_cbDartFlag[0] = wChairID;
	}
	else
	{
		if (m_cbDartPoint[1] <= 0)
		{
			m_cbDartPoint[1] = rand() % 6 + 1;
			m_cbDartFlag[1] = wChairID;
		}
	}

	for (INT_PTR nUserIdx = 0; nUserIdx < m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		if (wChairID == m_ApplyUserArray[nUserIdx] && wChairID != INVALID_CHAIR)
		{
			m_ApplyUserArray.RemoveAt(nUserIdx);
			break;
		}
	}

	//���͵�һ��Ͷ�����ڵĽ��
	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItemSend == NULL) continue;
		CMD_S_DartPoint sPoint;
		ZeroMemory(&sPoint,sizeof(sPoint));
		sPoint.wChairID = wChairID;
		memcpy(sPoint.cbDartPoint, m_cbDartPoint, sizeof(sPoint.cbDartPoint));

		//������Ϣ
		m_pITableFrame->SendTableData(i, SUB_S_DART, &sPoint, sizeof(sPoint));
		m_pITableFrame->SendLookonData(i, SUB_S_DART, &sPoint, sizeof(sPoint));
	}

	//����Ͷ����ϣ���Ϸ�������������
	if (m_cbDartPoint[0] > 0 && m_cbDartPoint[1] > 0)
	{
		if (m_pITableFrame->GetGameStatus() != GAME_SCENE_GAME_END)
		{
			//����״̬
			m_pITableFrame->SetGameStatus(GAME_SCENE_GAME_END);

			//������Ϸ
			OnEventGameConclude(INVALID_CHAIR, NULL, GER_NORMAL);

			//����ʱ��
			m_dwJettonTime = (DWORD)time(NULL);
			m_pITableFrame->SetGameTimer(IDI_GAME_END, (DWORD)m_nGameEndTime * 1000, 1, 0L);
		}
	}

	return true;
}

//�����˿�
bool CTableFrameSink::DispatchTableCard()
{
	//��������
	int const static nDispatchCardCount=25;
	bool bContiueCard=m_bContiueCard;
	if (m_cbLeftCardCount < nDispatchCardCount)
	{
		bContiueCard = false;
	}

	//bContiueCard = false;

	//��������
	if (true==bContiueCard)
	{
		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0],m_cbTableCard,sizeof(m_cbTableCardArray));
		m_cbLeftCardCount -= nDispatchCardCount;
		if (m_cbLeftCardCount>0) CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
	}
	else
	{
		//����ϴ��
		BYTE bcCard[CARD_COUNT];
		m_GameLogic.RandCardList(bcCard,52);
		m_bcFirstPostCard = bcCard[0];
		m_GameLogic.RandCardList(m_cbTableCard,CountArray(m_cbTableCard));	

		//�����˿�
		CopyMemory(&m_cbTableCardArray[0][0], m_cbTableCard, sizeof(m_cbTableCardArray));

// #ifdef DEBUG  
// 		{
// 
// 			BYTE temp1[5]={0x11,0x1A,0x3D,0x12,0x05};
// 			BYTE temp2[5]={0x41,0x3a,0x2c,0x08,0x03};
// 			BYTE temp3[5]={0x2A,0x0C,0x18,0x33,0x31};
// 			BYTE temp4[5]={0x27,0x03,0x42,0x0D,0x07};
// 			BYTE temp5[5]={0x16,0x12,0x1C,0x3B,0x3A};
// 			CopyMemory(&m_cbTableCardArray[0][0], temp1, sizeof(m_cbTableCardArray[0]));
// 			CopyMemory(&m_cbTableCardArray[1][0], temp2, sizeof(m_cbTableCardArray[1]));
// 			CopyMemory(&m_cbTableCardArray[2][0], temp3, sizeof(m_cbTableCardArray[2]));
// 			CopyMemory(&m_cbTableCardArray[3][0], temp4, sizeof(m_cbTableCardArray[3]));
// 			CopyMemory(&m_cbTableCardArray[4][0], temp5, sizeof(m_cbTableCardArray[3]));
// 
// 		}
// #endif

		//ׯ���ж�
		if (INVALID_CHAIR != m_wCurrentBanker)
		{
			m_cbLeftCardCount=CountArray(m_cbTableCard)-nDispatchCardCount;
			CopyMemory(m_cbTableCard, m_cbTableCard + nDispatchCardCount, sizeof(BYTE)*m_cbLeftCardCount);
		}
		else
		{
			m_cbLeftCardCount=0;
		}
	}

	//������Ŀ
	for (int i=0; i<CountArray(m_cbCardCount); ++i) m_cbCardCount[i]=5;

	//���Ʊ�־
	m_bContiueCard=false;

	return true;
}

//����б�
bool CTableFrameSink::GetAllPlayerList(WORD wChairID)
{
	CMD_S_PlayerList sList;
	memset(&sList, INVALID_CHAIR, sizeof(sList));

	int iIdx = 0;
	for (int i = 0; i < GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem = m_pITableFrame->GetTableUserItem(i);
		if (pUserItem != NULL)
		{
			sList.wPlayerList[iIdx] = pUserItem->GetChairID();
			iIdx++;
		}
	}

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID, SUB_S_PLAYER_LIST, &sList, sizeof(sList));
	m_pITableFrame->SendLookonData(wChairID, SUB_S_PLAYER_LIST, &sList, sizeof(sList));
	return true;
}

//��̨�б�
bool CTableFrameSink::GetAllBankerList(WORD wChairID)
{
	CMD_S_BankerList sList;
	memset(&sList, INVALID_CHAIR, sizeof(sList));
	
	int iIdx = 0;
	for (INT_PTR nUserIdx = 0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		sList.wBankList[iIdx] = m_ApplyUserArray[nUserIdx];
		iIdx++;
	}

	//������Ϣ
	m_pITableFrame->SendTableData(wChairID, SUB_S_BANKER_LIST, &sList, sizeof(sList));
	m_pITableFrame->SendLookonData(wChairID, SUB_S_BANKER_LIST, &sList, sizeof(sList));
	return true;
}

//k������
bool CTableFrameSink::GetAllRecordList(WORD wChairID)
{
	CMD_S_RecordList sList;
	ZeroMemory(&sList, sizeof(sList));

	memcpy(sList.tGameRecordArrary, m_GameRecordArrary, sizeof(sList.tGameRecordArrary));
	//������Ϣ
	m_pITableFrame->SendTableData(wChairID, SUB_S_RECORD_LIST, &sList, sizeof(sList));
	m_pITableFrame->SendLookonData(wChairID, SUB_S_RECORD_LIST, &sList, sizeof(sList));
	return true;
}

bool CTableFrameSink::OnUserSitDown(tagUserInfo userInfo, WORD wSeatID)
{
	if (wSeatID < 0 || wSeatID >= SEAT_COUNT)
	{
		return false;
	}

	//�����һ�ε�ռ��
	for (int i = 0; i < SEAT_COUNT;i++)
	{
		if (m_wSitDownInfo[i] == userInfo.wChairID)
		{
			m_wSitDownInfo[i] = INVALID_CHAIR;
		}
	}

	//�����λ�ѱ��������ռλ����������
	if (m_wSitDownInfo[wSeatID] >= 0 && m_wSitDownInfo[wSeatID] != INVALID_CHAIR)
	{
		return false;
	}

	//����������λ��Ч
	m_wSitDownInfo[wSeatID] = userInfo.wChairID;

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItemSend == NULL) continue;
		CMD_S_SeatInfo sInfo;
		memset(&sInfo,INVALID_CHAIR,sizeof(sInfo));

		memcpy(sInfo.tSeatInfo, m_wSitDownInfo, sizeof(sInfo.tSeatInfo));

		//������Ϣ
		m_pITableFrame->SendTableData(i, SUB_S_SEATINFO, &sInfo, sizeof(sInfo));
		m_pITableFrame->SendLookonData(i, SUB_S_SEATINFO, &sInfo, sizeof(sInfo));
	}

	return true;
}

bool CTableFrameSink::OnUserLeaveSeat(WORD wChairID)
{
	for (int i = 0; i < SEAT_COUNT;i++)
	{
		if (m_wSitDownInfo[i] >= 0 && wChairID == m_wSitDownInfo[i])
		{
			m_wSitDownInfo[i] = INVALID_CHAIR;
		}
	}

	for (WORD i = 0; i < GAME_PLAYER; i++)
	{
		IServerUserItem *pIServerUserItemSend = m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItemSend == NULL) continue;
		CMD_S_SeatInfo sInfo;
		memset(&sInfo,INVALID_CHAIR,sizeof(sInfo));

		memcpy(sInfo.tSeatInfo, m_wSitDownInfo, sizeof(sInfo.tSeatInfo));

		//������Ϣ
		m_pITableFrame->SendTableData(i, SUB_S_SEATINFO, &sInfo, sizeof(sInfo));
		m_pITableFrame->SendLookonData(i, SUB_S_SEATINFO, &sInfo, sizeof(sInfo));
	}
	return true;
}

//����ׯ��
bool CTableFrameSink::OnUserApplyBanker(IServerUserItem *pIApplyServerUserItem)
{
	//�Ϸ��ж�
	LONGLONG lUserScore=pIApplyServerUserItem->GetUserScore();
// 	if (lUserScore<m_lApplyBankerCondition)
// 	{
// 		m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("��Ľ�Ҳ���������ׯ�ң�����ʧ�ܣ�"),SMT_CHAT|SMT_EJECT);
// 		return true;
// 	}

	//�����ж�
	WORD wApplyUserChairID=pIApplyServerUserItem->GetChairID();
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];
		if (wChairID==wApplyUserChairID)
		{
			m_pITableFrame->SendGameMessage(pIApplyServerUserItem,TEXT("���Ѿ�������ׯ�ң�����Ҫ�ٴ����룡"),SMT_CHAT|SMT_EJECT);
			return true;
		}
	}

	if (pIApplyServerUserItem->IsAndroidUser()&&(m_ApplyUserArray.GetCount())>m_nRobotListMaxCount)
	{
		return true;
	}
	
	//������Ϣ 
	m_ApplyUserArray.Add(wApplyUserChairID);

	//�������
	CMD_S_ApplyBanker ApplyBanker;
	ZeroMemory(&ApplyBanker,sizeof(ApplyBanker));

	//���ñ���
	ApplyBanker.wApplyUser=wApplyUserChairID;

	//������Ϣ
	m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));

	//�л��ж�
	if (m_pITableFrame->GetGameStatus()==GAME_SCENE_FREE && m_ApplyUserArray.GetCount()==1)
	{
		ChangeBanker(false);
	}

	return true;
}

//ȡ������
bool CTableFrameSink::OnUserCancelBanker(IServerUserItem *pICancelServerUserItem)
{
	//��ǰׯ��
	if (pICancelServerUserItem->GetChairID()==m_wCurrentBanker && m_pITableFrame->GetGameStatus()!=GAME_SCENE_FREE)
	{
		//������Ϣ
		m_pITableFrame->SendGameMessage(pICancelServerUserItem,TEXT("��Ϸ�Ѿ���ʼ��������ȡ����ׯ��"),SMT_CHAT|SMT_EJECT);
		return true;
	}

	//�����ж�
	for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
	{
		//��ȡ���
		WORD wChairID=m_ApplyUserArray[i];
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);

		//��������
		if (pIServerUserItem==NULL) continue;
		if (pIServerUserItem->GetUserID()!=pICancelServerUserItem->GetUserID()) continue;

		//ɾ�����
		m_ApplyUserArray.RemoveAt(i);

		if (m_wCurrentBanker!=wChairID)
		{
			//�������
			CMD_S_CancelBanker CancelBanker;
			ZeroMemory(&CancelBanker,sizeof(CancelBanker));

			//���ñ���
			CancelBanker.wCancelUser = pIServerUserItem->GetChairID();

			//������Ϣ
			m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
			m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
		}
		else if (m_wCurrentBanker==wChairID)
		{
			//�л�ׯ�� 
			m_wCurrentBanker=INVALID_CHAIR;
			ChangeBanker(true);
		}

		return true;
	}

	return true;
}

//����ׯ��
bool CTableFrameSink::ChangeBanker(bool bCancelCurrentBanker)
{
	//�л���ʶ
	bool bChangeBanker=false;

	//��ׯ����
	LONGLONG wBankerTime = m_nBankerTimeLimit;

	//ȡ����ǰ
	if (bCancelCurrentBanker)
	{
		for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
		{
			//��ȡ���
			WORD wChairID=m_ApplyUserArray[i];

			//��������
			if (wChairID!=m_wCurrentBanker) continue;

			//ɾ�����
			m_ApplyUserArray.RemoveAt(i);

			break;
		}

		//����ׯ��
		m_wCurrentBanker=INVALID_CHAIR;

		//�ֻ��ж�
		TakeTurns();

		//���ñ���
		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime=0;
	}
	//��ׯ�ж�
	else if (m_wCurrentBanker!=INVALID_CHAIR)
	{
		//��ȡׯ��
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

		//�����ж�
		if(pIServerUserItem!= NULL)
		{
			LONGLONG lBankerScore=pIServerUserItem->GetUserScore();

			//�����ж�
			if (wBankerTime+m_wAddTime<=m_wBankerTime || lBankerScore<m_lApplyBankerCondition)
			{
				//ׯ�������ж� ͬһ��ׯ�������ֻ�ж�һ��
				if(wBankerTime <= m_wBankerTime && m_bExchangeBanker && lBankerScore >= m_lApplyBankerCondition)
				{
					//��ׯ�������ã���ׯ�������趨�ľ���֮��(m_wBankerTime)��
					//�������ֵ��������������ׯ���б�����������ҽ��ʱ��
					//�����ټ���ׯm_lBankerAdd�֣���ׯ���������á�

					//��ҳ���m_lExtraBankerScore֮��
					//������������ҵĽ��ֵ�������Ľ��ֵ����Ҳ�����ټ�ׯm_lBankerScoreAdd�֡�
					bool bScoreMAX = true;
					m_bExchangeBanker = false;

					for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
					{
						//��ȡ���
						WORD wChairID = m_ApplyUserArray[i];
						IServerUserItem *pIUserItem = m_pITableFrame->GetTableUserItem(wChairID);
						LONGLONG lScore = pIUserItem->GetUserScore();

						if ( wChairID != m_wCurrentBanker && lBankerScore <= lScore )
						{
							bScoreMAX = false;
							break;
						}
					}

					if ( bScoreMAX || (lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l) )
					{
						BYTE bType = 0;
						if ( bScoreMAX && m_nBankerTimeAdd != 0 && m_nBankerTimeAdd != 0 )
						{
							bType = 1;
							m_wAddTime = m_nBankerTimeAdd;
						}
						if ( lBankerScore > m_lExtraBankerScore && m_lExtraBankerScore != 0l && m_nExtraBankerTime != 0 )
						{
							bType += 2;
							if (bType == 3)
							{
								//bType = (m_nExtraBankerTime>m_nBankerTimeAdd?2:1);
								//m_wAddTime = (m_nExtraBankerTime>m_nBankerTimeAdd?m_nExtraBankerTime:m_nBankerTimeAdd);
								m_wAddTime = m_nBankerTimeAdd + m_nExtraBankerTime;
							}
							else
								m_wAddTime = m_nExtraBankerTime;
						}

						//��ʾ��Ϣ
						TCHAR szTipMsg[128] = {};
						if (bType == 1 )
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�����������������ׯ��ң����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_wAddTime);
						else if (bType == 2)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else if (bType == 3)
							_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]�ķ�������[%I64d]�ҳ�������������ң����%d�ζ�����ׯ����!"),pIServerUserItem->GetNickName(),m_lExtraBankerScore,m_wAddTime);
						else
							bType = 0;

						if (bType != 0)
						{
							//������Ϣ
							SendGameMessage(INVALID_CHAIR,szTipMsg);
							return true;
						}
					}
				}

				//�������
				for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
				{
					//��ȡ���
					WORD wChairID=m_ApplyUserArray[i];

					//��������
					if (wChairID!=m_wCurrentBanker) continue;

					//ɾ�����
					m_ApplyUserArray.RemoveAt(i);

					break;
				}

				//����ׯ��
				m_wCurrentBanker=INVALID_CHAIR;

				//�ֻ��ж�
				TakeTurns();

				//��ʾ��Ϣ
				TCHAR szTipMsg[128];
				if (lBankerScore<m_lApplyBankerCondition)
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��������(%I64d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),m_lApplyBankerCondition);
				else
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("[ %s ]��ׯ�����ﵽ(%d)��ǿ�л�ׯ!"),pIServerUserItem->GetNickName(),wBankerTime+m_wAddTime);

				bChangeBanker=true;
				m_bExchangeBanker = true;
				m_wAddTime = 0;

				//������Ϣ
				SendGameMessage(INVALID_CHAIR,szTipMsg);	
			}
		}
		else
		{
			for (WORD i=0; i<m_ApplyUserArray.GetCount(); ++i)
			{
				//��ȡ���
				WORD wChairID=m_ApplyUserArray[i];

				//��������
				if (wChairID!=m_wCurrentBanker) continue;

				//ɾ�����
				m_ApplyUserArray.RemoveAt(i);

				break;
			}
			//����ׯ��
			m_wCurrentBanker=INVALID_CHAIR;
		}

	}
	//ϵͳ��ׯ
	else if (m_wCurrentBanker==INVALID_CHAIR && m_ApplyUserArray.GetCount()!=0)
	{
		//�ֻ��ж�
		TakeTurns();

		bChangeBanker=true;
		m_bExchangeBanker = true;
		m_wAddTime = 0;
	}

	//�л��ж�
	if (bChangeBanker)
	{
		//���ñ���
		m_wBankerTime = 0;
		m_lBankerWinScore=0;

		//������Ϣ
		CMD_S_ChangeBanker ChangeBanker;
		ZeroMemory(&ChangeBanker,sizeof(ChangeBanker));
		ChangeBanker.wBankerUser=m_wCurrentBanker;
		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
			ChangeBanker.lBankerScore=pIServerUserItem->GetUserScore();
		}
		m_pITableFrame->SendTableData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));
		m_pITableFrame->SendLookonData(INVALID_CHAIR,SUB_S_CHANGE_BANKER,&ChangeBanker,sizeof(ChangeBanker));

		if (m_wCurrentBanker!=INVALID_CHAIR)
		{
			//��ȡ����
			m_pGameServiceOption=m_pITableFrame->GetGameServiceOption();
			ASSERT(m_pGameServiceOption!=NULL);

			//��ȡ��Ϣ
			int nMessageCount = 3;
			
			//��ȡ����
			INT nIndex=rand()%nMessageCount;
			TCHAR szMessage1[256],szMessage2[256];
			tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
			ASSERT(pCustomConfig);
			if(0 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem1, sizeof(pCustomConfig->CustomGeneral.szMessageItem1));
			}
			else if(1 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem2, sizeof(pCustomConfig->CustomGeneral.szMessageItem2));
			}
			else if(2 == nIndex)
			{
				CopyMemory(szMessage1, pCustomConfig->CustomGeneral.szMessageItem3, sizeof(pCustomConfig->CustomGeneral.szMessageItem3));
			}

			//��ȡ���
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_wCurrentBanker);

			//������Ϣ
			_sntprintf(szMessage2,CountArray(szMessage2),TEXT("�� %s ����ׯ�ˣ�%s"),pIServerUserItem->GetNickName(), szMessage1);
			SendGameMessage(INVALID_CHAIR,szMessage2);
		}
	}

	return bChangeBanker;
}

//�ֻ��ж�
void CTableFrameSink::TakeTurns()
{
	//��������
	int nInvalidApply = 0;

	for (int i = 0; i < m_ApplyUserArray.GetCount(); i++)
	{
		if (m_pITableFrame->GetGameStatus() == GAME_SCENE_FREE)
		{
			//��ȡ����
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(m_ApplyUserArray[i]);
			if (pIServerUserItem != NULL)
			{
				if (pIServerUserItem->GetUserScore() >= m_lApplyBankerCondition)
				{
					m_wCurrentBanker=m_ApplyUserArray[i];
					break;
				}
				else
				{
					nInvalidApply = i + 1;

					//������Ϣ
					CMD_S_CancelBanker CancelBanker = {};

					//���ñ���
					CancelBanker.wCancelUser=pIServerUserItem->GetChairID();					

					//������Ϣ
					m_pITableFrame->SendTableData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));
					m_pITableFrame->SendLookonData(INVALID_CHAIR, SUB_S_CANCEL_BANKER, &CancelBanker, sizeof(CancelBanker));

					//��ʾ��Ϣ
					TCHAR szTipMsg[128] = {};
					_sntprintf(szTipMsg,CountArray(szTipMsg),TEXT("������Ľ������%I64d��������ׯ����������%I64d��,���޷���ׯ��"),
						pIServerUserItem->GetUserScore(), m_lApplyBankerCondition);
					SendGameMessage(m_ApplyUserArray[i],szTipMsg);
				}
			}
		}
	}

	//ɾ�����
	if (nInvalidApply != 0)
		m_ApplyUserArray.RemoveAt(0, nInvalidApply);
}

//����ׯ��
void CTableFrameSink::SendApplyUser( IServerUserItem *pRcvServerUserItem )
{
	for (INT_PTR nUserIdx=0; nUserIdx<m_ApplyUserArray.GetCount(); ++nUserIdx)
	{
		WORD wChairID=m_ApplyUserArray[nUserIdx];

		//��ȡ���
		IServerUserItem *pServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (!pServerUserItem) continue;

		//ׯ���ж�
		if (pServerUserItem->GetChairID()==m_wCurrentBanker) continue;

		//�������
		CMD_S_ApplyBanker ApplyBanker;
		ApplyBanker.wApplyUser=wChairID;

		//������Ϣ
		m_pITableFrame->SendUserItemData(pRcvServerUserItem, SUB_S_APPLY_BANKER, &ApplyBanker, sizeof(ApplyBanker));
	}
}


//�����ע
LONGLONG CTableFrameSink::GetUserMaxJetton(WORD wChairID, BYTE cbJettonArea)
{
	IServerUserItem *pIMeServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
	if (NULL==pIMeServerUserItem) return 0L;

	int iTimer = 10;
	//����ע��
	LONGLONG lNowJetton = 0;
	ASSERT(AREA_COUNT<=CountArray(m_lUserJettonScore));
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lNowJetton += m_lUserJettonScore[nAreaIndex][wChairID];

	//ׯ�ҽ��
	LONGLONG lBankerScore=2147483647;
	if (m_wCurrentBanker!=INVALID_CHAIR)
		lBankerScore=m_lBankerScore*m_nEndGameMul/100;
	for (int nAreaIndex=1; nAreaIndex<=AREA_COUNT; ++nAreaIndex) lBankerScore-=m_lAllJettonScore[nAreaIndex]*iTimer;

	//������
	LONGLONG lAreaLimitScore = m_lAreaLimitScore - m_lAllJettonScore[cbJettonArea];

	//��������
	LONGLONG lMeMaxScore = min((pIMeServerUserItem->GetUserScore()-lNowJetton*iTimer)/iTimer, m_lUserLimitScore);

//	const tagUserScore *Info = pIMeServerUserItem->GetUserScore();

//	const tagUserInfo *Data = pIMeServerUserItem->GetUserInfo();

	//��������
	lMeMaxScore=min(lMeMaxScore,lAreaLimitScore);

	//ׯ������
	lMeMaxScore=min(lMeMaxScore,lBankerScore/iTimer);

	//��������
	ASSERT(lMeMaxScore >= 0);
	lMeMaxScore = max(lMeMaxScore, 0);

	return (lMeMaxScore);

}
//����÷�
LONGLONG CTableFrameSink::CalculateScore()
{
	//��������
	LONGLONG static wRevenue = m_pGameServiceOption->wRevenueRatio;

	//��ȡ����Ӯ�����򣬴����m_cbWinArea��
	GetWinArea();

	//��Ϸ��¼
	tagServerGameRecord &GameRecord = m_GameRecordArrary[m_nRecordLast];
	GameRecord.cbTotalPoint = m_cbDartPoint[0] + m_cbDartPoint[1];

	//�ƶ��±�
	m_nRecordLast = (m_nRecordLast + 1) % MAX_SCORE_HISTORY;
	if (m_nRecordLast == m_nRecordFirst) m_nRecordFirst = (m_nRecordFirst + 1) % MAX_SCORE_HISTORY;

	//ׯ������
	LONGLONG lBankerWinScore = 0;

	//��ҳɼ�
	ZeroMemory(m_lUserWinScore, sizeof(m_lUserWinScore));
	ZeroMemory(m_lUserReturnScore, sizeof(m_lUserReturnScore));
	ZeroMemory(m_lUserRevenue, sizeof(m_lUserRevenue));
	LONGLONG lUserLostScore[GAME_PLAYER];
	ZeroMemory(lUserLostScore, sizeof(lUserLostScore));

	//�������
	for (WORD wChairID = 0; wChairID < GAME_PLAYER; wChairID++)
	{
		//ׯ���ж�
		//if (m_wCurrentBanker == wChairID) continue;

		//��ȡ�û�
		IServerUserItem * pIServerUserItem = m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem == NULL) continue;

		for (BYTE wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
		{
			//Ӯ����Ӯע��*�������䣬��ֻ��ע��
			if (1 == m_cbWinArea[wAreaIndex])
			{
				int cbMultip = GetMultiple(wAreaIndex);
				m_lUserWinScore[wChairID] += (m_lUserJettonScore[wAreaIndex][wChairID] * cbMultip);
				m_lUserReturnScore[wChairID] += m_lUserJettonScore[wAreaIndex][wChairID];
				//lBankerWinScore -= (m_lUserJettonScore[wAreaIndex][wChairID] * cbMultip);
			}
			else
			{
				lUserLostScore[wChairID] -= m_lUserJettonScore[wAreaIndex][wChairID];
				//lBankerWinScore += m_lUserJettonScore[wAreaIndex][wChairID];
			}
		}

		//�ܵķ���
		m_lUserWinScore[wChairID] += lUserLostScore[wChairID];

		//����˰��
		if (0 < m_lUserWinScore[wChairID])
		{
			double fRevenuePer = double(wRevenue / 1000.0);
			m_lUserRevenue[wChairID] = LONGLONG(m_lUserWinScore[wChairID] * fRevenuePer);
			m_lUserWinScore[wChairID] -= m_lUserRevenue[wChairID];
		}
	}

	/*
	//ׯ�ҳɼ�
	if (m_wCurrentBanker != INVALID_CHAIR)
	{
		m_lUserWinScore[m_wCurrentBanker] = lBankerWinScore;

		//����˰��
		if (0 < m_lUserWinScore[m_wCurrentBanker])
		{
			double fRevenuePer = double(wRevenue / 1000.0);
			m_lUserRevenue[m_wCurrentBanker] = LONGLONG(m_lUserWinScore[m_wCurrentBanker] * fRevenuePer);
			m_lUserWinScore[m_wCurrentBanker] -= m_lUserRevenue[m_wCurrentBanker];
			lBankerWinScore = m_lUserWinScore[m_wCurrentBanker];
		}
	}
	*/

	//�ۼƻ���
	//m_lBankerWinScore += lBankerWinScore;

	//��ǰ����
	//m_lBankerCurGameScore = lBankerWinScore;

	return lBankerWinScore;
}

void CTableFrameSink::ControlWinArea()
{
	if (m_wCheatArea == 255 || m_wCheatArea < 0 || m_wCheatArea > AREA_COUNT) return;
	int bWinTian, bWinDi, bWinXuan, bWinHuan;
	DeduceWinnerEx(bWinTian, bWinDi, bWinXuan, bWinHuan);
	int compareResult[4] = { bWinTian, bWinDi, bWinXuan, bWinHuan };
	int minIndex = 0;
	int minValue = 255;
	for (int i = 1; i < AREA_COUNT+1; i++)
	{
		if (minValue > compareResult[i])
		{
			minValue = compareResult[i];
			minIndex = i;
		}
	}
	if (m_wCheatArea == minIndex) return;
	BYTE tempCard[MAX_CARD] = { 0 };
	CopyMemory(tempCard, m_cbTableCardArray[minIndex], MAX_CARD);
	CopyMemory(m_cbTableCardArray[minIndex], m_cbTableCardArray[m_wCheatArea], MAX_CARD);
	CopyMemory(m_cbTableCardArray[m_wCheatArea], tempCard, MAX_CARD);

}


void CTableFrameSink::DeduceWinner(bool &bWinTian, bool &bWinDi, bool &bWinXuan,bool &bWinHuan,BYTE &TianMultiple,BYTE &diMultiple,BYTE &TianXuanltiple,BYTE &HuangMultiple)
{
	//��С�Ƚ�
	bWinTian=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[SHUN_MEN_INDEX],5,TianMultiple)==1?true:false;
	bWinDi=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DUI_MEN_INDEX],5,diMultiple)==1?true:false;
	bWinXuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[DAO_MEN_INDEX],5,TianXuanltiple)==1?true:false;
	bWinHuan=m_GameLogic.CompareCard(m_cbTableCardArray[BANKER_INDEX],5,m_cbTableCardArray[HUAN_MEN_INDEX],5,HuangMultiple)==1?true:false;
}

int CTableFrameSink::GetMultiple(BYTE bAreaNum)
{
	BYTE cbMultiple = 0;
	switch (bAreaNum)
	{
	case ID_BIG:
		cbMultiple = m_tOdds.iBig;
		break;
	case ID_SMALL:
		cbMultiple = m_tOdds.iSmall;
		break;
	case ID_MID:
		cbMultiple = m_tOdds.iMiddle;
		break;
	case ID_MID16:
	case ID_MID25:
	case ID_MID34:
		cbMultiple = m_tOdds.iMiddle1;
		break;
	case ID_FU:
		cbMultiple = m_tOdds.iFu;
		break;
	case ID_FU5:
	case ID_FU6:
	case ID_FU8:
	case ID_FU9:
		cbMultiple = m_tOdds.iFu1;
		break;
	case ID_LU:
		cbMultiple = m_tOdds.iLu;
		break;
	case ID_LU10:
		cbMultiple = m_tOdds.iLu1;
		break;
	case ID_SHOU:
		cbMultiple = m_tOdds.iShou;
		break;
	case ID_SHOU3:
	case ID_SHOU11:
		cbMultiple = m_tOdds.iShou1;
		break;
	case ID_PAIR:
		cbMultiple = m_tOdds.iPair;
	case ID_PAIR1:
	case ID_PAIR6:
		cbMultiple = m_tOdds.iPair1;
		break;
	case ID_PAIR2:
	case ID_PAIR3:
	case ID_PAIR4:
	case ID_PAIR5:
		cbMultiple = m_tOdds.iPair2;
		break;
		break;
	default:
		break;
	}

	return cbMultiple;
}

void CTableFrameSink::GetWinArea()
{
	memset(m_cbWinArea, 0, sizeof(m_cbWinArea));
	BYTE cbTotal = m_cbDartPoint[0] + m_cbDartPoint[1];
	//��8-12
	if (cbTotal > 7 && cbTotal <= 12)
	{
		m_cbWinArea[ID_BIG] = 1;
	}
	//С��2-6
	else if (cbTotal > 1 && cbTotal < 7)
	{
		m_cbWinArea[ID_SMALL] = 1;
	}

	switch (cbTotal)
	{
	//���ӡ���1����6
	case 2:
		{
			  m_cbWinArea[ID_PAIR] = 1;
			  m_cbWinArea[ID_PAIR1] = 1;
			  break;
		}
	case 12:
		{
			m_cbWinArea[ID_PAIR] = 1;
			m_cbWinArea[ID_PAIR6] = 1;
			break;
		}
	//�١�3��11
	case 3:
		{
			m_cbWinArea[ID_SHOU] = 1;
			m_cbWinArea[ID_SHOU3] = 1;
			break;
		}
	case 11:
		{
			m_cbWinArea[ID_SHOU] = 1;
			m_cbWinArea[ID_SHOU11] = 1;
			break;
		}
	//»��4��10
	case 4:
		{
			m_cbWinArea[ID_LU] = 1;
			m_cbWinArea[ID_LU4] = 1;
			if (m_cbDartPoint[0] == m_cbDartPoint[1])
			{
				m_cbWinArea[ID_PAIR2] = 1;
			}
			break;
		}
	case 10:
		{
			m_cbWinArea[ID_LU] = 1;
			m_cbWinArea[ID_LU10] = 1;
			if (m_cbDartPoint[0] == m_cbDartPoint[1])
			{
				m_cbWinArea[ID_PAIR5] = 1;
			}
			break;
		}
	//����5��6��8��9
	case 5:
		{
			  m_cbWinArea[ID_FU] = 1;
			  m_cbWinArea[ID_FU5] = 1;
			  break;
		}
	case 6:
		{
			  m_cbWinArea[ID_FU] = 1;
			  m_cbWinArea[ID_FU6] = 1;
			  if (m_cbDartPoint[0] == m_cbDartPoint[1])
			  {
				  m_cbWinArea[ID_PAIR3] = 1;
			  }
			  break;
		}
	case 8:
		{
			  m_cbWinArea[ID_FU] = 1;
			  m_cbWinArea[ID_FU8] = 1;
			  if (m_cbDartPoint[0] == m_cbDartPoint[1])
			  {
				  m_cbWinArea[ID_PAIR4] = 1;
			  }
			  break;
		}
	case 9:
		{
			m_cbWinArea[ID_FU] = 1;
			m_cbWinArea[ID_FU9] = 1;
			break;
		}
	default:
		break;
	}
	return;
}

void CTableFrameSink::DeduceWinnerEx(int &nTian,int &nDi, int &nXuan, int &nHuang)
{
	BYTE bMultiple;
	for (WORD wAreaIndex = 1; wAreaIndex <= 4; wAreaIndex++)
	{
		for (WORD wAreaIndexEx = wAreaIndex + 1; wAreaIndexEx <= 4; wAreaIndexEx++)
		{
			if (m_GameLogic.CompareCard(m_cbTableCardArray[wAreaIndex], 5, m_cbTableCardArray[wAreaIndexEx], 5, bMultiple) == -1)
			{
				if (wAreaIndex == ID_TIAN_MEN)
				{
					nTian++;
				}

				if (wAreaIndex == ID_DI_MEN)
				{
					nDi++;
				}

				if (wAreaIndex == ID_XUAN_MEN)
				{
					nXuan++;
				}

				if (wAreaIndex == ID_HUANG_MEN)
				{
					nHuang++;
				}

			}

			if (m_GameLogic.CompareCard(m_cbTableCardArray[wAreaIndex], 5, m_cbTableCardArray[wAreaIndexEx], 5, bMultiple) == 1)

			{
				if (wAreaIndexEx == ID_TIAN_MEN)
				{
					nTian++;
				}

				if (wAreaIndexEx == ID_DI_MEN)
				{
					nDi++;
				}

				if (wAreaIndexEx == ID_XUAN_MEN)
				{
					nXuan++;
				}

				if (wAreaIndexEx == ID_HUANG_MEN)
				{
					nHuang++;
				}

			}

		}

	}
}

//���ͼ�¼
void CTableFrameSink::SendGameRecord(IServerUserItem *pIServerUserItem)
{
	WORD wBufferSize=0;
	BYTE cbBuffer[8192];
	int nIndex = m_nRecordFirst;
	while ( nIndex != m_nRecordLast )
	{
		if ((wBufferSize+sizeof(tagServerGameRecord))>sizeof(cbBuffer))
		{
			m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
			wBufferSize=0;
		}
		CopyMemory(cbBuffer+wBufferSize,&m_GameRecordArrary[nIndex],sizeof(tagServerGameRecord));
		wBufferSize+=sizeof(tagServerGameRecord);

		nIndex = (nIndex+1) % MAX_SCORE_HISTORY;
	}
	if (wBufferSize>0) m_pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_SEND_RECORD,cbBuffer,wBufferSize);
}

//������Ϣ
void CTableFrameSink::SendGameMessage(WORD wChairID, LPCTSTR pszTipMsg)
{
	if (wChairID==INVALID_CHAIR)
	{
		//��Ϸ���
		for (WORD i=0; i<GAME_PLAYER; ++i)
		{
			IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
			if (pIServerUserItem==NULL) continue;
			m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT);
		}

		//�Թ����
		WORD wIndex=0;
		do {
			IServerUserItem *pILookonServerUserItem=m_pITableFrame->EnumLookonUserItem(wIndex++);
			if (pILookonServerUserItem==NULL) break;

			m_pITableFrame->SendGameMessage(pILookonServerUserItem,pszTipMsg,SMT_CHAT);

		}while(true);
	}
	else
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(wChairID);
		if (pIServerUserItem!=NULL) m_pITableFrame->SendGameMessage(pIServerUserItem,pszTipMsg,SMT_CHAT|SMT_EJECT);
	}
}

//��ȡ����
void CTableFrameSink::ReadConfigInformation()
{
	//��ȡ�Զ�������
	tagCustomConfig *pCustomConfig = (tagCustomConfig *)m_pGameServiceOption->cbCustomRule;
	ASSERT(pCustomConfig);

	//��ׯ
	m_lApplyBankerCondition = pCustomConfig->CustomGeneral.lApplyBankerCondition;
	m_nBankerTimeLimit = pCustomConfig->CustomGeneral.lBankerTime;
	m_nBankerTimeAdd = pCustomConfig->CustomGeneral.lBankerTimeAdd;
	m_lExtraBankerScore = pCustomConfig->CustomGeneral.lBankerScoreMAX;
	m_nExtraBankerTime = pCustomConfig->CustomGeneral.lBankerTimeExtra;
	m_bEnableSysBanker = (pCustomConfig->CustomGeneral.nEnableSysBanker == TRUE)?true:false;
	
	//ʱ��
	m_nFreeTime = pCustomConfig->CustomGeneral.lFreeTime;
	m_nPlaceJettonTime = pCustomConfig->CustomGeneral.lBetTime;
	m_nDartTime = pCustomConfig->CustomGeneral.lDartTime;
	m_nGameEndTime = pCustomConfig->CustomGeneral.lEndTime;
	if(m_nFreeTime < TIME_FREE	|| m_nFreeTime > 99) m_nFreeTime = TIME_FREE;
	if(m_nPlaceJettonTime < TIME_PLACE_JETTON || m_nPlaceJettonTime > 99) m_nPlaceJettonTime = TIME_PLACE_JETTON;
	if (m_nDartTime < TIME_DART || m_nDartTime > 99) m_nDartTime = TIME_DART;
	if(m_nGameEndTime < TIME_GAME_END || m_nGameEndTime > 99) m_nGameEndTime = TIME_GAME_END;
	
	//����
	memcpy(&m_tOdds,&(pCustomConfig->CustomGeneral.tOdds),sizeof(m_tOdds));

	//��ע
	m_lAreaLimitScore = pCustomConfig->CustomGeneral.lAreaLimitScore;
	m_lUserLimitScore = pCustomConfig->CustomGeneral.lUserLimitScore;
	m_nEndGameMul = pCustomConfig->CustomGeneral.lEndGameMul;
	if(m_nEndGameMul < 1 || m_nEndGameMul > 100 ) m_nEndGameMul = 80;

	//���
	m_lStorageStart = pCustomConfig->CustomGeneral.StorageStart;
	m_lStorageCurrent = m_lStorageStart;
	m_nStorageDeduct = pCustomConfig->CustomGeneral.StorageDeduct;
	m_lStorageMax1 = pCustomConfig->CustomGeneral.StorageMax1;
	m_lStorageMul1 = pCustomConfig->CustomGeneral.StorageMul1;
	m_lStorageMax2 = pCustomConfig->CustomGeneral.StorageMax2;
	m_lStorageMul2 = pCustomConfig->CustomGeneral.StorageMul2;
	if(m_lStorageMul1 < 0 || m_lStorageMul1 > 100 ) m_lStorageMul1 = 50;
	if(m_lStorageMul2 < 0 || m_lStorageMul2 > 100 ) m_lStorageMul2 = 80;
	
	//������
	m_nRobotListMaxCount = pCustomConfig->CustomAndroid.lRobotListMaxCount;

	LONGLONG lRobotBetMinCount = pCustomConfig->CustomAndroid.lRobotBetMinCount;
	LONGLONG lRobotBetMaxCount = pCustomConfig->CustomAndroid.lRobotBetMaxCount;
	m_nMaxChipRobot = rand()%(lRobotBetMaxCount-lRobotBetMinCount+1) + lRobotBetMinCount;
	if (m_nMaxChipRobot < 0)	m_nMaxChipRobot = 8;
	m_lRobotAreaLimit = pCustomConfig->CustomAndroid.lRobotAreaLimit;
	
	return;
}

// ����Ż�
VOID CTableFrameSink::StorageOptimize()
{
	// ��ȡϵͳ��Ӯ
	LONGLONG lSystemScore = 0;
	bool bAllEat = false;
	JudgeSystemScore(m_cbTableCardArray, lSystemScore, bAllEat);

	// ϵͳ��Ǯ
	if( lSystemScore < 0 )
	{
		// ��Ǯ
		if( m_lStorageCurrent > -lSystemScore )
		{
			return;
		}

		// ����Ǯ, ��ϵͳӮǮ

		// ������ϵͳ��Ӯ
		LONGLONG lExchangeMoney[4] = { 0, 0, 0, 0 };
		bool bAllEat[4] = { true, true, true, true };

		// ׯ����
		BYTE bBankerCardData[5] = {0};
		CopyMemory(bBankerCardData, m_cbTableCardArray[0], sizeof(bBankerCardData));

		// ������
		for( int nExchange = 0; nExchange < 4; ++nExchange )
		{
			BYTE bExchangeData[5][5] = {0};
			CopyMemory(bExchangeData, m_cbTableCardArray, sizeof(bExchangeData));
			CopyMemory(bExchangeData[0], m_cbTableCardArray[nExchange + 1], sizeof(BYTE) * 5);
			CopyMemory(bExchangeData[nExchange + 1], bBankerCardData, sizeof(BYTE) * 5);

			JudgeSystemScore(bExchangeData, lExchangeMoney[nExchange], bAllEat[nExchange]);
		}

		// ��ȡϵͳ��Ӯ
		int nOkInedex = -1;
		for( int nOk = 0; nOk < 4; ++nOk )
		{
			if( lExchangeMoney[nOk] > 0 )
			{
				nOkInedex = nOk;

				// ����������ͨ�ԣ� ֱ�ӷ���
				if( !bAllEat[nOk] )
					break;
			}
		}

		// ���ҵ�����
		if( nOkInedex != -1 )
		{
			//��������
			BYTE bTempCardData[5] = {0};
			CopyMemory(bTempCardData,					m_cbTableCardArray[0],				sizeof(bTempCardData));
			CopyMemory(m_cbTableCardArray[0],			m_cbTableCardArray[nOkInedex + 1],		sizeof(bTempCardData));
			CopyMemory(m_cbTableCardArray[nOkInedex + 1],	bTempCardData,						sizeof(bTempCardData));
		}

		return;
	}
	// ϵͳӮǮ
	else
	{
		// �������ϵͳ���ڿ�����ֵ�� �Ǿ������ӮǮ
		if( (m_lStorageCurrent > m_lStorageMax1 && m_lStorageCurrent <= m_lStorageMax2 && rand()%100 <= m_lStorageMul1) ||
			(m_lStorageCurrent > m_lStorageMax2 && rand()%100 <= m_lStorageMul2))
		{
			// ������ϵͳ��Ӯ
			LONGLONG lExchangeMoney[4] = { 0, 0, 0, 0 };
			bool bAllEat[4] = { true, true, true, true };

			// ׯ����
			BYTE bBankerCardData[5] = {0};
			CopyMemory(bBankerCardData, m_cbTableCardArray[0], sizeof(bBankerCardData));

			// ������
			for( int nExchange = 0; nExchange < 4; ++nExchange )
			{
				BYTE bExchangeData[5][5] = {0};
				CopyMemory(bExchangeData, m_cbTableCardArray, sizeof(bExchangeData));
				CopyMemory(bExchangeData[0], m_cbTableCardArray[nExchange + 1], sizeof(BYTE) * 5);
				CopyMemory(bExchangeData[nExchange + 1], bBankerCardData, sizeof(BYTE) * 5);

				JudgeSystemScore(bExchangeData, lExchangeMoney[nExchange], bAllEat[nExchange]);
			}

			// ��ȡϵͳ���䣬����������ٵ���
			int nOkInedex = -1;
			LONGLONG nOkMonye = LLONG_MIN;
			for( int nOk = 0; nOk < 4; ++nOk )
			{
				if( lExchangeMoney[nOk] < 0 && nOkMonye < lExchangeMoney[nOk] && (-lExchangeMoney[nOk]) < m_lStorageCurrent)
				{
					nOkMonye = lExchangeMoney[nOk];
					nOkInedex = nOk;

					// ����������ͨ�ԣ� ֱ�ӷ���
					if( !bAllEat[nOk] )
						break;
				}
			}

			// ���ҵ�����
			if( nOkInedex != -1 )
			{
				//��������
				BYTE bTempCardData[5] = {0};
				CopyMemory(bTempCardData,					m_cbTableCardArray[0],				sizeof(bTempCardData));
				CopyMemory(m_cbTableCardArray[0],			m_cbTableCardArray[nOkInedex + 1],		sizeof(bTempCardData));
				CopyMemory(m_cbTableCardArray[nOkInedex + 1],	bTempCardData,						sizeof(bTempCardData));
			}
		}
		return;
	}
}

// �ж�ϵͳ�÷�
void CTableFrameSink::JudgeSystemScore(BYTE bCardData[5][5], LONGLONG& lSystemScore, bool& bAllEat )
{
	// ϵͳ��Ӯ
	bAllEat = true;
	lSystemScore = 0l;

	//ϵͳ��ׯ
	bool bSystemBanker = false;
	if ( m_wCurrentBanker == INVALID_CHAIR )
	{
		bSystemBanker = true;
	}
	else
	{
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(m_wCurrentBanker);
		if ( pServerUserItem != NULL )	
			bSystemBanker = pServerUserItem->IsAndroidUser();
	}

	// �Ƚϱ���
	bool bXianWin[4] = { false, false, false, false };							//�Ƚ���Ӯ
	BYTE bMultiple[4] = { 1, 1, 1, 1 };											//�Ƚϱ���
	for (int i = 0; i < 4; i++)
	{
		bXianWin[i] = (m_GameLogic.CompareCard(bCardData[0], 5,bCardData[i+1], 5, bMultiple[i]) == 1);
	}
	bAllEat = (bXianWin[0] == bXianWin[1]) && (bXianWin[1] == bXianWin[2]) && (bXianWin[2] == bXianWin[3]);

	// ����ϵͳ��Ӯ
	for (int nSiet = 0; nSiet < GAME_PLAYER; nSiet++)
	{
		// ��ȡ���
		IServerUserItem *pServerUserItem = m_pITableFrame->GetTableUserItem(nSiet);

		// ����ׯ��
		if ( nSiet == m_wCurrentBanker || pServerUserItem == NULL )	
			continue;

		// ���������ע
		for (int nAarea = 0; nAarea < 4; nAarea++)
		{
			if ( m_lUserJettonScore[nAarea+1][nSiet] != 0 )
			{
				if( bXianWin[nAarea] )
				{
					if( pServerUserItem->IsAndroidUser() && !bSystemBanker )
					{ 
						lSystemScore += m_lUserJettonScore[nAarea+1][nSiet] * bMultiple[nAarea];
					}
					if( !pServerUserItem->IsAndroidUser() && bSystemBanker )
					{
						lSystemScore -= m_lUserJettonScore[nAarea+1][nSiet] * bMultiple[nAarea];
					}
				}
				else
				{
					if( pServerUserItem->IsAndroidUser() && !bSystemBanker )
					{ 
						lSystemScore -= m_lUserJettonScore[nAarea+1][nSiet] * bMultiple[nAarea];
					}
					if( !pServerUserItem->IsAndroidUser() && bSystemBanker )
					{
						lSystemScore += m_lUserJettonScore[nAarea+1][nSiet] * bMultiple[nAarea];
					}
				}
			}
		}
	}

	return ;
}


//��ѯ�Ƿ�۷����
bool CTableFrameSink::QueryBuckleServiceCharge(WORD wChairID)
{
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		IServerUserItem *pUserItem=m_pITableFrame->GetTableUserItem(i);
		if(pUserItem==NULL) continue;
		if(wChairID==i)
		{
			//������ע
			for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
			{

				if (m_lUserJettonScore[nAreaIndex][wChairID] != 0)
				{
					return true;
				}
			}
			break;
		}
	}
	if (wChairID==m_wCurrentBanker)
	{
		return true;
	}
	return false;
}

//�Ƿ�˥��
bool CTableFrameSink::NeedDeductStorage()
{

	for ( int i = 0; i < GAME_PLAYER; ++i )
	{
		IServerUserItem *pIServerUserItem=m_pITableFrame->GetTableUserItem(i);
		if (pIServerUserItem == NULL ) continue; 

		if(!pIServerUserItem->IsAndroidUser())
		{
			for (int nAreaIndex=0; nAreaIndex<=AREA_COUNT; ++nAreaIndex) 
			{
				if (m_lUserJettonScore[nAreaIndex][i]!=0)
				{
					return true;
				}				
			}			
		}
	}

	return false;

}

//������ע��Ϣ
void CTableFrameSink::SendUserBetInfo( IServerUserItem *pIServerUserItem )
{
	if(NULL == pIServerUserItem) return;

	//Ȩ���ж�
	if(!CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight())) return;

	//��������
	CMD_S_SendUserBetInfo SendUserBetInfo;
	ZeroMemory(&SendUserBetInfo, sizeof(SendUserBetInfo));
	
	CopyMemory(&SendUserBetInfo.lUserStartScore, m_lUserStartScore, sizeof(m_lUserStartScore));
	CopyMemory(&SendUserBetInfo.lUserJettonScore, m_lUserJettonScore, sizeof(m_lUserJettonScore));

	//������Ϣ	
	m_pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_SEND_USER_BET_INFO, &SendUserBetInfo, sizeof(SendUserBetInfo));
	
	return;
}

//����д��Ϣ
void CTableFrameSink::WriteInfo( LPCTSTR pszFileName, LPCTSTR pszString )
{
	//������������
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CStdioFile myFile;
	CString strFileName;
	strFileName.Format(TEXT("%s"), pszFileName);
	BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		myFile.SeekToEnd();
		myFile.WriteString( pszString );
		myFile.Flush();
		myFile.Close();
	}

	//��ԭ�����趨
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}
//////////////////////////////////////////////////////////////////////////////////

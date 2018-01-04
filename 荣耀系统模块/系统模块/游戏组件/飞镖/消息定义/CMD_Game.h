#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

#define UR_GAME_CONTROL					0x20000000L				//��Ϸ�������
//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						55									//��Ϸ I D
#define GAME_NAME					TEXT("������Ϸ")					//��Ϸ����

//�������
#define GAME_PLAYER					MAX_CHAIR							//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾
#define SEAT_COUNT					7									//��λ��

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLACE_JETTON		GAME_STATUS_PLAY					//��ע״̬
#define GAME_SCENE_DART				GAME_STATUS_PLAY+1					//�����״̬
#define GAME_SCENE_GAME_END			GAME_STATUS_PLAY+2					//����״̬


//��������

#define ID_TIAN_MEN					1									//��
#define ID_DI_MEN					2									//��
#define ID_XUAN_MEN					3									//��
#define ID_HUANG_MEN				4									//��

#define ID_BIG					1									//��
#define ID_SMALL				2									//С
#define ID_MID					3									//��
#define ID_MID16				4									//1+6��2+5��3+4
#define ID_MID25				5									//1+6��2+5��3+4
#define ID_MID34				6									//1+6��2+5��3+4
#define ID_FU					7									//��
#define ID_FU5					8									//5��6��8��9
#define ID_FU6					9									//5��6��8��9
#define ID_FU8					10									//5��6��8��9
#define ID_FU9					11									//5��6��8��9
#define ID_LU					12									//»
#define ID_LU4					13									//4
#define ID_LU10					14									//10
#define ID_SHOU					15									//��
#define ID_SHOU3				16									//3
#define ID_SHOU11				17									//11
#define ID_PAIR					18									//����
#define ID_PAIR1				19									//��1
#define ID_PAIR6				20									//��6
#define ID_PAIR2				21									//��2
#define ID_PAIR3				22									//��3
#define ID_PAIR4				23									//��4
#define ID_PAIR5				24									//��5


//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define HUAN_MEN_INDEX				4									//��������
#define MAX_INDEX					3									//�������

#define AREA_COUNT					24									//������Ŀ
#define CONTROL_AREA				4

//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//����
struct tagOdds
{
	//����
	int								iBig;							//��
	int								iSmall;							//С		
	int								iMiddle;						//��
	int								iMiddle1;						//1+6��2+5��3+4		
	int								iFu;							//��
	int								iFu1;							//5��6��8��9
	int								iLu;							//»
	int								iLu1;							//4��10
	int								iShou;							//��
	int								iShou1;							//3��11
	int								iPair;							//���ӣ���1����6��
	int								iPair1;							//��1����6
	int								iPair2;							//��2��3��4��5
	tagOdds()
	{
		iBig = 2;
		iSmall = 2;
		iMiddle = 5;
		iMiddle1 = 12;
		iFu = 6;
		iFu1 = 6;
		iLu = 9;
		iLu1 = 9;
		iShou = 12;
		iShou1 = 12;
		iPair = 22;
		iPair1 = 22;
		iPair2 = 18;
	}
};

struct tagCustomAndroid
{
	//��ׯ
	BOOL							nEnableRobotBanker;				//�Ƿ���ׯ
	LONGLONG						lRobotBankerCountMin;			//��ׯ����
	LONGLONG						lRobotBankerCountMax;			//��ׯ����
	LONGLONG						lRobotListMinCount;				//�б�����
	LONGLONG						lRobotListMaxCount;				//�б�����
	LONGLONG						lRobotApplyBanker;				//����������
	LONGLONG						lRobotWaitBanker;				//��������

	//��ע
	LONGLONG						lRobotMinBetTime;				//��ע�������
	LONGLONG						lRobotMaxBetTime;				//��ע�������
	LONGLONG						lRobotMinJetton;				//��ע������
	LONGLONG						lRobotMaxJetton;				//��ע������
	LONGLONG						lRobotBetMinCount;				//��ע��������
	LONGLONG						lRobotBetMaxCount;				//��ע��������
	LONGLONG						lRobotAreaLimit;				//��������

	//��ȡ��
	LONGLONG						lRobotScoreMin;					//�������
	LONGLONG						lRobotScoreMax;					//�������
	LONGLONG						lRobotBankGetMin;				//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetMax;				//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMin;			//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMax;			//ȡ�����ֵ(��ׯ)
	int								lRobotBankStoMul;				//���ٷֱ�

	//���캯��
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = TRUE;
		lRobotBankerCountMin = 5;
		lRobotBankerCountMax = 10;
		lRobotListMinCount = 2;
		lRobotListMaxCount = 5;
		lRobotApplyBanker = 5;
		lRobotWaitBanker = 3;

		lRobotMinBetTime = 6;
		lRobotMaxBetTime = 8;
		lRobotMinJetton = 100;
		lRobotMaxJetton = 5000000;
		lRobotBetMinCount = 4;
		lRobotBetMaxCount = 8;
		lRobotAreaLimit = 10000000;
		
		lRobotScoreMin = 1000000;
		lRobotScoreMax = 100000000;
		lRobotBankGetMin = 100;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//��������Ϣ
struct tagRobotInfo
{
	int nChip[8];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[8] = {100, 1000, 10000, 50000, 100000, 500000, 1000000, 5000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 1, 1, 1};

		nMaxTime = 10;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE	cbTotalPoint;//�����ܵ���
};

//��ע��Ϣ
struct tagUserBet
{
	TCHAR							szNickName[32];						//�û��ǳ�
	DWORD							dwUserGameID;						//�û�ID
	LONGLONG						lUserStartScore;					//�û����
	LONGLONG						lUserWinLost;						//�û����
	LONGLONG						lUserBet[AREA_COUNT+1];				//�û���ע
};

//��ע��Ϣ����
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

//������
#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2
//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_DART					102									//�������
#define SUB_S_GAME_END				103									//��Ϸ����


#define SUB_S_APPLY_BANKER			113									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������	

#define SUB_S_AMDIN_COMMAND			110									//����Ա����
#define SUB_S_UPDATE_STORAGE        111									//���¿��
#define SUB_S_SEND_USER_BET_INFO    112									//������ע

#define SUB_S_PLAYER_LIST			114									//����б�
#define SUB_S_BANKER_LIST			115									//��̨�б�
#define SUB_S_RECORD_LIST			116									//k������

#define SUB_S_SEATINFO				117									//��λ��Ϣ
#define SUB_S_StartDart				118									//��ʼ�������

//���¿��
struct CMD_S_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageStart;					//��ʼ���
	LONGLONG						lStorageDeduct;					//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

//������ע
struct CMD_S_SendUserBetInfo
{
	LONGLONG						lUserStartScore[GAME_PLAYER];				//��ʼ����
	LONGLONG						lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];//������ע
};

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//����
#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
};
//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;						//ȡ�����
};

//����б�
struct CMD_S_PlayerList
{
	WORD					wPlayerList[GAME_PLAYER];
};

//��̨�б�
struct CMD_S_BankerList
{
	WORD						wBankList[GAME_PLAYER];
};

//k������
struct CMD_S_RecordList
{
	tagServerGameRecord				tGameRecordArrary[16];
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;						//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	INT								nEndGameMul;						//��ǰ���ưٷֱ�
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
	bool							bGenreEducate;						//��ϰģʽ

	tagCustomAndroid				CustomAndroid;						//����������

	tagOdds							tOdds;								//����
	WORD							tSeatInfo[SEAT_COUNT];				//���ӺŶ�Ӧ�������Ϣ
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[5][5];				//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	INT								nEndGameMul;						//��ǰ���ưٷֱ�
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	LONGLONG						cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������
	bool							bGenreEducate;						//��ϰģʽ

	tagCustomAndroid				CustomAndroid;						//����������

	tagOdds							tOdds;								//����
	WORD							tSeatInfo[SEAT_COUNT];				//��λ�Ŷ�Ӧ�����Ӻ�
	BYTE							cbPoint[2];							//���ڵ���
	bool							bIsBanker;							//�Ƿ�����̨
};

//��Ϸ����
struct CMD_S_GameFree
{
	LONGLONG						cbTimeLeave;						//ʣ��ʱ��
	INT64                           nListUserCount;						//�б�����
	LONGLONG						lStorageStart;						//
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	LONGLONG						cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������
	LONGLONG						nChipRobotCount;					//�������� (��ע������)
	LONGLONG						nAndriodApplyCount;					//�������б�����
	tagOdds							tOdds;								//����
};

//��ʼ
struct CMD_S_StartDart
{
	LONGLONG						cbTimeLeave;						//ʣ��ʱ��
	WORD							wFirstDartUser;						//��һ����������
	WORD							wSecDartUser;						//�ڶ�����������	
	BYTE							cbPoint[2];
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	bool							bIsAndroid;							//�Ƿ������
	bool							bAndroid;							//������ʶ
};

struct CMD_S_DartPoint
{
	WORD							wChairID;							//������û�
	BYTE							cbDartPoint[2];						//Ͷ�����ڵ���
};

struct CMD_S_SeatInfo
{
	WORD tSeatInfo[SEAT_COUNT];
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	LONGLONG						cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbDartPoint[2];						//Ͷ�����ڵ���
	BYTE							cbTotalPoint;						//�ܵ���

	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_DART					2									//�������
#define SUB_C_AMDIN_COMMAND			3									//����Ա����
#define SUB_C_UPDATE_STORAGE        4									//���¿��

#define SUB_C_APPLY_BANKER			5									//������̨
#define SUB_C_CANCEL_BANKER			6									//ȡ����̨
#define SUB_C_ALLPLAYER_LIST		7									//����б�
#define	SUB_C_ALLBANKER_LIST		8									//��̨�б�
#define SUB_C_ALLRECORD_LIST		9									//��Ϸ��¼�б�

#define SUB_C_SITDOWN				10									//������Ϣ
#define SUB_C_LEAVESEAT				11									//�뿪��λ

//�ͻ�����Ϣ
#define IDM_ADMIN_COMMDN			WM_USER+1000
#define IDM_UPDATE_STORAGE			WM_USER+1001

//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea[MAX_INDEX];			//��������
};

struct tagAdminReq
{
	BYTE							m_cbExcuteTimes;					//ִ�д���
	BYTE							m_cbControlStyle;					//���Ʒ�ʽ
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3
	bool							m_bWinArea[3];						//Ӯ������
};


struct CMD_C_AdminReq
{
	BYTE cbReqType;
#define RQ_SET_WIN_AREA		1
#define RQ_RESET_CONTROL	2
#define RQ_PRINT_SYN		3
	BYTE cbExtendData[20];			//��������
};
//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

//�û�����
struct CMD_C_SitDownOrLeave
{
	WORD							wSeatID;							//����ID
};

//���¿��
struct CMD_C_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageDeduct;					//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
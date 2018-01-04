#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							88									//��Ϸ  ID
#define GAME_PLAYER						5									//��Ϸ����
#define GAME_NAME						TEXT("�ȼ�")					    //��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define HAND_CARD_COUNT					9									//�˿���Ŀ

#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾
//��Ϸ״̬
#define GS_WK_FREE				    	GAME_STATUS_FREE						//�ȴ���ʼ
#define GS_WK_PLAYING			    	GAME_STATUS_PLAY+3						//��Ϸ����

//�ֶ�����
enum enSegmentType
{
	enFront ,																//ǰ������
	enMid ,																	//�ж�����
	enBack,																	//�������
	enAllSeg,																//�����˿�
	enErr,																	//��������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_GAME_START				206									//��Ϸ��ʼ
#define SUB_S_SEND_CARD					215									//��������
#define SUB_S_SETSEGMENT				102									//���÷ֶ�
#define SUB_S_SHOW_CARD					203									//���̯��
#define SUB_S_GAME_END					204									//��Ϸ����
#define SUB_S_GAME_COMPARE              205                                 //��Ϸ����
#define SUB_S_PLAYER_LEFT               210                                 //���ǿ��
#define SUB_S_SCORE_RESULT              107                                 //����
#define SUB_S_USEREXIT                  108                                 //����˳�
#define SUB_S_AMDIN_COMMAND				109									//����Ա����
#define SUB_S_MOBILE_PUTCARD			219									//�ֻ��й���


//��Ϸ��ʼ
struct CMD_S_GameStart 
{
	LONG								wUserToltalChip;					//�ܹ����
	WORD								wCurrentUser;						//��ǰ���
};

struct GameRule
{
	SCORE sCellScore;
};

//���ǿ��
struct CMD_S_PlayerLeft
{
	int                                 nPlayerLetfChairID;                 //ǿ����ҵ�����ID
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	INT									lBaseScore;
	BYTE								bGameStatus[GAME_PLAYER];
	BYTE								bHaveBanker;

	INT									nAllWinTimes;
	SCORE								lTurnScore[GAME_PLAYER];
	SCORE								lCollectScore[GAME_PLAYER];

	BYTE								cbTimeStartGame;
	BYTE								cbTimeCallBanker;
	BYTE								cbTimeSetChip;
	BYTE								cbTimeRangeCard;
	BYTE								cbTimeShowCard;
	WORD								wServerID;

	LONG								wUserToltalChip;					//�ܹ����
	BYTE                                btRobotCount;                       //����������
};



//�����˿�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //��ǰ���
	WORD								wBanker;							 //ׯ��
	BYTE								bCardData[HAND_CARD_COUNT];			 //�����˿�
	BYTE								cbPlayCount;
	bool								bGameStatus[GAME_PLAYER];
	SCORE								lChipArray[GAME_PLAYER];
// 	BYTE                                bAllHandCardData[GAME_PLAYER][HAND_CARD_COUNT];//������ҵ��˿�����
// 	LONG								lCellScore;							 //��Ϸ�׷�
};

struct CMD_S_ChangeCard
{
	BYTE bCard[9];		// ���Ƶ�����
};

//�ֻ��й���
struct CMD_S_MobilePutCard
{
	//BYTE cbMobilePutCard[13];
	BYTE cbMobilePutCount;
	BYTE cbMobilePutCard[GAME_PLAYER][HAND_CARD_COUNT];
};

//����˳�
struct CMD_S_UserExit
{
	int                                 nLeftChairID;                       //�뿪��ҵ�����ID        
};

struct SPECIALRESULT
{
	int									nSpecialScore;							// ϲ����
	int                                 nSpecialType;							//�Ƿ�ͨ��
	int                                 nSpecialCount;							// ϲ����
};
//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE								bEndMode;							//������ʽ
	int                                 cbCompareResult[GAME_PLAYER][3];    //ÿһ���ȽϽ��
	int                                 nIndexResult[GAME_PLAYER][3];		//û���ƵĴ�С���
	SPECIALRESULT						nSpecialResult[GAME_PLAYER];		// ÿһ����ҵ���������
	int									nEveryType[GAME_PLAYER][3];			// ÿ������
	int								    nAllResult[GAME_PLAYER];			//���ս��
};

//���̯��
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//��ǰ���
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[3];						//�ж��˿�
	BYTE								bBackCard[3];						//����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG								lCellScore;							//�׷�
	BYTE								bHandCardData[HAND_CARD_COUNT];		//�˿�����
	BYTE								bSegmentCard[GAME_PLAYER][3][3];	//�ֶ��˿�
	bool								bFinishSegment[GAME_PLAYER];		//��ɷֶ�
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bCbTimeStartGame;					//��ʼʱ��
	BYTE								bCbTimeCallBanker;					//��ׯʱ��
	BYTE								bCbTimeSetChip;						//��עʱ��
	BYTE								bCbTimeRangeCard;					//����ʱ��
	BYTE								bCbTimeShowCard;					//����ʱ��

};
//����
struct CMD_S_ScoreResult
{
	bool                                bScoreResult;                       //�Ƿ���ʾ����
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_SEGCARD					301									//�ֶ�����
#define SUB_C_SHOWCARD					304									//���̯��
#define SUB_C_OVERTIME					303									//̯�Ƴ�ʱ
#define SUB_C_USEREXIT                  302                                 //�û��뿪
#define SUB_C_AMDIN_COMMAND				305									//����Ա����

//�û��뿪
struct CMD_C_UserExit
{
	WORD                               wChairID;							//�뿪����ID
	WORD                               wTableID;							//�뿪����ID
	WORD                               wUserID;								//�û�ID
	bool                               bEnterExit;							//�Ƿ�һ������뿪
};

//�ֶ���Ϣ
struct CMD_C_ShowCard
{
	//WORD								wCurrentUser;						//��ǰ���
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[3];						//�ж��˿�
	BYTE								bBackCard[3];						//����˿�
	//bool								bCanSeeShowCard;					//�ܷ���
};

struct CMD_C_ControlData
{
	bool bWinloss;					//������Ӯ��1Ӯ��0�䣩
	TCHAR chUserName[MAX_PATH];		//�����������
	DWORD dwChairID;				//����λ��
	BYTE cbCount;					//���ƴ���
};

struct CMD_C_ChangeCard
{
	 BYTE bCard[9];		// ���Ƶ�����
};
//////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif
#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CDialog
{
	//��������
protected:
	LONG							m_lGameTax[GAME_PLAYER];		    //��Ϸ˰��
	LONG							m_lGameScore[GAME_PLAYER];			//��Ϸ�÷�
	TCHAR							m_szUserName[GAME_PLAYER][MAX_PATH];//�û�����
	int                             m_nTotalDaoShu[GAME_PLAYER];        //�ܹ�����
	bool                            m_bEnd;                             //
	int                             m_nChairWin;                        //
	int                             m_nChairLoss;                       //

	//��Դ����
protected:
	CBitImage											m_ImageBack;						//����ͼ��
	CPngImage                                           m_ImageStart;                       //��ʼ
	CPngImage											m_ImageEnd;                         //����

public:
   CSkinButton                      m_pClose;                             
   CSkinButton                      m_btStartAgain;                     //���¿�ʼ
   CSkinButton                      m_btEndAgain;                       //����
   CSkinResourceManager *           m_pSkinResourceManager;             //��Դ������

   int								m_nCompare[GAME_PLAYER][3];         //ÿһ�ص��ܵ���
   int								m_nCompareDouble[GAME_PLAYER];      //�����ĵ���
   int								m_nCompareSpecial[GAME_PLAYER];     //�������͵ĵ���
   int								m_cCompareKillThree[GAME_PLAYER];   //<ȫ�ݴ�

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	

	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetGameTax(LONG lGameEveryTax[]);
	//���û���
	void SetGameScore(WORD wChairID,LPCTSTR pszUserName, LONG lScore,int nTotalDaoShu);
	void SetEnd(bool bEnd);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//�滭����
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�ر���Ϣ
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//////////////////////////////////////////////////////////////////////////

#endif
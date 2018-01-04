#pragma once
#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
#define		IDM_MYSTERY		(WM_USER+1000)		//������Ϣ

//////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagMysteryInfo
{
	bool bHaveUser[GAME_PLAYER];				//�û�״̬
	WORD wBankerUser;							//ׯ��λ��
	WORD wMyChairID;							//�Լ�λ��
	TCHAR szAccounts[GAME_PLAYER][LEN_ACCOUNTS];	//�û�����
};

//////////////////////////////////////////////////////////////////////////

//��Ӯ����
class CMysteryDlg : public CDialog
{
	DECLARE_DYNAMIC(CMysteryDlg)

	//��������
protected:
	bool	m_bHaveUser[GAME_PLAYER];			//�û�״̬
	WORD	m_wBankerUser;						//ׯ���û�
	WORD	m_wMyChairID;						//�Լ�λ��
	CString m_strUserAccount[GAME_PLAYER];		//�û�����
	CWnd	*m_pSinkWindow;						//ͨ�Ŵ���

	//�ؼ�����
protected:
	CButton	m_CheckBox[4];						//��ѡ��ť
	
public:
	//���캯��
	CMysteryDlg(CWnd* pParent = NULL);   
	//��������
	virtual ~CMysteryDlg();

	//�Ի�������
	enum { IDD = IDD_MYSTERY };

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange* pDX);  
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();

	//���ܺ���
public:
	//���ô���
	void SetSinkWindow(CWnd * pSinkWindow);
	//���ô���
	void SetMysteryInfo(tagMysteryInfo * pTagMysteryInfo);
	//���´���
	void UpdateDlg();
	//��ȡ��Ϣ
	bool GetControlInfo(bool bControl[]);

	//��Ϣӳ��
protected:
	//ѡ��ť
	afx_msg void OnBnClickedCheck1();
	//ѡ��ť
	afx_msg void OnBnClickedCheck2();
	//ѡ��ť
	afx_msg void OnBnClickedCheck3();
	//ѡ��ť
	afx_msg void OnBnClickedCheck4();

	DECLARE_MESSAGE_MAP()
};

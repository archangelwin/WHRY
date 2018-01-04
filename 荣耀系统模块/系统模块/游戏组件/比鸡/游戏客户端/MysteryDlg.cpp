// MysteryDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MysteryDlg.h"

// CMysteryDlg �Ի���

IMPLEMENT_DYNAMIC(CMysteryDlg, CDialog)

//���캯��
CMysteryDlg::CMysteryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMysteryDlg::IDD, pParent)
{
	m_wBankerUser = INVALID_CHAIR;
	m_wMyChairID = INVALID_CHAIR;
	m_pSinkWindow = NULL;
	ZeroMemory(m_bHaveUser, sizeof(m_bHaveUser));
}

//��������
CMysteryDlg::~CMysteryDlg()
{
}

//�ؼ���
void CMysteryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, m_CheckBox[0]);
	DDX_Control(pDX, IDC_CHECK2, m_CheckBox[1]);
	DDX_Control(pDX, IDC_CHECK3, m_CheckBox[2]);
	DDX_Control(pDX, IDC_CHECK4, m_CheckBox[3]);
}

BEGIN_MESSAGE_MAP(CMysteryDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, OnBnClickedCheck4)
END_MESSAGE_MAP()

//��ʼ������
BOOL CMysteryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(_T("��ѡ��Ҫ����Ϊʤ������ҡ�"));

	return TRUE;
}

//ȷ����Ϣ
void CMysteryDlg::OnOK()
{
	//������Ϣ
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	 if (pGameFrameEngine!=NULL) pGameFrameEngine->PostMessage(IDM_MYSTERY);

	CDialog::OnOK();
}

//���ô���
void CMysteryDlg::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow = pSinkWindow;

	return;
}

//���ô���
void CMysteryDlg::SetMysteryInfo(tagMysteryInfo * pTagMysteryInfo)
{
	//��մ���
	m_wBankerUser = INVALID_CHAIR;
	ZeroMemory(m_bHaveUser, sizeof(m_bHaveUser));
	for (int i = 0; i < GAME_PLAYER; i++)
		m_strUserAccount[i] = _T("");

	if (pTagMysteryInfo != NULL)
	{
		//����ׯ��
		m_wBankerUser = pTagMysteryInfo->wBankerUser;
		m_wMyChairID = pTagMysteryInfo->wMyChairID;
		memcpy(m_bHaveUser, pTagMysteryInfo->bHaveUser, sizeof(m_bHaveUser));

		if (m_wBankerUser >= GAME_PLAYER || !m_bHaveUser[m_wBankerUser])
			m_wBankerUser = INVALID_CHAIR;

		if (m_wMyChairID >= GAME_PLAYER || !m_bHaveUser[m_wMyChairID])
			m_wMyChairID = INVALID_CHAIR;

		//��������
		for (int i = 0; i < GAME_PLAYER; i++)
		{
			if (m_bHaveUser[i])
			{
				if (m_wBankerUser == i)
					m_strUserAccount[i].Format(_T("ׯ %s"), pTagMysteryInfo->szAccounts[i]);
				else
					m_strUserAccount[i].Format(_T("%s"), pTagMysteryInfo->szAccounts[i]);
			}			
		}
	}

	//������
	for (int i = 0; i < 4; i++)
	{
		m_CheckBox[i].SetWindowText(_T(""));
		m_CheckBox[i].EnableWindow(FALSE);
		m_CheckBox[i].SetCheck(BST_UNCHECKED);
	}

	//ѡ���Լ�
	if (m_wMyChairID != INVALID_CHAIR)
		m_CheckBox[m_wMyChairID].SetCheck(BST_CHECKED);

	//���´���
	UpdateDlg();
}

//���´���
void CMysteryDlg::UpdateDlg()
{
	//�������
	bool bBankerCheck = false;//(m_wBankerUser!=INVALID_CHAIR && m_CheckBox[m_wBankerUser].GetCheck() == BST_CHECKED);

	//���ð�ť
	for (int i = 0; i < 4; i++)
	{
		if (m_bHaveUser[i])
		{
			m_CheckBox[i].SetWindowText(m_strUserAccount[i]);
			if (!bBankerCheck || m_wBankerUser == i)
				m_CheckBox[i].EnableWindow(TRUE);
			else
				m_CheckBox[i].EnableWindow(FALSE);
		}
		else
		{
			m_CheckBox[i].EnableWindow(FALSE);
		}
	}

	UpdateData(FALSE);
}

//��ȡ��Ϣ
bool CMysteryDlg::GetControlInfo(bool bControl[])
{
	//�������
	bool bBankerCheck = (m_wBankerUser!=INVALID_CHAIR && m_CheckBox[m_wBankerUser].GetCheck() == BST_CHECKED);
	bool bHaveMysteryUser = false;

	ZeroMemory(bControl, sizeof(bool)*GAME_PLAYER);

	if (bBankerCheck)
	{
		bControl[m_wBankerUser] = true;
		bHaveMysteryUser = true;
	}
	else
	{
		for (int i = 0; i < 4; i++)
		{
			if (m_wBankerUser == i)	continue;

			bControl[i] = (m_CheckBox[i].GetCheck() == BST_CHECKED);

			if (bControl[i])	bHaveMysteryUser = true;
		}
	}

	return bHaveMysteryUser;
}

//ѡ��ť
void CMysteryDlg::OnBnClickedCheck1()
{
	UpdateDlg();
}

//ѡ��ť
void CMysteryDlg::OnBnClickedCheck2()
{
	UpdateDlg();
}

//ѡ��ť
void CMysteryDlg::OnBnClickedCheck3()
{
	UpdateDlg();
}

//ѡ��ť
void CMysteryDlg::OnBnClickedCheck4()
{
	UpdateDlg();
}

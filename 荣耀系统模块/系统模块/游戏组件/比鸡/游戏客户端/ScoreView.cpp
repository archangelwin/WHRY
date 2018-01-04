#include "StdAfx.h"
#include "ScoreView.h"
#include ".\scoreview.h"

//////////////////////////////////////////////////////////////////////////

#define SCORE_VIEW_EXIT             102
#define IDC_STARTAGAIN                      112                             //���¿�ʼ
#define IDI_ENDAGAIN                        113                             //����
BEGIN_MESSAGE_MAP(CScoreView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(SCORE_VIEW_EXIT,OnClose)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CScoreView::CScoreView(void)
{
	//��������
	m_nChairWin=0;
	m_nChairLoss=0;
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_nCompare,0,sizeof(m_nCompare));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));
	m_bEnd=0;
    
	//������Դ
	m_ImageBack.LoadFromResource(AfxGetInstanceHandle(),IDB_GAME_END);
}

//��������
CScoreView::~CScoreView(void)
{
}
////�ؼ���
//void CScoreView::DoDataExchange(CDataExchange* pDX)
//{
//	__super::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_CLOSE, m_pClose);
//}
//��ʼ������
BOOL CScoreView::OnInitDialog()
{
	__super::OnInitDialog();

	//MessageBox(NULL,"you",NULL);
	//��������
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));
    
	//���ý���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	CRgn ImageRgn;
	m_ImageBack.CreateImageRegion(ImageRgn,RGB(255,0,255));
	SetWindowRgn(ImageRgn, FALSE);
	ImageRgn.DeleteObject();
	//�ƶ�����
	MoveWindow(300,200,m_ImageBack.GetWidth(),m_ImageBack.GetHeight());
	//SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER); 

	//���ð�ť
	/*m_pClose.Create(NULL,WS_CHILD|WS_VISIBLE,CRect(20,30,10,10),this,SCORE_VIEW_EXIT);
	m_btStartAgain.Create(TEXT(""),WS_CHILD,CRect(0,0,0,0),this,IDC_STARTAGAIN);
	m_btStartAgain.ShowWindow(SW_SHOW);*/
	//CImageHandle ImageHandle1(&m_ImageStart);
	//m_btStartAgain.MoveWindow(70,284,m_ImageStart.GetWidth()/5,m_ImageStart.GetHeight());

	//������Դ������
	m_pSkinResourceManager= (CSkinResourceManager *)CSkinResourceManager::GetInstance();
	ASSERT(m_pSkinResourceManager!=NULL);
	
	return TRUE;
}

//�ػ�����
void CScoreView::OnPaint() 
{
	//if(m_bEnd)
	{
		CPaintDC dc(this); 
		//���� DC
		dc.SetBkMode(TRANSPARENT);
		
		CFont DrawFont;
		DrawFont.CreateFont(15,0,0,0,600,0,0,0,0,0,0,0,0,TEXT("����"));
		dc.SelectObject(&DrawFont);
		//dc.SelectObject(&CSkinAttribute::m_DefaultFont);


		//�滭����
		m_ImageBack.BitBlt(dc,0,0);

		//��ʾ�û�
		CRect rcDraw;
		CRect rcDrawDun;
		CString strScore ;

		for (WORD i=0;i<CountArray(m_szUserName);i++)
		{
			int ndaoshu=0;
			LONG lScore=0;
			if(lstrcmp(m_szUserName[i],_T("")))
			{
				
				DrawFont.CreateFont(20,0,0,0,600,0,0,0,0,0,0,0,0,TEXT("����"));
				dc.SelectObject(&DrawFont);
				dc.SetTextColor(RGB(10,243,108));
				//�û�����
				rcDraw.top=15;
				rcDraw.bottom=rcDraw.top+40;
			/*	rcDrawDun.left = 30;
				rcDrawDun.right= 108;*/
				if(0==i)
				{
					rcDraw.left=90;
					rcDraw.right=rcDraw.left+78;
				}
				if(1==i)
				{
					rcDraw.left=170;
					rcDraw.right=rcDraw.left+78;
				}
				if(2==i)
				{
					rcDraw.left=250;
					rcDraw.right=rcDraw.left+78;
				}
				if(3==i)
				{
					rcDraw.left=330;
					rcDraw.right=rcDraw.left+78;
				}
				TCHAR UserName[256]=TEXT("womendeai");
				dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				//�û�ÿ�ص��ܵ���
 				
				dc.SetTextColor(RGB(166,158,255));
				rcDraw.top=49;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][0]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 
				rcDraw.top=86;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][1]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 
				rcDraw.top=122;
				rcDraw.bottom=rcDraw.top+40;
 				strScore.Format(_T("%d"),m_nCompare[i][2]);
 				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

				rcDraw.top=154;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_nCompareDouble[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);

				rcDraw.top=185;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_cCompareKillThree[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
                
				rcDraw.top=222;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%d"),m_nCompareSpecial[i]); 
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				rcDraw.top=147;
// 				rcDraw.bottom=181;
// 				if(m_nChairWin==i)
// 				{
//                    strScore.Format("%s",TEXT("��"));
// 				   dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				}
// 				if(m_nChairLoss==i)
// 				{
// 					strScore.Format("%s",TEXT("����"));
//                     dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
// 				}
//				dc.SetTextColor(RGB(0,255,100));
				//�û��ķ���
				rcDraw.top=253;
				rcDraw.bottom=rcDraw.top+40;
				//ndaoshu=m_nCompare[i][0]+m_nCompare[i][1]+m_nCompare[i][2]+m_nCompareDouble[i]+m_nCompareSpecial[i];
				strScore.Format(_T("%d"),m_nTotalDaoShu[i]);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				
				//dc.SetTextColor(RGB(255,0,255));
				//rcDraw.top=253;
				//rcDraw.bottom=rcDraw.top+40;
				//strScore.Format("%ld",m_lGameTax[i]/*111111*/);
				//dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
				dc.SetTextColor(RGB(255,255,0));
				rcDraw.top=290;
				rcDraw.bottom=rcDraw.top+40;
				strScore.Format(_T("%ld"),m_lGameScore[i]/*111111*/);
				dc.DrawText(strScore,strScore.GetLength(),&rcDraw,DT_VCENTER|DT_CENTER|DT_END_ELLIPSIS|DT_NOCLIP);
			}
		}
	}
	return;
}

//�滭����
BOOL CScoreView::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}



//�����Ϣ
void CScoreView::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//���û���
void CScoreView::ResetScore()
{
	//��������
	m_bEnd=0;
	m_nChairWin=0;
	m_nChairLoss=0;
	memset(m_lGameTax,0,sizeof(m_lGameTax));
	memset(m_lGameScore,0,sizeof(m_lGameScore));
	memset(m_szUserName,0,sizeof(m_szUserName));
	memset(m_nCompare,0,sizeof(m_nCompare));
	ZeroMemory(m_nTotalDaoShu,sizeof(m_nTotalDaoShu));

	//�滭��ͼ
	Invalidate(NULL);
	
	return;
}

//����˰��
void CScoreView::SetGameTax(LONG lGameEveryTax[])
{
	//���ñ���
	CopyMemory(m_lGameTax,lGameEveryTax,sizeof(m_lGameTax));
	Invalidate(FALSE);
	return;
}

//���û���
void CScoreView::SetGameScore(WORD wChairID,LPCTSTR pszUserName, LONG lScore,int nTotalDaoShu )
{
	//���ñ���
	if (wChairID<CountArray(m_lGameScore))
	{
		m_lGameScore[wChairID]=lScore;
		m_nTotalDaoShu[wChairID]=nTotalDaoShu;
		
		lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
		Invalidate(FALSE);
	}

	return;
}


//////////////////////////////////////////////////////////////////////////

void CScoreView::OnKillFocus(CWnd* pNewWnd)
{
	//���ش���
	//ShowWindow(SW_HIDE) ;

	CDialog::OnKillFocus(pNewWnd);

}
void CScoreView::OnClose()
{
	ShowWindow(SW_SHOW);
	return;
}

void CScoreView::SetEnd( bool bEnd )
{
   m_bEnd=bEnd;
}
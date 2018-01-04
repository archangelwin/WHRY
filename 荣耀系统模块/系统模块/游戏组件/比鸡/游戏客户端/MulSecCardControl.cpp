#include "StdAfx.h"
#include "Resource.h"
#include ".\mulseccardcontrol.h"

//////////////////////////////////////////////////////////////////////////
//��̬����
bool					CMulSecCardControl::m_bLoad=false;					//���ر�־
CSize					CMulSecCardControl::m_CardSize;						//�˿˴�С
CBitImage				CMulSecCardControl::m_ImageCard;					//ͼƬ��Դ
//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CMulSecCardControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

static void MyMsgBox1(LPCSTR lpszFormat, ...)
{
//	va_list args;
//	int		nBuf;
//	TCHAR	szBuffer[512];
//
//	va_start(args, lpszFormat);
//	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
//#ifdef _DEBUG
//	AfxMessageBox(szBuffer);
//#else
//	OutputDebugString(szBuffer);
//#endif
//
//	va_end(args);
}

CMulSecCardControl::CMulSecCardControl(void)
{
	//���ñ���
	m_bDisplay=false;

	//�������
	m_dwCardHSpace=18;
	m_dwCardVSpace=19;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);

	//�ֶα���
	m_bSecCount = MSCC_SECTION_NUM;
	m_bSecArray[0] = MSCC_SECTION_ONE;
	m_bSecArray[1] = MSCC_SECTION_TWO;
	m_bSecArray[2] = MSCC_SECTION_THREE;

	//���б���
	m_dwCurrentIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;

	//��Դ����
	if (m_bLoad==false)
	{
		//������Դ
		m_bLoad=true;
		if (GetSystemMetrics(SM_CXSCREEN)>=1024) m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD);
		else m_ImageCard.LoadFromResource(AfxGetInstanceHandle(),IDB_CARD_800);

		//��ȡ��С
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;
	}

	return;
}

CMulSecCardControl::~CMulSecCardControl(void)
{
}

//���ô���
void CMulSecCardControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow=pSinkWindow;

	return;
}

//������ʾ
void CMulSecCardControl::SetDisplayFlag(bool bDisplay)
{
	//״̬�ж�
	if (m_bDisplay==bDisplay) return;

	//���ñ���
	m_bDisplay=bDisplay;

	//�ػ��ؼ�
	if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//���ü��
void CMulSecCardControl::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace)
{
	//���ñ���
	m_dwCardHSpace=dwCardHSpace;
	m_dwCardVSpace=dwCardVSpace;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CMulSecCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos.x=nXPos;
	m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CMulSecCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPos=BenchmarkPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��ȡ��Ŀ
DWORD CMulSecCardControl::GetCardCount()
{
	return (DWORD)m_CardDataItem.GetCount();
}

//�����˿�
DWORD CMulSecCardControl::SetCardData(const BYTE bCardData[], DWORD dwCardCount)
{
	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����˿�
	if (bCardData == NULL)
	{
		m_CardDataItem.SetSize(0);
	}
	else
	{
		m_CardDataItem.SetSize(dwCardCount);
		for (DWORD i=0;i<dwCardCount;i++)
		{
			//������֤
			ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

			m_CardDataItem[i].bShoot=false;
			m_CardDataItem[i].bCardData=bCardData[i];
		}
	}

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();

	return dwCardCount;
}

//�����˿�
bool CMulSecCardControl::SetCardItem(BYTE bCardData, DWORD dwIndex)
{
	ASSERT(dwIndex>=0 && dwIndex<(DWORD)m_CardDataItem.GetCount()) ;
	if(dwIndex<0 || dwIndex>=(DWORD)m_CardDataItem.GetCount()) return false ;
	m_CardDataItem[dwIndex].bShoot=false;
	m_CardDataItem[dwIndex].bCardData=bCardData;

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();
	return true ;
}

//��ȡ�˿�
DWORD CMulSecCardControl::GetCardData(BYTE bCardData[], DWORD dwMaxCount)
{
	//Ч�����
	ASSERT(dwMaxCount>=(DWORD)m_CardDataItem.GetCount());

	ASSERT(dwMaxCount>0 && dwMaxCount<=HAND_CARD_COUNT) ;

	//�����˿�
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0 ; i<dwCardCount ; ++i)
		bCardData[i] = m_CardDataItem[i].bCardData ;

	return dwCardCount;
}

//��ȡ�˿�
DWORD CMulSecCardControl::GetCardData(BYTE bCardData)
{
	//�����˿�
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0 ; i<dwCardCount ; ++i)
		if(bCardData==m_CardDataItem[i].bCardData) return i ;	 
	return -1 ;
}

//�����˿�
DWORD CMulSecCardControl::SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount)
{
	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	CopyMemory(m_CardDataItem.GetData(),CardItemArray,dwCardCount*sizeof(tagCardItem));

	//�����ؼ�
	if (m_hWnd!=NULL) RectifyControl();
 
	return 0;
}

//����λ��
void CMulSecCardControl::RectifyControl()
{
	//��������
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	DWORD dwSecCCount[MSCC_SECTION_NUM] = {0};
	dwSecCCount[2] = (dwCardCount>8)?dwCardCount-8:0;
	dwSecCCount[1] = (dwCardCount-dwSecCCount[2]>3)?dwCardCount-dwSecCCount[2]-3:0;
	dwSecCCount[0] = dwCardCount-dwSecCCount[2]-dwSecCCount[1];

	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����С
	CSize ControlSize;
	if (dwCardCount > 0 && dwCardCount <= MSCC_SECTION_ONE)
	{
		ControlSize.cy = m_CardSize.cy;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else if (dwCardCount > MSCC_SECTION_ONE && dwCardCount <= MSCC_SECTION_TWO+MSCC_SECTION_ONE)
	{
		ControlSize.cy = m_CardSize.cy + m_dwCardVSpace;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else if (dwCardCount > MSCC_SECTION_TWO+MSCC_SECTION_ONE && dwCardCount <= HAND_CARD_COUNT)
	{
		ControlSize.cy = m_CardSize.cy + m_dwCardVSpace*2;
		ControlSize.cx = m_CardSize.cx + (MSCC_SECTION_TWO-1)*m_dwCardHSpace;
	}
	else
	{
		ControlSize.SetSize(0, 0);
	}

	//����λ��
	int nXPos=0;
	int nYPos=0;

	switch (m_XCollocateMode)
	{
	case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
	case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
	case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
	}
	switch (m_YCollocateMode)
	{
	case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
	case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
	case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
	}

	//�ƶ�λ��
	SetWindowPos(NULL, nXPos, nYPos, ControlSize.cx, ControlSize.cy, SWP_NOZORDER);

	//��������
	CRgn CardRegion,SignedRegion;
	tagCardItem * pCardItem=NULL;

	//��������
	CardRegion.CreateRectRgn(0,0,0,0);
	for (DWORD i = 0; i < dwCardCount; i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//����λ��
		if (i < MSCC_SECTION_ONE)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[0]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*i;
			nYPos = 0;
		}
		else if (i >= MSCC_SECTION_ONE && i < MSCC_SECTION_TWO+MSCC_SECTION_ONE)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[1]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE);
			nYPos = m_dwCardVSpace;
		}
		else if (i >= MSCC_SECTION_TWO+MSCC_SECTION_ONE && i < HAND_CARD_COUNT)
		{
			nXPos = (ControlSize.cx - ((dwSecCCount[2]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE - MSCC_SECTION_TWO);
			nYPos = m_dwCardVSpace*2;
		}

		//�ϲ�����
        SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+m_CardSize.cx+1,nYPos+m_CardSize.cy+1,2,2);

		CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
		SignedRegion.DeleteObject();
	}

	//��������
	SetWindowRgn(CardRegion,TRUE);
	m_CardRegion.DeleteObject();
	m_CardRegion.Attach(CardRegion.Detach());

	//�ػ�����
	Invalidate(TRUE);

	return;
}

//�����л�
DWORD CMulSecCardControl::SwitchCardPoint(const CPoint & MousePoint)
{//��ȡ������ƿؼ��ϵĵڼ�����
	return DWORD(-1);
}

//�ػ�����
void CMulSecCardControl::OnPaint()
{
	CPaintDC dc(this);

	//��������
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
	DWORD dwSecCCount[MSCC_SECTION_NUM] = {0};
	dwSecCCount[2] = (dwCardCount>8)?dwCardCount-8:0;
	dwSecCCount[1] = (dwCardCount-dwSecCCount[2]>3)?dwCardCount-dwSecCCount[2]-3:0;
	dwSecCCount[0] = dwCardCount-dwSecCCount[2]-dwSecCCount[1];

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����λͼ
	CBitmap BufferImage;
	BufferImage.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	CDC BufferDC;
	BufferDC.CreateCompatibleDC(&dc);
	BufferDC.SelectObject(&BufferImage);

	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	//������֤
	ASSERT(m_CardDataItem.GetCount()>=0 && m_CardDataItem.GetCount()<=HAND_CARD_COUNT) ;

	for (INT i=0;i<m_CardDataItem.GetCount();i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//��ȡ�˿�
		tagCardItem * pCardItem=&m_CardDataItem[i];

		//ͼƬλ��
		if ((m_bDisplay==true)&&(pCardItem->bCardData!=0))
		{
			dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
			dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
		}
		else
		{
			dwXImagePos=m_CardSize.cx*2;
			dwYImagePos=m_CardSize.cy*4;
		}

		//��Ļλ��
		if (i < MSCC_SECTION_ONE)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[0]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*i;
			dwYScreenPos = 0;
		}
		else if (i >= MSCC_SECTION_ONE && i < MSCC_SECTION_TWO+MSCC_SECTION_ONE)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[1]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE);
			dwYScreenPos = m_dwCardVSpace;
		}
		else if (i >= MSCC_SECTION_TWO+MSCC_SECTION_ONE && i < HAND_CARD_COUNT)
		{
			dwXScreenPos = (rcClient.Width() - ((dwSecCCount[2]-1)*m_dwCardHSpace+m_CardSize.cx))/2 + m_dwCardHSpace*(i - MSCC_SECTION_ONE - MSCC_SECTION_TWO);
			dwYScreenPos = m_dwCardVSpace*2;
		}

		//�滭�˿�
		m_ImageCard.BitBlt(BufferDC,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&BufferDC,0,0,SRCCOPY);

	//������Դ
	BufferDC.DeleteDC();
	BufferImage.DeleteObject();

	return;
}

//������Ϣ
int CMulSecCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	return 0;
}

//�����Ϣ
void CMulSecCardControl::OnRButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnRButtonUp(nFlags, Point);

	return;
}

//�����Ϣ
void CMulSecCardControl::OnLButtonUp(UINT nFlags, CPoint Point)
{
	__super::OnLButtonUp(nFlags, Point);

	return;
}

//�����Ϣ
void CMulSecCardControl::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);

	return;
}

//�����Ϣ
BOOL CMulSecCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
{
	return __super::OnSetCursor(pWnd,nHitTest,message);
}

#include "StdAfx.h"
#include "Resource.h"
#include "CardControl.h"

//////////////////////////////////////////////////////////////////////////
//��̬����

bool					CCardControl::m_bLoad=false;					//���ر�־
CSize					CCardControl::m_CardSize;						//�˿˴�С
CSize					CCardControl::m_BigCardSize;				    //���˿˴�С 
CSize                   CCardControl::m_User2BigCardSize;               //�Լ��˿˴�С
CSize					CCardControl::m_RightCardSize;					//�˿˴�С
//////////////////////////////////////////////////////////////////////////

// BEGIN_MESSAGE_MAP(CCardControl, CWnd)
// // 	ON_WM_PAINT()
// // 	ON_WM_CREATE()
// // 	ON_WM_SETCURSOR()
// // 	ON_WM_RBUTTONUP()
// // 	ON_WM_LBUTTONUP()
// // 	ON_WM_LBUTTONDOWN()
// // 	ON_WM_MOUSEMOVE()
// END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCardControl::CCardControl()
{
	//������Դ
	//HINSTANCE hInstance=AfxGetInstanceHandle();
	//if(m_bLoad == false)
	//{
		m_bLoad = true;
		HINSTANCE hInstance=AfxGetInstanceHandle();
		m_CardType.LoadImage(hInstance,TEXT("IDB_CARD_TYPE_BIG"));
		m_CardSpecialType.LoadImage(hInstance,TEXT("IDB_SPECIAL_CARD_TYPE"));
		m_MidThreeDeouble.LoadImage(hInstance,TEXT("IDB_MID_THREE_DEOUBLE"));
		m_Ayaka.LoadImage(hInstance,TEXT("IDB_AYAKA"));
		m_ImageCard.LoadImage(hInstance,TEXT("IDB_CARD"));
		m_ImageBigCard.LoadImage(hInstance,TEXT("IDB_BIG_CARD"));
		m_ISendCard.LoadImage(hInstance,TEXT("IDB_SEND_CARD"));
		m_IShuffleCard.LoadImage(hInstance,TEXT("IDB_SHUFFLE_CARD"));

		m_SendCardFlash.cx = m_ISendCard.GetWidth()/10;
		m_SendCardFlash.cy = m_ISendCard.GetHeight();
		m_CardSize.cx=m_ImageCard.GetWidth()/13;
		m_CardSize.cy=m_ImageCard.GetHeight()/5;

		m_BigCardSize.cx=m_ImageBigCard.GetWidth()/13;
		m_BigCardSize.cy=m_ImageBigCard.GetHeight()/4;
	//}
	//���ñ���
	m_enSide=enError ;
	m_DrawType = -1;
	m_nShuffleNum =-1;         ///<��ʾϴ�Ƶڼ�֡
	m_nCardNum	 = 0;
	m_PosCard.cx = 0;
	m_PosCard.cy = 0;
	m_NumCard	 =-3;
	m_nI		 = 0;
	m_nFireGunStatus=0;
	m_nAyakaFrames=0;
	m_bDrawCardFlash = false;  ///<�Ƿ���
	m_bDisplay=false;
	m_bHorizontal=true;
	m_bPositively=false;
	m_bDrawType=false;
	m_bDrawFire=false;
	m_bDrawCard=false;
	m_bLButtonUp=true;
	m_bMoveCard=false;
	m_bAyaka=false;

	//�������
	m_dwCardHSpace=18;
	m_dwCardVSpace=19;
	m_dwShootAltitude=20;
	m_bCompare=true;

	//λ�ñ���
	m_XCollocateMode=enXCenter;
	m_YCollocateMode=enYCenter;
	m_BenchmarkPos.SetPoint(0,0);
	m_BenchmarkPosOld.SetPoint(0,0);

	//���б���
	m_dwCurrentIndex=0xFFFFFFFF;

	//�ڲ�����
	m_pSinkWindow=NULL;
	m_bBigCard=false;
    m_bMyselfUser=false;
	return;
}

//��������
CCardControl::~CCardControl()
{
}

//���ô���
void CCardControl::SetSinkWindow(CWnd * pSinkWindow)
{
	//���ñ���
	m_pSinkWindow=pSinkWindow;

	return;
}

//���÷���
void CCardControl::SetDirection(bool bHorizontal)
{
	//״̬�ж�
	if (m_bHorizontal==bHorizontal) return;

	//���ñ���
	m_bHorizontal=bHorizontal;

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();

	return;
}

//�����˿�
void CCardControl::ShootAllCard(bool bShoot)
{
	//��������
	bool bRectify=false;
	tagCardItem * pCardItem=NULL;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����˿�
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot!=bShoot)
		{
			bRectify=true;
			pCardItem->bShoot=bShoot;
		}
	}

	//�����ؼ�
	//if (bRectify==true) RectifyControl();

	return;
}

//������ʾ
void CCardControl::SetDisplayFlag(bool bDisplay)
{
	//״̬�ж�
	if (m_bDisplay==bDisplay) return;

	//���ñ���
	m_bDisplay=bDisplay;

	//�ػ��ؼ�
	//if (m_hWnd!=NULL) Invalidate(FALSE);

	return;
}

//������Ӧ
void CCardControl::SetPositively(bool bPositively)
{
	//���ñ���
	m_bPositively=bPositively;

	return;
}

//���ü��
void CCardControl::SetCardSpace(DWORD dwCardHSpace, DWORD dwCardVSpace, DWORD dwShootAltitude)
{
	//���ñ���
	m_dwCardHSpace=dwCardHSpace;
	m_dwCardVSpace=dwCardVSpace;
	m_dwShootAltitude=dwShootAltitude;

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	m_BenchmarkPosOld.x=m_BenchmarkPos.x=nXPos;
    m_BenchmarkPos.y=nYPos;
	m_XCollocateMode=XCollocateMode;
	m_YCollocateMode=YCollocateMode;

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();

	return;
}

//��׼λ��
void CCardControl::SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)
{
	//���ñ���
	//m_BenchmarkPos=BenchmarkPos;
	//m_XCollocateMode=XCollocateMode;
	//m_YCollocateMode=YCollocateMode;

	////�����ؼ�
	////if (m_hWnd!=NULL) RectifyControl();

	//return;
}

//��ȡ��Ŀ
DWORD CCardControl::GetCardCount()
{
	return (DWORD)m_CardDataItem.GetCount();
}

//��ȡ�˿�
DWORD CCardControl::GetShootCard(BYTE bCardData[], DWORD dwMaxCount)
{
	//��������
	DWORD bShootCount=0L;
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();

	ASSERT(dwCardCount>0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����˿�
	tagCardItem * pCardItem=NULL;
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		//��ȡ�˿�
		pCardItem=&m_CardDataItem[i];

		//�˿˷���
		if (pCardItem->bShoot==true) bCardData[bShootCount++]=pCardItem->bCardData;
	}

	return bShootCount;
}

//�����˿�
DWORD CCardControl::SetCardData(const BYTE bCardData[], DWORD dwCardCount)
{
	//MessageBox(NULL,"she",NULL);
	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;
    if(m_bBigCard)
	{
       m_BenchmarkPos.x=m_BenchmarkPosOld.x;
	}
    
	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	for (DWORD i=0;i<dwCardCount;i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		m_CardDataItem[i].bShoot=false;
		m_CardDataItem[i].bCardData=bCardData[i];
	}

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();
	int CardWidth=0;
	if(m_bBigCard)
	{
        CardWidth=m_BigCardSize.cx;
		DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
		if(dwCardCount==13)
		{
			int AllCardWidth=(CardWidth+(dwCardCount-1)*m_dwCardHSpace);
			m_BenchmarkPos.x=m_BenchmarkPos.x-AllCardWidth/4;
			m_bMoveCard=true;
		}
		
	}
	
   
	return dwCardCount;
}

//�����˿�
bool CCardControl::SetCardItem(BYTE bCardData, DWORD dwIndex)
{
	ASSERT(dwIndex>=0 && dwIndex<(DWORD)m_CardDataItem.GetCount()) ;
	if(dwIndex<0 || dwIndex>=(DWORD)m_CardDataItem.GetCount()) return false ;
	m_CardDataItem[dwIndex].bShoot=false;
	m_CardDataItem[dwIndex].bCardData=bCardData;

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();
	return true ;
}

//��ȡ�˿�
DWORD CCardControl::GetCardData(BYTE bCardData[], DWORD dwMaxCount)
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
DWORD CCardControl::GetCardData(BYTE bCardData)
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
DWORD CCardControl::SetCardItem(const tagCardItem CardItemArray[], DWORD dwCardCount)
{
	//������֤
	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;

	//�����˿�
	m_CardDataItem.SetSize(dwCardCount);
	CopyMemory(m_CardDataItem.GetData(),CardItemArray,dwCardCount*sizeof(tagCardItem));

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();
 
	return 0;
}
//���õ����˿�
DWORD CCardControl::SetShootCard(const BYTE bCardDataIndex[], DWORD dwCardCount)
{
	//MessageBox(NULL,"tan",NULL);
	//������֤
	ASSERT(dwCardCount>0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0;i<dwCardCount;i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		tagCardItem * pCardItem=&m_CardDataItem[bCardDataIndex[i]];
		pCardItem->bShoot=true;
		//if (m_hWnd!=NULL) RectifyControl();
	}
	return 0;
}
//����λ��
void CCardControl::RectifyControl()
{
  	//if(!m_bDrawFire)
  	//	CSize SPoint;
  	//	if(m_bBigCard)
  	//	{
  	//		SPoint.cx=m_BigCardSize.cx;
  	//		SPoint.cy=m_BigCardSize.cy;
  	//	}
  	//	else
  	//	{
			//if(m_bMyselfUser)
			//{
			//	SPoint.cx=m_User2BigCardSize.cx;
			//	SPoint.cy=m_User2BigCardSize.cy;
			//}
			//else
			//{
			//	SPoint.cx=m_CardSize.cx;
			//	SPoint.cy=m_CardSize.cy;
			//}
  	//		
  	//	}
  	//	//��������
  	//	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
  	//	if(0!=dwCardCount)
  	//	{
  	//		int i ;
  	//		i=3 ;
  	//	}
  
  	//	//������֤
  	//	ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;
  
  	//	//�����С
  	//	CSize ControlSize;
  	//	if (m_bHorizontal==true)
  	//	{
  	//		ControlSize.cy=SPoint.cy+m_dwShootAltitude;
  	//		ControlSize.cx=(dwCardCount>0)?(SPoint.cx+(dwCardCount-1)*m_dwCardHSpace):0;
			////ControlSize.cx=m_IGun[0].GetWidth();
			////ControlSize.cy=m_IGun[0].GetHeight();
  	//	}
  	//	else
  	//	{
  	//		//ControlSize.cx=m_ImageLBackCard->GetWidth()-25;
  	//		//ControlSize.cy=(dwCardCount>0)?(m_ImageLBackCard->GetWidth()+(dwCardCount-1)*m_dwCardVSpace):0;
  
  	//		//		CImageHandle CardImageHandle(&m_ImageLBackCard);		
  	//		//ZBD		ControlSize.cx=m_ImageLBackCard->GetWidth();
  	//		//ZBD		ControlSize.cy=(dwCardCount>0)?(m_ImageLBackCard->GetHeight()+(dwCardCount-1)*m_dwCardVSpace):0;
  	//	}
  
  	//	//����λ��
  	//	int nXPos=0;
  	//	int nYPos=0;
  	//	if(enHorizon==m_enSide)
  	//	{
  	//		switch (m_XCollocateMode)
  	//		{
  	//		case enXLeft:	{ nXPos=m_BenchmarkPos.x; break; }
  	//		case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
  	//		case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx; break; }
  	//		}
  	//		switch (m_YCollocateMode)
  	//		{
  	//		case enYTop:	{ nYPos=m_BenchmarkPos.y; break; }
  	//		case enYCenter: { nYPos=55+m_BenchmarkPos.y+15-ControlSize.cy/2; break; }
  	//		case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
  	//		}
  	//	}
  	//	else if(enRight==m_enSide || enLeft==m_enSide)
  	//	{
  	//		switch (m_XCollocateMode)
  	//		{
  	//		case enXLeft:	{ nXPos=m_BenchmarkPos.x+30; break; }
  	//		case enXCenter: { nXPos=m_BenchmarkPos.x-ControlSize.cx/2; break; }
  	//		case enXRight:	{ nXPos=m_BenchmarkPos.x-ControlSize.cx-20; break; }
  	//		}
  
  	//		//����λ��
  	//		switch (m_YCollocateMode)
  	//		{
  	//		case enYTop:	{ nYPos=m_BenchmarkPos.y-60; break; }
  	//		case enYCenter: { nYPos=m_BenchmarkPos.y-ControlSize.cy/2; break; }
  	//		case enYBottom: { nYPos=m_BenchmarkPos.y-ControlSize.cy; break; }
  	//		}
  	//	}
  
  		//�ƶ�λ��
  		//SetWindowPos(NULL,nXPos,nYPos,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);
  
  		//��������
//   		CRgn CardRegion,SignedRegion;
//   		tagCardItem * pCardItem=NULL;
  
  		//��������
//   		CardRegion.CreateRectRgn(0,0,0,0);
//   		for (DWORD i=0;i<dwCardCount;i++)
//   		{
//   			//������֤
//   			ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;
//   
//   			//��ȡ�˿�
//   			pCardItem=&m_CardDataItem[i];
//   
//   			//����λ��
//   			if (m_bHorizontal==true)
//   			{
//   				nXPos=m_dwCardHSpace*i;
//   				nYPos=(pCardItem->bShoot==false)?m_dwShootAltitude:0;
//   			}
//   			else 
//   			{
//   				nXPos=0;
//   				nYPos=m_dwCardVSpace*i;
//   			}
//   
//   			//�ϲ�����
//   			if(enHorizon==m_enSide)
//   				SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+SPoint.cx+1,nYPos+SPoint.cy+1,2,2);
//   			else 
//   				SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+ControlSize.cx+1,nYPos+ControlSize.cy+1,2,2);
//   			CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
//   			SignedRegion.DeleteObject();
//   		}
//   
//   		//��������
//   		SetWindowRgn(CardRegion,TRUE);
//   		m_CardRegion.DeleteObject();
//   		m_CardRegion.Attach(CardRegion.Detach());
//   
//   	}
//   	else
// 	{
// 		
// 		//MessageBox(NULL,"lai",NULL);
// 		CSize SPoint;
// 		CImageHandle HandleCard1(&m_IGun[0]);
// 		SPoint.cx=m_IGun[0].GetWidth();
// 		SPoint.cy=m_IGun[0].GetHeight();
// 		//��������
// 		DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
// // 		if(0!=dwCardCount)
// // 		{
// // 			int i ;
// // 			i=3 ;
// // 		}
// // 
// // 		//������֤
// // 		ASSERT(dwCardCount>=0 && dwCardCount<=HAND_CARD_COUNT) ;
// 
// 		//�����С
// 		CSize ControlSize;
// 			ControlSize.cy=SPoint.cy;
// 			ControlSize.cx=SPoint.cx;
// 		//����λ��
// 		int nXPos=0;
// 		int nYPos=0;
// 		//�ƶ�λ��
// 		SetWindowPos(NULL,m_BenchmarkPos.x,m_BenchmarkPos.y,ControlSize.cx,ControlSize.cy,SWP_NOZORDER);
// 
// 		//��������
// 		CRgn CardRegion,SignedRegion;
// 		tagCardItem * pCardItem=NULL;
// 
// 		//��������
// 		CardRegion.CreateRectRgn(0,0,0,0);
// 		for (DWORD i=0;i<dwCardCount;i++)
// 		{
// 			//������֤
// 			ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;
// 
// 			//��ȡ�˿�
// 			pCardItem=&m_CardDataItem[i];
// 
// 			//����λ��
// 			if (m_bHorizontal==true)
// 			{
// 				nXPos=m_dwCardHSpace*0;
// 				nYPos=(pCardItem->bShoot==false)?0:0;
// 			}
// 			else 
// 			{
// 				nXPos=0;
// 				nYPos=m_dwCardVSpace*0;
// 			}
// 
// 			//�ϲ�����
// 			if(enHorizon==m_enSide)
// 				SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+SPoint.cx+1,nYPos+SPoint.cy+1,2,2);
// 			else 
// 				SignedRegion.CreateRoundRectRgn(nXPos,nYPos,nXPos+ControlSize.cx+1,nYPos+ControlSize.cy+1,2,2);
// 			CardRegion.CombineRgn(&CardRegion,&SignedRegion,RGN_OR);
// 			SignedRegion.DeleteObject();
// 		}
// 
// 		//��������
// 		SetWindowRgn(CardRegion,TRUE);
// 		m_CardRegion.DeleteObject();
// 		m_CardRegion.Attach(CardRegion.Detach());
// 
// 	}
	
	//�ػ�����
	//Invalidate(TRUE);

	return;
}

//�����л�
DWORD CCardControl::SwitchCardPoint(const CPoint & MousePoint)
{
	//��ȡ������ƿؼ��ϵĵڼ�����

	if (m_bHorizontal==true)
	{
		//��������
		DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
		if(dwCardCount>0)
		{
			if(MousePoint.x<m_BenchmarkPos.x||MousePoint.x>m_BenchmarkPos.x+(dwCardCount-1)*m_dwCardHSpace+m_CardSize.cx)
			{
				return -1;
			}
			//��ȡ����
			DWORD dwCardIndex=(MousePoint.x-m_BenchmarkPos.x)/m_dwCardHSpace;
			if (dwCardIndex>=dwCardCount&&MousePoint.x>=m_BenchmarkPos.x&&MousePoint.x<=m_BenchmarkPos.x+(dwCardCount-1)*m_dwCardHSpace+m_CardSize.cx)
			{
				dwCardIndex=(dwCardCount-1);
			}

			//�жϰ���
			bool bCurrentCard=true;
			bool bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
			if ((bCardShoot==true)&&(MousePoint.y>m_CardSize.cy+m_BenchmarkPos.y||MousePoint.y<m_BenchmarkPos.y)) bCurrentCard=false;
			if ((bCardShoot==false)&&(MousePoint.y<(INT)m_dwShootAltitude+m_BenchmarkPos.y||MousePoint.y>(INT)m_dwShootAltitude+m_BenchmarkPos.y+m_CardSize.cy)) bCurrentCard=false;
			bool bFind=true;
			//��ǰѰ��
			if (bCurrentCard==false)
			{
				if(dwCardIndex==0)
				{
					return -1;
				}
				while (dwCardIndex>0)
				{
					dwCardIndex--;
					bCardShoot=m_CardDataItem[dwCardIndex].bShoot;
					if ((bCardShoot==false)&&(MousePoint.y>=(INT)m_dwShootAltitude+m_BenchmarkPos.y&&MousePoint.y<=(INT)m_dwShootAltitude+m_BenchmarkPos.y+m_CardSize.cy)
						&&MousePoint.x>=m_BenchmarkPos.x+dwCardIndex*m_dwCardHSpace&&MousePoint.x<=m_BenchmarkPos.x+dwCardIndex*m_dwCardHSpace+m_CardSize.cx)
					{
						bFind=true;
						break;
					}

					if ((bCardShoot==true)&&(MousePoint.y>=m_BenchmarkPos.y&&MousePoint.y<=m_BenchmarkPos.y+m_CardSize.cy)
						&&MousePoint.x>=m_BenchmarkPos.x+dwCardIndex*m_dwCardHSpace&&MousePoint.x<=m_BenchmarkPos.x+dwCardIndex*m_dwCardHSpace+m_CardSize.cx)
					{
						bFind=true;
						break;
					}
					bFind=false;	

				}

			}
			if(bFind)
			{
				return dwCardIndex;
			}
		}
	}

	return DWORD(-1);
}

//�ػ�����
// void CCardControl::OnPaint()
// {
// 	return;
// }

//������Ϣ
// int CCardControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
// {
// 	if (__super::OnCreate(lpCreateStruct)==-1) return -1;
// 
// 	//���ñ���
// 	SetClassLONG(m_hWnd,GCL_HBRBACKGROUND,NULL);
//     
//      return 0;
// }

//�����Ϣ
// void CCardControl::OnRButtonUp(UINT nFlags, CPoint Point)
// {
// 	__super::OnRButtonUp(nFlags, Point);
// 
// 	//״̬�ж�
// 	if ((m_bHorizontal==false)||(m_bPositively==false)) return;
// 
// 	//������Ϣ
// 	if (m_pSinkWindow!=NULL) m_pSinkWindow->PostMessage(IDM_RIGHT_HIT_CARD,0,0);
// 
// 	return;
// }

//�����Ϣ
 int CCardControl::OnLButtonUp(CPoint Point)
 {
 	//״̬�ж�
 	if (m_dwCurrentIndex==0xFFFFFFFF||m_dwCurrentIndex==-1)
	{
        ShootAllCard(false);
		return -1;
	}
 	if ((m_bHorizontal==false)||(m_bPositively==false)) return 0;

 	bool bChange = false;
 	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
 	tagCardItem * pCardItem = NULL;
 
 	for (DWORD i=0; i<dwCardCount; i++)
 	{
 		pCardItem=&m_CardDataItem[i];
 
 		if (pCardItem->bMultiSelect)
 		{
 			pCardItem->bMultiSelect = false;
 			pCardItem->bShoot = !pCardItem->bShoot;
 			bChange	= true;
            m_bLButtonUp=true;
 		}
 	}
 
 	if (! bChange)
 	{
 		//��ȡ����
 		DWORD dwCurrentIndex=SwitchCardPoint(Point);
        if(dwCurrentIndex<0||dwCurrentIndex>dwCardCount-1)
		{
			return 0;
		}
 		//���ÿؼ�
 		pCardItem = &m_CardDataItem[dwCurrentIndex];
 		pCardItem->bShoot = !pCardItem->bShoot;
		m_bLButtonUp=true;
	}
	//������Ϣ
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();
	if (pGameFrameEngine!=NULL)
		pGameFrameEngine->PostMessage(IDM_LEFT_HIT_CARD,0,0);
		
    m_dwCurrentIndex=-1;
 	//�����ؼ�
 	//RectifyControl();
 
 	return 0;
 }

//�����Ϣ
 void CCardControl::OnLButtonDown(CPoint Point)
 {
 	//״̬�ж�
 	if ((m_bHorizontal==false)||(m_bPositively==false)) return;
 
 	//��ȡ����
 	m_dwCurrentIndex=SwitchCardPoint(Point);
	//printf("m_dwCurrentIndex=%d\n",m_dwCurrentIndex);
	DWORD dwCardCount=(DWORD)m_CardDataItem.GetCount();
    if(m_dwCurrentIndex>=0&&m_dwCurrentIndex<dwCardCount)
	{
		m_bLButtonUp=false;
	}
 	return;
 }

//�����Ϣ
// BOOL CCardControl::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message)
// {
// 	//���ù��
// 	if (m_bPositively==true)
// 	{
// 		SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_CARD_CUR)));
// 		return TRUE;
// 	}
// 
// 	return __super::OnSetCursor(pWnd,nHitTest,message);
// }

//����λ��
void CCardControl::SetCardSide(enCardSide	enSide) 
{
	if(m_enSide == enSide) return ;

	m_enSide = enSide ;

	//�����ؼ�
	//if (m_hWnd!=NULL) RectifyControl();

}
 void CCardControl::OnMouseMove(CPoint point)
 {
	 if(m_bLButtonUp==false)
	 {
		 //MessageBox(NULL,"yidong",NULL);
		 //��ȡ����	
		 DWORD dwCurrentIndex=SwitchCardPoint(point);
		 if(-1 == m_dwCurrentIndex || -1 == dwCurrentIndex) 
			 return;

		 tagCardItem * pCardItemTemp = NULL;
		 for (DWORD i=0; i<static_cast<DWORD>(m_CardDataItem.GetCount()); i++)
		 {
			 pCardItemTemp=&m_CardDataItem[i];
			 if (pCardItemTemp->bMultiSelect)
			 {
				 pCardItemTemp->bMultiSelect = false;
			 }
		 }

		 if(m_dwCurrentIndex > dwCurrentIndex)
		 {
			 tagCardItem * pCardItemTemp = NULL;
			 for (DWORD i=dwCurrentIndex; i<=m_dwCurrentIndex; i++)
			 {
				 pCardItemTemp=&m_CardDataItem[i];
				 pCardItemTemp->bMultiSelect = true;			
			 }
		 }
		 else
		 {
			 tagCardItem * pCardItemTemp = NULL;
			 for (DWORD i=m_dwCurrentIndex; i<=dwCurrentIndex; i++)
			 {
				 pCardItemTemp=&m_CardDataItem[i];
				 pCardItemTemp->bMultiSelect = true;			
			 }
		 }
	 }
 	
 }

DWORD CCardControl::SetRegainCard(DWORD dwCardCount )
{
	//������֤
	ASSERT(dwCardCount>0 && dwCardCount<=HAND_CARD_COUNT) ;

	for(DWORD i=0;i<dwCardCount;i++)
	{
		//������֤
		ASSERT(i>=0 && i<=HAND_CARD_COUNT) ;

		tagCardItem * pCardItem=&m_CardDataItem[i];
		pCardItem->bShoot=false;
		//if (m_hWnd!=NULL) RectifyControl();
	}
	return 0;
}

void CCardControl::SetbBigCard( bool bBigCard )
{
   m_bBigCard=bBigCard;
}

void CCardControl::SetbDrawType( bool bDrawType )
{
   m_bDrawType=bDrawType;
}

void CCardControl::SetDrawTypePoint( int nx,int ny )
{
	m_nTypeX=nx;
	m_nTypeY=ny;
	//RectifyControl();
}

void CCardControl::SetDraw()
{
  //RectifyControl();
}

bool CCardControl::GetDraw()
{
   return m_bDrawType;
}

void CCardControl::SetCardType(BYTE CardType)
{
  m_DrawType=CardType;
}
void CCardControl::SetbDrawFire( bool bDrawFire )
{
	m_bDrawFire=bDrawFire;
	//�����ؼ�
// 	if (m_hWnd!=NULL)
// 	{
// 		//MessageBox(NULL,"zhen",NULL);
// 		RectifyControl();
// 	}
}

void CCardControl::SetFireGunStatus( int nFireGunStatus )
{
	m_nFireGunStatus=nFireGunStatus;
}

void CCardControl::SetbCompare( bool bCompare )
{
   m_bCompare=bCompare;
}

void CCardControl::SetbMyselfUser( bool bMyselfUser )
{
   m_bMyselfUser=bMyselfUser;
}
void CCardControl::DrawCardFlash( CDC* pDc,int nWidth,int nHeight )
{
	//�滭�˿�
	DWORD dwXImagePos,dwYImagePos;
	DWORD dwXScreenPos,dwYScreenPos;
	//������֤
	ASSERT(m_CardDataItem.GetCount()>=0 && m_CardDataItem.GetCount()<=HAND_CARD_COUNT) ;
	
	
	
	if(m_nI>=13)
	{
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
				dwXImagePos=0+m_CardSize.cx*2;
				dwYImagePos=m_CardSize.cy*4;
			}

			//��Ļλ��
			if (m_bHorizontal==true) 
			{
				dwXScreenPos=m_dwCardHSpace*i+m_BenchmarkPos.x;
				dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude+m_BenchmarkPos.y:0+m_BenchmarkPos.y;
			}
			else
			{
				dwXScreenPos=0+m_BenchmarkPos.x;
				dwYScreenPos=m_dwCardVSpace*i+m_BenchmarkPos.y;
			}
			//�滭�˿�
			m_ImageCard.DrawImage(pDc,dwXScreenPos+nWidth,dwYScreenPos+nHeight,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
		}
	}
	else
	{
		for(int j=0;j<m_nI;j++)
		{
			//������֤
			ASSERT(j>=0 && j<=HAND_CARD_COUNT) ;
			//��ȡ�˿�
			tagCardItem * pCardItem=&m_CardDataItem[j];
			//ͼƬλ��
			if ((m_bDisplay==true)&&(pCardItem->bCardData!=0))
			{
				dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_CardSize.cx;
				dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_CardSize.cy;
			}
			else
			{
				dwXImagePos=0+m_CardSize.cx*2;
				dwYImagePos=m_CardSize.cy*4;
			}
			//��Ļλ��
			if (m_bHorizontal==true) 
			{
				dwXScreenPos=m_dwCardHSpace*j+m_BenchmarkPos.x;
				dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude+m_BenchmarkPos.y:0+m_BenchmarkPos.y;
			}
			else
			{
				dwXScreenPos=0+m_BenchmarkPos.x;
				dwYScreenPos=m_dwCardVSpace*j+m_BenchmarkPos.y;
			}
			//�滭�˿�
			m_ImageCard.DrawImage(pDc,dwXScreenPos+nWidth,dwYScreenPos+nHeight,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
		}
		if(m_nI>=m_CardDataItem.GetCount())
		{
			m_nI=m_CardDataItem.GetCount();
		}
	}	
}

void CCardControl::DrawCard( CDC* pDc )
{
	//if(!m_bDrawFire)
	if(m_bDrawCard)  
	{
		if(m_bBigCard)
		{
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
					dwXImagePos=((pCardItem->bCardData&CARD_MASK_VALUE)-1)*m_BigCardSize.cx;
					dwYImagePos=((pCardItem->bCardData&CARD_MASK_COLOR)>>4)*m_BigCardSize.cy;
				}
				else
				{
					dwXImagePos=0;
					dwYImagePos=m_BigCardSize.cy*4;
				}

				//��Ļλ��
				if (m_bHorizontal==true) 
				{
					dwXScreenPos=m_dwCardHSpace*i+m_BenchmarkPos.x;
					dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude+m_BenchmarkPos.y:0+m_BenchmarkPos.y;
				}
				else
				{
					dwXScreenPos=0+m_BenchmarkPos.x;
					dwYScreenPos=m_dwCardVSpace*i+m_BenchmarkPos.y;
				}


				//�滭�˿�
				m_ImageBigCard.DrawImage(pDc,dwXScreenPos,dwYScreenPos,m_BigCardSize.cx,m_BigCardSize.cy,dwXImagePos,dwYImagePos);
			}
			if(m_DrawType<9)
			{
				m_CardType.DrawImage(pDc,m_BenchmarkPos.x+(dwXScreenPos+m_BigCardSize.cx-m_BenchmarkPos.x-m_CardType.GetWidth()/9)/2,
											m_BenchmarkPos.y+(m_BigCardSize.cy-m_CardType.GetHeight())/2,
											m_CardType.GetWidth()/9,m_CardType.GetHeight(),m_CardType.GetWidth()/9*m_DrawType,0);
			}
			else if(m_DrawType>=CT_THREE_STRAIGHT && m_DrawType<=CT_THIRTEEN_FLUSH)
			{
				int nWidth = m_CardSpecialType.GetWidth()/14;
				int nHeight = m_CardSpecialType.GetHeight();
				BYTE Type = -1;
				if(m_DrawType<=CT_FOUR_THREESAME)
					Type = m_DrawType-CT_THREE_STRAIGHT;
				else
					Type = m_DrawType-CT_THREE_STRAIGHT+1;
				m_CardSpecialType.DrawImage(pDc,m_BenchmarkPos.x+(dwXScreenPos+m_BigCardSize.cx-m_BenchmarkPos.x-nWidth)/2,
					m_BenchmarkPos.y+(m_BigCardSize.cy-nHeight)/2,
					nWidth,nHeight,nWidth*Type,0);
				nWidth = m_Ayaka.GetWidth()/7;
				nHeight = m_Ayaka.GetHeight();
				if(m_bAyaka)
				{
					m_Ayaka.DrawImage(pDc,m_BenchmarkPos.x+(dwXScreenPos+m_BigCardSize.cx-m_BenchmarkPos.x-nWidth)/2,
						m_BenchmarkPos.y+(m_BigCardSize.cy-nHeight)/2,
						nWidth,nHeight,nWidth*m_nAyakaFrames,0);
				}

			}
			else if(m_DrawType==CT_FIVE_MID_THREE_DEOUBLE)
			{
				m_MidThreeDeouble.DrawImage(pDc,m_BenchmarkPos.x+(dwXScreenPos+m_BigCardSize.cx-m_BenchmarkPos.x-m_MidThreeDeouble.GetWidth())/2,
				m_BenchmarkPos.y+(m_BigCardSize.cy-m_MidThreeDeouble.GetHeight())/2);
			}
		}
		else
		{
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
					dwXImagePos=0+m_CardSize.cx*2;
					dwYImagePos=m_CardSize.cy*4;
				}

				//��Ļλ��
				if (m_bHorizontal==true) 
				{
					dwXScreenPos=m_dwCardHSpace*i+m_BenchmarkPos.x;
					dwYScreenPos=(pCardItem->bShoot==false)?m_dwShootAltitude+m_BenchmarkPos.y:0+m_BenchmarkPos.y;
				}
				else
				{
					dwXScreenPos=0+m_BenchmarkPos.x;
					dwYScreenPos=m_dwCardVSpace*i+m_BenchmarkPos.y;
				}

				//�滭�˿�
				m_ImageCard.DrawImage(pDc,dwXScreenPos,dwYScreenPos,m_CardSize.cx,m_CardSize.cy,dwXImagePos,dwYImagePos);
			
			}

		}
	}
}
void CCardControl::ShuffleCardFlash(CDC* pDc,int nXPos,int nYPos)
{
	int CardWidth  = m_IShuffleCard.GetWidth()/9;
	int CardHeight = m_IShuffleCard.GetHeight();
	
	if(m_nShuffleNum>=0)
	{
		
		m_IShuffleCard.DrawImage(pDc,nXPos,nYPos,CardWidth,CardHeight,CardWidth*m_nShuffleNum,0);
		
	}


}
void CCardControl::SendCardFlash0(CDC* pDc,int nWidth,int nHeight)
{
	int j=nWidth-30;
	
	for(int i=0;i<m_nCardNum;i++)
	{
		m_ImageCard.DrawImage(pDc,j+=2,nHeight,m_CardSize.cx,m_CardSize.cy,m_CardSize.cx*2,m_CardSize.cy*4);
	}
	j=nWidth-30;
	
}
///<���ƶ���
void CCardControl::SendCardFlash(CDC* pDc,int nWidth,int nHeight)
{
	if(m_NumCard>=0&&m_NumCard<10)
	{
		m_ISendCard.DrawImage(pDc,nWidth+m_PosCard.cx,nHeight+m_PosCard.cy,m_SendCardFlash.cx,m_SendCardFlash.cy,m_SendCardFlash.cx*(float)m_NumCard,0);
	}
}

// BOOL CCardControl::PreTranslateMessage( MSG* pMsg )
// {
//     return true;
// }
// BEGIN_MESSAGE_MAP(CCardControl, CWnd)
// 	ON_BN_CLICKED(IDOK, OnBnClickedOk)
// 	ON_BN_CLICKED(IDC_ENABLE_SOUND, OnBnClickedEnableSound)
// END_MESSAGE_MAP()
// void CCardControl::OnBnClickedOk()
// {
//    
// }
// 
// void CCardControl::OnBnClickedEnableSound()
// {
//     
// }

void CCardControl::SetDrawCard( bool bDrawCard )
{
    m_bDrawCard=bDrawCard;
}

void CCardControl::LoadCardImage( CPngImage CardImage,CPngImage BigCardImage,CPngImage SendCard,CPngImage ShuffleCard )
{	
   m_IShuffleCard=ShuffleCard;
   m_ISendCard=SendCard;
   m_ImageCard=CardImage;
   m_ImageBigCard=BigCardImage;

}

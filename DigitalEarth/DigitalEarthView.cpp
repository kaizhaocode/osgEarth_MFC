// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// DigitalEarthView.cpp: CDigitalEarthView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DigitalEarth.h"
#endif

#include "DigitalEarthDoc.h"
#include "DigitalEarthView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDigitalEarthView

IMPLEMENT_DYNCREATE(CDigitalEarthView, CView)

BEGIN_MESSAGE_MAP(CDigitalEarthView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_COMMAND(ID_Setting, &CDigitalEarthView::OnSetting)
	ON_COMMAND(ID_ButtonFlyTo, &CDigitalEarthView::OnButtonflyto)
	ON_COMMAND(ID_log, &CDigitalEarthView::Onlog)
	ON_COMMAND(ID_lat, &CDigitalEarthView::Onlat)
	ON_COMMAND(ID_hei, &CDigitalEarthView::Onhei)
	ON_COMMAND(ID_PITCH, &CDigitalEarthView::OnPitch)
END_MESSAGE_MAP()

// CDigitalEarthView 构造/析构

CDigitalEarthView::CDigitalEarthView() noexcept
{
	// TODO: 在此处添加构造代码
	//初始化add
	mOSG = 0;
	mThreadHandle = 0;
}

CDigitalEarthView::~CDigitalEarthView()
{
}

BOOL CDigitalEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CDigitalEarthView 绘图

void CDigitalEarthView::OnDraw(CDC* /*pDC*/)
{
	CDigitalEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CDigitalEarthView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDigitalEarthView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	//theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDigitalEarthView 诊断

#ifdef _DEBUG
void CDigitalEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CDigitalEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDigitalEarthDoc* CDigitalEarthView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDigitalEarthDoc)));
	return (CDigitalEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CDigitalEarthView 消息处理程序


int CDigitalEarthView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//add
	mOSG = new COSGObject(m_hWnd);

	return 0;
}


BOOL CDigitalEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
		//add
	if (0 == mOSG)
	{
		return CView::OnEraseBkgnd(pDC);
	}
	else
	{
		return FALSE;
	}

	return CView::OnEraseBkgnd(pDC);
}


void CDigitalEarthView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//add
	if (mOSG != 0)delete mOSG;
	WaitForSingleObject(mThreadHandle,1000);

}


//启动函数
void CDigitalEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//add
	mOSG->InitOSG();
	//启动线程
	mThreadHandle = (HANDLE)_beginthread(&COSGObject::Render,0,mOSG);

}


void CDigitalEarthView::OnSetting()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(TEXT("本项目主要实现多架模型飞机模拟飞行,可以实时接收服务器端发送的经纬度坐标以及姿态角，实时调整飞机状态。"));
}


void CDigitalEarthView::OnButtonflyto()
{
	// TODO: 在此添加命令处理程序代码
	CDigitalEarthApp*pApp = (CDigitalEarthApp*)AfxGetApp();
	CMainFrame *pWnd = (CMainFrame *)pApp->GetMainWnd();
	{	//经度
		CMFCRibbonEdit *edit = dynamic_cast<CMFCRibbonEdit*>(pWnd->m_wndRibbonBar.FindByID(ID_log));
		if (edit)
		{
			CString str = edit->GetEditText();
			std::string strTemp=CT2A(str.GetBuffer());
			double log = std::atof(strTemp.c_str());
			if ((log > 180) || (log < -180))
			{
				MessageBox(TEXT("经度必须在-180~180之间"),TEXT("错误"),MB_OK|MB_ICONERROR);
				//double转CString
				str.Format(TEXT("%f"),flylog);
				edit->SetEditText(str);
				return;
			}
			else 
			{
				flylog = log;
			}

		}

	}

	{	//纬度
		CMFCRibbonEdit *edit = dynamic_cast<CMFCRibbonEdit*>(pWnd->m_wndRibbonBar.FindByID(ID_lat));
		if (edit)
		{
			CString str = edit->GetEditText();
			std::string strTemp = CT2A(str.GetBuffer());
			double lat = std::atof(strTemp.c_str());
			if ((lat > 90) || (lat < -90))
			{
				MessageBox(TEXT("经度必须在-90~90之间"), TEXT("错误"), MB_OK | MB_ICONERROR);
				//double转CString
				str.Format(TEXT("%f"), flylat);
				edit->SetEditText(str);
				return;
			}
			else
			{
				flylat = lat;
			}

		}

	}

	{	//高度
		CMFCRibbonEdit *edit = dynamic_cast<CMFCRibbonEdit*>(pWnd->m_wndRibbonBar.FindByID(ID_hei));
		if (edit)
		{
			CString str = edit->GetEditText();
			std::string strTemp = CT2A(str.GetBuffer());
			double hei = std::atof(strTemp.c_str());
			if (hei<0)
			{
				MessageBox(TEXT("高度必须为正"), TEXT("错误"), MB_OK | MB_ICONERROR);
				//double转CString
				str.Format(TEXT("%f"), flyhei);
				edit->SetEditText(str);
				return;
			}
			else
			{
				flyhei = hei;
			}

		}

	}

	{	//俯仰角
		CMFCRibbonEdit *edit = dynamic_cast<CMFCRibbonEdit*>(pWnd->m_wndRibbonBar.FindByID(ID_PITCH));
		if (edit)
		{
			CString str = edit->GetEditText();
			std::string strTemp = CT2A(str.GetBuffer());
			double pitch = std::atof(strTemp.c_str());
			if (pitch > 0||pitch<-90)
			{
				MessageBox(TEXT("俯仰角在-90~0之间"), TEXT("错误"), MB_OK | MB_ICONERROR);
				//double转CString
				str.Format(TEXT("%f"), pitch);
				edit->SetEditText(str);
				return;
			}
			else
			{
				flypitch = pitch;
			}

		}

	}

	mOSG->FlyTo(flylog,flylat,flyhei,flypitch);
}


void CDigitalEarthView::Onlog()
{
	// TODO: 在此添加命令处理程序代码
	  
}


void CDigitalEarthView::Onlat()
{
	// TODO: 在此添加命令处理程序代码
}


void CDigitalEarthView::Onhei()
{
	// TODO: 在此添加命令处理程序代码
}


void CDigitalEarthView::OnPitch()
{
	// TODO: 在此添加命令处理程序代码
}


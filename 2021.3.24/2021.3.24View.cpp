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

// 2021.3.24View.cpp: CMy2021324View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "2021.3.24.h"
#endif

#include "2021.3.24Doc.h"
#include "2021.3.24View.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy2021324View

IMPLEMENT_DYNCREATE(CMy2021324View, CView)

BEGIN_MESSAGE_MAP(CMy2021324View, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CMy2021324View 构造/析构

CMy2021324View::CMy2021324View() noexcept
{
	// TODO: 在此处添加构造代码
	mOSG = 0;
	mThreadHandle = 0;


}

CMy2021324View::~CMy2021324View()
{
}

BOOL CMy2021324View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMy2021324View 绘图

void CMy2021324View::OnDraw(CDC* /*pDC*/)
{
	CMy2021324Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CMy2021324View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMy2021324View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMy2021324View 诊断

#ifdef _DEBUG
void CMy2021324View::AssertValid() const
{
	CView::AssertValid();
}

void CMy2021324View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMy2021324Doc* CMy2021324View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMy2021324Doc)));
	return (CMy2021324Doc*)m_pDocument;
}
#endif //_DEBUG


// CMy2021324View 消息处理程序


int CMy2021324View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	mOSG = new COSGObject(m_hWnd);

	return 0;
}


BOOL CMy2021324View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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


void CMy2021324View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	if (mOSG != 0)
	{
		delete mOSG;
	}
	WaitForSingleObject(mThreadHandle,1000);
}


void CMy2021324View::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	mOSG->InitOSG();
	mThreadHandle = (HANDLE)_beginthread(&COSGObject::Render,0,mOSG);
}

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

// 2021.3.24View.h: CMy2021324View 类的接口
//

#pragma once

#include "2021.3.24Doc.h"
#include"COSGObject.h"


class CMy2021324View : public CView
{
protected: // 仅从序列化创建
	CMy2021324View() noexcept;
	DECLARE_DYNCREATE(CMy2021324View)

// 特性
public:
	CMy2021324Doc* GetDocument() const;

public:
	COSGObject*mOSG;
	HANDLE mThreadHandle;


// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CMy2021324View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // 2021.3.24View.cpp 中的调试版本
inline CMy2021324Doc* CMy2021324View::GetDocument() const
   { return reinterpret_cast<CMy2021324Doc*>(m_pDocument); }
#endif


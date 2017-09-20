
// TNView.h : CTNView ��Ľӿ�
//

#pragma once

#include "MFC_OSG.h"
#include "TNDoc.h"

class CTNView : public CView
{
protected: // �������л�����
    CTNView();
    DECLARE_DYNCREATE(CTNView)

    // ����
public:
    CTNDoc* GetDocument() const;

    //����osg����
    cOSG* mOSG;
    //�߳̾��
    HANDLE mThreadHandle;

    // �����
    
    // ����
public:

    bool CTNView::saveScene(CString path, int cfm);
    void CTNView::loadScene(CString filePath);

    CString getProjectionMatrixAsPerspective()
    {
        CString cstr;
        double fovy, aspectRatio, z1, z2;
        mOSG->getViewer()->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, z1, z2);
        //mOSG->getViewer()->getCamera()->setProjectionMatrixAsPerspective(fovy, abs(aspectRatio), z1, z2);
        cstr.Format(_T("%f, %f, %f, %f"), fovy, aspectRatio, z1, z2);
        return cstr;
    }

    void setNewModel(CString modelname)
    {
        mOSG->setNewModel(modelname);
    };

    // ��д
public:
    virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

    // ʵ��
public:
    virtual ~CTNView();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // ���ɵ���Ϣӳ�亯��
protected:
    afx_msg void OnFilePrintPreview();
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    DECLARE_MESSAGE_MAP()
public:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    virtual void OnInitialUpdate();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

    afx_msg void OnOsgPan();
    afx_msg void OnOsgOrbit();
    afx_msg void OnOsgSelect();

    afx_msg void OnOsgAddmd();
    afx_msg void OnOsgAddef();
    afx_msg void OnOsgRotate();
    afx_msg void OnOsgTrans();
    afx_msg void OnOsgScale();

    afx_msg void OnDestroy();
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnOsgAddlabel();
};

#ifndef _DEBUG  // TNView.cpp �еĵ��԰汾
inline CTNDoc* CTNView::GetDocument() const
{
    return reinterpret_cast<CTNDoc*>(m_pDocument);
}
#endif


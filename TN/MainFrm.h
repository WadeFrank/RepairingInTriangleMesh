
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "NodeView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

#include "TNApp.h"


class CMainFrame : public CFrameWndEx
{

protected: // �������л�����
    CMainFrame();
    DECLARE_DYNCREATE(CMainFrame)

    // ����
public:

    CString m_curLabel;

    // ����
public:

    // �ڵ���ͼ
    void clearNodeTree()
    {
        m_wndNodeView.clearNodeTree();
    };

    void insertRootNodeName(CString cstr);
    void insertNodeNameToRoot(CString cstr, UINT a, UINT b);
    void insertNameToParent(CString newName, CString parent);


    // ���Դ���
    void setModelProp(CString cstr);
    void SetManipulatorProperties(const float c1, const float c2, const float c3,
                                  const float q1, const float q2, const float q3, const float q4,
                                  const float d
                                  )
    {
        /*
        // _center
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(0)->SetValue(c1);
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(1)->SetValue(c2);
        m_wndProperties.m_wndPropList.GetProperty(2)->GetSubItem(2)->SetValue(c3);
        //_rotation
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(0)->SetValue(q1);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(1)->SetValue(q2);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(2)->SetValue(q3);
        m_wndProperties.m_wndPropList.GetProperty(3)->GetSubItem(3)->SetValue(q4);
        //_distance
        m_wndProperties.m_wndPropList.GetProperty(4)->GetSubItem(0)->SetValue(d);
        */
    };

    void setDebugInfo(CString cstr)
    {
        // debug
        m_wndProperties.m_wndPropList.GetProperty(5)->GetSubItem(0)
            ->SetValue(cstr.GetBuffer());
    };


    // �������
    void clearBuildString()
    {
        m_wndOutput.clearBuildString();
    };
    void addBuildString(CString cstr)
    {
        m_wndOutput.addBuildString(cstr);
    };
    void clearDebugdString()
    {
        m_wndOutput.clearDebugdString();
    };
    void addDebugString(CString cstr)
    {
        m_wndOutput.addDebugString(cstr);
    };
    void clearFindString()
    {
        m_wndOutput.clearFindString();
    };
    void addFindString(CString cstr)
    {
        m_wndOutput.addFindString(cstr);
    };

    // ��д
public:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);


    // ʵ��
public:
    virtual ~CMainFrame();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
    CMFCMenuBar       m_wndMenuBar;
    CMFCToolBar       m_wndToolBar;
    CMFCStatusBar     m_wndStatusBar;
    CMFCToolBarImages m_UserImages;
    CNodeView         m_wndNodeView;
    //CClassView        m_wndClassView;
    COutputWnd        m_wndOutput;
    CPropertiesWnd    m_wndProperties;

    // ���ɵ���Ϣӳ�亯��
protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnViewCustomize();
    afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
    afx_msg void OnApplicationLook(UINT id);
    afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    DECLARE_MESSAGE_MAP()

    BOOL CreateDockingWindows();
    void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
    afx_msg void OnUpdateOsgOrbit(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgPan(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgSelect(CCmdUI *pCmdUI);

    // ��Ϣ������
    // ���ýڵ���ͼ
    afx_msg LRESULT OnHandleUpdateNodeView(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnEditNodeView(WPARAM wParam, LPARAM lParam);
    // ��������
    afx_msg LRESULT OnHandleSetProp(WPARAM wParam, LPARAM lParam);
    // ���ģ��������Ϣ
    afx_msg LRESULT OnHandleAddLabelProp(WPARAM wParam, LPARAM lParam);
    // ���������ʾbuild��Ϣ
    afx_msg LRESULT OnHandleOutputBuildStr(WPARAM wParam, LPARAM lParam);

    afx_msg void OnUpdateOsgScale(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgAddmd(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgAddef(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgRotate(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgTrans(CCmdUI *pCmdUI);
    afx_msg void OnUpdateOsgAddlabel(CCmdUI *pCmdUI);
    void deleteLabel(CString curLabel);

    afx_msg LRESULT OnUserDbclklabel(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose();
    afx_msg void OnRectifyH();
};



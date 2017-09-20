
#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList ����

class COutputList : public CListBox
{
    // ����
public:
    COutputList();

    // ʵ��
public:
    virtual ~COutputList();

protected:
    afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnEditCopy();
    afx_msg void OnEditClear();
    afx_msg void OnViewOutput();

    DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane
{
    // ����
public:
    COutputWnd();

    void UpdateFonts();

    void clearBuildString()
    {
        m_wndOutputBuild.ResetContent();
    };
    void addBuildString(CString cstr)
    {
        m_wndOutputBuild.AddString(cstr.GetBuffer());
        m_wndOutputBuild.SetCurSel(m_wndOutputBuild.GetCount() - 1);
    };
    void clearDebugdString()
    {
        m_wndOutputDebug.ResetContent();
    };
    void addDebugString(CString cstr)
    {
        m_wndOutputDebug.AddString(cstr);
    };
    void clearFindString()
    {
        m_wndOutputFind.ResetContent();
    };
    void addFindString(CString cstr)
    {
        m_wndOutputFind.AddString(cstr);
    };

    // ����
protected:
    CMFCTabCtrl	m_wndTabs;

    COutputList m_wndOutputBuild;
    COutputList m_wndOutputDebug;
    COutputList m_wndOutputFind;

protected:
    void FillBuildWindow();
    void FillDebugWindow();
    void FillFindWindow();

    void AdjustHorzScroll(CListBox& wndListBox);

    // ʵ��
public:
    virtual ~COutputWnd();

protected:
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSize(UINT nType, int cx, int cy);

    DECLARE_MESSAGE_MAP()
};


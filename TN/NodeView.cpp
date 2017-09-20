
#include "stdafx.h"
#include "mainfrm.h"
#include "TNView.h"
#include "NodeView.h"
#include "Resource.h"
#include "TNApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeView

CNodeView::CNodeView()
{}

CNodeView::~CNodeView()
{}

BEGIN_MESSAGE_MAP(CNodeView, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_CONTEXTMENU()
    ON_COMMAND(ID_LOCATE_NODE, &CNodeView::OnLocateNode)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
    ON_COMMAND(ID_PROPERTIES, OnProperties)
    ON_WM_PAINT()
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar ��Ϣ�������

int CNodeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // ������ͼ: 
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

    if (!m_wndNodeViewTree.Create(dwViewStyle, rectDummy, this, 4))
    {
        TRACE0("δ�ܴ����ڵ���ͼ\n");
        return -1;      // δ�ܴ���
    }

    // ������ͼͼ��: 
    m_NodeViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));
    m_wndNodeViewTree.SetImageList(&m_NodeViewImages, TVSIL_NORMAL);

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
    m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE /* ������*/);

    OnChangeVisualStyle();

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

    m_wndToolBar.SetOwner(this);

    // �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    // ����һЩ��̬����ͼ����(�˴�ֻ������������룬�����Ǹ��ӵ�����)
    FillNodeView();
    AdjustLayout();

    return 0;
}

void CNodeView::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CNodeView::FillNodeView()
{
    HTREEITEM hRoot = m_wndNodeViewTree.InsertItem(_T("Root"), 0, 0);
    m_wndNodeViewTree.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

    /*HTREEITEM hSrc = m_wndNodeView.InsertItem(_T("FakeApp Դ�ļ�"), 0, 0, hRoot);

    m_wndNodeView.InsertItem(_T("FakeApp.cpp"), 1, 1, hSrc);

    HTREEITEM hInc = m_wndNodeView.InsertItem(_T("FakeApp ͷ�ļ�"), 0, 0, hRoot);

    m_wndNodeView.InsertItem(_T("FakeApp.h"), 2, 2, hInc);

    HTREEITEM hRes = m_wndNodeView.InsertItem(_T("FakeApp ��Դ�ļ�"), 0, 0, hRoot);

    m_wndNodeView.InsertItem(_T("FakeApp.ico"), 2, 2, hRes);
    */

}

void CNodeView::OnContextMenu(CWnd* pWnd, CPoint point)
{
    CTreeCtrl* pWndTree = (CTreeCtrl*)&m_wndNodeViewTree;
    ASSERT_VALID(pWndTree);

    if (pWnd != pWndTree)
    {
        CDockablePane::OnContextMenu(pWnd, point);
        return;
    }

    if (point != CPoint(-1, -1))
    {
        // ѡ���ѵ�������: 
        CPoint ptTree = point;
        pWndTree->ScreenToClient(&ptTree);

        UINT flags = 0;
        HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
        if (hTreeItem != NULL)
        {
            pWndTree->SelectItem(hTreeItem);
        }
    }

    pWndTree->SetFocus();
    theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);
}

void CNodeView::AdjustLayout()
{
    if (GetSafeHwnd() == NULL)
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndNodeViewTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + cyTlb + 1, rectClient.Width() - 2, rectClient.Height() - cyTlb - 2, SWP_NOACTIVATE | SWP_NOZORDER);
}


void CNodeView::OnPaint()
{
    CPaintDC dc(this); // ���ڻ��Ƶ��豸������

    CRect rectTree;
    m_wndNodeViewTree.GetWindowRect(rectTree);
    ScreenToClient(rectTree);

    rectTree.InflateRect(1, 1);
    dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void CNodeView::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);

    m_wndNodeViewTree.SetFocus();
}

void CNodeView::OnChangeVisualStyle()
{
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* ����*/);

    m_NodeViewImages.DeleteImageList();

    UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

    CBitmap bmp;
    if (!bmp.LoadBitmap(uiBmpId))
    {
        TRACE(_T("�޷�����λͼ:  %x\n"), uiBmpId);
        ASSERT(FALSE);
        return;
    }

    BITMAP bmpObj;
    bmp.GetBitmap(&bmpObj);

    UINT nFlags = ILC_MASK;

    nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

    m_NodeViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
    m_NodeViewImages.Add(&bmp, RGB(255, 0, 255));

    m_wndNodeViewTree.SetImageList(&m_NodeViewImages, TVSIL_NORMAL);
}

void CNodeView::insertItemToParent(CString newName, CString parent)
{
    HTREEITEM hRoot = m_wndNodeViewTree.GetRootItem();
    // ���Ҹ��ڵ�
    HTREEITEM hParent = m_wndNodeViewTree.findItem(hRoot, parent);
    if (hParent != NULL)
    {
        // ���뵽���ڵ�
        m_wndNodeViewTree.InsertItem(newName.GetBuffer(), 2, 2, hParent);
        m_wndNodeViewTree.Expand(hParent, TVE_EXPAND);
    }
    else
    {
        insertItem(newName, 2, 2, true);
    }
}

void CNodeView::editItem(CString target, CString text)
{
    HTREEITEM hTarget = findItem(target);
    if (hTarget != NULL)
    {
        // �༭Ŀ��ڵ�
        m_wndNodeViewTree.SetItemText(hTarget,text.GetBuffer());
        m_wndNodeViewTree.Expand(hTarget, TVE_EXPAND);
    }
    else
    {      
        ASSERT(0);
    }
}

void CNodeView::deleteItem(CString curLabel)
{    
    HTREEITEM hTarget = findItem(curLabel);
    if (hTarget != NULL)
    {
        // TODO:�ӳ���ɾ����Ӧ�ڵ㼰���ӽڵ�

        m_wndNodeViewTree.DeleteItem(hTarget);        
    }
}


HTREEITEM CNodeView::findItem(CString curLabel)
{
    HTREEITEM hRoot = m_wndNodeViewTree.GetRootItem();
    // ����Ŀ��ڵ�
    return m_wndNodeViewTree.findItem(hRoot, curLabel);
};



/////////////////////////////// �Ҽ��˵����� ///////////////////////////////////

void CNodeView::OnLocateNode()
{
    // ��λ����ѡ�ڵ�λ��
    HTREEITEM hTreeItem = m_wndNodeViewTree.GetSelectedItem();
    CString cstr = m_wndNodeViewTree.GetItemText(hTreeItem);
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNView* pView = (CTNView*)pFrame->GetActiveView();
    wchar_t* ptemp = cstr.GetBuffer(0);
    //std::string str(ptemp);
    //pView->mOSG->moveCameratoNode(str);
}


void CNodeView::OnEditCut()
{
    // TODO:  ���нڵ���Ϣ
}

void CNodeView::OnEditCopy()
{
    // TODO:  ���ƽڵ���Ϣ
}

void CNodeView::OnEditClear()
{
    // �ӳ���������ڵ���Ϣ
    HTREEITEM hTreeItem = m_wndNodeViewTree.GetSelectedItem();
    CString cstr = m_wndNodeViewTree.GetItemText(hTreeItem);
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    pFrame->deleteLabel(cstr);
}

void CNodeView::OnProperties()
{
    // ��ʾ��ѡ�ڵ�������Ϣ
    HTREEITEM hTreeItem = m_wndNodeViewTree.GetSelectedItem();
    CString cstr = m_wndNodeViewTree.GetItemText(hTreeItem);
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    pFrame->OnUserDbclklabel(NULL, (LPARAM)cstr.GetBuffer(cstr.GetAllocLength()));

}
//////////////////////////////////////////////////////////////////////////
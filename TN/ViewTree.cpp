
#include "stdafx.h"
#include "ViewTree.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree()
{}

CViewTree::~CViewTree()
{}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
    ON_NOTIFY_REFLECT(NM_DBLCLK, &CViewTree::OnNMDblclk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree ��Ϣ�������

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

    NMHDR* pNMHDR = (NMHDR*)lParam;
    ASSERT(pNMHDR != NULL);

    if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
    {
        GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
    }

    return bRes;
}

HTREEITEM CViewTree::findItem(HTREEITEM item, CString strtext)
{
    HTREEITEM hfind;
    //������ֱ�ӷ���NULL
    if (item == NULL)
        return NULL;

    //��������
    while (item != NULL)
    {
        //��ǰ�ڵ㼴������ҽڵ�
        if (GetItemText(item) == strtext)
            return item;
        
        //���ҵ�ǰ�ڵ���ӽڵ�
        if (ItemHasChildren(item))
        {
            item = GetChildItem(item);
            //�ݹ���ò����ӽڵ��½ڵ�
            hfind = findItem(item, strtext);
            if (hfind)
            {
                return hfind;
            }
            else //�ӽڵ���δ��������ڵ㣬���������ֵܽڵ�
                item = GetNextSiblingItem(GetParentItem(item));
        }
        else
        { //�����ӽڵ㣬���������ֵܽڵ�
            item = GetNextSiblingItem(item);
        }
    }
    return item;
}

void CViewTree::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
    HTREEITEM hTreeItem = GetSelectedItem();
    //��GetItemText()����Ϊ����   
    CString cstr = GetItemText(hTreeItem);
    // AfxMessageBox(L"You have selected " + cstr);
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    pFrame->OnUserDbclklabel(NULL, (LPARAM)cstr.GetBuffer(cstr.GetAllocLength()));
    *pResult = 0;
}
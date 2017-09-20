
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "TNApp.h"
#include "TNDoc.h"
#include "TNView.h"

#include "MainFrm.h"

#include <string>
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
    ON_WM_CREATE()
    ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
    ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
    ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
    ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
    ON_WM_SETTINGCHANGE()

    ON_UPDATE_COMMAND_UI(ID_OSG_ORBIT, &CMainFrame::OnUpdateOsgOrbit)
    ON_UPDATE_COMMAND_UI(ID_OSG_PAN, &CMainFrame::OnUpdateOsgPan)
    ON_UPDATE_COMMAND_UI(ID_OSG_SELECT, &CMainFrame::OnUpdateOsgSelect)
    ON_UPDATE_COMMAND_UI(ID_OSG_SCALE, &CMainFrame::OnUpdateOsgScale)

    ON_MESSAGE(WM_USER_THREADEND, &CMainFrame::OnHandleOutputBuildStr)
    ON_MESSAGE(WM_USER_PROP, &CMainFrame::OnHandleSetProp)
    ON_MESSAGE(WM_USER_ADDMODELNAME, &CMainFrame::OnHandleUpdateNodeView)
    ON_MESSAGE(WM_USER_EDITMODELNAME, &CMainFrame::OnEditNodeView)
    ON_MESSAGE(WM_USER_EDITMODELLABEL, &CMainFrame::OnHandleAddLabelProp)
    ON_MESSAGE(WM_USER_DBCLKLABEL, &CMainFrame::OnUserDbclklabel)

    ON_UPDATE_COMMAND_UI(ID_OSG_ADDMD, &CMainFrame::OnUpdateOsgAddmd)
    ON_UPDATE_COMMAND_UI(ID_OSG_ADDEF, &CMainFrame::OnUpdateOsgAddef)
    ON_UPDATE_COMMAND_UI(ID_OSG_ROTATE, &CMainFrame::OnUpdateOsgRotate)
    ON_UPDATE_COMMAND_UI(ID_OSG_TRANS, &CMainFrame::OnUpdateOsgTrans)
    ON_UPDATE_COMMAND_UI(ID_OSG_ADDLABEL, &CMainFrame::OnUpdateOsgAddlabel)
    ON_WM_CLOSE()
    ON_COMMAND(ID_RECTIFY_H, &CMainFrame::OnRectifyH)
END_MESSAGE_MAP()

static UINT indicators[] =
{
    ID_SEPARATOR,           // ״̬��ָʾ��
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{

    theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
}

CMainFrame::~CMainFrame()
{}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
        return -1;
    

    BOOL bNameValid;

    if (!m_wndMenuBar.Create(this))
    {
        TRACE0("δ�ܴ����˵���\n");
        return -1;      // δ�ܴ���
    }

    m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

    // ��ֹ�˵����ڼ���ʱ��ý���
    CMFCPopupMenu::SetForceMenuFocus(FALSE);

    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
    {
        TRACE0("δ�ܴ���������\n");
        return -1;      // δ�ܴ���
    }

    CString strToolBarName;
    bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
    ASSERT(bNameValid);
    m_wndToolBar.SetWindowText(strToolBarName);

    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);
    m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

    // �����û�����Ĺ���������: 
    InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

    if (!m_wndStatusBar.Create(this))
    {
        TRACE0("δ�ܴ���״̬��\n");
        return -1;      // δ�ܴ���
    }
    m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

    // TODO:  �������ϣ���������Ͳ˵�����ͣ������ɾ��������
    m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndMenuBar);
    DockPane(&m_wndToolBar);


    // ���� Visual Studio 2005 ��ʽͣ��������Ϊ
    CDockingManager::SetDockingMode(DT_SMART);
    // ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
    EnableAutoHidePanes(CBRS_ALIGN_ANY);

    // ���ز˵���ͼ��(�����κα�׼��������): 
    CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

    // ����ͣ������
    if (!CreateDockingWindows())
    {
        TRACE0("δ�ܴ���ͣ������\n");
        return -1;
    }

    m_wndNodeView.EnableDocking(CBRS_ALIGN_ANY);
    // m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndNodeView);
    //CDockablePane* pTabbedBar = NULL;
    //m_wndClassView.AttachToTabWnd(&m_wndNodeView, DM_SHOW, FALSE, &pTabbedBar);
    m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
    m_wndOutput.ShowPane(FALSE, FALSE, FALSE);
    DockPane(&m_wndOutput);
    m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
    DockPane(&m_wndProperties);
    m_wndProperties.ShowPane(FALSE, FALSE, FALSE);

    // ���ڳ־�ֵ�����Ӿ�����������ʽ
    OnApplicationLook(theApp.m_nAppLook);

    // ���ù�������ͣ�����ڲ˵��滻
    EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

    // ���ÿ���(��ס Alt �϶�)�������Զ���
    CMFCToolBar::EnableQuickCustomization();

    if (CMFCToolBar::GetUserImages() == NULL)
    {
        // �����û�����Ĺ�����ͼ��
        if (m_UserImages.Load(_T(".\\UserImages.bmp")))
        {
            CMFCToolBar::SetUserImages(&m_UserImages);
        }
    }

    //// ���ò˵����Ի�(���ʹ�õ�����)
    //// TODO:  �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
    //CList<UINT, UINT> lstBasicCommands;

    //lstBasicCommands.AddTail(ID_FILE_NEW);
    //lstBasicCommands.AddTail(ID_FILE_OPEN);
    //lstBasicCommands.AddTail(ID_FILE_SAVE);
    //lstBasicCommands.AddTail(ID_FILE_PRINT);
    //lstBasicCommands.AddTail(ID_APP_EXIT);
    //lstBasicCommands.AddTail(ID_EDIT_CUT);
    //lstBasicCommands.AddTail(ID_EDIT_PASTE);
    //lstBasicCommands.AddTail(ID_EDIT_UNDO);
    //lstBasicCommands.AddTail(ID_APP_ABOUT);
    //lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
    //lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
    //lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
    //lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
    //lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
    //lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
    //lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

    //CMFCToolBar::SetBasicCommands(lstBasicCommands);

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if (!CFrameWndEx::PreCreateWindow(cs))
        return FALSE;
    // TODO:  �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ

    return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
    BOOL bNameValid;

    // ��������ͼ
    //CString strClassView;
    //bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
    //ASSERT(bNameValid);
    //if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    //{
    //    TRACE0("δ�ܴ���������ͼ������\n");
    //    return FALSE; // δ�ܴ���
    //}

    // �����ڵ���ͼ
    CString strFileView;
    bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
    ASSERT(bNameValid);
    if (!m_wndNodeView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ������ڵ���ͼ������\n");
        return FALSE; // δ�ܴ���
    }

    // �����������
    CString strOutputWnd;
    bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
    ASSERT(bNameValid);
    if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ����������\n");
        return FALSE; // δ�ܴ���
    }

    // �������Դ���
    CString strPropertiesWnd;
    bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
    ASSERT(bNameValid);
    if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
    {
        TRACE0("δ�ܴ��������ԡ�����\n");
        return FALSE; // δ�ܴ���
    }

    SetDockingWindowIcons(theApp.m_bHiColorIcons);
    return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
    HICON hNodeViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndNodeView.SetIcon(hNodeViewIcon, FALSE);

    //HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    //m_wndClassView.SetIcon(hClassViewIcon, FALSE);

    HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

    HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
    m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnViewCustomize()
{
    CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
    pDlgCust->EnableUserDefinedToolbars();
    pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp, LPARAM lp)
{
    LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp, lp);
    if (lres == 0)
    {
        return 0;
    }

    CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
    ASSERT_VALID(pUserToolbar);

    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
    return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
    CWaitCursor wait;

    theApp.m_nAppLook = id;

    switch (theApp.m_nAppLook)
    {
    case ID_VIEW_APPLOOK_WIN_2000:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
        break;

    case ID_VIEW_APPLOOK_OFF_XP:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
        break;

    case ID_VIEW_APPLOOK_WIN_XP:
        CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
        break;

    case ID_VIEW_APPLOOK_OFF_2003:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    case ID_VIEW_APPLOOK_VS_2005:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    case ID_VIEW_APPLOOK_VS_2008:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    case ID_VIEW_APPLOOK_WINDOWS_7:
        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
        CDockingManager::SetDockingMode(DT_SMART);
        break;

    default:
        switch (theApp.m_nAppLook)
        {
        case ID_VIEW_APPLOOK_OFF_2007_BLUE:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_BLACK:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_SILVER:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
            break;

        case ID_VIEW_APPLOOK_OFF_2007_AQUA:
            CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
            break;
        }

        CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
        CDockingManager::SetDockingMode(DT_SMART);
    }

    m_wndOutput.UpdateFonts();
    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

    theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
    pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext)
{
    // ���ཫִ�������Ĺ���

    if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
    {
        return FALSE;
    }


    // Ϊ�����û������������Զ��尴ť
    BOOL bNameValid;
    CString strCustomize;
    bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
    ASSERT(bNameValid);

    for (int i = 0; i < iMaxUserToolbars; i++)
    {
        CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
        if (pUserToolbar != NULL)
        {
            pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
        }
    }

    return TRUE;
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CFrameWndEx::OnSettingChange(uFlags, lpszSection);
    m_wndOutput.UpdateFonts();
}


void CMainFrame::OnUpdateOsgOrbit(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int naviMode = app->naviMode;

    pCmdUI->SetCheck(naviMode == NAVI_MODE_ORBIT);

}


void CMainFrame::OnUpdateOsgPan(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int naviMode = app->naviMode;

    pCmdUI->SetCheck(naviMode == NAVI_MODE_PAN);
}


void CMainFrame::OnUpdateOsgSelect(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int naviMode = app->naviMode;

    pCmdUI->SetCheck(naviMode == NAVI_MODE_SELECT);
}


afx_msg LRESULT CMainFrame::OnHandleOutputBuildStr(WPARAM wParam, LPARAM lParam)
{
    // clearBuildString();
    CString str;
    str.Format(_T("%s"), (lParam));
    addBuildString(str);
    return 0;
}

// ���ýڵ���ͼ
afx_msg LRESULT CMainFrame::OnHandleUpdateNodeView(WPARAM wParam, LPARAM lParam)
{
    CString cstr;
    cstr.Format(_T("%s"), (lParam));

    bool toRoot = wParam == NULL;
    if (toRoot)
    {
        insertNodeNameToRoot(cstr, 2, 2);
    }
    else
    {
        CString parent = cstr, newName;
        cstr.Format(_T("%s"), (wParam));
        newName = cstr;
        m_wndNodeView.insertItemToParent(newName, parent);
    }
    return 0;
}

afx_msg LRESULT CMainFrame::OnEditNodeView(WPARAM wParam, LPARAM lParam)
{
    CString cstr;
    cstr.Format(_T("%s"), (lParam));
    CString target = cstr, text;
    cstr.Format(_T("%s"), (wParam));
    text = cstr;
    m_wndNodeView.editItem(target, text);

    return 0;
}

afx_msg LRESULT CMainFrame::OnHandleSetProp(WPARAM wParam, LPARAM lParam)
{
    CString str;
    str.Format(_T("%s"), (lParam));
    setModelProp(str);
    // addBuildString(str);
    return 0;
}

void CMainFrame::setModelProp(CString cstr)
{
    CString modelName = cstr.Left(cstr.Find('$'))
        , propStr = cstr.Right(cstr.GetLength() - cstr.Find('$') - 1)
        , tstr, pistr;
    int propNum = cstr.Replace(_T(","), _T(",")) + 1
        , curPID = 0;

    // addBuildString(modelName);

    CString propValue[10] = { 0 };
    tstr = propStr;
    int i;
    for (i = 0; i < propNum - 1; i++)
    {
        pistr = tstr.Left(tstr.Find(_T(',')));
        propValue[i] = pistr;
        tstr = tstr.Right(tstr.GetLength() - tstr.Find(',') - 1);
    }
    propValue[i] = tstr;

    int propListPropNum = m_wndProperties.m_wndPropList.GetPropertyCount();
    //tstr.Format(_T("�������� %d ��"), propListPropNum);//5
    //addBuildString(tstr);

    CString pnamestr;
    bool existFlag = false;
    for (int i = 0; i < propListPropNum; i++)
    {
        pnamestr = m_wndProperties.m_wndPropList.GetProperty(i)->GetName();
        curPID += m_wndProperties.m_wndPropList.GetProperty(i)->GetSubItemsCount();
        if (pnamestr == "ѡ��ģ��")
        {
            if (cstr == "")
            {
                // �����ѡ��ģ�͡�����
                CMFCPropertyGridProperty* pModelGroup = m_wndProperties.m_wndPropList.GetProperty(i);
                m_wndProperties.m_wndPropList.DeleteProperty(pModelGroup);
                existFlag = false;
                m_wndProperties.ShowPane(FALSE, FALSE, FALSE);
            }
            else
            {
                existFlag = true;
                m_wndProperties.m_wndPropList.GetProperty(i)
                    ->GetSubItem(0)->SetValue(
                    modelName.Left(modelName.ReverseFind('.')));

                m_wndProperties.m_wndPropList.GetProperty(i)
                    ->GetSubItem(1)->SetValue(
                    modelName.Right(modelName.GetLength() - modelName.ReverseFind('.') - 1));

                m_wndProperties.m_wndPropList.GetProperty(i)
                    ->GetSubItem(2)->SetValue(propValue[0]);

                m_wndProperties.m_wndPropList.GetProperty(i)
                    ->GetSubItem(3)->SetValue(propValue[1]);

                m_wndProperties.m_wndPropList.GetProperty(i)
                    ->GetSubItem(4)->SetValue(propValue[2]);
                m_wndProperties.ShowPane(TRUE, FALSE, FALSE);
            }
            break;
        }
    }
    if (cstr != "" && !existFlag)
    {
        m_wndProperties.selModelNamePID = curPID;
        CMFCPropertyGridProperty* pModelGroup = new CMFCPropertyGridProperty(
            _T("ѡ��ģ��"));
        CMFCPropertyGridProperty* pModelProp = new CMFCPropertyGridProperty(
            _T("����"), modelName.Left(modelName.ReverseFind('.'))
            , _T(""), curPID++);
        pModelProp->AllowEdit(true);
        pModelGroup->AddSubItem(pModelProp);

        pModelProp = new CMFCPropertyGridProperty(
            _T("����"), modelName.Right(modelName.GetLength() - modelName.ReverseFind('.') - 1)
            , _T(""), curPID++);
        pModelProp->AllowEdit(FALSE);
        pModelGroup->AddSubItem(pModelProp);

        pModelProp = new CMFCPropertyGridProperty(
            _T("����x"), propValue[0], _T(""), curPID++);
        pModelProp->AllowEdit(FALSE);
        pModelGroup->AddSubItem(pModelProp);

        pModelProp = new CMFCPropertyGridProperty(
            _T("����y"), propValue[1], _T(""), curPID++);
        pModelProp->AllowEdit(FALSE);
        pModelGroup->AddSubItem(pModelProp);

        pModelProp = new CMFCPropertyGridProperty(
            _T("����z"), propValue[2], _T(""), curPID++);
        pModelProp->AllowEdit(FALSE);
        pModelGroup->AddSubItem(pModelProp);
        m_wndProperties.m_wndPropList.AddProperty(pModelGroup);
        m_wndProperties.ShowPane(TRUE, FALSE, FALSE);
    }
}

afx_msg LRESULT CMainFrame::OnHandleAddLabelProp(WPARAM wParam, LPARAM lParam)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
    CString path = doc->m_datapath, propIni = doc->m_inipath;

    CString cstrLabel, cstrValue, cstrPropName, cstrPropValue;
    cstrLabel.Format(_T("%s"), (wParam));// ��ǩ��
    if (wParam == NULL)
    {
        m_curLabel = "";
        int propListPropNum = m_wndProperties.m_wndPropList.GetPropertyCount();
        CString pnamestr;
        int labelPropGroupID = 0;
        bool existFlag = false;
        for (int i = 0; i < propListPropNum; i++)
        {
            pnamestr = m_wndProperties.m_wndPropList.GetProperty(i)->GetName();
            if (pnamestr == "��ǩ")
            {
                existFlag = true;
                labelPropGroupID = i;
                break;
            }
        }
        if (existFlag)
        {
            // ������С���ǩ���������µ�����
            CMFCPropertyGridProperty* pModelGroup
                = m_wndProperties.m_wndPropList.GetProperty(labelPropGroupID);
            m_wndProperties.m_wndPropList.DeleteProperty(pModelGroup);
        }
        m_wndProperties.ShowPane(FALSE, FALSE, FALSE);
        return 0;
    }
    else if (lParam == NULL)
    {
        // �鿴����
        m_curLabel = cstrLabel;
        int propListPropNum = m_wndProperties.m_wndPropList.GetPropertyCount();
        CString pnamestr;
        int labelPropGroupID = 0;
        bool existFlag = false;
        for (int i = 0; i < propListPropNum; i++)
        {
            pnamestr = m_wndProperties.m_wndPropList.GetProperty(i)->GetName();
            if (pnamestr == "��ǩ")
            {
                existFlag = true;
                labelPropGroupID = i;
                break;
            }
        }
        CMFCPropertyGridProperty* pModelGroup;
        if (existFlag)
        {
            // ������С���ǩ���������µ�����
            pModelGroup = m_wndProperties.m_wndPropList.GetProperty(labelPropGroupID);
            m_wndProperties.m_wndPropList.DeleteProperty(pModelGroup);
        }
        pModelGroup = new CMFCPropertyGridProperty(L"��ǩ");
        m_wndProperties.m_wndPropList.AddProperty(pModelGroup);
        int curPID = labelPropGroupID * 100;
        CMFCPropertyGridProperty* pModelProp = new CMFCPropertyGridProperty(
            L"����", cstrLabel, cstrLabel + L"\r\n(��ǩ��Ψһ��ʶ��)", curPID++);
        pModelProp->AllowEdit(true);
        pModelGroup->AddSubItem(pModelProp);

        if (::PathFileExists(propIni))
        {
            propListPropNum = m_wndProperties.m_wndPropList.GetPropertyCount();
            for (int i = 0; i < propListPropNum; i++)
            {
                pnamestr = m_wndProperties.m_wndPropList.GetProperty(i)->GetName();
                if (pnamestr == "��ǩ")
                {
                    labelPropGroupID = i;
                    break;
                }
            }
            CString tcstr;
            TCHAR szKeyValue[MAX_PROP_LEN] = { 0 };
            int nValue = 0; // ������Ŀ
            nValue = ::GetPrivateProfileInt(
                cstrLabel, TEXT("PropNum"), 0, propIni);
            for (int i = 0; i < nValue; i++)
            {
                // ��ȡ����
                tcstr.Format(L"PropName%d",i);
                ::GetPrivateProfileString(cstrLabel,
                                          tcstr, NULL,
                                          szKeyValue, MAX_PROP_LEN,
                                          propIni);
                cstrPropName = szKeyValue;
                tcstr.Format(L"PropValue%d", i);
                ::GetPrivateProfileString(cstrLabel,
                                          tcstr, NULL,
                                          szKeyValue, MAX_PROP_LEN,
                                          propIni);
                cstrPropValue = szKeyValue;
                // ��ӵ����Դ���
                cstrPropValue.Replace(L"\\r\\n", L"\r\n");
                pModelProp = new CMFCPropertyGridProperty(
                    cstrPropName, cstrPropValue, cstrPropValue, curPID++);
                pModelProp->AllowEdit(true);
                pModelGroup->AddSubItem(pModelProp);
            }
        }
        m_wndProperties.OnExpandAllProperties();
        m_wndProperties.ShowPane(TRUE, FALSE, FALSE);
    }
    else
    {
        // ��ӱ༭��ǩcstrLabel���ԣ��޸�Label��������ֵ
        cstrValue.Format(_T("%s"), (lParam));// ����ֵ��
        CString cstrLabelName;
        if (cstrValue.Find(L"$", 0) > -1)
        {
            cstrLabelName = cstrValue.Left(cstrValue.Find(L"$", 0));
        }
        else
        {
            cstrLabelName = cstrValue;
        }
        if (cstrLabelName != cstrLabel)
        {
            m_curLabel = cstrLabelName;
            // �޸Ľڵ���ͼ�е�Label��    
            m_wndNodeView.editItem(cstrLabel, cstrLabelName);
            // NameSet�е�Label��    
            CTNApp *app = (CTNApp *)AfxGetApp();
            app->nodeNameSet.erase(app->nodeNameSet.find(cstrLabel));
            app->insertNodeName2(cstrLabelName);
        }
        m_wndProperties.OnExpandAllProperties();
        m_wndProperties.ShowPane(TRUE, FALSE, FALSE);
    }

    return 0;
}

void CMainFrame::OnUpdateOsgAddmd(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_ADD_MODEL);
}


void CMainFrame::OnUpdateOsgAddlabel(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_ADD_LABEL);
}


void CMainFrame::OnUpdateOsgAddef(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_ADD_FIRE);
}


void CMainFrame::OnUpdateOsgRotate(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_ROTATE);
}


void CMainFrame::OnUpdateOsgTrans(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_TRANS);
}

void CMainFrame::OnUpdateOsgScale(CCmdUI *pCmdUI)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int editMode = app->editMode;

    pCmdUI->SetCheck(editMode == EDIT_MODE_SCALE);
}

// ---------------- �ڵ���ͼ -------------------
void CMainFrame::insertRootNodeName(CString cstr)
{
    //������ڵ�  
    m_wndNodeView.clearNodeTree();
    HTREEITEM hRoot = m_wndNodeView.insertItem(cstr, 0, 0);
    m_wndNodeView.setItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
    m_wndNodeView.expand(hRoot, TVE_EXPAND);
    hRoot = NULL;
}
void CMainFrame::insertNodeNameToRoot(CString cstr, UINT a, UINT b)
{
    //���뵽���ڵ�  
    m_wndNodeView.insertItem(cstr, a, b, true);
}


void CMainFrame::insertNameToParent(CString newName, CString parent)
{
    //���뵽ָ�����ڵ�  
    m_wndNodeView.insertItemToParent(newName, parent);
}

void CMainFrame::deleteLabel(CString curLabel)
{
    m_wndNodeView.deleteItem(curLabel);

    CTNApp *app = (CTNApp *)AfxGetApp();
    app->nodeNameSet.erase(app->nodeNameSet.find(curLabel));

    CTNView* pView = (CTNView*)this->GetActiveView();
    pView->mOSG->deleteLabel(curLabel);
}


afx_msg LRESULT CMainFrame::OnUserDbclklabel(WPARAM wParam, LPARAM lParam)
{
    CString cstrLabel;
    cstrLabel.Format(_T("%s"), (lParam));// ˫���ı�ǩ����
    
    // AfxMessageBox(L"You have selected " + cstr);
    CTNView* pView = (CTNView*)this->GetActiveView();
    pView->mOSG->moveCameratoLabel(cstrLabel);
    OnHandleAddLabelProp(WPARAM(cstrLabel.GetBuffer(cstrLabel.GetAllocLength())), (LPARAM)NULL);
    return 0;
}


void CMainFrame::OnClose()
{
    if (::MessageBox(NULL, L"ȷ���˳�����",
        L"��ʾ", MB_YESNO) == IDYES)
    {
        CFrameWndEx::OnClose();
    }
}


void CMainFrame::OnRectifyH()
{
    // TODO:  �ڴ���������������
    CTNView* pView = (CTNView*)this->GetActiveView();
    pView->mOSG->RectifyH();
}

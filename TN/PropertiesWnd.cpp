
#include "stdafx.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "TNView.h"
#include "TNApp.h"

#include "LabelEditor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
    m_nComboHeight = 0;
    selModelNamePID = -1;
}

CPropertiesWnd::~CPropertiesWnd()
{}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
    ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
    ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
    ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
    ON_COMMAND(ID_LABEL_ADDPROP, OnAddLabelProperties)
    ON_UPDATE_COMMAND_UI(ID_LABEL_ADDPROP, OnUpdateLabelProperties)
    ON_COMMAND(ID_PROPERTIES2, OnProperties2)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
    ON_WM_SETFOCUS()
    ON_WM_SETTINGCHANGE()
    ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, &CPropertiesWnd::OnPropertyChanged)
    ON_COMMAND(ID_PROPERTIES3, &CPropertiesWnd::OnProperties3)
    ON_UPDATE_COMMAND_UI(ID_PROPERTIES3, &CPropertiesWnd::OnUpdateProperties3)
    ON_WM_LBUTTONDBLCLK()
    ON_COMMAND(ID_LABEL_EDITPROP, &CPropertiesWnd::OnLabelEditprop)
    ON_COMMAND(ID_LABEL_DEL, &CPropertiesWnd::OnLabelDel)
    ON_COMMAND(ID_LABEL_DELPROP, &CPropertiesWnd::OnLabelDelprop)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar ��Ϣ�������

void CPropertiesWnd::AdjustLayout()
{
    if (GetSafeHwnd() == NULL || (AfxGetMainWnd() != NULL && AfxGetMainWnd()->IsIconic()))
    {
        return;
    }

    CRect rectClient;
    GetClientRect(rectClient);

    int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

    m_wndObjectCombo.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
    m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + m_nComboHeight + cyTlb, rectClient.Width(), rectClient.Height() - (m_nComboHeight + cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    CRect rectDummy;
    rectDummy.SetRectEmpty();

    // �������: 
    const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

    if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
    {
        TRACE0("δ�ܴ���������� \n");
        return -1;      // δ�ܴ���
    }

    m_wndObjectCombo.AddString(_T("Ӧ�ó���"));
    m_wndObjectCombo.AddString(_T("���Դ���"));
    m_wndObjectCombo.SetCurSel(0);

    CRect rectCombo;
    m_wndObjectCombo.GetClientRect(&rectCombo);

    m_nComboHeight = rectCombo.Height();

    if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
    {
        TRACE0("δ�ܴ�����������\n");
        return -1;      // δ�ܴ���
    }

    InitPropList();

    m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
    m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* ������*/);
    m_wndToolBar.CleanUpLockedImages();
    m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ?
                        IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* ����*/);

    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
    m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() &
                              ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
    m_wndToolBar.SetOwner(this);

    // �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��: 
    m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

    AdjustLayout();
    return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);
    AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
    m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{

}

void CPropertiesWnd::OnSortProperties()
{
    m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
    pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

LRESULT CPropertiesWnd::OnPropertyChanged(__in WPARAM wparam, __in LPARAM lparam)
{
    CMFCPropertyGridProperty* pProp = reinterpret_cast<CMFCPropertyGridProperty*>(lparam);
    int nID = pProp->GetData();
    CString ppPN = pProp->GetParent()->GetName()
        , pName = pProp->GetName()
        , pValue = pProp->GetValue().bstrVal;

    if (ppPN == L"��ǩ")
    {
        if (pName == L"����")
        {
            // �༭��ǩ����
            if (pValue.Find(L"$", 0) > -1)
            {
                AfxMessageBox(L"��ǩ�����ܰ���$�ַ�!");
                return 0;
            }
            CTNApp* app = (CTNApp*)AfxGetApp();
            if (app->nodeNameSet.find(pValue) != app->nodeNameSet.end())
            {
                AfxMessageBox(L"��ǩ���Ѵ���!");
                pProp->SetValue(pProp->GetOriginalValue());
                return 0;
            }
            CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
            CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
            CTNView* pView = (CTNView*)pFrame->GetActiveView();
            CString oldLabel = pFrame->m_curLabel;
            CString inipath = doc->m_inipath, tcstr;
            TCHAR szSection[MAX_LABELPROP_LEN] = { 0 };
            TCHAR* p = NULL;

            pProp->SetDescription(pValue + L"\r\n(��ǩ��Ψһ��ʶ��)");
            // �޸�Label�ڵ�����
            pView->mOSG->editLabelName(oldLabel, pValue);
            // �޸������ļ�
            int nNum = GetPrivateProfileSection(oldLabel, szSection
                                                , MAX_LABELPROP_LEN, inipath);
            if (nNum > MAX_LABELPROP_LEN - 1)
            {
                AfxMessageBox(L"��ǰ��ǩ������̫���ˣ�");
                return 0;
            }
            WritePrivateProfileSection(pValue
                                       , szSection
                                       , inipath);
            int nValue = 0; // ������Ŀ
            nValue = ::GetPrivateProfileInt(
                oldLabel, L"PropNum", 0, inipath);
            for (int i = 0; i < nValue; i++)
            {
                tcstr.Format(L"PropName%d", i);
                ::WritePrivateProfileString(oldLabel,
                                            tcstr, L"",
                                            inipath);
                tcstr.Format(L"PropValue%d", i);
                ::WritePrivateProfileString(oldLabel,
                                            tcstr, L"",
                                            inipath);
            }
            ::WritePrivateProfileString(oldLabel,
                                        L"PropNum", L"",
                                        inipath);


        }
        else
        {
            // �༭��ǩ����
            pProp->SetDescription(pValue);
            m_wndPropList.RedrawWindow();
            CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
            CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
            CString inipath = doc->m_inipath, tcstr,pcstr;
            CMFCPropertyGridProperty* pLabelPropGroup = pProp->GetParent();
            CString LabelName = pLabelPropGroup->GetSubItem(0)->GetValue().bstrVal;
            TCHAR szKeyValue[MAX_PROP_LEN] = { 0 };
            int nValue = 0; // ������Ŀ
            nValue = ::GetPrivateProfileInt(LabelName, L"PropNum", 0, inipath);
            for (int i = 0; i < nValue; i++)
            {
                tcstr.Format(L"PropName%d", i);
                ::GetPrivateProfileString(LabelName, tcstr, NULL
                                          , szKeyValue, MAX_PROP_LEN
                                          , inipath);
                pcstr = szKeyValue;
                if (pcstr == pName)
                {
                    pValue.Replace(L"\r\n", L"\\r\\n");
                    tcstr.Format(L"PropValue%d", i);
                    ::WritePrivateProfileString(LabelName,
                                                tcstr, pValue,
                                                inipath);
                    break;
                }
            }
        }
    }

    //if (1 == nID) //modelname
    //{
    //    //�Զ���Ĳ���
    //    //CString cpStr(pProp->GetValue().bstrVal);
    //    //view mOSG handler
    //    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //    CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //    pView->mOSG->setNewModel(getModelName().GetBuffer());

    //}
    //else if (2 == nID)
    //{
    //    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //    CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //    pView->mOSG->setModelSize(pProp->GetValue().fltVal);
    //}
    //else if (3 == nID)
    //{
    //    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //    CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //    pView->mOSG->setFireSize(pProp->GetValue().fltVal);
    //}

    return 0;
}

void CPropertiesWnd::InitPropList()
{
    SetPropListFont();

    m_wndPropList.EnableHeaderCtrl(FALSE);
    m_wndPropList.EnableDescriptionArea();
    m_wndPropList.SetVSDotNetLook();
    m_wndPropList.MarkModifiedProperties();

    CMFCPropertyGridProperty* pModelGroup = new CMFCPropertyGridProperty(_T("ģ��"));

    CMFCPropertyGridProperty* pModelNameProp = new CMFCPropertyGridProperty(_T("ģ������"), _T("cow"), _T("ָ�������ģ�͵�����"), 1);
    pModelNameProp->AddOption(_T("avatar"));
    pModelNameProp->AddOption(_T("bignathan"));
    pModelNameProp->AddOption(_T("cessna"));
    pModelNameProp->AddOption(_T("robot"));
    pModelNameProp->AddOption(_T("cow"));
    pModelNameProp->AllowEdit(FALSE);

    pModelGroup->AddSubItem(pModelNameProp);

    CMFCPropertyGridProperty* pModelSizeProp = new CMFCPropertyGridProperty(_T("ģ�ʹ�С"), 1.0f, _T("ָ�������ģ�͵Ĵ�С"), 2);
    pModelSizeProp->AllowEdit(TRUE);

    pModelGroup->AddSubItem(pModelSizeProp);


    CMFCPropertyGridProperty* pEffectGroup = new CMFCPropertyGridProperty(
        _T("��Ч"));
    CMFCPropertyGridProperty* pFireSizeProp = new CMFCPropertyGridProperty(
        _T("�����С"), 0.0f, _T("ָ������ӻ���Ĵ�С\n0��ʾ�����С(0~1)"), 3);
    pFireSizeProp->AllowEdit(TRUE);
    pEffectGroup->AddSubItem(pFireSizeProp);

    /*
    CMFCPropertyGridProperty* pManipulatorGroup =
    new CMFCPropertyGridProperty(_T("����ӵ�"));
    CMFCPropertyGridProperty* pManipulatorProp = new CMFCPropertyGridProperty(
    _T("x"), 0.0f, _T("eye v1"), 4);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("y"), 0.0f, _T("eye v2"), 5);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("z"), 0.0f, _T("eye v4"), 6);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pModelGroup);			//0
    m_wndPropList.AddProperty(pEffectGroup);		//1
    m_wndPropList.AddProperty(pManipulatorGroup);   //2  _center

    pManipulatorGroup =
    new CMFCPropertyGridProperty(_T("�������"));
    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("x"), 0.0f, _T("quat v1"), 7);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("y"), 0.0f, _T("quat v2"), 8);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("z"), 0.0f, _T("quat v3"), 9);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("w"), 0.0f, _T("quat v4"), 10);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pManipulatorGroup);//3 _rotation


    pManipulatorGroup =
    new CMFCPropertyGridProperty(_T("����"));

    pManipulatorProp = new CMFCPropertyGridProperty(
    _T("distance"), 0.0f, _T("_distance"), 10);
    pManipulatorProp->AllowEdit(FALSE);
    pManipulatorGroup->AddSubItem(pManipulatorProp);

    m_wndPropList.AddProperty(pManipulatorGroup);//4 _distance
    */
    /*CMFCPropertyGridProperty * pDebugGroup =
        new CMFCPropertyGridProperty(_T("������Ϣ"));
        CMFCPropertyGridProperty* pDebugProp =
        new CMFCPropertyGridProperty(_T("��Ϣ"), _T(""), _T(""), 11);
        pDebugGroup->AddSubItem(pDebugProp);
        m_wndPropList.AddProperty(pDebugGroup);*/

}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
    CDockablePane::OnSetFocus(pOldWnd);
    m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
    CDockablePane::OnSettingChange(uFlags, lpszSection);
    SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
    ::DeleteObject(m_fntPropList.Detach());

    LOGFONT lf;
    afxGlobalData.fontRegular.GetLogFont(&lf);

    NONCLIENTMETRICS info;
    info.cbSize = sizeof(info);

    afxGlobalData.GetNonClientMetrics(info);

    lf.lfHeight = info.lfMenuFont.lfHeight;
    lf.lfWeight = info.lfMenuFont.lfWeight;
    lf.lfItalic = info.lfMenuFont.lfItalic;

    m_fntPropList.CreateFontIndirect(&lf);

    m_wndPropList.SetFont(&m_fntPropList);
    m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::OnAddLabelProperties()
{
    //CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //pView->mAddModelValid = !pView->mAddModelValid;
    //pView->mOSG->addNewModels(pView->mAddModelValid);
    int propListPropNum = m_wndPropList.GetPropertyCount();
    CString pnamestr;
    int labelPropGroupID = 0;
    bool existFlag = false;
    for (int i = 0; i < propListPropNum; i++)
    {
        pnamestr = m_wndPropList.GetProperty(i)->GetName();
        if (pnamestr == "��ǩ")
        {
            existFlag = true;
            labelPropGroupID = i;
            break;
        }
    }
    if (existFlag)
    {
        CMFCPropertyGridProperty* labelGroup = m_wndPropList.GetProperty(labelPropGroupID);
        int curPID = labelGroup->GetSubItemsCount();

        CLabelEditor ledlg = new CLabelEditor();
        ledlg.m_labelName = labelGroup->GetSubItem(0)->GetValue().bstrVal;
        if (ledlg.DoModal() == IDOK)
        {
            CString cstrRaw = ledlg.m_PropValue;
            CString cstrFix = cstrRaw;
            cstrFix.Replace(L"\r\n", L"\\r\\n");
            CMFCPropertyGridProperty* pModelProp = new CMFCPropertyGridProperty(
                ledlg.m_PropName, cstrRaw, cstrRaw, curPID++);
            pModelProp->AllowEdit(true);
            labelGroup->AddSubItem(pModelProp);

            m_wndPropList.ExpandAll();

            CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
            CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
            CString inipath = doc->m_inipath, tcstr;
            // д�������ļ�
            // ��ȡ��ǰ�������ID = PropNum
            int nValue = 0; // ������Ŀ
            nValue = ::GetPrivateProfileInt(
                ledlg.m_labelName, TEXT("PropNum"), 0, inipath);

            tcstr.Format(L"PropName%d", nValue);
            ::WritePrivateProfileString(ledlg.m_labelName,
                                        tcstr,
                                        ledlg.m_PropName,
                                        inipath);

            tcstr.Format(L"PropValue%d", nValue);
            ::WritePrivateProfileString(ledlg.m_labelName,
                                        tcstr,
                                        cstrFix,
                                        inipath);
            nValue++;
            tcstr.Format(L"%d", nValue);
            ::WritePrivateProfileString(ledlg.m_labelName,
                                        L"PropNum",
                                        tcstr,
                                        inipath);
            ledlg.DestroyWindow();
        }
    }
}

void CPropertiesWnd::OnLabelEditprop()
{
    CMFCPropertyGridProperty* pProp = m_wndPropList.GetCurSel();
    if (!pProp)
        return;
    CString ppPN = pProp->GetParent()->GetName()
        , pName = pProp->GetName()
        , pValue = pProp->GetValue().bstrVal;
    if (ppPN == L"��ǩ")
    {
        if (pName == L"����")
        {
            return;
        }
        CMFCPropertyGridProperty* labelGroup = pProp->GetParent();
        CLabelEditor ledlg = new CLabelEditor();
        ledlg.m_labelName = labelGroup->GetSubItem(0)->GetValue().bstrVal;
        ledlg.m_PropName = pName;
        ledlg.m_PropValue = pValue;
        bool isNameChanged;
        bool isValueChanged;
        if (ledlg.DoModal() == IDOK)
        {
            pProp->SetName(ledlg.m_PropName);
            pProp->SetValue(ledlg.m_PropValue);
            pProp->SetDescription(ledlg.m_PropValue);
            isNameChanged = ledlg.isNameChanged;
            isValueChanged = ledlg.isValueChanged;
            m_wndPropList.ExpandAll();
            CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
            CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
            CString inipath = doc->m_inipath, tcstr, pcstr;
            CString LabelName = labelGroup->GetSubItem(0)->GetValue().bstrVal;
            // д�������ļ�
            // ��ȡ��ǰ�������
            int nValue = 0; // ������Ŀ
            nValue = ::GetPrivateProfileInt(
                ledlg.m_labelName, TEXT("PropNum"), 0, inipath);
            TCHAR szKeyValue[MAX_PROP_LEN] = { 0 };
            // �޸���������
            for (int i = 0; i < nValue; i++)
            {
                tcstr.Format(L"PropName%d", i);
                ::GetPrivateProfileString(LabelName, tcstr, NULL
                                          , szKeyValue, MAX_PROP_LEN
                                          , inipath);
                pcstr = szKeyValue;
                if (pcstr == pName)
                {
                    if (isNameChanged)
                    {
                        tcstr.Format(L"PropName%d", i);
                        ::WritePrivateProfileString(LabelName,
                                                    tcstr, pProp->GetName(),
                                                    inipath);
                    }
                    if (isValueChanged)
                    {
                        CString cstrFix = pProp->GetValue().bstrVal;
                        cstrFix.Replace(L"\r\n", L"\\r\\n");
                        tcstr.Format(L"PropValue%d", i);
                        ::WritePrivateProfileString(LabelName,
                                                    tcstr, cstrFix,
                                                    inipath);
                    }
                    break;
                }
            }            
        }
    }
}

void CPropertiesWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    CDockablePane::OnLButtonDblClk(nFlags, point);
}

void CPropertiesWnd::OnUpdateLabelProperties(CCmdUI* pCmdUI)
{
    //CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //pCmdUI->SetCheck(pView->mAddModelValid);
}

void CPropertiesWnd::OnProperties2()
{
    //CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //pView->mAddEffectValid = !pView->mAddEffectValid;
    //pView->mOSG->addEffects(pView->mAddEffectValid);
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* pCmdUI)
{
    //CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //pCmdUI->SetCheck(pView->mAddEffectValid);
}


void CPropertiesWnd::OnProperties3()
{
    //CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    //CTNView* pView = (CTNView*)pFrame->GetActiveView();
    //pView->OnOsgPan();
}

void CPropertiesWnd::OnUpdateProperties3(CCmdUI *pCmdUI)
{
    //CTNApp *app = (CTNApp *)AfxGetApp();
    //pCmdUI->SetCheck(app->naviMode == NAVI_MODE_PAN);
}



void CPropertiesWnd::OnLabelDel()
{
    // ɾ��ѡ�б�ǩ
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
    CString inipath = doc->m_inipath, tcstr;
    CString curLabel = pFrame->m_curLabel;
    if (::MessageBox(NULL, L"ɾ����ǩ��" + curLabel + L"��",
        L"��ʾ", MB_YESNO) == IDYES)
    {
        // AfxMessageBox(L"ɾ����ǩ�������ڿ������У�");
        // �޸�������µ�label�����
        pFrame->deleteLabel(curLabel);
        // �޸����Դ���
        int propListPropNum = m_wndPropList.GetPropertyCount();
        CString pnamestr;
        int labelPropGroupID = 0;
        bool existFlag = false;
        for (int i = 0; i < propListPropNum; i++)
        {
            pnamestr = m_wndPropList.GetProperty(i)->GetName();
            if (pnamestr == "��ǩ")
            {
                existFlag = true;
                labelPropGroupID = i;
                break;
            }
        }
        if (existFlag)
        {
            CMFCPropertyGridProperty* labelGroup = m_wndPropList.GetProperty(labelPropGroupID);
            m_wndPropList.DeleteProperty(labelGroup);
        }
        // �޸������ļ�
        int nValue = 0; // ������Ŀ
        nValue = ::GetPrivateProfileInt(
            curLabel, L"PropNum", 0, inipath);
        for (int i = 0; i < nValue; i++)
        {
            tcstr.Format(L"PropName%d", i);
            ::WritePrivateProfileString(curLabel,
                                        tcstr, L"",
                                        inipath);
            tcstr.Format(L"PropValue%d", i);
            ::WritePrivateProfileString(curLabel,
                                        tcstr, L"",
                                        inipath);
        }
        ::WritePrivateProfileString(curLabel,
                                    L"PropNum", L"",
                                    inipath);

        ShowPane(FALSE, FALSE, FALSE);
    }
}


void CPropertiesWnd::OnLabelDelprop()
{
    // TODO:  ɾ����ǩ��ĳһ����
    CMFCPropertyGridProperty* pProp = m_wndPropList.GetCurSel();
    if (!pProp)
        return;
    CString ppPN = pProp->GetParent()->GetName()
        , pName = pProp->GetName()
        , pValue = pProp->GetValue().bstrVal;
    if (ppPN == L"��ǩ")
    {
        if (pName == L"����")
        {
            return;
        }
        CMFCPropertyGridProperty* labelGroup = pProp->GetParent();
        CString LabelName = labelGroup->GetSubItem(0)->GetValue().bstrVal;
        if (::MessageBox(NULL, L"ɾ����ǩ��" + LabelName
            + L" ���ԣ�" + pName + L"��",
            L"��ʾ", MB_YESNO) == IDYES)
        {
            // AfxMessageBox(L"ɾ����ǩ���Թ������ڿ������У�");
            
            labelGroup->RemoveSubItem(pProp);

            CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
            CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
            CString inipath = doc->m_inipath, tcstr, pcstr;
            // д�������ļ�
            // ��ȡ��ǰ�������ID = PropNum
            int nValue = 0; // ������Ŀ
            TCHAR szKeyValue[MAX_PROP_LEN] = { 0 };
            nValue = ::GetPrivateProfileInt(
                LabelName, TEXT("PropNum"), 0, inipath);
            int i;
            for (i = 0; i < nValue; i++)
            {
                tcstr.Format(L"PropName%d", i);
                ::GetPrivateProfileString(LabelName, tcstr, NULL
                                          , szKeyValue, MAX_PROP_LEN
                                          , inipath);
                pcstr = szKeyValue;
                if (pcstr == pName)
                {                   
                    break;
                }
            }
            if (i < nValue)
            {
                // ���������ǰ��
                for (; i < nValue - 1; i++)
                {
                    tcstr.Format(L"PropName%d", i + 1);
                    ::GetPrivateProfileString(LabelName, tcstr, NULL
                                              , szKeyValue, MAX_PROP_LEN
                                              , inipath);
                    tcstr.Format(L"PropName%d", i);
                    ::WritePrivateProfileString(LabelName,
                                                tcstr, szKeyValue,
                                                inipath);
                    tcstr.Format(L"PropValue%d", i + 1);
                    ::GetPrivateProfileString(LabelName, tcstr, NULL
                                              , szKeyValue, MAX_PROP_LEN
                                              , inipath);
                    tcstr.Format(L"PropValue%d", i);
                    ::WritePrivateProfileString(LabelName,
                                                tcstr, szKeyValue,
                                                inipath);
                }
                nValue--;
                tcstr.Format(L"%d", nValue);
                ::WritePrivateProfileString(LabelName,
                                            L"PropNum",
                                            tcstr,
                                            inipath);
                tcstr.Format(L"PropName%d", i);
                ::WritePrivateProfileString(LabelName,
                                            tcstr, L"",
                                            inipath);
                tcstr.Format(L"PropValue%d", i);
                ::WritePrivateProfileString(LabelName,
                                            tcstr, L"",
                                            inipath);
            }
            m_wndPropList.RedrawWindow();
            m_wndPropList.ExpandAll();
        }
    }
}

// TNDoc.cpp : CTNDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TNApp.h"
#endif

#include "TNDoc.h"
#include "MainFrm.h"
#include "TNView.h"

#include <propkey.h>

#include <fstream>
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTNDoc

IMPLEMENT_DYNCREATE(CTNDoc, CDocument)

BEGIN_MESSAGE_MAP(CTNDoc, CDocument)
    ON_COMMAND(ID_SCENE_SAVE, &CTNDoc::OnSceneSave)
    ON_COMMAND(ID_FILE_OPEN, &CTNDoc::OnFileOpen)
    ON_COMMAND(ID_FILE_SAVE, &CTNDoc::OnFileSave)
    ON_COMMAND(ID_FILE_SAVE_AS, &CTNDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CTNDoc ����/����

CTNDoc::CTNDoc()
{
}

CTNDoc::~CTNDoc()
{}

BOOL CTNDoc::OnNewDocument()
{
    if (!CDocument::OnNewDocument())
        return FALSE;

    // (SDI �ĵ������ø��ĵ�)
    // �õ�exeִ��·��.  
    TCHAR tcExePath[MAX_PATH] = { 0 };
    ::GetModuleFileName(NULL, tcExePath, MAX_PATH);
    // ����ini·����exeͬһĿ¼��  
#ifndef CONFIG_FILE  
#define CONFIG_FILE     (TEXT("setting.ini"))  
#endif  
    //_tcsrchr() ��������������һ��'\\'��λ�ã������ظ�λ�õ�ָ��  
    TCHAR *pFind = _tcsrchr(tcExePath, '\\');
    if (pFind != NULL)
    {
        *pFind = '\0';
    }

    CString szIniPath = tcExePath;
    szIniPath += "\\";
    m_path = szIniPath; // ��ǰexe·��
    szIniPath += CONFIG_FILE;

    //����ִ�ж�ȡ ----------------------------------  
    if (::PathFileExists(szIniPath))
    {
        TCHAR szKeyValue[MAX_PATH] = { 0 };
        int nValue = 0;
        ::GetPrivateProfileString(TEXT("Environment"), TEXT("Path"), 
                                  NULL, szKeyValue, MAX_PATH, 
                                  szIniPath);
        //nValue = ::GetPrivateProfileInt(TEXT("Environment"), TEXT("Path"), 0, szIniPath);
        m_datapath = szKeyValue;
        // CString lastchar = m_datapath.Right(1);
        if (m_datapath.Right(1) != L"\\"
            && m_datapath.Right(1) != L"/")
        {
            m_datapath += L"\\";
        }
        m_inipath = m_datapath + L"Ĭ����Ŀ.ini";
        if (::PathFileExists(m_datapath + L"Ĭ����Ŀ.ini"))
        {
            DeleteFile(m_inipath);
        }
        CFile   file;
        file.Open(m_datapath + L"Ĭ����Ŀ.ini", CFile::modeCreate);
        file.Close();
        initModelZ = ::GetPrivateProfileInt(
            TEXT("Environment"), TEXT("initModelZ"), 0, szIniPath);
        memset(szKeyValue,0, MAX_PATH);
        ::GetPrivateProfileString(TEXT("Scene"), TEXT("initModelName"),
                                  NULL, szKeyValue, MAX_PATH,
                                  szIniPath);
        if (szKeyValue[0] != '\0')
        {
            m_initModelName = m_datapath + szKeyValue;
            // ���ó��������
            CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
            pFrame->SetWindowTextW(m_initModelName + L" - Scene Editor");
        }
        else
        {
            m_initModelName = L"";
        }
    }
       
    return TRUE;
}




// CTNDoc ���л�

void CTNDoc::Serialize(CArchive& ar)
{
    if (ar.IsStoring())
    {
        // ����

    }
    else
    {
       // ����

    }
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CTNDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
    // �޸Ĵ˴����Ի����ĵ�����
    dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

    CString strText = _T("TODO: implement thumbnail drawing here");
    LOGFONT lf;

    CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
    pDefaultGUIFont->GetLogFont(&lf);
    lf.lfHeight = 36;

    CFont fontDraw;
    fontDraw.CreateFontIndirect(&lf);

    CFont* pOldFont = dc.SelectObject(&fontDraw);
    dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
    dc.SelectObject(pOldFont);
}

// ������������֧��
void CTNDoc::InitializeSearchContent()
{
    CString strSearchContent;
    // ���ĵ����������������ݡ�
    // ���ݲ���Ӧ�ɡ�;���ָ�

    // ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
    SetSearchContent(strSearchContent);
}

void CTNDoc::SetSearchContent(const CString& value)
{
    if (value.IsEmpty())
    {
        RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
    }
    else
    {
        CMFCFilterChunkValueImpl *pChunk = NULL;
        ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
        if (pChunk != NULL)
        {
            pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
            SetChunkValue(pChunk);
        }
    }
}

#endif // SHARED_HANDLERS

// CTNDoc ���

#ifdef _DEBUG
void CTNDoc::AssertValid() const
{
    CDocument::AssertValid();
}

void CTNDoc::Dump(CDumpContext& dc) const
{
    CDocument::Dump(dc);
}
#endif //_DEBUG


// CTNDoc ����


void CTNDoc::OnSceneSave()
{
    // ���泡��
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
    CTNView* view = (CTNView*)pFrame->GetActiveView();

    BOOL isOpen = FALSE;     //�Ƿ��(����Ϊ����)  
    CString defaultDir = doc->m_datapath;   //Ĭ�ϴ򿪵��ļ�·��  
    CString fileName = L"";         //Ĭ�ϴ򿪵��ļ���  
    CString filter = L"�ļ� (*.osg; *.osgb)|*.osg; *.osgb||";   //�ļ����ǵ�����  
    CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
    openFileDlg.GetOFN().lpstrInitialDir = defaultDir;
    INT_PTR result = openFileDlg.DoModal();
    CString filePath = defaultDir;
    int confirmed = -1;
    if (result == IDOK)
    {
        filePath = openFileDlg.GetPathName();
        if (::PathFileExists(filePath))
        {
            if (::MessageBox(NULL, L"�滻�Ѵ����ļ���",
                L"��ʾ", MB_YESNO) == IDYES)
            {
                confirmed = 2;
            }
        }
        else
        {
            confirmed = 1;
        }

    }
    if (confirmed > 0)
    {
        m_projname = openFileDlg.GetFileTitle();
        m_inipath = m_datapath + m_projname + L".ini";
        // ���ó��������
        pFrame->SetWindowTextW(filePath + L" - Scene Editor");
        view->saveScene(filePath, confirmed);
    }
}


void CTNDoc::OnFileOpen()
{
    // ���س���
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNView* view = (CTNView*)pFrame->GetActiveView();

    BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
    CString defaultDir = m_datapath;   //Ĭ�ϴ򿪵��ļ�·��  
    CString fileName = L"";         //Ĭ�ϴ򿪵��ļ���  
    CString filter = L"�ļ� (*.osg; *.osgb)|*.osg; *.osgb||";   //�ļ����ǵ�����  
    CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
    openFileDlg.GetOFN().lpstrInitialDir = defaultDir;
    INT_PTR result = openFileDlg.DoModal();
    CString filePath = defaultDir;
    bool confirmed = false;
    if (result == IDOK)
    {
        filePath = openFileDlg.GetPathName();
        m_projname = openFileDlg.GetFileTitle();
        m_inipath = m_datapath + m_projname + L".ini";

        if (!::PathFileExists(filePath))
        {
            AfxMessageBox(L"�ļ������ڣ�");
        }
        //else if (!::PathFileExists(m_inipath))
        //{
        //    AfxMessageBox(L"�Ƿ���Ŀ�ļ�����Ϊ��Ŀ���������ļ������ڣ�");
        //}
        //else
        {
            confirmed = true;
        }

    }
    if (confirmed)
    {
        // ���ó��������
        pFrame->SetWindowTextW(filePath + L" - Scene Editor");
        view->loadScene(filePath);
    }
}


void CTNDoc::OnFileSave()
{
    CTNDoc::OnSceneSave();
}


void CTNDoc::OnFileSaveAs()
{
    CTNDoc::OnSceneSave();
}


void CTNDoc::OnCloseDocument()
{
    // ɾ��Ĭ����Ŀ.ini
    if (::PathFileExists(m_datapath + L"Ĭ����Ŀ.ini"))
    {
        DeleteFile(m_datapath + L"Ĭ����Ŀ.ini");
    }
    CDocument::OnCloseDocument();
}

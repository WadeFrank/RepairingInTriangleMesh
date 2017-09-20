
// TNView.cpp : CTNView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "TNApp.h"
#endif

#include "TNDoc.h"
#include "TNView.h"

#include "MainFrm.h"

#include <osgGA/StateSetManipulator>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTNView

IMPLEMENT_DYNCREATE(CTNView, CView)

BEGIN_MESSAGE_MAP(CTNView, CView)
    ON_WM_CONTEXTMENU()
    ON_WM_RBUTTONUP()
    ON_WM_CREATE()
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_COMMAND(ID_OSG_ADDMD, &CTNView::OnOsgAddmd)
    ON_COMMAND(ID_OSG_ADDEF, &CTNView::OnOsgAddef)
    ON_COMMAND(ID_OSG_PAN, &CTNView::OnOsgPan)
    ON_COMMAND(ID_OSG_ROTATE, &CTNView::OnOsgRotate)
    ON_COMMAND(ID_OSG_TRANS, &CTNView::OnOsgTrans)
    ON_COMMAND(ID_OSG_ORBIT, &CTNView::OnOsgOrbit)
    ON_COMMAND(ID_OSG_SELECT, &CTNView::OnOsgSelect)
    ON_COMMAND(ID_OSG_SCALE, &CTNView::OnOsgScale)

    ON_WM_DESTROY()
    ON_WM_MOUSEWHEEL()
    ON_COMMAND(ID_OSG_ADDLABEL, &CTNView::OnOsgAddlabel)
END_MESSAGE_MAP()

void sendBuildString(CString cstr)
{
    // ������Ϣ ���µ�����Ϣ
    HWND hMainWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();

    SendMessage(hMainWnd, WM_USER_THREADEND,
                WPARAM(cstr.GetAllocLength()), (LPARAM)cstr.GetBuffer(cstr.GetAllocLength()));

};

// CTNView ����/����

CTNView::CTNView()
{
    mThreadHandle = NULL;
}

CTNView::~CTNView()
{}

BOOL CTNView::PreCreateWindow(CREATESTRUCT& cs)
{
    //  CREATESTRUCT cs ���޸Ĵ��������ʽ

    return CView::PreCreateWindow(cs);
}

// CTNView ����

void CTNView::OnDraw(CDC* /*pDC*/)
{
    CTNDoc* pDoc = GetDocument();
    ASSERT_VALID(pDoc);
    if (!pDoc)
        return;

    // TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}

void CTNView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
    ClientToScreen(&point);
    OnContextMenu(this, point);
}

void CTNView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
    //theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTNView ���

#ifdef _DEBUG
void CTNView::AssertValid() const
{
    CView::AssertValid();
}

void CTNView::Dump(CDumpContext& dc) const
{
    CView::Dump(dc);
}

CTNDoc* CTNView::GetDocument() const // �ǵ��԰汾��������
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTNDoc)));
    return (CTNDoc*)m_pDocument;
}
#endif //_DEBUG


// CTNView ��Ϣ�������


int CTNView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CView::OnCreate(lpCreateStruct) == -1)
        return -1;

    mOSG = new cOSG(m_hWnd);

    return 0;
}


void CTNView::OnInitialUpdate()
{
    CView::OnInitialUpdate();
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();

    if (mThreadHandle)
    {
        ref_ptr<Group> Root = dynamic_cast<Group*>(mOSG->getViewer()->getSceneData());
        mOSG->getViewer()->setSceneData(NULL);
        int rootChildNum = Root->getNumChildren();
        for (int i = 0; i < rootChildNum; i++)
        {
            Root->removeChild(i);
        }
        // �����ʼģ��       
        CTNApp *app = (CTNApp *)AfxGetApp();
        app->nodeNameSet.clear();
        mOSG->InitSceneGraph();

        mOSG->getViewer()->setSceneData(mOSG->getRoot().get());
        mOSG->fixInitCamera();

        HWND hMainWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
        ::SendMessage(hMainWnd, WM_USER_EDITMODELLABEL,
                    WPARAM(NULL), (LPARAM)NULL);
    }
    else //if (!mThreadHandle)
    {
        if (doc->m_initModelName != "")
        {
            mOSG->InitOSG(doc->m_initModelName);
        }
        else
        {
            mOSG->InitOSG();
        }

        sendBuildString(_T("��������OSG�߳�..."));
        mThreadHandle = (HANDLE)_beginthread(&cOSG::Render, 0, mOSG);
        sendBuildString(_T("OSG�߳�������ɣ�"));
    }
    
    //float c1, c2, c3, q1, q2, q3, q4, d;
    //Vec3d eye;
    //Quat rotation;
    //mOSG->m_naviManipulator->getTransformation(eye, rotation);

    //Vec3d center = eye;
    //c1 = center.x();
    //c2 = center.y();
    //c3 = center.z();

    //Vec4d rtasvc4d = rotation.asVec4();
    //q1 = rtasvc4d.x();
    //q2 = rtasvc4d.y();
    //q3 = rtasvc4d.z();
    //q4 = rtasvc4d.w();
    //d = mOSG->m_naviManipulator->getDistance();
    //pFrame->SetManipulatorProperties(c1, c2, c3, q1, q2, q3, q4, d);

}

void CTNView::OnMouseMove(UINT nFlags, CPoint point)
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    int naviMode = app->naviMode;
    if (naviMode == NAVI_MODE_PAN && app->editMode < 0)
    {
        HCURSOR hCur = LoadCursor(NULL, IDC_HAND);
        ::SetCursor(hCur);
    }
    else
    {
        HCURSOR hCur = LoadCursor(NULL, IDC_ARROW);
        ::SetCursor(hCur);
    }

 /*   CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    float c1, c2, c3, q1, q2, q3, q4, d;

    Vec3d eye;
    Quat rotation;
    mOSG->m_naviManipulator->getTransformation(eye, rotation);

    Vec3d center = eye;
    c1 = center.x();
    c2 = center.y();
    c3 = center.z();

    Vec4d rtasvc4d = rotation.asVec4();
    q1 = rtasvc4d.x();
    q2 = rtasvc4d.y();
    q3 = rtasvc4d.z();
    q4 = rtasvc4d.w();
    d = mOSG->m_naviManipulator->getDistance();
    pFrame->SetManipulatorProperties(c1, c2, c3, q1, q2, q3, q4, d);
    */

    CView::OnMouseMove(nFlags, point);
}


BOOL CTNView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    return CView::OnSetCursor(pWnd, nHitTest, message);
}


// --------------  �༭ģ��  --------------

void CTNView::OnOsgAddmd()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_ADD_MODEL)
    {
        app->editMode = EDIT_MODE_ADD_MODEL;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}

void CTNView::OnOsgAddlabel()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_ADD_LABEL)
    {
        app->editMode = EDIT_MODE_ADD_LABEL;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}

void CTNView::OnOsgAddef()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_ADD_FIRE)
    {
        app->editMode = EDIT_MODE_ADD_FIRE;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}

void CTNView::OnOsgRotate()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_ROTATE)
    {
        app->editMode = EDIT_MODE_ROTATE;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}

void CTNView::OnOsgTrans()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_TRANS)
    {
        app->editMode = EDIT_MODE_TRANS;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}

void CTNView::OnOsgScale()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->editMode != EDIT_MODE_SCALE)
    {
        app->editMode = EDIT_MODE_SCALE;
        app->editModeChanged = true;
    }
    else
    {
        app->editMode = -1;
    }
    mOSG->setEditMode(app->editMode);
}


// --------------  ���  --------------

void CTNView::OnOsgPan()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->naviMode != NAVI_MODE_PAN)
    {
        app->naviMode = NAVI_MODE_PAN;
        mOSG->setNaviMode(app->naviMode);
        app->naviModeChanged = true;
    }
}

void CTNView::OnOsgOrbit()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->naviMode != NAVI_MODE_ORBIT)
    {
        app->naviMode = NAVI_MODE_ORBIT;
        mOSG->setNaviMode(app->naviMode);
        app->naviModeChanged = true;
    }
}

void CTNView::OnOsgSelect()
{
    CTNApp *app = (CTNApp *)AfxGetApp();
    if (app->naviMode != NAVI_MODE_SELECT)
    {
        app->naviMode = NAVI_MODE_SELECT;
        mOSG->setNaviMode(app->naviMode);
        app->naviModeChanged = true;
    }
}

void CTNView::OnDestroy()
{
    mOSG->stopThread();
    
    CView::OnDestroy();
}


BOOL CTNView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
    float c1, c2, c3, q1, q2, q3, q4, d;

    Vec3d eye;
    Quat rotation;
    mOSG->m_naviManipulator->getTransformation(eye, rotation);

    Vec3d center = eye;
    c1 = center.x();
    c2 = center.y();
    c3 = center.z();

    Vec4d rtasvc4d = rotation.asVec4();
    q1 = rtasvc4d.x();
    q2 = rtasvc4d.y();
    q3 = rtasvc4d.z();
    q4 = rtasvc4d.w();
    d = mOSG->m_naviManipulator->getDistance();
    pFrame->SetManipulatorProperties(c1, c2, c3, q1, q2, q3, q4, d);

    return CView::OnMouseWheel(nFlags, zDelta, pt);
}

// ���泡��
bool CTNView::saveScene(CString path, int cfm)
{
    ref_ptr<Group> Root = dynamic_cast<Group *>(mOSG->getViewer()->getSceneData());
    osgDB::Registry::instance()
        ->writeNode(*(Root->asNode()), CStringA(path).GetBuffer(0),
        osgDB::Registry::instance()->getOptions());
    // д�븽��������Ϣ
    CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->GetMainWnd();
    CTNDoc* doc = (CTNDoc*)pFrame->GetActiveDocument();
    CString inipath = doc->m_inipath, dpstr = doc->m_datapath, tcstr;

    if ((!::PathFileExists(inipath)))
    {
        tcstr = dpstr + L"Ĭ����Ŀ.ini";
        CopyFile(tcstr, inipath, true);
    }
    else if (cfm >= 2)
    {
        // ���Ϊ-�������Ƶ���Ŀ-�滻�Ѵ�����Ŀ
        CString work, tar = inipath;
        work = doc->m_projname;
        // CopyFile(work, inipath, true);
    }
    AfxMessageBox(L"����ɹ���");
    return 0;
}

void CTNView::loadScene(CString filePath)
{
    // mOSG->stopThread();
    // ���س���
    ref_ptr<Group> Root = dynamic_cast<Group*>(mOSG->getViewer()->getSceneData());
    mOSG->getViewer()->setSceneData(NULL);
    int rootChildNum = Root->getNumChildren();
    for (int i = 0; i < rootChildNum; i++)
    {
        Root->removeChild(i);
    }
    ref_ptr<Group> newSceneNode = dynamic_cast<Group*>(osgDB::readNodeFile(CStringA(filePath).GetBuffer(0)));

    // ������Label���޸��¼ӵ�Label����ȷ��������Label
    CTNApp *app = (CTNApp *)AfxGetApp();
    app->nodeNameSet.clear();
    app->insertNodeName(L"Root");
    mOSG->setRoot(newSceneNode);
    // �����³����ڵ��е�Label�����뵽nameSet��
    int initSceneChildNum = newSceneNode->getNumChildren();
    for (int i = 0; i < initSceneChildNum; i++)
    {
        ref_ptr<Group> childi = dynamic_cast<Group*>(newSceneNode->getChild(i));
        CString childiName;
        childiName = childi->getName().c_str();
        if (childiName == "Model" || childiName == "Label")
        {
            Transform* trans = childi->getChild(0)->asTransform();
            Node* node = trans->getChild(0);
            childiName = node->getName().c_str();
        }
        if (app->insertNodeName2(childiName) >= 0)
        {
            HWND hMainWnd = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
            // ������Ϣ ���µ�����Ϣ
            ::SendMessage(hMainWnd, WM_USER_ADDMODELNAME
                          , WPARAM(childiName.GetBuffer(childiName.GetAllocLength()))
                          , (LPARAM)"Root");
        }
    }

    mOSG->getViewer()->setSceneData(newSceneNode.get());
    mOSG->fixInitCamera();
    mOSG->moveCameratoNode(newSceneNode.get());

    // ��ʾ����
    mOSG->getViewer()->addEventHandler(
        new osgGA::StateSetManipulator(
        mOSG->getViewer()->getCamera()->getOrCreateStateSet()));
    // AfxMessageBox(L"���سɹ���");
}

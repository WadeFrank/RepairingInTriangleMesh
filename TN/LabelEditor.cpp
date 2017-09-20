// LabelEditor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TNApp.h"
#include "LabelEditor.h"
#include "afxdialogex.h"


// CLabelEditor �Ի���

IMPLEMENT_DYNAMIC(CLabelEditor, CDialogEx)

CLabelEditor::CLabelEditor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLabelEditor::IDD, pParent)
    , m_PropName(_T(""))
    , m_PropValue(_T(""))
{
    isNameChanged = false;
    isValueChanged = false;
}

CLabelEditor::~CLabelEditor()
{
}

void CLabelEditor::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PNAME, m_PropName);
    DDV_MaxChars(pDX, m_PropName, 200);
    DDX_Text(pDX, IDC_EDIT_PVALUE, m_PropValue);
	DDV_MaxChars(pDX, m_PropValue, MAX_PROP_LEN);
}


BEGIN_MESSAGE_MAP(CLabelEditor, CDialogEx)
    ON_BN_CLICKED(IDOK, &CLabelEditor::OnBnClickedOk)
    ON_EN_CHANGE(IDC_EDIT_PNAME, &CLabelEditor::OnChangeEditPname)
    ON_EN_CHANGE(IDC_EDIT_PVALUE, &CLabelEditor::OnEnChangeEditPvalue)
END_MESSAGE_MAP()


// CLabelEditor ��Ϣ�������


void CLabelEditor::OnBnClickedOk()
{
    UpdateData(TRUE);
    if (m_PropName == L"")
    {
        AfxMessageBox(L"�������Ʋ���Ϊ�գ�");
        return;
    }
    CDialogEx::OnOK();
}


BOOL CLabelEditor::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetWindowText(L"��ӱ�ǩ���ԣ�"+ m_labelName);

    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣:  OCX ����ҳӦ���� FALSE
}


void CLabelEditor::OnChangeEditPname()
{
    isNameChanged = true;
}


void CLabelEditor::OnEnChangeEditPvalue()
{
    isValueChanged = true;
}

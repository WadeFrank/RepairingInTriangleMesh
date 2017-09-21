#pragma once
#include "afxcmn.h"
#include "resource.h"
#include <vector>
using namespace std;

// CModelDlg �Ի���

class CModelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CModelDlg)

public:
	CModelDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModelDlg();

    enum ModelType { MODEL_SLZ, MODEL_SLT, MODEL_SILZ, MODEL_SILT,
        MODEL_CUBE, MODEL_BALL, MODEL_CYLINDER, MODEL_CONE, MODEL_FRUSTUM_CONE
    };

// �Ի�������
	enum { IDD = IDD_DIALOG_MODEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    static vector<LPWSTR> models;
    CImageList m_imgList;
    CListCtrl m_ModelItemList;
    CString m_NodeName;
    CString m_SelModelName;
    CString m_ModelName;
    enum ModelType m_ModelType;

    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedOk();
    afx_msg void OnNMClickListItem(NMHDR *pNMHDR, LRESULT *pResult);
};

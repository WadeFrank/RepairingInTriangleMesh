
// TN.h : TN Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include <iostream>

#include <set>

// CTNApp:
// �йش����ʵ�֣������ TN.cpp
//

class CTNApp : public CWinAppEx
{
public:
    CTNApp();

    // ���ģʽ
    int naviMode;
    bool naviModeChanged;

    // �༭ģʽ
    int editMode;
    bool editModeChanged;

    std::set<CString> nodeNameSet;

    void clearNameSet()
    {
        nodeNameSet.clear();
    }

    int insertNodeName(CString str)
    {
        std::pair<std::set<CString>::iterator, bool> pr;
        pr = nodeNameSet.insert(str);
        if (pr.second)
        {
            // ���½ڵ���ͼ
            onUpdateNodeView(str);
            // ����λ��
            std::set<CString>::iterator it = pr.first;
            return 0;
        }
        return -1;
    }
    int insertNodeName2(CString str)
    {
        std::pair<std::set<CString>::iterator, bool> pr;
        pr = nodeNameSet.insert(str);
        if (pr.second)
        {
            // ����λ��
            std::set<CString>::iterator it = pr.first;
            return 0;
        }
        return -1;
    }
    void onUpdateNodeView(CString str);

    // ��д
public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();

    // ʵ��
    UINT  m_nAppLook;
    BOOL  m_bHiColorIcons;

    virtual void PreLoadState();
    virtual void LoadCustomState();
    virtual void SaveCustomState();

    afx_msg void OnAppAbout();
    DECLARE_MESSAGE_MAP()
};

extern CTNApp theApp;

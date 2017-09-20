
// TNDoc.h : CTNDoc ��Ľӿ�
//


#pragma once

class CTNDoc : public CDocument
{
protected: // �������л�����
    CTNDoc();
    DECLARE_DYNCREATE(CTNDoc)

    // ����
public:
    CString m_path; // ��ǰ·��
    CString m_inipath;// ��ǰ�����ļ�·��
    CString m_datapath; // ��ǰ������Դ·��
    CList<CString> m_dataList;
    CString m_projname; // ��ǰ��Ŀ����,����ƥ���ǩ�����ļ�

    // 
    int initModelZ;
    CString m_initModelName;
    CString workproj;
    // ����
public:

    // ��д
public:
    virtual BOOL OnNewDocument();
    virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
    virtual void InitializeSearchContent();
    virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

    // ʵ��
public:
    virtual ~CTNDoc();
#ifdef _DEBUG
    virtual void AssertValid() const;
    virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // ���ɵ���Ϣӳ�亯��
protected:
    DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
    // ����Ϊ����������������������ݵ� Helper ����
    void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
    afx_msg void OnSceneSave();
    afx_msg void OnFileOpen();
    afx_msg void OnFileSave();
    afx_msg void OnFileSaveAs();
    virtual void OnCloseDocument();
};

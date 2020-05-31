// CHeaderDialog.h
//
// Header file for class CHeaderDialog
//
// Copyright 1997 Creative Consulting for Research & Education
//
// Last Revised: 9-7-97

#ifndef __CHEADERDIALOG_H__
#define __CHEADERDIALOG_H__

#include <afxres.h>
#include <afxwin.h>
#include <string.h>
#include "resource.h"

class CHeaderDialog : public CModalDialog {
	CEdit* m_pNameField;
	CEdit* m_pHeaderField;
	BOOL m_bNameModified;
	BOOL m_bHeaderModified;
public:
    char m_szName [256];
    char m_szHeader [8192];
    CHeaderDialog (CWnd* pWndParent = NULL) :
    CModalDialog ("HeaderDialog", pWndParent) {}

	BOOL OnInitDialog ();
    afx_msg void OnOK();

    void SetHeader (char*);
    void GetHeader (char*);
    void SetName (char* name) {strcpy(m_szName, name);}
    void GetName (char*);
    BOOL HeaderModified() {return m_bHeaderModified;}
    BOOL NameModified() {return m_bNameModified;}

    DECLARE_MESSAGE_MAP()
};


#endif

// CVerifyDialog.h
//
// Header file for class CVerifyDialog
//
// Copyright 1998 Creative Consulting for Research & Education
//
// Last Revised: 7-15-98

#ifndef __CVERIFYDIALOG_H__
#define __CVERIFYDIALOG_H__

class CVerifyDialog : public CModalDialog {
public:
	static CString m_szPromptString;
    CVerifyDialog (CWnd* pWndParent = NULL) :
    CModalDialog ("VerifyDialog", pWndParent) {}

	BOOL OnInitDialog ();
    afx_msg void OnNo ();

	DECLARE_MESSAGE_MAP()

};


#endif

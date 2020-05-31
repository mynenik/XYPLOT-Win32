// CInputDialog.h
//
// Header file for class CInputDialog
//
// Copyright 1995 Creative Consulting for Research & Education
//
// Last Revised: 2-25-96

#ifndef __CINPUTDIALOG_H__
#define __CINPUTDIALOG_H__

class CInputDialog : public CModalDialog {
public:
	static CString m_szResponseString;
	static CString m_szPromptString;
    CInputDialog (CWnd* pWndParent = NULL) :
    CModalDialog ("InputDialog", pWndParent) {}

	BOOL OnInitDialog ();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

	CEdit* InputField() {return (CEdit*)GetDlgItem(IDC_INPUT_EDIT);}
};


#endif

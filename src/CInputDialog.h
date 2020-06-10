// CInputDialog.h
//
// Header file for class CInputDialog
//
// Copyright 1995-2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

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

// CPickDataDialog.h
//
// Header file for class CPickDataDialog
//
// Copyright 1995--1997 Creative Consulting for Research & Education
//
// Last Revised: 4-26-97

#ifndef __CPICKDATADIALOG_H__
#define __CPICKDATADIALOG_H__

#define MAX_SETS            32

class CPickDataDialog : public CModalDialog {
public:
	static CString m_szDataList[MAX_SETS];
	static int m_nSelection;
	static int m_nSets;
    CPickDataDialog (CWnd* pWndParent = NULL) :
    CModalDialog ("PickDataDialog", pWndParent) {}

	BOOL OnInitDialog ();
    afx_msg void OnOK ();

	DECLARE_MESSAGE_MAP()

	CListBox& DListBox() {return *(CListBox*)GetDlgItem(IDC_PICK_LIST);}
};


#endif

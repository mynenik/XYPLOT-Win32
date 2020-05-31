// CGridDialog.h
//
// Header file for class CGridDialog
//
// Copyright 1997--1998 Creative Consulting for Research & Education
//
// Last Revised: 1-18-98

#ifndef __CGRIDDIALOG_H__
#define __CGRIDDIALOG_H__

class CGridDialog : public CModalDialog {
public:
    CGrid* m_pGrid;

    CGridDialog (CWnd* pWndParent = NULL) :
    CModalDialog ("GridDialog", pWndParent) {}

	BOOL OnInitDialog ();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

    CButton* LinesField() {return (CButton*)GetDlgItem(IDC_GRID_LINES);}
	CEdit* XticsField() {return (CEdit*)GetDlgItem(IDC_GRID_XTICS);}
	CEdit* YticsField() {return (CEdit*)GetDlgItem(IDC_GRID_YTICS);}
};


#endif

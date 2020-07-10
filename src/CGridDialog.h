// CGridDialog.h
//
// Header file for class CGridDialog
//
// Copyright 1997--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

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

    CButton* HorLinesField() {return (CButton*)GetDlgItem(IDC_HOR_GRID_LINES);}
    CButton* VerLinesField() {return (CButton*)GetDlgItem(IDC_VER_GRID_LINES);}
    CEdit* XticsField() {return (CEdit*)GetDlgItem(IDC_GRID_XTICS);}
    CEdit* YticsField() {return (CEdit*)GetDlgItem(IDC_GRID_YTICS);}
};


#endif

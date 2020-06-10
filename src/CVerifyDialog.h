// CVerifyDialog.h
//
// Header file for class CVerifyDialog
//
// Copyright 1998--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

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

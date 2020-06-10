// CSaveOptionsDialog.h
//
// Header file for class CSaveOptionsDialog
//
// Copyright 1998--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero Public License
// (AGPL) v 3.0 or later.

#ifndef __CSAVEOPTIONSDIALOG_H__
#define __CSAVEOPTIONSDIALOG_H__

class CSaveOptionsDialog : public CModalDialog {
    CButton* m_pButtons[11];
    SaveOptions* m_pSaveOptions;
public:
    CSaveOptionsDialog (SaveOptions* p, CWnd* pWndParent = NULL) :
    CModalDialog ("SaveOptionsDialog", pWndParent) {m_pSaveOptions = p;}

    BOOL OnInitDialog ();
    afx_msg void OnOK ();

    DECLARE_MESSAGE_MAP()

};


#endif

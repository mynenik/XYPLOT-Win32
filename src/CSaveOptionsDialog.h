// CSaveOptionsDialog.h
//
// Header file for class CSaveOptionsDialog
//
// Copyright 1998 Creative Consulting for Research & Education
//
// Last Revised: 10-11-1998

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

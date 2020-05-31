// CVerifyDialog.cpp
//
// Copyright 1998 Creative Consulting for Research & Education
//
// Last Revised: 7-15-98

#include "stdafx.h"
#include "resource.h"
#include "CVerifyDialog.h"

CString CVerifyDialog::m_szPromptString = "\0";

//---------------------------------------------------------------

BOOL CVerifyDialog::OnInitDialog()
{
	if (!CModalDialog::OnInitDialog()) return FALSE;
	SetDlgItemText(IDC_VERIFY_PROMPT, m_szPromptString);
	GotoDlgCtrl(GetDlgItem(IDC_NO));
    return TRUE;
}
//---------------------------------------------------------------
void CVerifyDialog::OnNo()
{
    EndDialog(IDC_NO);
}

// CVerifyDialog.cpp
//
// Copyright 1998--2020 Krishna Myneni
//
// Provided under the terms of GNU Affero General Public License
// (AGPL) v 3.0 or later.

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

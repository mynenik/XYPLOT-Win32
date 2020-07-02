// CInputDialog.cpp
//
// Copyright 1995--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#include "stdafx.h"
#include "resource.h"
#include "CInputDialog.h"
CString CInputDialog::m_szResponseString = "\0";
CString CInputDialog::m_szPromptString = "\0";

//---------------------------------------------------------------

BOOL CInputDialog::OnInitDialog()
{
	if (!CModalDialog::OnInitDialog()) return FALSE;
	m_szResponseString.Empty();
	SetDlgItemText(IDC_INPUT_PROMPT, m_szPromptString);
	GotoDlgCtrl(GetDlgItem(IDC_INPUT_EDIT));
	return TRUE;
}
//---------------------------------------------------------------

void CInputDialog::OnOK()
{
	char s[256];

	InputField()->GetLine(0, s, 255);
	s[InputField()->LineLength()] = '\0';
	// int l = strlen (s);
	m_szResponseString = s;
	CModalDialog::OnOK();
}

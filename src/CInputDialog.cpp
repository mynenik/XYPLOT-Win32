// CInputDialog.cpp
//
// Copyright 1995 Creative Consulting for Research & Education
//
// Last Revised: 2-25-96

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
	char s[255];

	InputField()->GetLine(0, s, 254);
	s[InputField()->LineLength()] = '\0';
	// int l = strlen (s);
	m_szResponseString = s;
	CModalDialog::OnOK();
}

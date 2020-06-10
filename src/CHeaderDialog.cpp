// CHeaderDialog.cpp
//
// Copyright 1997--2020 Krishna Myneni
//
// Provided under the terms of GNU Affero General Public License
// (AGPL) v 3.0 or later.

#include "stdafx.h"
#include <string.h>
#include "resource.h"
#include "CHeaderDialog.h"

//---------------------------------------------------------------

BOOL CHeaderDialog::OnInitDialog()
{
	if (!CModalDialog::OnInitDialog()) return FALSE;
	m_pNameField = (CEdit*) GetDlgItem(IDC_NAME_EDIT);
	m_pHeaderField = (CEdit*) GetDlgItem(IDC_HEADER_EDIT);
	m_pNameField->SetWindowText(m_szName);
	m_pHeaderField->SetWindowText(m_szHeader);
	m_bNameModified = FALSE;
	m_bHeaderModified = FALSE;
//	GotoDlgCtrl(m_pNameField);
	return TRUE;
}
//---------------------------------------------------------------

void CHeaderDialog::OnOK()
{
    m_bNameModified = m_pNameField->GetModify();
    m_bHeaderModified = m_pHeaderField->GetModify();

    if (m_bNameModified)
        m_pNameField->GetWindowText(m_szName, 256);
    if (m_bHeaderModified)
        m_pHeaderField->GetWindowText(m_szHeader, 8196);

    CModalDialog::OnOK();
}
//---------------------------------------------------------------

void CHeaderDialog::SetHeader(char* hdr)
{
// Copy a multi-line header, with the lines delineated by
//   the newline character '\n' into another string
//   where the lines are delineated by the sequence "\r\n"
//   so that they will display properly in the edit control.
//
    char *hp = hdr, *sp = m_szHeader;
    int i = 0;
    while (*hp && (i < 8191))
    {
        if (*hp == '\n')
        {
            *sp++ = '\r';
            ++i;
        }

        *sp++ = *hp++;
        ++i;
    }
    *sp = '\0';
}
//---------------------------------------------------------------

void CHeaderDialog::GetHeader(char* hdr)
{
// Store the current header at the destination hdr,
//   compressing "\r\n" to '\n'.

    char *hp = hdr, *sp = m_szHeader;
    int i = 0;

    while (*sp && (i < 4095))
    {
        if (*sp == '\r') ++sp;
        *hp++ = *sp++;
        ++i;
    }
    *hp = '\0';
}
//---------------------------------------------------------------

void CHeaderDialog::GetName(char* name)
{
    strcpy (name, m_szName);
}

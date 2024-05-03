// CSaveOptionsDialog.cpp
//
// Copyright 1998--2024 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#include "stdafx.h"
#include "resource.h"
#include "saveopts.h"
#include "CSaveOptionsDialog.h"

//---------------------------------------------------------------

BOOL CSaveOptionsDialog::OnInitDialog()
{
    if (!CModalDialog::OnInitDialog()) return FALSE;

    m_pButtons[0] = (CButton*)GetDlgItem(IDC_HDR_NONE);
    m_pButtons[1] = (CButton*)GetDlgItem(IDC_HDR_XYPLOT);
    m_pButtons[2] = (CButton*)GetDlgItem(IDC_HDR_USER);

    m_pButtons[3] = (CButton*)GetDlgItem(IDC_DELIM_SPACE);
    m_pButtons[4] = (CButton*)GetDlgItem(IDC_DELIM_TAB);
    m_pButtons[5] = (CButton*)GetDlgItem(IDC_DELIM_COMMA);

    m_pButtons[6] = (CButton*)GetDlgItem(IDC_FORMAT_EXP);
    m_pButtons[7] = (CButton*)GetDlgItem(IDC_FORMAT_FP);
    m_pButtons[8] = (CButton*)GetDlgItem(IDC_FORMAT_INT);
    m_pButtons[9] = (CButton*)GetDlgItem(IDC_CRLF_UNIX);
    m_pButtons[10] = (CButton*)GetDlgItem(IDC_CRLF_DOS);

    int htype = m_pSaveOptions->HeaderType;
    int delim = m_pSaveOptions->Delimiter;
    int fmt = m_pSaveOptions->NumberFormat;
    int crlf = m_pSaveOptions->CrLf;

    char prefix[16];
    strcpy(prefix, m_pSaveOptions->UserPrefix);

    m_pButtons[htype]->SetCheck(TRUE);
    m_pButtons[delim + 3]->SetCheck(TRUE);
    m_pButtons[fmt + 6]->SetCheck(TRUE);
    m_pButtons[crlf + 9]->SetCheck(TRUE);

    SetDlgItemText(IDC_HDR_PREFIX, prefix);

    return TRUE;
}
//---------------------------------------------------------------

void CSaveOptionsDialog::OnOK()
{
    int i;
    for (i = 0; i < 3; i++) if (m_pButtons[i]->GetCheck())
        m_pSaveOptions->HeaderType = i;
    for (i = 3; i < 6; i++) if (m_pButtons[i]->GetCheck())
        m_pSaveOptions->Delimiter = i - 3;
    for (i = 6; i < 9; i++) if (m_pButtons[i]->GetCheck())
        m_pSaveOptions->NumberFormat = i - 6;
    for (i = 9; i < 11; i++) if (m_pButtons[i]->GetCheck())
        m_pSaveOptions->CrLf = i - 9;

    char prefix[16];
    ((CEdit*) GetDlgItem(IDC_HDR_PREFIX))->GetLine(0, prefix, 16);
    strcpy(m_pSaveOptions->UserPrefix, prefix);

    CModalDialog::OnOK();
}


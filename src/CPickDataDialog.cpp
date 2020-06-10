// CPickDataDialog.cpp
//
// Copyright 1995--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#include "stdafx.h"
#include "resource.h"
#include "CPickDataDialog.h"

CString CPickDataDialog::m_szDataList[MAX_SETS];
int CPickDataDialog::m_nSelection = 0;
int CPickDataDialog::m_nSets = 0;

//---------------------------------------------------------------

BOOL CPickDataDialog::OnInitDialog()
{
	if (!CModalDialog::OnInitDialog()) return FALSE;
	m_nSelection = LB_ERR;
	if (m_nSets > 0) {
		for (int i = 0; i < m_nSets; ++i)
			DListBox().AddString(m_szDataList[i]);
	}
	GotoDlgCtrl(GetDlgItem(IDC_PICK_LIST));
	return TRUE;
}
//---------------------------------------------------------------

void CPickDataDialog::OnOK()
{
	m_nSelection = DListBox().GetCurSel();
	CModalDialog::OnOK();
}


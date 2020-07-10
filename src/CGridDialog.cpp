// CGridDialog.cpp
//
// Copyright 1997--2020 Krishna Myneni,
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#include "stdafx.h"
#include <stdlib.h>
#include <vector>
using namespace std;
#include "resource.h"
#include "CDataset.h"
#include "CTransform.h"
#include "CPlotObject.h"
#include "CPlot.h"
#include "CGrid.h"
#include "CGridDialog.h"

//---------------------------------------------------------------

BOOL CGridDialog::OnInitDialog()
{
	if (!CModalDialog::OnInitDialog()) return FALSE;
	int nx, ny, bXlines, bYlines;
	char s[16];
	if (m_pGrid->m_pLines->m_bHorizontal) 
	  HorLinesField()->SetCheck(1);
	if (m_pGrid->m_pLines->m_bVertical)
	  VerLinesField()->SetCheck(1);
	m_pGrid->GetTics(&nx, &ny);
	sprintf (s, "%3d", nx);
	SetDlgItemText(IDC_GRID_XTICS, s);
	sprintf (s, "%3d", ny);
	SetDlgItemText(IDC_GRID_YTICS, s);
//	GotoDlgCtrl(GetDlgItem(IDC_GRID_XTICS));
	GotoDlgCtrl(GetDlgItem(IDC_HOR_GRID_LINES));
	return TRUE;
}
//---------------------------------------------------------------

void CGridDialog::OnOK()
{
    char s[8];

    BOOL bXlines, bYlines, bAxes;
    int nXtics, nYtics, nLstyle;

    bXlines = VerLinesField()->GetCheck();
    bYlines = HorLinesField()->GetCheck();
    m_pGrid->m_pLines->m_bVertical = bXlines;
    m_pGrid->m_pLines->m_bHorizontal = bYlines;

    XticsField()->GetLine(0, s, 6);
    nXtics = atoi(s);
    YticsField()->GetLine(0, s, 6);
    nYtics = atoi(s);

    m_pGrid->SetTics (nXtics, nYtics);
    CModalDialog::OnOK();
}

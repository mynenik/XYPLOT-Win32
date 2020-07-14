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
    int nX, nY;
    bool bXaxis, bYaxis, bXlines, bYlines;
    char s[16];
    m_pGrid->GetAxes(&bXaxis, &bYaxis);
    m_pGrid->GetLines(&bXlines, &bYlines);
    m_pGrid->GetTics(&nX, &nY);
    if (bXaxis) XaxisField()->SetCheck(1);
    if (bYaxis) YaxisField()->SetCheck(1) ;
    if (bYlines) HorLinesField()->SetCheck(1);
    if (bXlines) VerLinesField()->SetCheck(1);
    sprintf (s, "%3d", nX);
    SetDlgItemText(IDC_GRID_XTICS, s);
    sprintf (s, "%3d", nY);
    SetDlgItemText(IDC_GRID_YTICS, s);
//  GotoDlgCtrl(GetDlgItem(IDC_GRID_XTICS));
    GotoDlgCtrl(GetDlgItem(IDC_HOR_GRID_LINES));
    return TRUE;
}
//---------------------------------------------------------------

void CGridDialog::OnOK()
{
    char s[8];

    bool bXlines, bYlines, bXaxis, bYaxis;
    int n, nXtics, nYtics, nLstyle;

    m_pGrid->GetTics(&nXtics, &nYtics);
    bXaxis = XaxisField()->GetCheck();
    bYaxis = YaxisField()->GetCheck();
    bXlines = VerLinesField()->GetCheck();
    bYlines = HorLinesField()->GetCheck();
    m_pGrid->SetAxes(bXaxis, bYaxis);
    m_pGrid->SetLines(bXlines, bYlines);

    XticsField()->GetLine(0, s, 6);
    n = atoi(s);
    if ((n > 0) && (n <= MAX_TICS)) nXtics = n;

    YticsField()->GetLine(0, s, 6);
    n = atoi(s);
    if ((n > 0) && (n <= MAX_TICS)) nYtics = n;
    
    m_pGrid->SetTics (nXtics, nYtics);
    CModalDialog::OnOK();
}

// CGridDialog.cpp
//
// Copyright 1997--2003 Krishna Myneni,
// Creative Consulting for Research & Education
//
// Last Revised: 2003-08-22

#include "stdafx.h"
#include <stdlib.h>
#include <vector>
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
	int nx, ny;
	char s[16];
	if (m_pGrid->m_pLines)
	    LinesField()->SetCheck(1);

	m_pGrid->GetTics(&nx, &ny);
	sprintf (s, "%3d", nx);
	SetDlgItemText(IDC_GRID_XTICS, s);
	sprintf (s, "%3d", ny);
	SetDlgItemText(IDC_GRID_YTICS, s);
//	GotoDlgCtrl(GetDlgItem(IDC_GRID_XTICS));
	GotoDlgCtrl(GetDlgItem(IDC_GRID_LINES));
	return TRUE;
}
//---------------------------------------------------------------

void CGridDialog::OnOK()
{
	char s[255];

    BOOL bLines, bAxes;
    int nXtics, nYtics, nLstyle;

    bLines = LinesField()->GetCheck();
    if (bLines)
    {
        if (! m_pGrid->m_pLines)
        {
            m_pGrid->m_pLines = new CGridLines();
            m_pGrid->m_pLines->SetAttributes(PS_DOT, 1, RGB(0,0,0));
            CGridLines::m_bVertical = TRUE;
            CGridLines::m_bHorizontal = TRUE;

        }
    }
    else
    {
        delete m_pGrid->m_pLines;
        m_pGrid->m_pLines = 0;
        CGridLines::m_bVertical = FALSE;
        CGridLines::m_bHorizontal = FALSE;
    }

	XticsField()->GetLine(0, s, 254);
	nXtics = atoi(s);
	YticsField()->GetLine(0, s, 254);
	nYtics = atoi(s);

	m_pGrid->SetTics (nXtics, nYtics);
	CModalDialog::OnOK();
}

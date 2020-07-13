// CGrid.cpp
//
// Copyright 1995--2020 Krishna Myneni
//
// This software is provided under the terms of the GNU
// Affero General Public License (AGPL) v 3.0 or later.
//
#include "stdafx.h"
#include <vector>
#include <string.h>
using namespace std;
#include "CDataset.h"
#include "CTransform.h"
#include "CPlotObject.h"
#include "CPlot.h"
#include "CGrid.h"

extern char* LabelFormat (float, float, char);

//---------------------------------------------------------------

void CGridFrame::Draw (CDC* pDc)
{
    CRect rect = m_pT->GetPhysical();
    CPen FramePen(m_nStyle, m_nWidth, m_nColor);
    CPen* pOldPen;
    int k;
    int ix1, ix2, iy1, iy2, ixL, iyL, ixT, iyT;

    // Draw the frame box

    pOldPen = pDc->SelectObject (&FramePen);
//  pDc->Rectangle(rect);
    ix1 = rect.TopLeft().x;
    iy1 = rect.TopLeft().y;
    ix2 = rect.BottomRight().x;
    iy2 = rect.BottomRight().y;

    pDc->MoveTo(ix1,iy1);
    pDc->LineTo(ix2, iy1);
    pDc->LineTo(ix2, iy2);
    pDc->LineTo(ix1, iy2);
    pDc->LineTo(ix1, iy1);

    // Draw Tic Marks

    ixL = rect.Width()/50;
    iyL = rect.Height()/50;

    // yL1 = log((double) iy2);
    // yLd = log((double) iy1) - yL1;

    if (nXtics > 0)
      for (k = 0; k <= nXtics; ++k)
        nXt[k] = ix1 + k*rect.Width()/nXtics;

    if (nYtics > 0)
      for (k = 0; k <= nYtics; ++k)
        nYt[k] = iy1 + k*rect.Height()/nYtics;

    if (nXtics > 0)
    {
        for (k = 1; k < nXtics; ++k)
        {
            pDc->MoveTo(nXt[k], iy1);
            pDc->LineTo(nXt[k], iy1+iyL);
            pDc->MoveTo(nXt[k], iy2);
            pDc->LineTo(nXt[k], iy2-iyL);
        }
    }

    if (nYtics > 0) {
        for (k = 1; k < nYtics; ++k) {
       	    pDc->MoveTo(ix1, nYt[k]);
       	    pDc->LineTo(ix1+ixL, nYt[k]);
       	    pDc->MoveTo(ix2, nYt[k]);
       	    pDc->LineTo(ix2-ixL, nYt[k]);
	  }
     }

     pDc->SelectObject(pOldPen);
}
//---------------------------------------------------------------

void CGridLines::Draw (CDC* pDc)
{
    int i, k, ixT, iyT, ix1, ix2, iy1, iy2;
    CRect rect = m_pT->GetPhysical();

    ix1 = rect.TopLeft().x;
    iy1 = rect.TopLeft().y;
    ix2 = rect.BottomRight().x;
    iy2 = rect.BottomRight().y;

    if (m_bVertical)
      {
        if (nXtics > 0)
        {
	  for (k = 1; k < nXtics; ++k)
	    {
	      ixT = nXt[k];
              for (i = iy1; i < iy2; i++)
                if (i % 2) pDc->SetPixel (ixT, i, m_nColor);
            }
        }
    }

  if (m_bHorizontal)
    {
      if (nYtics > 0)
      {
        for (k = 1; k < nYtics; ++k)
        {
            iyT = nYt[k];
            for (i = ix1; i < ix2; i++)
                if (i % 2) pDc->SetPixel(i, iyT, m_nColor);
        }
      }
    }
}
//---------------------------------------------------------------

void CAxes::Draw (CDC* pDc)
{
    float ax[2] = {0., 0.};
    vector<float> x;
    CPoint p;
    CRect r = m_pT->GetPhysical();
    x = m_pT->GetLogical();
    p = m_pT->Physical(ax);

    if (m_bYaxis) {
      if ((x[0] < ax[0]) && (x[1] > ax[0])) {
        pDc->MoveTo(p.x, r.TopLeft().y);
        pDc->LineTo(p.x, r.BottomRight().y);
      }
    }
    if (m_bXaxis) {
      if ((x[2] < ax[1]) && (x[3] > ax[1])) {
        pDc->MoveTo(r.TopLeft().x, p.y);
        pDc->LineTo(r.BottomRight().x, p.y);
      }
    }
}

//---------------------------------------------------------------

CGrid::CGrid()
{
    m_nStyle = PS_DOT;
    m_nColor = RGB(0,0,0);
    m_nFontPointSize = 100;
    m_szFontName = new char[32];
    strcpy (m_szFontName, "Courier New");

    m_pFrame = new CGridFrame ();
    m_pFrame->SetAttributes (PS_SOLID, 1, m_nColor);
    m_pLines = new CGridLines();
    m_pAxes = new CAxes();
    SetTics(10, 10);
    SetLines(false, false);
    SetAxes(true, true);
}
//---------------------------------------------------------------
CGrid::~CGrid()
{
    delete [] m_szFontName;
    delete m_pFrame;
    delete m_pLines;
    delete m_pAxes;
}
//---------------------------------------------------------------

void CGrid::SetTics (int xtics, int ytics)
{
    nXtics = xtics;
    nYtics = ytics;
}

void CGrid::GetTics (int* pXtics, int* pYtics)
{
    *pXtics = nXtics;
    *pYtics = nYtics;
}
//---------------------------------------------------------------

void CGrid::SetLines (bool bVer, bool bHor)
{
  if (m_pLines) {
    m_pLines->m_bVertical = bVer;
    m_pLines->m_bHorizontal = bHor;
  }
}

void CGrid::GetLines (bool* pHor, bool* pVer)
{
  if (m_pLines) {
    *pVer = m_pLines->m_bVertical;
    *pHor = m_pLines->m_bHorizontal;
  }
}

void CGrid::SetAxes (bool bx, bool by)
{
  if (m_pAxes) {
    m_pAxes->SetAttributes (PS_SOLID, 1, m_nColor);
    m_pAxes->m_bXaxis = bx;
    m_pAxes->m_bYaxis = by;
  }
}

void CGrid::GetAxes (bool* pX, bool* pY)
{
  if (m_pAxes) {
    *pX = m_pAxes->m_bXaxis;
    *pY = m_pAxes->m_bYaxis;
  }
}
//---------------------------------------------------------------

void CCartesianGrid::Draw(CDC* pDc)
{
    // Draw the frame and tics
    if (m_pFrame) m_pFrame->Draw(pDc);

    // Draw the grid lines
    if (m_pLines) m_pLines->Draw(pDc);

    // Draw the axes
    if (m_pAxes) m_pAxes->Draw(pDc);

}
//---------------------------------------------------------------

void CCartesianGrid::Labels(CDC* pDc)
{
	int k;
	vector<float> x;
	float xt, yt;
	char xl[20], yl[20];
	char xform[16], yform[16];

	CFont font, *pOldFont;

    if (! font.CreatePointFont(m_nFontPointSize, m_szFontName, pDc))
	{
	    AfxMessageBox("Font not available!");
	}
    pOldFont = pDc->SelectObject(&font);

	TEXTMETRIC tm;
	pDc->GetTextMetrics (&tm);

	CRect rect = m_pT->GetPhysical();
    x = m_pT->GetLogical();

	float xmin = x[0];
	float xmax = x[1];
	float ymin = x[2];
	float ymax = x[3];
	float xdel = (xmax - xmin)/(float) nXtics;
	float ydel = (ymax - ymin)/(float) nYtics;

    strcpy (xform, LabelFormat (xmin, xmax, 'X'));
    strcpy (yform, LabelFormat (ymin, ymax, 'Y'));
	int xl_width = atoi(&xform[1]);
	int yl_width = atoi(&yform[1]);
	if (xl_width < 6) xl_width = 6;
	if (yl_width < 6) yl_width = 6;

    int xl_pixel_width = xl_width*tm.tmAveCharWidth;

    int xkstep = 2;

    while (nXtics*xl_pixel_width/xkstep > rect.Width())
        xkstep *= 2;
    if (xkstep > nXtics) xkstep = nXtics;

// xkstep = (rect.Width()/((float) xlwidth * tm.tmAveCharWidth) > 4.5) ? 2 : 5;

    for (k = 0; k <= nXtics; k += xkstep)
    {
        xt = xmin + k*xdel;
        memset (xl, 32, 20);
        sprintf (xl, xform, xt);

        pDc->TextOut(nXt[k] - xl_pixel_width/2,
          rect.BottomRight().y + tm.tmHeight/2,
          xl, xl_width);

    }

    int ykstep = 2;

    while (nYtics*tm.tmHeight/ykstep > rect.Height()/2)
        ykstep *= 2;
    if (ykstep > nYtics) ykstep = nYtics;

// ykstep = (rect.Height()/(float) tm.tmHeight > 4.5) ? 2 : 5;

    for (k = 0; k <= nYtics; k += ykstep)
    {
        yt = ymax - k*ydel;
        memset (yl, 32, 20);
        sprintf (yl, yform, yt);

        pDc->TextOut(rect.TopLeft().x - (yl_width + 1)*tm.tmAveCharWidth,
          nYt[k] - tm.tmHeight/2, yl, yl_width);

    }

	pDc->SelectObject(pOldFont);
}

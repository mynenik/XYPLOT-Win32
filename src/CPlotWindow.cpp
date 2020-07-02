// CPlotWindow.cpp
//
// Copyright 1996--2020 Krishna Myneni
//
// This software is provided under the terms of the
// GNU Affero General Public License (AGPL) v 3.0 or later.
//
#include "stdafx.h"
#include "consts.h"
#include <math.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <deque>
using namespace std;
#include "CDataset.h"
#include "CTransform.h"
#include "CPlotObject.h"
#include "CPlot.h"
#include "CGrid.h"
#include "CPlotList.h"
#include "saveopts.h"
#include "CXyFile.h"
#include "xyp41.h"
#include "CWorkspace41.h"
#include "CDatabase.h"
#include "CPlotDisplay.h"
#include "resource.h"
#include "CInputDialog.h"
#include "CPickDataDialog.h"
#include "CGridDialog.h"
#include "CHeaderDialog.h"
#include "CVerifyDialog.h"
#include "CSaveOptionsDialog.h"
#include <afxdlgs.h>
#include "CPlotWindow.h"
#include "fbc.h"
#include "ForthCompiler.h"
#include "ForthVM.h"


extern char* CommandLine;
extern CString wClass;
extern void SortRect (CRect *);
extern int NumberParse (float*, char*);
extern COLORREF LookupColor (char*);
extern COLORREF colors_rgb_table[];
extern int CompileAE (vector<BYTE>*, char* exp);
extern int ExecuteForthExpression (char*, ostringstream*, int*, int*);

vector<char*> GetStartupFileList(char*);

static UINT statusID [] = {ID_SEPARATOR, ID_SEPARATOR,
        ID_SEPARATOR};

CString szFilter;
extern int nForthMenuItems;
extern vector<char*> ForthMenuCommandList;

//---------------------------------------------------------------

CPlotWindow::CPlotWindow()
{

    vector<char*> FileList = GetStartupFileList(CommandLine);

    LoadAccelTable ("XyplotAccelTable");

    if (! Create (wClass, "XYPLOT", WS_OVERLAPPEDWINDOW, rectDefault,
      NULL, "XyplotMenu")) {
        AfxMessageBox ("Unable to create window");
    }

    m_pStatusBar = new CStatusBar;
    if (! m_pStatusBar->Create(this))
    {
        AfxMessageBox ("Unable to create status bar");
    }

    if (! m_pStatusBar->SetIndicators (statusID, 3))
    {
        AfxMessageBox ("StatusBar SetIndicators failed.");
    }

    UINT id, style;
    int width;

    m_pStatusBar->GetPaneInfo (1, id, style, width);
    m_pStatusBar->SetPaneInfo (1, id, style, 80);
    m_pStatusBar->GetPaneInfo (2, id, style, width);
    m_pStatusBar->SetPaneInfo (2, id, style, 80);


    CRect consoleRect;
    GetClientRect(&consoleRect);

/*
    m_pForthConsole = new CFrameWnd;
    if (! m_pForthConsole->Create(NULL, "Xyplot Forth Output Console",
        WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL,
        CRect(0, 0, consoleRect.Width(), 100), NULL, NULL, NULL))
    {
        AfxMessageBox ("Unable to create Forth console");
    }
    else
    {
        m_pForthConsole->ShowWindow(SW_SHOW);
    }
*/

	m_pDb = NULL;
	m_pDi = NULL;

    m_pWinBuffer = NULL; // pointer to the shared memory buffer for Win apps
    m_bWinBuffer = FALSE; // indicate ptr is uninitialized

    // OpenForth();        // initialize the Forth virtual machine

	OnFileNew();

    if (FileList.size() > 1)
    {
        vector<char*>::iterator i;

        for (i = FileList.begin() + 1; i < FileList.end(); i++)
        {
            LoadFile(*i);
        }
    }

	szFilter.LoadString(IDS_FILTERSTRING);  // Load the file filters
}

//---------------------------------------------------------------

CPlotWindow::~CPlotWindow()
{
	// Cleanup

    CloseForth();
    // delete m_pForthConsole;
    delete m_pStatusBar;

	delete m_pDb;
	delete m_pDi;
}
//---------------------------------------------------------------

void CPlotWindow::OnMouseMove(UINT mf, CPoint p)
{
    char cur_x [64], cur_y [64];
	vector<float> x;

    m_pDi->m_nMousePt = p;
    x = m_pDi->Logical(p);
    sprintf (cur_x, m_pDi->m_szXform, x[0]);
    sprintf (cur_y, m_pDi->m_szYform, x[1]);
    m_pStatusBar->SetPaneText (1, cur_x);
    m_pStatusBar->SetPaneText (2, cur_y);
}
//---------------------------------------------------------------

void CPlotWindow::OnLButtonDown(UINT mf, CPoint p)
{
	MSG msg;
    CRect rect, ExpRect;

    CDC* pDc = GetDC();
    CPlot* pl = m_pDi->SelectedPlot(p);

    if (pl)
    {
        m_pDi->SetActivePlot(pl);
        m_pDi->DisplayList(pDc);
    }
    else
    {
        ExpRect.TopLeft() = p;
        ExpRect.BottomRight() = p;
        rect = ExpRect;

        while (GetMessage (&msg, NULL, 0, 0))
        {
            if (msg.message == WM_MOUSEMOVE)
            {
                DispatchMessage (&msg);
                ExpRect.BottomRight() = m_pDi->m_nMousePt;
                pDc->DrawFocusRect (&rect);
                rect = ExpRect;
                SortRect (&rect);
                pDc->DrawFocusRect (&rect);
            }
            if (msg.message == WM_LBUTTONUP) break;
	    }

        SortRect (&ExpRect);

        if ((ExpRect.Width() > 9) && (ExpRect.Height() > 9))
        {
            vector<float> x = m_pDi->Logical(ExpRect);
            m_pDi->CreateView(CARTESIAN, x);
	        Invalidate ();
	    }
	}

    ReleaseDC (pDc);

}
//---------------------------------------------------------------

void CPlotWindow::OnRButtonDown(UINT mf, CPoint p)
{

    CPlot* pl = m_pDi->SelectedPlot(p);

    if (pl)
    {
        CDC* pDc = GetDC();
        m_pDi->SetOperandPlot(pl);
        m_pDi->DisplayList(pDc);
        ReleaseDC (pDc);
    }

}
//---------------------------------------------------------------

void CPlotWindow::OnRefresh()
{
    Invalidate();
}

void CPlotWindow::OnBack ()
{
    m_pDi->GoBack();
    Invalidate();
}

void CPlotWindow::OnForward ()
{
    m_pDi->GoForward();
    Invalidate();
}


void CPlotWindow::SetBackgroundColor (char* color_name)
{
  m_cBackgroundColor = LookupColor(color_name);
  Invalidate();
}

void CPlotWindow::SetForegroundColor (char* color_name)
{
  // unsigned fg = m_pDc->GetColor(color_name);
  // pDc->SetForeground(fg);
  // strcpy (m_szForegroundColor, color_name);

  if (m_pDi)
    {
      // CGrid* pGrid = m_pDi->m_pGrid;
      // if (pGrid) pGrid->SetColor(fg);
    }
  Invalidate();
}
//---------------------------------------------------------------


void CPlotWindow::OnDeleteView ()
{
    m_pDi->DeleteView();
    Invalidate();
}
//---------------------------------------------------------------

void CPlotWindow::OnReset ()
{
    m_pDi->ResetExtrema();
    m_pDi->GoHome();
    Invalidate ();
}
//---------------------------------------------------------------

void CPlotWindow::SelectPlot (int plot_index, int select_type)
{
  // Select the specified plot as the active or operand plot
  //   The plot_index starts at zero.
  //   select_type = 0 for operand, 1 for active

  if (plot_index >= 0 && plot_index < m_pDi->Nplots())
    {
      CPlot* p = (*m_pDi)[plot_index];

      if (select_type == 1)
	m_pDi->SetActivePlot(p);
      else
	m_pDi->SetOperandPlot(p);

      // WriteStatusMessage(m_pDi->GetList());
    }
  return ;
}
//---------------------------------------------------------------

void CPlotWindow::OnExtrema ()
{
    MSG msg = *GetCurrentMessage();
    float x[6];
    vector<float> e = m_pDi->GetExtrema();
    char s[255];

    if (msg.lParam == 1)
    {
        // get extrema from win16 shared buffer

        if (m_bWinBuffer)
        {
            double* p = (double *) m_pWinBuffer;
            x[0] = *p++;
            x[1] = *p++;
            x[2] = *p++;
            x[3] = *p;

            // sprintf (s, "xmin = %f  ymin = %f  xmax = %f  ymax = %f",
            //    x[0], x[1], x[2], x[3]);
            // AfxMessageBox(s);
        }
        else
            return;
    }
    else
    {
        CString prompt = "Enter xmin, ymin, xmax, ymax:";
        CString LimitString;

        GetInput (prompt, &LimitString);
        if (LimitString.IsEmpty()) return;

        strcpy (s, (const char *) LimitString);

        x[0] = e[0];
        x[1] = e[2];
        x[2] = e[1];
        x[3] = e[3];

        int n = NumberParse (x, s);

    }

    e[0] = x[0];
    e[1] = x[2];
    e[2] = x[1];
    e[3] = x[3];

    m_pDi->CreateView(CARTESIAN, e);

    Invalidate();

}

//---------------------------------------------------------------

void CPlotWindow::OnPaint()
{
	CRect rect, upd_rect;

	GetUpdateRect (&upd_rect, TRUE);
	if (upd_rect.IsRectEmpty()) return;

	CPaintDC dc(this);
    dc.SetBkColor(m_cBackgroundColor);

    GetClientRect(&rect);

    m_pDi->SetPlotRect(rect, &dc);
    m_pDi->Draw(&dc);
    m_pDi->DisplayList(&dc);
}
//---------------------------------------------------------------

void CPlotWindow::OnPrint()
{
	CDC printDC, screenDC;
	DOCINFO pdoc;

	CPrintDialog pd(FALSE, PD_NOPAGENUMS, NULL);
	if (pd.DoModal() != IDOK) return;
	if (!printDC.Attach(pd.GetPrinterDC()))
	{
	  AfxMessageBox ("Unable to attach to printer DC!");
	  return;
	}
	pdoc.lpszDocName = " Xyplot Output";
	const char* port_name = (const char *) pd.GetPortName();
	pdoc.lpszOutput = (LPCSTR) port_name;

    screenDC.CreateDC ("DISPLAY", NULL, NULL, NULL);
    int sXres = screenDC.GetDeviceCaps (HORZRES);
    int sYres = screenDC.GetDeviceCaps (VERTRES);

    CRect plRect;
    plRect = m_pDi->GetPlotRect();

    int plXres = plRect.Width();
    int plYres = plRect.Height();
    float aspect = plXres/plYres;
    float fx = plXres/(float)sXres;

	int pXres = printDC.GetDeviceCaps (HORZRES);
	int pYres = printDC.GetDeviceCaps (VERTRES);

    int width = pXres*fx;
    int height = width/aspect;
    int x1 = (pXres - width)/2;     // compute coordinates to center on page
    int y1 = (pYres - height)/2;
    int x2 = x1 + width;
    int y2 = y1 + height;

    m_pDi->SetPlotRect(CRect(x1, y1, x2, y2), &printDC);

	if (printDC.StartDoc(&pdoc) < 0)
	{
	  AfxMessageBox ("StartDoc error");
	  return;
	}
	if (printDC.StartPage() < 0)
	{
	  AfxMessageBox ("StartPage error");
	  return;
	}
	m_pDi->Draw(&printDC);
	if (printDC.EndPage() < 0)
	{
	  AfxMessageBox ("EndPage error");
	  return;
	}
	if (printDC.EndDoc() < 0)
	{
	  AfxMessageBox ("EndDoc error");
	  return;
	}
	printDC.Detach();
	Invalidate();
}
//---------------------------------------------------------------

void CPlotWindow::OnCopyBitmap ()
{
    if (! OpenClipboard())
    {
        AfxMessageBox ("Clipboard in use by another application");
        return;
    }

    EmptyClipboard();

    CBitmap* bm = new CBitmap();
    CBitmap *pOldBitmap;
    CDC bmDC;
    CClientDC wDC (this);
    CRect rect;

    GetClientRect (&rect);
    bm->CreateCompatibleBitmap (&wDC, rect.Width(), rect.Height());
    bmDC.CreateCompatibleDC(NULL);
    pOldBitmap = bmDC.SelectObject(bm);

    if (! bmDC.BitBlt (0, 0, rect.Width(), rect.Height(), &wDC,
      0, 0, SRCCOPY))
    {
        AfxMessageBox ("Error creating bitmap");
    }

    if (!SetClipboardData (CF_BITMAP, bm->m_hObject))
    {
        AfxMessageBox ("Error copying to clipboard");
    }
    else
    {
        m_pStatusBar->SetPaneText (0, "Plot copied to clipboard as a bitmap");
    }

    bmDC.SelectObject (pOldBitmap);
    CloseClipboard();
}
//------------------------------------------------------------------

void CPlotWindow::OnCopyMetafile ()
{
    if (! OpenClipboard())
    {
        AfxMessageBox ("Clipboard in use by another application");
        return;
    }

    EmptyClipboard();

    CMetaFileDC mfDC;
    if (mfDC.Create())
    {
//        CClientDC wDC (this);
//        mfDC.SetAttribDC (wDC.m_hDC);
        CRect rect(0,0,1000,1000);
        m_pDi->SetPlotRect(rect, NULL);
        m_pDi->Draw(&mfDC);
    }
    else
    {
        AfxMessageBox ("Metafile creation failed!");
        return;
    }

    METAFILEPICT mfp;
    mfp.mm = MM_ANISOTROPIC;
    mfp.xExt = 5000;
    mfp.yExt = 5000;
    mfp.hMF = mfDC.Close();

    if (!SetClipboardData (CF_METAFILEPICT, &mfp))
    {
        AfxMessageBox ("Error copying to clipboard");
    }
    else
    {
        m_pStatusBar->SetPaneText (0, "Display copied to clipboard as a metafile");
    }
    CloseClipboard();
}
//---------------------------------------------------------------

int CPlotWindow::AddMenuItem(int nMenu, char* name)
{
    char s[80];
    int id = 0;
    CMenu* pTopMenu = this->GetMenu();
    int ncount = pTopMenu->GetMenuItemCount();
    // sprintf (s, "%d items in the top menu", ncount);
    // AfxMessageBox(s);
    CMenu* pMenu = pTopMenu->GetSubMenu(nMenu);
    if (pMenu)
    {
        ncount = pMenu->GetMenuItemCount();
        // sprintf (s, "%d items in submenu %d", ncount, n);
        // AfxMessageBox(s);
        if (pMenu->AppendMenu(MF_STRING, ID_FORTH_MENUITEMS + nForthMenuItems,
          name))
          {
            id = ID_FORTH_MENUITEMS + nForthMenuItems;
            ++nForthMenuItems;
          }
    }
    return id;
}
//---------------------------------------------------------------

void CPlotWindow::OnForthMenuItem ()
{
    MSG* pMsg = (MSG*) this->GetCurrentMessage();
    int id = pMsg->wParam - ID_FORTH_MENUITEMS;

    if (id < 0)
    {
      MessageBox("Invalid Function Number");
      return;
    }

    char* pCommand = ForthMenuCommandList[id];
    if (pCommand == NULL)
    {
        MessageBox("Null Function");
        return;
    }

    int nError, line;
    char out_s[4096];
    memset(out_s, 0, 4096);
    stringstream ForthOutput;

    ExecuteForthExpression (pCommand, (ostringstream*) &ForthOutput, &nError, &line);
    ForthOutput.getline(out_s, 4095, 0);
    if (nError) {
	char s[64];
	if (nError & 0x100)
	   sprintf (s, "Compiler Error %d, line %d\n", nError & 0xFF, line);
	else
	   sprintf (s, "VM Error %d\n", nError);

	 strcat(out_s, s);
     }
     if (strlen(out_s)) MessageBox(out_s);
}
//---------------------------------------------------------------

void CPlotWindow::WriteOutput(char* s)
{
    // Display a string in the Forth Output Console

    static int out_x = 0, out_y = 0;
    int nchars;
    char line[256], *sCurrent = s, *sLast = s;
    while (*sCurrent)
    {
        if (*sCurrent == '\n')
        {
            nchars = (int) (sCurrent - sLast);
            if (nchars > 255) nchars = 255;
            strncpy (line, sLast, nchars);
            line[nchars] = 0;
            sLast= sCurrent + 1;
            // CDC* pDc = m_pForthConsole->GetDC();
            // pDc->TextOut(out_x, out_y, line);
            out_y += 16;
        }
        ++sCurrent;
    }

}
// --------------------------------------------------------------

void CPlotWindow::OnSymbolLine ()
{
    m_pDi->SetSymbol (sym_LINE);
    Invalidate();
}

void CPlotWindow::OnSymbolPoint ()
{
    m_pDi->SetSymbol (sym_POINT);
    Invalidate();
}

void CPlotWindow::OnSymbolBigPoint ()
{
    m_pDi->SetSymbol (sym_BIG_POINT);
    Invalidate();
}

void CPlotWindow::OnSymbolLinePt ()
{
    m_pDi->SetSymbol (sym_LINE_PLUS_POINT);
    Invalidate();
}

void CPlotWindow::OnSymbolStick ()
{
    m_pDi->SetSymbol (sym_STICK);
    Invalidate();
}

void CPlotWindow::OnSymbolHistogram ()
{
    m_pDi->SetSymbol (sym_HISTOGRAM);
    Invalidate();
}

void CPlotWindow::OnSymbolToggle ()
{

    if (m_pDi->GetSymbol() == sym_LINE)
        m_pDi->SetSymbol (sym_POINT);
    else
        m_pDi->SetSymbol (sym_LINE);

    Invalidate();
}
//---------------------------------------------------------------

void CPlotWindow::OnTemplate()
{
    CString tmpl, prompt;

    prompt = "Enter x1, x2, dx:";
    if (GetInput(prompt, &tmpl) == IDCANCEL) return;

    float x[6], temp;
    vector<float> e = m_pDi->GetExtrema();
    x[0] = e[0];
    x[1] = e[1];
    x[2] = (x[1] - x[0])/100.f;

    char s[255];
    strcpy (s, (const char *) tmpl);

    NumberParse (x, s);
    if (x[0] > x[1])
    {
        temp = x[1];
        x[1] = x[0];
        x[0] = temp;
    }

    int npts;

    if (x[2] == 0.)
    {
        npts = 1;
    }
    else
    {
        npts = 1 + (x[1] - x[0])/x[2] ;
    }

    CReal* d = new CReal (2, npts, "Template", "\0");

    if (d == NULL)
    {
        AfxMessageBox ("Unable to create template data set.");
        return;
    }

    float* data = d->begin();
    temp = x[0];
    for (int i = 0; i < npts; i++)
    {
        *data++ = temp;
        *data++ = temp;
        temp += x[2];
    }
    d->SetExtrema();

    m_pDb->AddSet(d);
    m_pDi->MakePlot (d, 0);

    OnReset();

}
//---------------------------------------------------------------

void CPlotWindow::OnExpression()
{
	char s[256], emsg[256];
    *s = 0;
    ostringstream* pSS = NULL;
    int i, *StackPtr;
    BYTE* TypePtr;

    MSG msg = *GetCurrentMessage();
    if (msg.lParam == 1)
    {
        // expression set up by another Win application

        if (m_bWinBuffer)
            strcpy (s, (char*) m_pWinBuffer);

    }
    else
    {
	    CString math_exp, prompt;
	    prompt = "Enter the math expression:";
	    if (GetInput(prompt, &math_exp) == IDCANCEL) return;
	    math_exp.MakeUpper();
	    strcpy (s, (const char*) math_exp);
	}

    // AfxMessageBox(s);

    if (*s)
    {
	    vector<BYTE> opcodes, prefix;
        delete pSS;
	    pSS = new ostringstream(emsg, 256);

	    int ecode = CompileAE (&opcodes, s);

        // OutputForthByteCode (&opcodes, *pSS);
        // AfxMessageBox(emsg);

	    if (ecode)
	    {
	        sprintf (emsg, "%s %d", "Expression Compiler Error:", ecode);
	        AfxMessageBox (emsg);
	    }
	    else
	    {

            // Perform the rescale operation


	        CDataset* d = m_pDi->GetActiveSet();
            int npts = d->NumberOfPoints();
            int nsize = d->SizeOfDatum();
            float *p = d->begin();
            BYTE *bp = (BYTE*) &p;
            prefix.push_back(OP_ADDR);
            for (i = 0; i < sizeof(void*); i++)
                prefix.push_back(*(bp + i));
            bp = (BYTE*) &nsize;
            prefix.push_back(OP_IVAL);
            for (i = 0; i < sizeof(int); i++)
                prefix.push_back(*(bp + i));
            bp = (BYTE*) &npts;
            prefix.push_back(OP_IVAL);
            for (i = 0; i < sizeof(int); i++)
                prefix.push_back(*(bp + i));
            // prefix.push_back(OP_DOTS);
            prefix.push_back(OP_RET);
            SetForthOutputStream(*pSS);
	        ecode = ForthVM (&prefix, &StackPtr, &TypePtr);
	        if (ecode)
	        {
	            sprintf (emsg, "%s %d", "Execution Error:", ecode);
	            AfxMessageBox (emsg);
	        }
	        ecode = ForthVM (&opcodes, &StackPtr, &TypePtr);
	        if (ecode)
	        {
	            sprintf (emsg, "%s %d", "Execution Error:", ecode);
	            AfxMessageBox (emsg);
	        }
	        d->SetExtrema();
            m_pDi->ResetExtrema();
            Invalidate();

	    }
    }
}
//--------------------------------------------------------------

void CPlotWindow::OnGrid()
{
    // Allow user to setup grid style

    MSG msg = *GetCurrentMessage();
    if (msg.lParam == 1)
    {
        // WinApp is sending grid parameters

        if (m_bWinBuffer)
        {
            short int* p = (short int*) m_pWinBuffer;
            int nXtics = *p++;
            int nYtics = *p++;
            int bLines = *p;
            m_pDi->m_pGrid->SetTics(nXtics, nYtics);

            Invalidate();
        }
    }
    else
    {
        CGridDialog gd(this);
        gd.m_pGrid = m_pDi->m_pGrid;   // point to the grid to be modified
        int i = gd.DoModal();
        if (i == IDOK)
        {
            // m_pDi->m_pGrid = gd.m_pGrid;
            Invalidate();
        }
    }
}
//--------------------------------------------------------------

void CPlotWindow::OnSaveOptions()
{
    // Allow user to setup the save options

    SaveOptions s = m_pDb->GetSaveOptions();
    CSaveOptionsDialog sd(&s, this);
    if (sd.DoModal() == IDOK) m_pDb->SetSaveOptions(s);
}
//--------------------------------------------------------------

// static int CPlotWindow::GetInput(CString prompt, CString *response)
int CPlotWindow::GetInput(CString prompt, CString *response)
{
	int i;

	CInputDialog::m_szPromptString = prompt;
	CInputDialog getinp;
	i = getinp.DoModal();
	*response = CInputDialog::m_szResponseString;
	return i;

}
//---------------------------------------------------------------

void CPlotWindow::OnDelete()
{
    CDataset* d = m_pDi->GetActiveSet();
    int i;
    char s[256];

    if (d)
    {
        // Remove the dataset from the database

        m_pDb->RemoveSet(d);

        // Delete all plots of set d

        m_pDi->DeletePlotsOf(d);

        sprintf (s, "Dataset %s deleted from workspace", d->m_szName);
        m_pStatusBar->SetPaneText (0, s);

        // Delete the actual data set

        delete d;

        // Refresh the display

        m_pDi->ResetExtrema();
        Invalidate();
    }


}
//---------------------------------------------------------------

void CPlotWindow::OnDuplicate()
{
    // Duplicate the active data set and make a new plot

    CDataset* d = m_pDi->GetActiveSet();
    if (d)
    {
        CDataset* d2 = d->Duplicate();
        if (d2)
        {
            m_pDb->AddSet(d2);
            m_pDi->MakePlot(d2, 0);
            Invalidate();
        }
        else
        {
            AfxMessageBox ("Dataset Duplication Failed!");
        }
    }
}
//---------------------------------------------------------------

void CPlotWindow::OnPick()
{

    int nsets = m_pDb->Nsets();
    if (nsets == 0) {
      AfxMessageBox ("There are no datasets in the database.");
      return;
    }

    CPickDataDialog::m_nSets = nsets;

    char** sp = new char* [nsets];
    int i;

    for (i = 0; i < nsets; i++) sp[i] = new char [256];

    m_pDb->MakeListOfNames(sp);

    for (i = 0; i < nsets; i++)
    {
        CPickDataDialog::m_szDataList[i] = sp[i];
    }

    CPickDataDialog pick(this);

    if (pick.DoModal() != IDCANCEL) {
      if (CPickDataDialog::m_nSelection != LB_ERR) {
	int nsel = CPickDataDialog::m_nSelection;
	// CDataset* ds = m_pDb->FindInList(sp[n]);
	if ((nsel >= 0) && (nsel < nsets)) {
	  CDataset* ds = (*m_pDb)[nsel];
          m_pDi->MakePlot(ds, 0);
          OnReset();
	}
	else {
	  AfxMessageBox("Selection Error.");
	}
      }
    }

    for (i = 0; i < nsets; i++) delete [] sp[i];
    delete [] sp;
}
//---------------------------------------------------------------

void CPlotWindow::OnDrop()
{
    m_pDi->DeletePlot();
    m_pDi->ResetExtrema();
    Invalidate();
}
//---------------------------------------------------------------

void CPlotWindow::OnColor()
{
	COLORREF c = NULL;
	MSG msg = *GetCurrentMessage();

	if (msg.lParam == 1)
	{
	    if (m_bWinBuffer)
	        c = *((COLORREF*)m_pWinBuffer);
	}
	else
    	{
	    CColorDialog cd;
	    cd.m_cc.Flags |= CC_PREVENTFULLOPEN;
	    cd.m_cc.rgbResult = RGB(0,0,0);
	    cd.m_cc.lpCustColors = &colors_rgb_table[0];
	    if (cd.DoModal() == IDOK)
		    c = cd.GetColor();
	}

	if (c)
	{
        	m_pDi->SetColor(c);
		Invalidate();
	}
}
//---------------------------------------------------------------


void CPlotWindow::OnView2DCartesian()
{
//
    m_pDi->ResetExtrema();
    m_pDi->CreateView (CARTESIAN, m_pDi->GetExtrema());
	Invalidate();
}
//---------------------------------------------------------------

void CPlotWindow::OnView2DPolar()
{

    vector<float> e(4);
    float rmax;

    m_pDi->ResetExtrema();
    e = m_pDi->GetExtrema();
    if (fabs(e[2]) > fabs(e[3])) rmax = e[2]; else rmax = e[3];
    e[0] = -rmax;
    e[1] = rmax;
    e[2] = -rmax;
    e[3] = rmax;

	m_pDi->CreateView (POLAR, e);

	Invalidate();
}
//---------------------------------------------------------------
/*
void CPlotWindow::OnView3DCartesian()
{
	m_pDi->Set3D (CARTESIAN);
	OnReset();
}
*/
//---------------------------------------------------------------

void CPlotWindow::OnAspect()
{
    float x[4], aspect;

    MSG msg = *GetCurrentMessage();
    if (msg.lParam == 1)
    {
        // Aspect ratio specified by Win app

        if (m_bWinBuffer)
            aspect = *((double*) m_pWinBuffer);
    }
    else
    {
        char s[255];
        CString prompt, cs;

        sprintf (s, "Current aspect ratio = %4.2f"
          "\nEnter desired ratio:", m_pDi->GetAspect());
        prompt = s;

        if (GetInput(prompt, &cs) == IDCANCEL) return;
        if (cs.IsEmpty()) return;

        strcpy (s, (const char*) cs);
        if (NumberParse(x, s)) aspect = x[0];
    }

    if ((aspect < 0.1f) || (aspect > 10.f))
    {
        AfxMessageBox ("Illegal aspect ratio");
    }
    else
    {
        m_pDi->SetAspect (aspect);
        Invalidate();
    }

}
//---------------------------------------------------------------
/*
void CPlotWindow::OnRotate()
{
	CString cs, prompt;
	float angles[4];

	prompt = "Enter theta and phi:";

	if (GetInput(prompt, &cs) != IDCANCEL)
	{
	    if (! cs.IsEmpty())
        {
            char s[255];
            strcpy (s, (const char *) cs);

	        if (NumberParse(angles, s))
	            m_pDi->SetViewAngles(angles[0], angles[1]);

    	    Invalidate();
	    }
	}
}
*/
//---------------------------------------------------------------

void CPlotWindow::OnHeader ()
{
    CDataset* d = m_pDi->GetActiveSet();
    if (d)
    {
        CHeaderDialog hd(this);
        hd.SetName(d->m_szName);
        hd.SetHeader(d->m_szHeader);
        int i = hd.DoModal();
        if (i == IDOK)
        {
            if (hd.NameModified()) hd.GetName(d->m_szName);
            if (hd.HeaderModified()) hd.GetHeader(d->m_szHeader);
        }
    }
}
//---------------------------------------------------------------

BOOL CPlotWindow::OnFileNew ()
{
   if (m_pDi != NULL) delete m_pDi;
// AfxMessageBox("Deleted display");
   if (m_pDb != NULL) delete m_pDb;
// AfxMessageBox("Deleted database");
   m_pDb = new CDatabase();	// create database and display
// AfxMessageBox("Created new database");
   m_pDi = new CPlotDisplay();
// AfxMessageBox("Created new display");
    m_pStatusBar->SetPaneText (0, "Workspace cleared.");
    this->SetWindowText("XYPLOT");

    OnReset();
    return TRUE;
}
//---------------------------------------------------------------

void CPlotWindow::OnAbout()
{
    CModalDialog about("AboutBox", NULL);
    about.DoModal();
}
//---------------------------------------------------------------

BOOL CPlotWindow::FileOpenDialog(const char* strFilter, char* strFileName)
{
    CFileDialog fd(TRUE);
    CString LoadFileName;
    static char szf[256];
    int n = strlen(strFilter);
    if (n > 255) {
      AfxMessageBox("Filter string too long!");
      return FALSE;
    }
    strncpy (szf, strFilter, n);
    szf[n] = (char) 0;

    for (int i = 0; i < n; ++i)
       if (szf[i] == '|') szf[i] = '\0';

    fd.m_ofn.lpstrFilter = szf;
    fd.m_ofn.lpstrTitle = "Open File";
    if (fd.DoModal() == IDCANCEL) return FALSE;
    LoadFileName = fd.GetFileName();
    strcpy (strFileName, (const char *) LoadFileName);
    return TRUE;
}

BOOL CPlotWindow::OnFileOpen ()
{
    char szf[256];
    MSG msg = *GetCurrentMessage();

    if (msg.lParam == 1)
    {
        // Win App requests file to be loaded

        if (m_bWinBuffer)
            strcpy (szf, (char const*) m_pWinBuffer);
        else
            *szf = 0;
	return LoadFile(szf);
    }
    else
    {
	char s[256];
	strcpy(s, (const char*) szFilter);
	if (FileOpenDialog( s, szf ))
	  return LoadFile(szf);
    }
    return FALSE;
}
//---------------------------------------------------------------

void CPlotWindow::SelectColumns (char* fname, int nCols)
{
// Provide dialog box to select columns to load from input file
//   Return number of columns selected

    CString cs, prompt;
    char s[255];
    float x[8];
    int i, nSel = 0;

    sprintf (s, "File contains %2d columns"
     "\nSelect columns to load:", nCols);
    prompt = s;

    if (GetInput(prompt, &cs) != IDCANCEL)
    {
        strcpy (s, (const char *) cs);
        LoadDatasetFile (fname, s);
     }
}
//---------------------------------------------------------------

vector<int> CPlotWindow::ParseColumns (char* cs)
{
// Parse a column specifier string into a vector of column numbers

  float x[8];
  int i, nSel = 0;
  char s[64];
  vector<int> Cols;

  if (*cs)
  {
    strcpy (s, cs);
    nSel = NumberParse(x, s);
    for (i = 0; i < nSel; i++) Cols.push_back((int) x[i] - 1);
  }

  return Cols;

}
//---------------------------------------------------------------

void CPlotWindow::LoadDatasetFile (char* fname, char* col_spec)
{
  // Load a dataset file into the database, with the columns
  //   to load specified in the col_spec

  char s[128];
  CDataset* d;

  int ecode = m_pDb->LoadDataset(&d, fname, ParseColumns(col_spec), 0);
  if (ecode <= 0)
    {
      AfxMessageBox ("Error reading file.");
    }
  else
    {
      sprintf (s, "%d points read from %s", ecode, fname);
      m_pStatusBar->SetPaneText(0, s);
      m_pDi->MakePlot(d, 0);
      OnReset();
    }
  // *m_pFileName = '\0';
}
//-------------------------------------------------------------------

BOOL CPlotWindow::LoadFile(char* fname)
{
    int i, ecode, ns;
	char s[128];

    if (*fname)
    {
        strupr(fname);
        CDataset* d;

       	if (strstr(fname, ".XSP"))
       	{
       	    CWorkspace41* pWs;
            ecode = m_pDb->LoadWorkspace (&pWs, fname);
            if (ecode)
            {
                sprintf (s, "XSP FILE CORRUPT");
                AfxMessageBox (s);
            }
            else
            {
                for (i = 0; i < pWs->di.nplots; i++)
                {
                    ns = pWs->pv[i].set - 1;  // array origin is now 0
                    d = m_pDb->FindInList(pWs->ds[ns].fname);
                    if (d) m_pDi->MakePlot(pWs, d, i);
                }
                OnReset();
   	            delete pWs;     // delete the workspace (we're done with it)

            }

        }
        else if (strstr(fname, ".4TH"))
        {
            // Load a Forth source file into the built-in Forth environment

            ecode = LoadForthFile (fname);
        }
        else
        {
            int nCols = m_pDb->GetColumnCount(fname);
            if (nCols < 0)
            {
                AfxMessageBox ("Cannot open input file.");
            }
            else if (nCols == 0)
            {
                AfxMessageBox ("Cannot find first line of data in file.");
            }
            else if (nCols <= 2)
            {
                LoadDatasetFile (fname, "");
            }
            else
            {
                SelectColumns (fname, nCols);
            }

        }
    }

    return FALSE;
}

//---------------------------------------------------------------

BOOL CPlotWindow::OnFileSave ()
{
	CFileDialog fd(FALSE);
	CString szSaveTypes, szFileExt, SaveFileName;
	char szt[256], szf[256];
	int i;
	BOOL success = FALSE;

	szSaveTypes.LoadString(IDS_SAVETYPES);
	strcpy (szt, (const char *) szSaveTypes);

	for (i = 0; i < szSaveTypes.GetLength(); ++i)
	  if (szt[i] == '|') szt[i] = '\0';

	fd.m_ofn.lpstrFilter = szt;

	if (m_pDb->Nsets())
	{
        if (fd.DoModal() != IDCANCEL)
        {
            SaveFileName = fd.GetPathName();
            szFileExt = fd.GetFileExt();
            szFileExt.MakeUpper();
            strcpy (szf, (const char *) SaveFileName);

            if (! SaveFileName.IsEmpty())
            {
                if (szFileExt == "DAT")
                {
	                success = SaveFile (szf);
                }
                else if (szFileExt == "XSP")
                {
                    // success = m_pDb->SaveWorkspace (szf);
                }
                else if (szFileExt == "WMF")
                {
                    success = SaveMetafile (szf);
                }
                else
                {
                    success = SaveFile (szf);
                }
            }
        }
    }

    return success;
}
//---------------------------------------------------------------

BOOL CPlotWindow::SaveFile (char* name)
{
    BOOL success = FALSE;
    CDataset* d = m_pDi->GetActiveSet();
    if (d)
    {
        vector<float> e = m_pDi->GetExtrema();
        int bOverlap;
        vector<int> lim = d->IndexLimits(e[0], e[1], bOverlap);
        if (bOverlap)
        {
            if ((lim[0] > 0) || (lim[1] < (d->NumberOfPoints()-1)))
            {
                CVerifyDialog::m_szPromptString =
                    "Save only data in window domain?";
                CVerifyDialog vd;
                int result = vd.DoModal();
                if (result == IDC_NO)
                {
                    lim[0] = 0;
                    lim[1] = d->NumberOfPoints()-1;
                }

                if (result != IDCANCEL)
                    success = m_pDb->SaveDataset (d, lim, name);
                else
                    m_pStatusBar->SetPaneText (0, "Save cancelled.");
            }
            else
                success = m_pDb->SaveDataset (d, lim, name);

        }
        else
        {
            AfxMessageBox("Active dataset not visible in plot window.");
        }
    }
    return success;
}
//---------------------------------------------------------------

BOOL CPlotWindow::SaveMetafile(char* name)
{

    char tempName[256];
    CRect rect;
    CClientDC wDC (this);
    CMetaFileDC mfDC;

    strcpy (tempName, "tmp.wmf");

    GetClientRect(&rect);
    if (mfDC.Create(tempName))
    {
        mfDC.SetAttribDC (wDC.m_hDC);
        m_pDi->SetPlotRect(rect, NULL);
        m_pDi->Draw(&mfDC);
    }
    else
    {
        AfxMessageBox ("Metafile creation failed!");
        return FALSE;
    }
    HMETAFILE hmf = mfDC.Close();

    // Now, we need to add a header to the file to indicate
    //   that it is a ``placeable'' metafile --- this is for
    //   compatibility with the older 16-bit windows metafiles.

    DWORD key = 0x9ac6cdd7L;
    WORD w = 0;
    WORD x1 = rect.TopLeft().x;
    WORD y1 = rect.TopLeft().y;
    WORD x2 = rect.BottomRight().x;
    WORD y2 = rect.BottomRight().y;
    WORD inch = 576;
    DWORD reserved = 0;
    WORD checksum = key + x1 + y1 + x2 + y2 + inch;

    ofstream ofile (name, ios::binary);
    if (! ofile.fail())
    {
        ofile.write ((char const *) &key, 4);
        ofile.write ((char const *) &w, 2);
        ofile.write ((char const *) &x1, 2);
        ofile.write ((char const *) &y1, 2);
        ofile.write ((char const *) &x2, 2);
        ofile.write ((char const *) &y2, 2);
        ofile.write ((char const *) &inch, 2);
        ofile.write ((char const *) &reserved, 4);
        ofile.write ((char const *) &checksum, 2);

        char ch;

        ifstream ifile (tempName, ios::binary);
        if (! ifile.fail())
        {
            while (1)
            {
                ifile.read(&ch, 1);
                if (ifile.fail()) break;
                ofile.write(&ch, 1);
            }
        }
    }
    else
    {
        AfxMessageBox ("Error opening output file!");
    }

    DeleteMetaFile (hmf);
    return TRUE;

}
//---------------------------------------------------------------

int CPlotWindow::LoadForthFile(char* fname)
{
    char s[256];
    int lnum;
    int *sp;
    BYTE *tp;

    strcpy (s, "include ");
    strcat (s, fname);
    istringstream* pSS = new istringstream (s);

    vector<BYTE> op;
    SetForthInputStream(*pSS);
    int ec = ForthCompiler (&op, &lnum);
    delete pSS;

    if (ec) {
      AfxMessageBox ("Forth Compiler error");
    }
    else {
      if (op.size()) {
         ec = ForthVM (&op, &sp, &tp);
         if (ec) {
            AfxMessageBox ("Forth VM error");
          }
      }
    }
    return ec;
}

//---------------------------------------------------------------

void CPlotWindow::OnSetWinBuffer()
{
// Setup the linear address to share data with Win apps
//   notably, WinForth

    MSG msg = *(GetCurrentMessage());

    if (m_bWinBuffer)
    {
        // reset the linear address

        m_pWinBuffer = NULL;
        m_bWinBuffer = FALSE;
    }

    if (m_pWinBuffer)
    {
        // Set the upper 32 bits of the linear address

        m_pWinBuffer = (void*) (((DWORD) m_pWinBuffer) | (msg.lParam << 16));
        m_bWinBuffer = TRUE;

        char s[64];
        sprintf (s+1, "XYPLOT v1.0W");
        *((BYTE*) m_pWinBuffer) = (BYTE) strlen(s+1); // store counted string
        strcpy ((char*)m_pWinBuffer, s+1);
        // AfxMessageBox(s);
    }
    else
    {
        // Set the lower 16 bits of the linear address

        m_pWinBuffer = (void*) msg.lParam;
    }
}
//---------------------------------------------------------------

void CPlotWindow::OnReceiveWinDataset()
{
// Create a new data set from the Win buffer contents

    if (m_bWinBuffer)
    {

        CDataset* d = m_pDb->MakeDataset ((int*) m_pWinBuffer);

        if (d)
        {
            m_pDi->MakePlot(d, 0);
            m_pDi->ResetExtrema();
            m_pDi->GoHome();
            m_pStatusBar->SetPaneText (0, "Dataset received from Win App.");

            // Don't refresh display until refresh command is explicitly sent

        }
    }
}
//---------------------------------------------------------------

void CPlotWindow::OnSendWinDataset()
{
// Place indicated data set (active or operand) into the Win buffer

    CDataset* d = m_pDi->GetActiveSet();
    if (d)
    {
        if (m_bWinBuffer)
        {
            // Copy 128 bytes of the header

            strncpy ((char*) m_pWinBuffer, d->m_szHeader, 128);

            // Set up the NROWS, NCOLS, NSIZE, and DATA TYPE FIELDS

            BYTE* bp = (BYTE*) m_pWinBuffer;
            int* p = (int*) m_pWinBuffer;
            int nrows = d->NumberOfPoints();
            int ncols = d->SizeOfDatum();
            p = (int*)(bp + 128);
            *p++ = nrows;
            *p++ = ncols;
            *p++ = 256 + nrows*ncols*sizeof(float);
            *p = 5;     // float data type

            // Copy the data

            d->CopyToBuffer((float*)(bp + 256));
            m_pStatusBar->SetPaneText (0, "Dataset sent to Win App.");
        }
    }

    return;
}
//---------------------------------------------------------------

vector<char*> GetStartupFileList (char* cl)
{
// Parse the command line into a vector of filenames.
//  The first element in the vector is the program pathname.

    vector<char*> filelist;
    char* delim = " ,\t\n";
    char* token = cl;

    // Replace all quotes with spaces

    while (*token) { if (*token == '"') *token = ' '; ++token;}

    // Parse into individual tokens

    token = strtok(cl, delim);
    while (token)
    {
        filelist.push_back(token);
        token = strtok(NULL, delim);
    }

    return filelist;
}





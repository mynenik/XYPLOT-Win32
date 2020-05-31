/*
XYPLOT.CPP

Copyright (c) 1995--2003 Creative Consulting for Research & Education

Provided under the terms of the GNU General Public License.

Written by:
        Krishna Myneni
Contributions by:
        John Kielkopf
        Bryan Frazar

Created: 2-1-94
Last Update: 12-8-1998, 2003-08-14

System: Windows 95/98
Developed under Symantec C++ v7.5, using MFC 4.2
*/

#include <math.h>
#include "xyplot.h"
#include "fbc.h"
#include "ForthCompiler.h"
#include "ForthVM.h"


CString wClass;
char* CommandLine;

extern "C" int *GlobalSp;
extern "C" byte *GlobalTp;

#define MAX_COLORS 32
extern COLORREF color_table[];
extern char* color_list[];

int debug = FALSE;                   // global variable for debug mode
volatile int InputData;

// Forth interface constant function pointers

const void* FN_GET_ACTIVE_SET =  get_active_set;
const void* FN_GET_OPERAND_SET = get_operand_set;
const void* FN_GET_ACTIVE_PLOT = get_active_plot;
const void* FN_GET_OPERAND_PLOT = get_operand_plot;
const void* FN_SET_ACTIVE_PLOT = set_active_plot;
const void* FN_SET_OPERAND_PLOT = set_operand_plot;
const void* FN_GET_DS = get_ds;
const void* FN_SET_DS_EXTREMA = set_ds_extrema;
const void* FN_MAKE_DS = make_ds;
const void* FN_GET_PLOT = get_plot;
const void* FN_MAKE_PLOT = make_plot;
const void* FN_SET_PLOT_SYMBOL = set_plot_symbol;
const void* FN_SET_PLOT_COLOR = set_plot_color;
const void* FN_DRAW_PLOT = draw_plot;
const void* FN_SET_GRID_TICS = set_grid_tics;
const void* FN_SET_GRID_LINES = set_grid_lines;
const void* FN_CLEAR_WINDOW = clear_window;
const void* FN_DRAW_WINDOW = draw_window;
const void* FN_RESET_WINDOW = reset_window;
const void* FN_GET_WINDOW_LIMITS = get_window_limits;
const void* FN_SET_WINDOW_LIMITS = set_window_limits;
const void* FN_ADD_MENU_ITEM = add_menu_item;
const void* FN_SET_BACKGROUND = set_background;
const void* FN_SET_FOREGROUND = set_foreground;
const void* FN_GET_INPUT = get_input;

int nForthMenuItems = 0;
vector<char*> ForthMenuCommandList;
CPlotWindow* pMainWnd;

CXyplotApp XyPlot;

BEGIN_MESSAGE_MAP (CPlotWindow, CFrameWnd)
	ON_WM_PAINT ()
	ON_WM_MOUSEMOVE ()
	ON_WM_LBUTTONDOWN ()
	ON_WM_RBUTTONDOWN ()
    ON_COMMAND (ID_FILE_NEW, OnFileNew)
    ON_COMMAND (ID_FILE_OPEN, OnFileOpen)
    ON_COMMAND (ID_FILE_PRINT, OnPrint)
    ON_COMMAND (ID_FILE_SAVE, OnFileSave)
    ON_COMMAND (ID_SAVE_OPTS, OnSaveOptions)
    ON_COMMAND (ID_EDIT_COPY_WMF, OnCopyMetafile)
    ON_COMMAND (ID_EDIT_COPY_BMP, OnCopyBitmap)
    ON_COMMAND (ID_DATA_HDR, OnHeader)
    ON_COMMAND (ID_DATA_DEL, OnDelete)
    ON_COMMAND (ID_DATA_DUP, OnDuplicate)
    ON_COMMAND (ID_VIEW_CARTESIAN, OnView2DCartesian)
    ON_COMMAND (ID_VIEW_POLAR, OnView2DPolar)
    ON_COMMAND (ID_VIEW_ASPECT, OnAspect)
    ON_COMMAND (ID_VIEW_REFRESH, OnRefresh)
    ON_COMMAND (ID_VIEW_BACK, OnBack)
    ON_COMMAND (ID_VIEW_FORWARD, OnForward)
    ON_COMMAND (ID_PLOT_EXTREMA, OnExtrema)
    ON_COMMAND (ID_PLOT_GRID, OnGrid)
    ON_COMMAND (ID_PLOT_PICK, OnPick)
    ON_COMMAND (ID_PLOT_DROP, OnDrop)
    ON_COMMAND (ID_PLOT_COLOR, OnColor)
    ON_COMMAND (ID_SYMBOL_LINE, OnSymbolLine)
    ON_COMMAND (ID_SYMBOL_POINT, OnSymbolPoint)
    ON_COMMAND (ID_SYMBOL_BIGPOINT, OnSymbolBigPoint)
    ON_COMMAND (ID_SYMBOL_LINEPT, OnSymbolLinePt)
    ON_COMMAND (ID_SYMBOL_STICK, OnSymbolStick)
    ON_COMMAND (ID_SYMBOL_HISTOGRAM, OnSymbolHistogram)
    ON_COMMAND (ID_SYMBOL_TOGGLE, OnSymbolToggle)
    ON_COMMAND (ID_MATH_EXPRESSION, OnExpression)
    ON_COMMAND (ID_MATH_TEMPLATE, OnTemplate)
    ON_COMMAND (ID_SET_WIN_BUFFER, OnSetWinBuffer)
    ON_COMMAND (ID_RECEIVE_WIN_DATASET, OnReceiveWinDataset)
    ON_COMMAND (ID_SEND_WIN_DATASET, OnSendWinDataset)
    ON_COMMAND (ID_ESCAPE, OnDeleteView)
    ON_COMMAND (ID_ABOUT, OnAbout)
    ON_COMMAND (ID_FORTH_MENUITEMS +  0, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  1, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  2, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  3, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  4, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  5, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  6, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  7, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  8, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS +  9, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 10, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 11, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 12, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 13, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 14, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 15, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 16, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 17, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 18, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 19, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 20, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 21, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 22, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 23, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 24, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 25, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 26, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 27, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 28, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 29, OnForthMenuItem)
    ON_COMMAND (ID_FORTH_MENUITEMS + 30, OnForthMenuItem)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CInputDialog, CModalDialog)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CPickDataDialog, CModalDialog)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CGridDialog, CModalDialog)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CHeaderDialog, CModalDialog)
	ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CVerifyDialog, CModalDialog)
    ON_COMMAND(IDC_NO, OnNo)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP (CSaveOptionsDialog, CModalDialog)
    ON_COMMAND(IDOK, OnOK)
END_MESSAGE_MAP()

//---------------------------------------------------------------

BOOL CXyplotApp::InitInstance()
{
    // Get the command line info

    CommandLine = GetCommandLine();

	// Register xyplot plot window class

    wClass = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW,
        LoadStandardCursor (IDC_CROSS),
        (HBRUSH) GetStockObject (WHITE_BRUSH),
        LoadIcon ("XYPLOTICON"));

    m_pMainWnd = new CPlotWindow();
    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();

    pMainWnd = (CPlotWindow*) m_pMainWnd;

    // Initialize the Forth environment

    OpenForth();
    InitForthInterface();

    // Load initialization file (xyplot.4th)

    LoadInitializationFile();

    return TRUE;
}


//------------------------------------------------------------------------

int BlankLine (char* s)
{
// Return TRUE if the line is blank

    char* p = s;
    while (*p != '\0')
    {
        if ((*p != ' ') && (*p != '\t') && (*p != '\n')) return 0;
        ++p;
    }

    return 1;
}
//---------------------------------------------------------------

int NumberParse (float* nums, char* s)
{
// Parse a string into array of floats; Return number of values

    const char d1 = 44; // comma
    const char d2 = 9;  // tab
    const char d3 = 32; // space
    char* p = s, *d, *ep = s + strlen(s);

    if (*s == '\0' || *s == '\r' || *s == 27) return 0;
	if (BlankLine(s)) return 0;

	int n = 0;

	do
	{
      d = strchr (p, d1);
      if (!d)
      {
        d = strchr (p, d2);
        if (!d)
        {
          d = strchr (p, d3);
        }
      }

	  if (!d)       // last delimited value
	  {
        nums [n] = atof (p);
        ++n;
        break;
	  }

      if (d == p)
      {
        if (*p == d1 || *p == d2) ++n;
        ++p;
      }
      else
      {
        nums [n] = atof (p);
        ++n;
        p = d + 1;
      }

	} while (p < ep) ;

    return n;

}
//---------------------------------------------------------------

void SortRect (CRect* prect)
{
	CPoint p1 = prect->TopLeft();
	CPoint p2 = prect->BottomRight();

	int i;

	if (p2.x < p1.x)
	{
		i = p1.x;
		p1.x = p2.x;
		p2.x = i;
	}
	if (p2.y < p1.y)
	{
		i = p1.y;
		p1.y = p2.y;
        p2.y = i;
	}
	prect->SetRect (p1.x, p1.y, p2.x, p2.y);
}
//---------------------------------------------------------------------

char* LabelFormat (float x1, float x2, char axis)
{
// Determine label output format for range of numbers between x1 and x2.

    static char format[255];

    float diff = fabs (x2 - x1);
    int j = (x1 < 0.) || (x2 < 0.);

	switch (axis)
	{
	  case 'Y':
        if ((diff < .001f) || (diff >= 100000.f))
          strcpy (format, "%9.1e");
        else if ((diff >= .001f) && (diff < 1.f))
          strcpy (format, "%6.4f");
        else if ((diff >= 1.f) && (diff < 10.f))
          strcpy (format, "%6.3f");
        else if ((diff >= 10.f) && (diff < 100.f))
          strcpy (format, "%6.2f");
        else if ((diff >= 100.f) && (diff < 1000.f))
          strcpy (format, "%6.1f");
        else
          strcpy (format, "%6f");
	    break;
	  case 'X':
        strcpy (format, DisplayFormat(x1, x2));
        break;
      default:
        ;
    }

    return format;

}
//----------------------------------------------------------------

char* DisplayFormat (float x1, float x2)
{
// Determine display format for range of numbers between x1 and x2.

    static char format[255];
    float diff = fabs ((x2 - x1));
    float a1 = fabs (x1);
    float a2 = fabs (x2);
    float vmax = (a2 > a1) ? a2 : a1;
    int field_width = 6;
    int precision = 4;

	if ((diff < .01f) || (diff >= 100000.f))
	  field_width += 6;
	else if ((diff >= .01f) && (diff < .1f))
	  ++precision;
	else if ((diff >= .1f) && (diff < 1.f))
	  ;
	else if ((diff >= 1.f) && (diff < 10.f))
	  --precision;
	else if ((diff >= 10.f) && (diff < 100.f))
	  precision -= 2;
	else if ((diff >= 100.f) && (diff < 1000.f))
	  precision -= 3;
	else
	{
	    field_width = 8;
	    precision = 0;
	}

    float a3 = vmax/diff;
    if (a3 >= 10. && a3 <= 1.e6)
    {
        if (diff != 0.)
        {
            int i = (int) (log(a3)/log(10.)) - 1;
            if ((x1 < 0.) || (x2 < 0.)) i++;
            if (i < 10) field_width += i;
	    }
	}

	if (field_width > 8)
      sprintf (format, "%c%2d.%1de", '%', field_width, precision);
    else
      sprintf (format, "%c%1d.%1df", '%', field_width, precision);

    return format;
}
//--------------------------------------------------------------

COLORREF LookupColor (char* color_name)
{
    // Return the COLORREF (color value) associated with
    //   a recognized color name

    strupr(color_name);

    for (int i = 0; i < MAX_COLORS; i++)
      {
        if (strcmp(color_name, strupr(color_list[i])) == 0)
            return color_table[i];
      }
      return RGB(0,0,0);
}
//--------------------------------------------------------------

void LoadInitializationFile ()
{
// Load the Forth file xyplot.4th, residing in the
// user's HOME/.xyplot directory. This file can
// load other forth files containing initialization scripts.

  char start_dir [512];
  GetCurrentDirectory(511, start_dir); // getcwd (start_dir, 511);

  char* home_dir = start_dir; // char* home_dir = getenv("HOME");
  char path[512];
  strcpy (path, home_dir);
  // strcat (path, "/.xyplot");
  // if (chdir(path) == 0)
  //  {
        LoadForthFile("xyplot.4th");
  //  }
  SetCurrentDirectory(start_dir); // chdir(start_dir);

}
//-------------------------------------------------------------

int AddToHeader (char* text, char* hdr, bool prefix)
{
  // Add text to the beginning of a header string
  // Return non-zero if length exceeded.

  int ecode = 0;
  int new_length = strlen(text) + strlen(hdr);
  if (prefix) new_length += strlen(ANNOTATION_PREFIX);

  if (new_length < HEADER_LENGTH)
    {
      char* temp_str = new char[HEADER_LENGTH];
      if (prefix)
	{
	  strcpy (temp_str, ANNOTATION_PREFIX);
	  strcat (temp_str, text);
	}
      else
	{
	  strcpy (temp_str, text);
	}
      strcat (temp_str, hdr);
      strcpy (hdr, temp_str);
      delete [] temp_str;
    }
  else
    {
      ecode = -1;  // new header will exceed the HEADER_LENGTH
    }

  return ecode;
}
//-------------------------------------------------------------

int ExecuteForthExpression (char* s, ostrstream* pOutput, int* pError, int* pLine)
{
  // Return zero if no error, 1 if compiler error, 2 if VM error

  istrstream* pSS = new istrstream (s);
  int* sp;
  byte* tp;
  vector<byte> op;

  *pLine = 0;
  SetForthInputStream(*pSS);
  SetForthOutputStream(*pOutput);
  *pError = ForthCompiler (&op,pLine);
  delete pSS;

  if (*pError) return 1;

  if (op.size())
    {
      SetForthInputStream(cin);
      *pError = ForthVM (&op, &sp, &tp);
      if (*pError) return 2;
    }

  return 0;
}
//-------------------------------------------------------------

void InitForthInterface ()
{
  char fs[4096], s[64];
  char* cs = " constant ";
  int esrc, ec, lnum;

  sprintf (s, "%lu%sFN_GET_ACTIVE_SET\n", FN_GET_ACTIVE_SET, cs);
  strcpy (fs, s);
  sprintf (s, "%lu%sFN_GET_OPERAND_SET\n", FN_GET_OPERAND_SET, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_ACTIVE_PLOT\n", FN_GET_ACTIVE_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_OPERAND_PLOT\n", FN_GET_OPERAND_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_ACTIVE_PLOT\n", FN_SET_ACTIVE_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_OPERAND_PLOT\n", FN_SET_OPERAND_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_DS\n", FN_GET_DS, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_DS_EXTREMA\n", FN_SET_DS_EXTREMA, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_MAKE_DS\n", FN_MAKE_DS, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_PLOT\n", FN_GET_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_MAKE_PLOT\n", FN_MAKE_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_PLOT_SYMBOL\n", FN_SET_PLOT_SYMBOL, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_PLOT_COLOR\n", FN_SET_PLOT_COLOR, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_DRAW_PLOT\n", FN_DRAW_PLOT, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_GRID_TICS\n", FN_SET_GRID_TICS, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_GRID_LINES\n", FN_SET_GRID_LINES, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_CLEAR_WINDOW\n", FN_CLEAR_WINDOW, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_DRAW_WINDOW\n", FN_DRAW_WINDOW, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_RESET_WINDOW\n", FN_RESET_WINDOW, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_WINDOW_LIMITS\n", FN_GET_WINDOW_LIMITS, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_WINDOW_LIMITS\n", FN_SET_WINDOW_LIMITS, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_ADD_MENU_ITEM\n", FN_ADD_MENU_ITEM, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_BACKGROUND\n", FN_SET_BACKGROUND, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_SET_FOREGROUND\n", FN_SET_FOREGROUND, cs);
  strcat (fs, s);
  sprintf (s, "%lu%sFN_GET_INPUT\n", FN_GET_INPUT, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_FILE\n", pMainWnd->m_nFileMenu, cs);
  sprintf (s, "%lu%sMN_FILE\n", 0, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_EDIT\n", pMainWnd->m_nEditMenu, cs);
  sprintf (s, "%lu%sMN_EDIT\n", 1, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_DATA\n", pMainWnd->m_nDataMenu, cs);
  sprintf (s, "%lu%sMN_DATA\n", 2, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_PLOT\n", pMainWnd->m_nPlotMenu, cs);
  sprintf (s, "%lu%sMN_PLOT\n", 3, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_MATH\n", pMainWnd->m_nMathMenu, cs);
  sprintf (s, "%lu%sMN_MATH\n", 4, cs);
  strcat (fs, s);
//  sprintf (s, "%lu%sMN_HELP\n", pMainWnd->m_nHelpMenu, cs);
  sprintf (s, "%lu%sMN_HELP\n", 5, cs);
  strcat (fs, s);

  char out_s[256];
  memset (out_s, 0, 256);
  ostrstream* pSS = new ostrstream(out_s, 255);

  esrc = ExecuteForthExpression(fs, pSS, &ec, &lnum);
  if (esrc)
    {
      pMainWnd->MessageBox(out_s);
    }
  delete pSS;
}
//-----------------------------------------------------------------

int LoadForthFile(char* fname)
{
  char s[256], out_s[256];
  int esrc, ec, lnum;

  strcpy (s, "include ");
  strcat (s, fname);

  memset (out_s, 0, 256);
  ostrstream* pSS = new ostrstream(out_s, 255);
  esrc = ExecuteForthExpression (s, pSS, &ec, &lnum);

if (esrc)
    {
      pMainWnd->MessageBox(out_s); // pMainWnd->MessageBox (out_s);
    }
    // else pMainWnd->MessageBox("Successfully read xyplot.4th");
  delete pSS;

  return ec;
}

//----------------------------------------------------------------
// WARNING:
//
//  DO NOT CALL the following functions from C, C++, or
//  assembly code. They are intended to be called from the
//  Forth environment only.
//
//----------------------------------------------------------------

int get_active_set ()
{
  // Return the active set index number on top of the Forth stack
  // Return a -1 if error.

  CDataset* d = pMainWnd->m_pDi->GetActiveSet();
  CDatabase* pDb = pMainWnd->m_pDb;

  int nSet = pDb->IndexOf(d);

  // Place the set number on top of the stack

  *GlobalSp = nSet;
  --GlobalSp;
  *GlobalTp = OP_IVAL;
  --GlobalTp;

  return 0;
}
//-----------------------------------------------------------------

int get_operand_set ()
{
  // Return the operand set index on top of the Forth stack; -1 if error

  CDataset* d = pMainWnd->m_pDi->GetOperandSet();
  CDatabase* pDb = pMainWnd->m_pDb;
  int nSet = pDb->IndexOf(d);
  *GlobalSp-- = nSet; *GlobalTp-- = OP_IVAL;
  return 0;
}
//-----------------------------------------------------------------

int get_active_plot ()
{
  // Return the active plot index on top of the stack; -1 if error

  CPlotDisplay* pDi = pMainWnd->m_pDi;
  CPlot* p = pDi->GetActivePlot();
  int nPlot = pDi->IndexOf(p);
  *GlobalSp-- = nPlot; *GlobalTp-- = OP_IVAL;
  return 0;
}
//-----------------------------------------------------------------

int get_operand_plot ()
{
  // Return the operand plot index on top of the stack; -1 if error

  CPlotDisplay* pDi = pMainWnd->m_pDi;
  CPlot* p = pDi->GetOperandPlot();
  int nPlot = pDi->IndexOf(p);
  *GlobalSp-- = nPlot; *GlobalTp-- = OP_IVAL;
  return 0;
}
//-----------------------------------------------------------------

int set_active_plot ()
{
  // Set the active plot to the index on top of the stack

  ++GlobalSp; ++GlobalTp;
  int nPlot = *GlobalSp;
  pMainWnd->SelectPlot(nPlot, 1);

  return 0;
}
//-----------------------------------------------------------------

int set_operand_plot ()
{
  // Set the operand plot to the index on top of the stack

  ++GlobalSp; ++GlobalTp;
  int nPlot = *GlobalSp;
  pMainWnd->SelectPlot(nPlot, 2);

  return 0;
}
//-----------------------------------------------------------------

int get_ds ()
{
  // Return information about the specified dataset

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      // obtain the pointer to the dataset info structure
      unsigned int* ds_info = (unsigned int*) *((int**)GlobalSp);

      ++GlobalSp;
      int nSet = *GlobalSp;
      CDatabase* pDb = pMainWnd->m_pDb;
      int n = pDb->Nsets();

      if (nSet >= 0 && nSet < n)
	{
	  // Fill in the dataset info structure
	  CDataset* d = (*pDb)[nSet];
	  *ds_info++ = (unsigned int) d->m_szName;
	  *ds_info++ = (unsigned int) d->m_szHeader;
	  *ds_info++ = 0;  // set zero for the dataset type
	  *ds_info++ = d->NumberOfPoints();
	  *ds_info++ = d->SizeOfDatum();
	  *ds_info = (unsigned int) d->begin();
	  *GlobalSp = 0;
	}
      else
	{
	  *GlobalSp = -1;
	}
      --GlobalSp;
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for get_ds");
    }
  return 0;
}
//-----------------------------------------------------------------

int set_ds_extrema ()
{
  // Reset extrema for specified dataset

  ++GlobalSp; ++GlobalTp;
  int i = *GlobalSp;
  if (i >= 0 && i < pMainWnd->m_pDb->Nsets())
    {
      CDataset* d = (*(pMainWnd->m_pDb))[i];
      if (d) d->SetExtrema();
    }
  return 0;
}
//-----------------------------------------------------------------

int make_ds ()
{
  // Make a dataset from the dataset info structure passed
  // on top of the Forth stack.

  char s[256];

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      int* ds_info = *((int**) GlobalSp);
      char* name = *((char**) ds_info);
      CDataset* d = pMainWnd->m_pDb->MakeDataset(ds_info);
      if (d)
	{
	  sprintf (s, "Created new dataset %s", name);
	  // pMainWnd->WriteStatusMessage(s);
	}
      else
	{
	  sprintf (s, "Cannot create dataset %s", name);
	  pMainWnd->MessageBox(s);
	}
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for make_ds");
    }
  return 0;
}
//-----------------------------------------------------------------

int get_plot ()
{
  // Return information about the specified plot

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      int* pl_info = *((int**)GlobalSp);
      ++GlobalSp;
      int nPlot = *GlobalSp;
      CPlotDisplay* pDi = pMainWnd->m_pDi;
      CDatabase* pDb = pMainWnd->m_pDb;
      int n = pDi->Nplots();

      if (nPlot >= 0 && nPlot < n)
	{
	  // Fill in the plot info structure
	  CPlot* p = (*pDi)[nPlot];
	  if (p)
	    {
	      CDataset* d = p->GetSet();
	      int nSet = pDb->IndexOf(d);
	      *pl_info++ = nSet;  // dataset index number
	      *pl_info++ = 0;     // set zero for the plot type
	      *pl_info++ = p->GetSymbol();  // plot symbol
	      *pl_info = p->GetColor();   // plot color
	      *GlobalSp = 0;
	    }
	  else
	    *GlobalSp = -1;
	}
      else
	{
	  *GlobalSp = -1;
	}
      --GlobalSp;
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for get_plot");
    }
  return 0;
}
//-----------------------------------------------------------------

int make_plot ()
{
  // Make a new plot

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      CDatabase* pDb = pMainWnd->m_pDb;
      int* pl_info = *((int**) GlobalSp);
      int nSet = *pl_info++;
      if ((nSet >= 0) && (nSet < pDb->Nsets()))
	  {
	    int nType = *pl_info++;
	    CDataset* d = (*pDb)[nSet];
	    if (d)
	      {
		CPlot* p = pMainWnd->m_pDi->MakePlot(d, nType);
		if (p)
		  {
		    Symbol nSym = (enum Symbol) *pl_info++;
		    p->SetSymbol(nSym);
		    int nColor = *pl_info;
		    p->SetColor(nColor);
		  }
	      }
	  }
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for make_plot");
    }
  return 0;
}
//------------------------------------------------------------------

int set_plot_symbol ()
{
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      char* symbol_name = *((char**)GlobalSp);
      ++symbol_name;
      Symbol nSym = pMainWnd->GetSymbol(symbol_name);
      pMainWnd->m_pDi->GetActivePlot()->SetSymbol(nSym);
      pMainWnd->Invalidate();
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for set_plot_symbol");
    }
  return 0;
}
//------------------------------------------------------------------

int set_plot_color ()
{
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      char* color_name = *((char**)GlobalSp);
      ++color_name;
      unsigned c = pMainWnd->GetColor(color_name);
      pMainWnd->m_pDi->GetActivePlot()->SetColor(c);
      pMainWnd->Invalidate();
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for set_plot_color");
    }
  return 0;
}
//------------------------------------------------------------------

int draw_plot ()
{
  ++GlobalSp; ++GlobalTp;
  int vis = *GlobalSp;
  CPlot* p = pMainWnd->m_pDi->GetActivePlot();
  unsigned c = p->GetColor();
  if (! vis)
    {
      unsigned bkg = pMainWnd->GetBackgroundColor();
      p->SetColor(bkg);
    }
  // CDeviceContext* pDc = pMainWnd->GetDC();
  // p->Draw(pDc);
  // XFlush(XtDisplay(pMainWnd->m_nPlotWindow));
  // XSync(XtDisplay(pMainWnd->m_nPlotWindow), False);
  p->SetColor(c);
  return 0;
}
//------------------------------------------------------------------

int set_grid_tics ()
{
  ++GlobalSp; ++GlobalTp;
  int ny = *GlobalSp++; ++GlobalTp;
  int nx = *GlobalSp;

  pMainWnd->m_pDi->m_pGrid->SetTics(nx, ny);
  pMainWnd->Invalidate();
  return 0;
}

int set_grid_lines ()
{
  ++GlobalSp; ++GlobalTp;
  bool by = *GlobalSp++; ++GlobalTp;
  bool bx = *GlobalSp;
  pMainWnd->m_pDi->m_pGrid->SetLines(bx, by);
  pMainWnd->Invalidate();
  return 0;
}

//------------------------------------------------------------------

int clear_window ()
{
  // Clear the plot window

  pMainWnd->Clear();
  return 0;
}

int reset_window ()
{
  // Reset the plot window

  pMainWnd->OnReset();
  return 0;
}

int draw_window ()
{
  // Force drawing of the plot window

  pMainWnd->Invalidate();
  pMainWnd->OnPaint();

  return 0;
}
//-----------------------------------------------------------------

int get_window_limits ()
{

  // Return x1, y1, x2, y2 on top of the stack

  vector<float> e = pMainWnd->m_pDi->GetExtrema();

  --GlobalSp; --GlobalTp;
  *((double*)GlobalSp) = (double) e[0];
  *GlobalTp = OP_FVAL;
  GlobalSp -= 2; GlobalTp -= 2;
  *((double*)GlobalSp) = (double) e[2];
  *GlobalTp = OP_FVAL;
  GlobalSp -= 2; GlobalTp -= 2;
  *((double*)GlobalSp) = (double) e[1];
  *GlobalTp = OP_FVAL;
  GlobalSp -= 2; GlobalTp -= 2;
  *((double*)GlobalSp) = (double) e[3];
  *GlobalTp = OP_FVAL;
  --GlobalSp; --GlobalTp;

  return 0;
}
//-----------------------------------------------------------------

int set_window_limits ()
{
  // Set the plot window limits

  vector<float> e(4);
  ++GlobalSp;
  e[3] = *((double*)GlobalSp);
  GlobalSp += 2;
  e[1] = *((double*)GlobalSp);
  GlobalSp += 2;
  e[2] = *((double*)GlobalSp);
  GlobalSp += 2;
  e[0] = *((double*)GlobalSp);
  ++GlobalSp;
  GlobalTp += 8;

  pMainWnd->m_pDi->CreateView(CARTESIAN, e);
  pMainWnd->Invalidate();

  return 0;
}
//---------------------------------------------------------------

int add_menu_item ()
{
  // Add item to the specified menu.
  // Stack: ( menu item_name forth_command -- )
  //   menu is the menu widget id (e.g. MN_MATH)
  //   item_name is the address of a Forth counted string
  //   forth_command is the address of a Forth counted string

  char name[256];
  char* emsg1 = "add_menu_item: Invalid parameter";
  unsigned char *cp;
//  Widget menu, new_item;
  int menu, new_item;

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp != OP_ADDR)
    {
      pMainWnd->MessageBox(emsg1);
      return 0;
    }
  cp = *((unsigned char**) GlobalSp);
  int nCount = (int) *cp;

  char* fc = new char[256];  // allocate a new command string

  strncpy (fc, (char*) cp+1, nCount);
  fc[nCount] = '\0';
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp != OP_ADDR)
    {
      pMainWnd->MessageBox(emsg1);
      return 0;
    }
  cp = *((unsigned char**) GlobalSp);
  nCount = (int) *cp;
  strncpy (name, (char*) cp+1, nCount);
  name[nCount] = '\0';
  ++GlobalSp; ++GlobalTp;

  menu = *GlobalSp;

//  if (menu == pMainWnd->m_nMathMenu)
  if (menu >= 0 && menu < (pMainWnd->GetMenu())->GetMenuItemCount())
    {
//      new_item = pMainWnd->AddMenuItem (menu, name, MathWidgets);
      new_item = pMainWnd->AddMenuItem (menu, name);
      if (new_item)
	  {
	    ForthMenuCommandList.push_back(fc);
//	    XtAddCallback (new_item, XmNactivateCallback, ForthCB, fc);
	  }
    }
  else
    {
      pMainWnd->MessageBox ("Unknown menu");
    }
  return 0;
}
//---------------------------------------------------------------

int set_background ()
{
  // Stack: ( addr -- | counted string contains color name )

  char color_name[256];

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      char* name = *((char**) GlobalSp);
      unsigned nLen = (unsigned) *name;
      ++name;
      strncpy (color_name, name, nLen);
      color_name[nLen] = '\0';
      pMainWnd->SetBackgroundColor(color_name);
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for set_background");
    }

  return 0;
}
//----------------------------------------------------------------

int set_foreground ()
{
  // Stack: ( addr -- | counted string contains color name )

  char color_name[256];

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      char* name = *((char**) GlobalSp);
      unsigned nLen = (unsigned) *name;
      ++name;
      strncpy (color_name, name, nLen);
      color_name[nLen] = '\0';
      pMainWnd->SetForegroundColor(color_name);
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for set_foreground");
    }

  return 0;
}
//----------------------------------------------------------------

int get_input ()
{
  char* prompt;
  int nc;

  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      prompt = *((char**) GlobalSp);
      nc = (int) *prompt;
      ++prompt;  // counted string on stack
    }
  CString p, response;
  if (nc > 0) p = prompt;
  static char rs[128];
  int retcode;
  if (pMainWnd->GetInput(p, &response) == IDOK)
  {
    if (response.GetLength() > 0)
    {
       strcpy (rs+1, (const char*) response);
       *rs = response.GetLength();
       retcode = TRUE;
    }
  }
  else
  {
    // Cancel button has been pressed
    *rs = 0;
    retcode = 0;
  }
  *GlobalSp-- = (int) rs;
  *GlobalTp-- = OP_ADDR;
  *GlobalSp-- = retcode;
  *GlobalTp-- = OP_IVAL;
  return 0;
}
/*
XYPLOT.CPP

Copyright (c) 1995--2024 Krishna Myneni

Provided under the terms of the GNU Affero General Public License
(AGPL) v 3.0 or later.

Written by:
        Krishna Myneni
Contributions by:
        John Kielkopf
        Bryan Frazar

System: Windows 95/98/NT/XP/7/8/10
*/

#include "stdafx.h"
#include <math.h>
#include <vector>
#include <deque>
#include <sstream>
#include <fstream>
using namespace std;
#include "saveopts.h"
#include "xyp41.h"
#include "resource.h"
#include "CDataset.h"
#include "CXyFile.h"
#include "CWorkspace41.h"
#include "CDatabase.h"
#include "CTransform.h"
#include "CPlotObject.h"
#include "CPlot.h"
#include "CGrid.h"
#include "CXyPlot.h"
#include "CPlotList.h"
#include "CPlotView.h"
#include "CPlotDisplay.h"
#include "CGridDialog.h"
#include "CHeaderDialog.h"
#include "CInputDialog.h"
#include "CPickDataDialog.h"
#include "CSaveOptionsDialog.h"
#include "CVerifyDialog.h"
#include "CPlotWindow.h"
#include "xyplot.h"
#include "fbc.h"
#include "ForthCompiler.h"
#include "ForthVM.h"


CString wClass;
char* CommandLine;

extern "C" int *GlobalSp;
extern "C" byte *GlobalTp;

#define MAX_COLORS 32
#define MAX_COLOR_NAME_LEN 16
extern COLORREF colors_rgb_table[];
extern char* color_names[];

bool debug = FALSE;                   // global variable for debug mode
volatile int InputData;

// Template array for Forth interface functions
        
IfcFuncTemplate IfcFuncList[] = {
        { (const void*) get_color_map,     "FN_GET_COLOR_MAP"    },
        { (const void*) get_active_set,    "FN_GET_ACTIVE_SET"   },
        { (const void*) get_operand_set,   "FN_GET_OPERAND_SET"  },
        { (const void*) get_active_plot,   "FN_GET_ACTIVE_PLOT"  },
        { (const void*) get_operand_plot,  "FN_GET_OPERAND_PLOT" },
        { (const void*) set_active_plot,   "FN_SET_ACTIVE_PLOT"  },
        { (const void*) set_operand_plot,  "FN_SET_OPERAND_PLOT" },
        { (const void*) get_ds,            "FN_GET_DS"           },
        { (const void*) set_ds_extrema,    "FN_SET_DS_EXTREMA"   },
        { (const void*) make_ds,           "FN_MAKE_DS"          },
        { (const void*) get_plot,          "FN_GET_PLOT"         },
        { (const void*) drop_plot,         "FN_DROP_PLOT"        },
        { (const void*) make_plot,         "FN_MAKE_PLOT"        },
        { (const void*) set_plot_symbol,   "FN_SET_PLOT_SYMBOL"  },
        { (const void*) set_plot_color,    "FN_SET_PLOT_COLOR"   },
        { (const void*) set_plot_rgbcolor, "FN_SET_PLOT_RGBCOLOR"},
        { (const void*) draw_plot,         "FN_DRAW_PLOT"        },
        { (const void*) get_grid,          "FN_GET_GRID"         },
        { (const void*) set_grid_tics,     "FN_SET_GRID_TICS"    },
        { (const void*) set_grid_lines,    "FN_SET_GRID_LINES"   },
        { (const void*) get_window_title,  "FN_GET_WINDOW_TITLE" },
        { (const void*) set_window_title,  "FN_SET_WINDOW_TITLE" },
        { (const void*) clear_window,      "FN_CLEAR_WINDOW"     },
        { (const void*) draw_window,       "FN_DRAW_WINDOW"      },
        { (const void*) reset_window,      "FN_RESET_WINDOW"     },
        { (const void*) get_window_limits, "FN_GET_WINDOW_LIMITS" },
        { (const void*) set_window_limits, "FN_SET_WINDOW_LIMITS" },
        { (const void*) add_menu_item,     "FN_ADD_MENU_ITEM"     },
        { (const void*) make_menu,         "FN_MAKE_MENU"         },
        { (const void*) make_submenu,      "FN_MAKE_SUBMENU"      },
        { (const void*) set_background,    "FN_SET_BACKGROUND"    },
        { (const void*) set_foreground,    "FN_SET_FOREGROUND"    },
//        { (const void*) radio_box,         "FN_RADIO_BOX"         },
        { (const void*) message_box,       "FN_MESSAGE_BOX"       },
        { (const void*) get_input,         "FN_GET_INPUT"         },
        { (const void*) file_open_dialog,  "FN_FILE_OPEN_DIALOG"  },
        { (const void*) file_save_dialog,  "FN_FILE_SAVE_DIALOG"  },
        { (const void*) set_save_options,  "FN_SET_SAVE_OPTIONS"  }
};

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
    CommandLine = GetCommandLine();
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

    static char format[16];
    float diff = fabs (x2 - x1);
    int j = (x1 < 0.) || (x2 < 0.);

    switch (axis) {
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
          strcpy (format, "%6.0f");
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

    static char format[16];
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
    else {
      field_width = 8;
      precision = 0;
    }

    if ( diff != 0.) {
      float a3 = vmax/diff;
      if (a3 >= 10. && a3 <= 1.e6) {
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
    char c1[MAX_COLOR_NAME_LEN];
    char c2[MAX_COLOR_NAME_LEN];
    if (strlen(color_name) > MAX_COLOR_NAME_LEN-1) {
      strncpy(c1, color_name, MAX_COLOR_NAME_LEN-1);
      c1[MAX_COLOR_NAME_LEN-1] = (char) 0;
    }
    else
      strcpy(c1, color_name);

    strupr(c1);

    for (int i = 0; i < MAX_COLORS; i++)
      {
        strcpy(c2, color_names[i]);
        if (strcmp(c1, strupr(c2)) == 0)
            return colors_rgb_table[i];
      }
      return RGB(0,0,0);
}
//--------------------------------------------------------------

void LoadInitializationFile ()
{
// Load the Forth file xyplot.4th, located in the path given by
// the environment variable, XYPLOT_DIR. The xyplot.4th file can
// then load other forth files containing initialization scripts.

  char start_dir [512];
  GetCurrentDirectory(511, start_dir);

  LoadForthFile("xyplot.4th");

  SetCurrentDirectory(start_dir);
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

int ExecuteForthExpression (char* s, ostringstream* pOutput, long int* pLine)
{
  // Return zero if no error; otherwise return the Forth error code.

    istringstream* pSS = new istringstream (s);
    long int* sp;
    int nError;
    byte* tp;
    vector<byte> op;

    *pLine = 0;
    SetForthInputStream(*pSS);
    SetForthOutputStream(*pOutput);
    nError = ForthCompiler (&op, pLine);
    delete pSS;

    if (nError) return nError;

    if (op.size()) {
      // SetForthInputStream(cin);
      nError = ForthVM (&op, &sp, &tp);
    }

    return nError;
}
//-------------------------------------------------------------

void InitForthInterface ()
{
    char s[256], out_s[2048];
    const void* funcPtr;
    const char* constName;
    int i, nError, nCount, nDisplay;
    long int lnum;
    size_t nIFCfuncs = sizeof(IfcFuncList) / sizeof(IfcFuncList[0]);
    stringstream* pForthMessages = new stringstream();

    for (i = 0; i < nIFCfuncs; i++) {
      funcPtr = IfcFuncList[i].Function;
      constName = IfcFuncList[i].constantName;
      sprintf (s, "%lu constant %s\n", funcPtr, constName);
      nError = ExecuteForthExpression(s, (ostringstream*) pForthMessages, &lnum);
      if (nError) {
	nCount = pForthMessages->str().size();
	nDisplay = min(nCount, 2047);
        pForthMessages->getline(out_s, nDisplay);
	out_s[nDisplay] = '\0';
        pMainWnd->MessageBox(out_s);
	delete pForthMessages;
        return;
      }
    }

    // pMainWnd->MessageBox("Completed Forth Interface Functions.");
    const char* cs = "  CONSTANT ";
    unsigned long hFileMenu, hEditMenu, hDataMenu, hPlotMenu, hMathMenu, hHelpMenu;
    hFileMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(0)->m_hMenu;
    hEditMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(1)->m_hMenu;
    hDataMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(2)->m_hMenu;
    hPlotMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(3)->m_hMenu;
    hMathMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(4)->m_hMenu;
    hHelpMenu = (unsigned long) pMainWnd->GetMenu()->GetSubMenu(5)->m_hMenu;
    char fs[512];
    fs[0] = '\0'; 
    sprintf (s, "%lu%sMN_FILE\n", hFileMenu, cs);
    strcat (fs, s);
    sprintf (s, "%lu%sMN_EDIT\n", hEditMenu, cs);
    strcat (fs, s);
    sprintf (s, "%lu%sMN_DATA\n", hDataMenu, cs);
    strcat (fs, s);
    sprintf (s, "%lu%sMN_PLOT\n", hPlotMenu, cs);
    strcat (fs, s);
    sprintf (s, "%lu%sMN_MATH\n", hMathMenu, cs);
    strcat (fs, s);
    sprintf (s, "%lu%sMN_HELP\n", hHelpMenu, cs);
    strcat (fs, s);

    // pMainWnd->MessageBox(fs);
    pForthMessages->str("");
    nError = ExecuteForthExpression(fs, (ostringstream*) pForthMessages, &lnum);
    if (nError) {
      // sprintf(out_s, "\nError: %d\n", nError);
      nCount = pForthMessages->str().size();
      nDisplay = min(nCount, 2046);
      pForthMessages->getline(out_s, nDisplay);
      out_s[nDisplay] = '\0';
      pMainWnd->MessageBox(out_s);
    }
    delete pForthMessages;
}
//-----------------------------------------------------------------

int LoadForthFile(char* fname)
{
  char s[512], out_s[1024];
  int nError;
  long int lnum;

  sprintf (s, "include %s\n", fname);
  stringstream* pForthMessages = new stringstream();
  nError = ExecuteForthExpression (s, (ostringstream*) pForthMessages, &lnum);
  // sprintf(out_s, "Command: %sError: %d\n", s, nError);
  // pMainWnd->MessageBox(out_s);
   
  if (nError) {
      int nCount = pForthMessages->str().size();
      // printf(s, "\nError: %d\n", nError);
      int nDisplay = min(nCount, 1023);
      pForthMessages->getline(out_s, nDisplay);
      out_s[nDisplay] = '\0';
      pMainWnd->MessageBox(out_s);
  }
  delete pForthMessages;
  return nError;
}

//----------------------------------------------------------------
// WARNING:
//
//  DO NOT CALL the following functions from C, C++, or
//  assembly code. They are intended to be called from the
//  Forth environment only.
//
//----------------------------------------------------------------

int get_color_map ()
{
   // Fill in the xyplot color rgb values and names in the
   // data arrays
   // stack: ( acolorref acolornames max-name-len max-colors -- ncolors )
   ++GlobalSp; ++GlobalTp;
   int maxcolors = *GlobalSp;
   if (maxcolors <= 0) return 0;
   if (maxcolors > MAX_COLORS) maxcolors = MAX_COLORS;
   ++GlobalSp; ++GlobalTp;
   int max_name_len = *GlobalSp;
   ++GlobalSp; ++GlobalTp;
   if (*GlobalTp != OP_ADDR) return( -1 ); // invalid array
   char* pcolornames = (char*)(*GlobalSp);
   ++GlobalSp; ++GlobalTp;
   if (*GlobalTp != OP_ADDR) return( -1 ); // invalid array
   COLORREF* colorrefs = (COLORREF*)(*GlobalSp);

   *GlobalTp-- = OP_IVAL;
   int i, n;
   char *p = pcolornames;
   for (i = 0; i < maxcolors; i++) {
     *colorrefs = colors_rgb_table[i];
     ++colorrefs;
     char* name = color_names[i];
     n = strlen(name);
     if (n >= max_name_len) n = max_name_len-1;
     strncpy(p, name, n);
     *(p + n) = (char) 0;
     p += max_name_len;
   }
   *GlobalSp-- = maxcolors;
   return 0;
}
// --------------------------------------------------------------

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
  long int i = *GlobalSp;
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
  if (*GlobalTp == OP_ADDR) {
      int* ds_info = *((int**) GlobalSp);
      char* name = *((char**) ds_info);
      CDataset* d = pMainWnd->m_pDb->MakeDataset(ds_info);
      if (d)
	{
	  sprintf (s, "Created new dataset %s", name);
	  // pMainWnd->WriteStatusMessage(s);
	  int n = pMainWnd->m_pDb->Nsets();
	  *GlobalSp-- = n-1;
	}
      else
	{
	  sprintf (s, "Cannot create dataset %s", name);
	  pMainWnd->MessageBox(s);
	  *GlobalSp-- = -1;
	}
    }
  else
    {
      pMainWnd->MessageBox("Invalid parameter for make_ds");
      *GlobalSp-- = -1;
    }
  *GlobalTp-- = OP_IVAL;
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
      long int nPlot = *GlobalSp;
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
	      *pl_info = p->GetColor();   // (COLOREF RGB value)
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

int drop_plot ()
{
   // Drop the active plot
   pMainWnd->OnDrop();
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
		    unsigned int uRGB_Color = *pl_info;
		    p->SetColor(uRGB_Color);
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
  // stack: ( ^color_name -- )
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR)
    {
      char* color_name = *((char**)GlobalSp);
      ++color_name;
      // pMainWnd->MessageBox(color_name);
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

int set_plot_rgbcolor ()
{
  // stack: ( COLORREF -- )
  ++GlobalSp; ++GlobalTp;
  COLORREF c = *((unsigned long *) GlobalSp);
  pMainWnd->m_pDi->GetActivePlot()->SetColor(c);
  pMainWnd->Invalidate();
  return 0;
}
//------------------------------------------------------------------

int draw_plot ()
{
  ++GlobalSp; ++GlobalTp;
  long int vis = *GlobalSp;
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

int get_grid ()
{
   // stack: ( -- nXtics nYtics bXGridLines bYGridLines bXaxis bYaxis )
   int nx, ny;
   bool bXlines, bYlines, bXaxis, bYaxis;
   CPlotView* pView = pMainWnd->m_pDi->GetCurrentView();
   pView->m_pGrid->GetTics(&nx, &ny);
   *GlobalSp-- = nx; *GlobalTp-- = OP_IVAL;
   *GlobalSp-- = ny; *GlobalTp-- = OP_IVAL;
   pView->m_pGrid->GetLines(&bXlines, &bYlines);
   *GlobalSp-- = bXlines; *GlobalTp-- = OP_IVAL;
   *GlobalSp-- = bYlines; *GlobalTp-- = OP_IVAL;
   pView->m_pGrid->GetAxes(&bXaxis, &bYaxis);
   *GlobalSp-- = (int) bXaxis; *GlobalTp-- = OP_IVAL;
   *GlobalSp-- = (int) bYaxis; *GlobalTp-- = OP_IVAL;
   return 0;
}

int set_grid_tics ()
{
  // stack: ( nx ny -- )
  ++GlobalSp; ++GlobalTp;
  int ny = *GlobalSp++; ++GlobalTp;
  int nx = *GlobalSp;
  CPlotView* pView = pMainWnd->m_pDi->GetCurrentView();
  pView->m_pGrid->SetTics(nx, ny);
  pMainWnd->Invalidate();
  return 0;
}

int set_grid_lines ()
{
  // stack: ( bx by -- | bx and by are flags )
  ++GlobalSp; ++GlobalTp;
  bool by = *GlobalSp++; ++GlobalTp;
  bool bx = *GlobalSp;
  CPlotView* pView = pMainWnd->m_pDi->GetCurrentView();
  pView->m_pGrid->SetLines(bx, by);
  pMainWnd->Invalidate();
  return 0;
}
//------------------------------------------------------------------

int get_window_title ()
{
  // stack: ( abuf nmax -- abuf nret )
  char* buf;
  ++GlobalSp; ++GlobalTp;
  int nmax = *((unsigned long *)GlobalSp);
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR) {
    char* buf = *((char**)GlobalSp);
    int nret = pMainWnd->GetWindowText(buf, nmax);
    nret = min(nmax-1, nret);
    buf[nret] = (char) 0;
    --GlobalSp; --GlobalTp;
    *GlobalSp-- = nret; --GlobalTp;
  }
  else {
    --GlobalSp; --GlobalTp;
    *GlobalSp-- = 0; --GlobalTp;
    AfxMessageBox("get_window_title buffer address error!");
  }
  return 0;
}  

int set_window_title ()
{
  // stack: ( c-addr u -- )
  ++GlobalSp; ++GlobalTp;
  int ulen = *GlobalSp;
  ++GlobalSp; ++GlobalTp;
  if (*GlobalTp == OP_ADDR) {
    char* buf = *((char**)GlobalSp);
    buf[ulen] = (char) 0;
    pMainWnd->SetWindowText(buf);
  }
  else {
    AfxMessageBox("set_window_title buffer address error!");
  }
  return 0;
}

int clear_window ()
{
  // stack: ( -- )
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
  // stack: ( -- x1 y1 x2 y2 )

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
  // stack: ( x1 y1 x2 y2 -- )

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
  // Stack: ( menu ^itemname ^forthcommand -- )
  //   menu is the menu widget id (e.g. MN_MATH)
  //   itemname is address of a counted string
  //   forthcommand is address of a counted string

  char name[256];
  char* emsg1 = "add_menu_item: Invalid parameter";
  unsigned char *cp;
//  Widget menu, new_item;
  unsigned long menu;
  int new_item;

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

  new_item = pMainWnd->AddMenuItem (menu, name);
  if (new_item)
	    ForthMenuCommandList.push_back(fc);
  else
      pMainWnd->MessageBox ("Unknown menu");
  
  return 0;
}
//---------------------------------------------------------------

int make_submenu ()
{
    // Create a new menu, attached to a parent menu
    // Stack: ( parent_id ^menu_name -- submenu_id )
    char name[256];

    ++GlobalSp; ++GlobalTp;
    if (*GlobalTp != OP_ADDR) { 
      pMainWnd->MessageBox("make_submenu: Invalid parameter");
      return 0;
    }

    unsigned char *cp = *((unsigned char**) GlobalSp);
    int nCount = (int) *cp;
    ++GlobalSp; ++GlobalTp;

    unsigned int parent_id = (unsigned int) *GlobalSp;
    unsigned int submenu_id = pMainWnd->AddSubMenu(parent_id, (char*) cp+1);

    *GlobalTp-- = OP_IVAL; *GlobalSp-- = submenu_id;
    return 0;
}
//---------------------------------------------------------------

int make_menu ()
{
    // Create a new menu.
    // Stack: ( ^menu_name -- menu_id )

    char name[256];
    ++GlobalSp; ++GlobalTp;
    if (*GlobalTp != OP_ADDR) { 
      pMainWnd->MessageBox("make_menu: Invalid parameter"); 
      return 0;
    }
    unsigned char *cp = *((unsigned char**) GlobalSp);
    int nCount = max((int) *cp, 255);

    strncpy(name, (char*) cp+1, nCount);
    name[nCount] = '\0';

    // Create the menu and return the menu id on the stack
    unsigned long menu_id = pMainWnd->MakeMenu (name);
    *GlobalTp-- = OP_IVAL; *GlobalSp-- = menu_id;

    return 0;
}

int set_background ()
{
  // stack: ( ^colorname -- )

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
  // Stack: ( ^colorname -- )

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

int file_open_dialog ()
{
    // stack: ( ^filter -- ^filename flag )

    char szFileName[256];
    static char szf[256], *cpFilter;
    unsigned nLen;

    ++GlobalSp; ++GlobalTp;
    if (*GlobalTp == OP_ADDR) {
	    cpFilter = *((char**) GlobalSp);
	    unsigned nLen = min((unsigned) *cpFilter, 255);
	    strncpy(szf, cpFilter+1, nLen);
	    szf[nLen] = (char) 0; 
	    if (pMainWnd->FileOpenDialog( (const char*) szf, szFileName )) {
	      nLen = strlen(szFileName);
	      strncpy( szf+1, szFileName, nLen);
	      szf[0] = nLen;
	      szf[nLen+1] = (char) 0;
              *GlobalSp-- = (int) szf; --GlobalTp;
	      *GlobalSp-- = -1; *GlobalTp-- = OP_IVAL;
	    }
	    else {
	      --GlobalSp; --GlobalTp;
	      *GlobalSp-- = 0; *GlobalTp-- = OP_IVAL;
            }
       }
       else {
	       --GlobalSp; --GlobalTp;
	       *GlobalSp-- = 0; *GlobalTp-- = OP_IVAL;
       }

    return 0;
}
//----------------------------------------------------------------

int file_save_dialog ()
{
    // stack: ( ^filter -- ^filename flag )

    CFileDialog fd(TRUE);
    CString SaveFileName;
    static char szFileName[256];
    char szFilter[256], *cpFilter;
    unsigned nLen;

    ++GlobalSp; ++GlobalTp;
    if (*GlobalTp == OP_ADDR) {
            cpFilter = *((char**) GlobalSp);
            unsigned nLen = min((unsigned) *cpFilter, 255);
            strncpy(szFilter, cpFilter+1, nLen);
            szFilter[nLen] = (char) 0;
            for (int i = 0; i < nLen; ++i)
              if (szFilter[i] == '|') szFilter[i] = '\0';
            fd.m_ofn.lpstrFilter = szFilter;
	    fd.m_ofn.lpstrTitle = "Save to File";

            if (fd.DoModal() == IDCANCEL) {
              --GlobalSp; --GlobalTp;
              *GlobalSp-- = 0; *GlobalTp-- = OP_IVAL;
            }
            else {
              SaveFileName = fd.GetFileName();
              szFileName[0] = (unsigned char) SaveFileName.GetLength();
              strcpy (szFileName+1, (const char *) SaveFileName);
              *GlobalSp-- = (int) szFileName; --GlobalTp;
              *GlobalSp-- = -1; *GlobalTp-- = OP_IVAL;
            }
       }
       else {
               --GlobalSp; --GlobalTp;
               *GlobalSp-- = 0; *GlobalTp-- = OP_IVAL;
       }

    return 0;
}
//----------------------------------------------------------------

int set_save_options ()
{
    ++GlobalSp; ++GlobalTp;
    SaveOptions *p = (SaveOptions*)(*GlobalSp);
    SaveOptions s = *p;
    pMainWnd->m_pDb->SetSaveOptions(s);
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
  static char rs[256];
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
// -----------------------------------------------------------------

int message_box ()
{
    // stack: a u --
    ++GlobalSp; ++GlobalTp;
    int nc = *GlobalSp;
    ++GlobalSp; ++GlobalTp;
    if (*GlobalTp == OP_ADDR)
    {
        char message[4096];
        if (nc > 4095) nc = 4095;
        strncpy(message, *((char**) GlobalSp), nc);
        message[nc] = 0;
        pMainWnd->MessageBox(message);
    }
    return 0;
}


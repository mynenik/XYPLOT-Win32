/*
CPlotWindow.h

Header file for the CPlotWindow class

Copyright (c) 1995--2020 Krishna Myneni

This software is provided under the terms of the
GNU Affero General Public License 3.0 or later

*/

#ifndef __CPLOTWINDOW_H__
#define __CPLOTWINDOW_H__

extern COLORREF LookupColor (char*);

class CPlotWindow : public CFrameWnd {
        CStatusBar* m_pStatusBar;
        CFrameWnd* m_pForthConsole;
        COLORREF m_cBackgroundColor;
public:
        CPlotWindow();
        ~CPlotWindow();
        int AddMenuItem(int, char*);
        void MakeMenu(void);
        afx_msg void OnForthMenuItem ();
        afx_msg void OnPaint ();
        afx_msg void OnMouseMove (UINT, CPoint);
        afx_msg void OnLButtonDown (UINT, CPoint);
        afx_msg void OnLButtonUp (UINT, CPoint);
        afx_msg void OnRButtonDown (UINT, CPoint);
        afx_msg BOOL OnFileNew ();
        afx_msg BOOL OnFileOpen ();
        afx_msg BOOL OnFileSave ();
        afx_msg void OnSaveOptions ();
        afx_msg void OnPrint ();
        afx_msg void OnCopyBitmap ();
        afx_msg void OnCopyMetafile ();
        afx_msg void OnHeader ();
        afx_msg void OnDelete ();
        afx_msg void OnDuplicate ();
        afx_msg void OnView2DCartesian ();
        afx_msg void OnView2DPolar ();
//        afx_msg void OnView3DCartesian ();
        afx_msg void OnAspect ();
//        afx_msg void OnRotate ();
        afx_msg void OnRefresh ();
        afx_msg void OnBack ();
        afx_msg void OnForward ();
        afx_msg void OnDeleteView ();
        afx_msg void OnGrid ();
        afx_msg void OnPick ();
        afx_msg void OnDrop ();
        afx_msg void OnColor ();
        afx_msg void OnSymbolLine ();
        afx_msg void OnSymbolPoint ();
        afx_msg void OnSymbolBigPoint ();
        afx_msg void OnSymbolLinePt ();
        afx_msg void OnSymbolStick ();
        afx_msg void OnSymbolHistogram ();
        afx_msg void OnSymbolToggle ();
        afx_msg void OnTemplate ();
        afx_msg void OnExpression ();
        afx_msg void OnAbout ();
        afx_msg void OnReset ();
        afx_msg void OnExtrema ();
        afx_msg void OnSetWinBuffer();
        afx_msg void OnReceiveWinDataset();
        afx_msg void OnSendWinDataset();

        BOOL LoadFile (char*);
        BOOL SaveFile (char*);
        void LoadDatasetFile (char*, char*);
        void SelectColumns (char*, int);
        vector<int> ParseColumns (char*);
        int GetInput (CString, CString *);
        BOOL SaveMetafile (char*);
        int LoadForthFile (char*);
        enum Symbol GetSymbol (char* sym_name) {return (enum Symbol) 0;}
        unsigned GetColor (char* color_name) {return LookupColor(color_name);}
        unsigned GetBackgroundColor (void) {return 0;}
        void SetBackgroundColor (char*) ;
        void SetForegroundColor (char*) ;
        void Clear() {return;}
	    void SelectPlot (int, int);
        void WriteOutput (char*);

        // member variables

        CDatabase *m_pDb;       // pointer to database object
        CPlotDisplay *m_pDi;    // pointer to display object
        void* m_pWinBuffer;     // pointer to the shared memory area with Win apps
        BOOL m_bWinBuffer;      // ptr status

        DECLARE_MESSAGE_MAP()

};

#endif

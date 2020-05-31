// CPlotDisplay.h
//
// Header file for class CPlotDisplay
//
// Copyright 1996-2003 Krishna Myneni
// Creative Consulting for Research & Education
//
// This software is provided under the terms of the
// GNU General Public License
//
// Last Revised: 11-15-1999, 2003-06-15

#ifndef __CPLOTDISPLAY_H__
#define __CPLOTDISPLAY_H__

#include <vector.h>
#include <deque.h>
#include "CPlotList.h"
#include "CPolarGrid.h"
#include "CXyPlot.h"
#include "CWorkspace41.h"

enum COORDINATE_SYSTEM {CARTESIAN, CARTESIAN_INVERTED, POLAR};

class CPlotDisplay {
    deque<CTransform*> m_qCT;    // double ended queue of coordinate transforms
    deque<CTransform*>::iterator m_pCt;  // iterator to current transform
    CPlotList* m_pPlotList;     // the plot list
    float m_fAspect;            // plot display desired aspect ratio
public:
    CGrid* m_pGrid;             // ptr to grid object
    CPoint m_nMousePt;
    char m_szXform[16];
    char m_szYform[16];

	CPlotDisplay ();
	~CPlotDisplay ();
	void CreateView (COORDINATE_SYSTEM, vector<float>);
	void DeleteView ();
    void GoBack();
    void GoForward();
    void GoHome() {m_pCt = m_qCT.begin();}
//	void SetViewAngles (float, float);
    int Nplots() {return m_pPlotList->Nplots();}
	void SetAspect (float aspect) {m_fAspect = aspect;}
	float GetAspect ();
    void SetPlotRect (CRect, CDC*);
    CRect GetPlotRect () {return (**m_pCt).GetPhysical();}
    CPlot* SelectedPlot (CPoint p) {return m_pPlotList->Selection(p);}
    CDataset* GetActiveSet ();
    CDataset* GetOperandSet ();
    CPlot* GetActivePlot () {return m_pPlotList->Active();}
    CPlot* GetOperandPlot () {return m_pPlotList->Operand();}
    BOOL SetActivePlot (CPlot* p) {return m_pPlotList->SetActive(p);}
    BOOL SetOperandPlot (CPlot* p) {return m_pPlotList->SetOperand(p);}
    char* GetList () {return m_pPlotList->GetList();}
    void DisplayList (CDC* pDC) {m_pPlotList->DisplayList(pDC);}
    void Draw (CDC*);
    vector<float> Logical (CPoint p) {return (**m_pCt).Logical(p);}
    vector<float> Logical (CRect r) {return (**m_pCt).Logical(r);}
    void SetExtrema (vector<float> x);
    vector<float> GetExtrema () {return (**m_pCt).GetLogical();}
    void ResetExtrema();
    void Reset();
    CPlot* MakePlot(CDataset*, int) ;
    CPlot* MakePlot (CWorkspace41*, CDataset*, int) ;
    void DeletePlot ();
    void DeletePlotsOf (CDataset*);
    int IndexOf (CPlot*);
    CPlot* operator[] (const int i) { return (*m_pPlotList)[i];}
    int Write (ofstream&);

    void SetColor (COLORREF cr);
    void SetSymbol (Symbol s);
    Symbol GetSymbol () {return m_pPlotList->Active()->GetSymbol();}
};

#endif
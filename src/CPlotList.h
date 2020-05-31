// CPlotList.h
//
// Copyright (c) 1996-2001 Krishna Myneni
// Creative Consulting for Research & Education
//
// This software is provided under the terms of the
// GNU General Public License
//
// Created: 9-5-96
// Last Revised: 1-28-2001
//

#ifndef __CPLOTLIST_H__
#define __CPLOTLIST_H__

#include "CPlot.h"

struct PlotListNode             // node structure for linked list of plots
{
    CPlot* Plot;
    CRgn* Rgn;
    PlotListNode* Prev;
    PlotListNode* Next;
};

class CPlotList
{
    PlotListNode* m_pHead;        // ptr to start of a linked list of plots
    CPlot* m_pActive;
    CPlot* m_pOperand;
    BOOL FindInList (CPlot*);
public:
    CPlotList ();
    ~CPlotList ();
    CPlot* Active() {return m_pActive;}
    CPlot* Operand() {return m_pOperand;}
    CPlot* PlotOf (CDataset*) ;
    CPlot* Selection (CPoint);
    int Nplots ();
    COLORREF NextColor();
    void AddPlot (CPlot*);
    void RemovePlot (CPlot*);
    BOOL SetActive(CPlot*);
    BOOL SetOperand(CPlot*);
    char* GetList();
    void DisplayList (CDC*);
    void Draw (CDC*);
    vector<float> GetExtrema ();
    CPlot* operator[] (const int);
//    BOOL Write (CFile*);
};

#endif

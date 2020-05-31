// CPlot.h
//
// Header file for class CPlot
//
// Copyright 1995--1998 Creative Consulting for Research & Education
//
// Last Revised: 12-8-98

#ifndef __CPLOT_H__
#define __CPLOT_H__

#include "CDataset.h"
#include "CPlotObject.h"

enum Symbol {
    sym_LINE,
    sym_DASHED,
    sym_POINT,
    sym_BIG_POINT,
    sym_LINE_PLUS_POINT,
    sym_STICK,
    sym_HISTOGRAM
};

class CPlot : public CPlotObject {
protected:
    CDataset* m_pSet;                   // ptr to data set
    Symbol m_nSymbol;                   // plotting symbol (e.g. line, point)
public:
	CPlot (CDataset*);
    CDataset* GetSet () {return m_pSet;}
    void SetSymbol (Symbol s) {m_nSymbol = s;}
    Symbol GetSymbol () {return m_nSymbol;}
};


#endif

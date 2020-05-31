// CXyPlot.h
//
// Header file for class CXyPlot
//
// Copyright 1995--2002 Creative Consulting for Research & Education
//
// This software is provided under the GNU General Public License
// Last Revised: 6-14-2002

#ifndef __CXYPLOT_H__
#define __CXYPLOT_H__

#include "CPlot.h"

class CXyPlot : public CPlot {
    int m_nPointWidthScale;
public:
	CXyPlot (CDataset*);
	void Draw (CDC*);
	void DrawPointPlot (CDC*, int);
	void DrawLinePlot (CDC*);
	void DrawStickPlot (CDC*);
	void DrawHistogram (CDC*);
};

#endif

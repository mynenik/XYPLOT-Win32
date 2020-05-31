// CGrid.h
//
// Header file for class CGrid
//
// Copyright 1996--2003 Krishna Myneni
// Creative Consulting for Research & Education
//
// This software is provided under the terms of the GNU
// General Public License.
//
// Last Revised: 6-29-2003

#ifndef __CGRID_H__
#define __CGRID_H__

const int MAX_TICS = 100;

int nXtics;
int nYtics;
int nXt [MAX_TICS];
int nYt [MAX_TICS];

class CGridFrame : public CPlotObject
{
public:
    CGridFrame (int xtics, int ytics);
    void Draw (CDC*);
};

class CGridLines : public CPlotObject
{
public:
    static bool m_bHorizontal;
    static bool m_bVertical;
    void Draw (CDC*);
};

class CAxes : public CPlotObject
{
public:
    void Draw (CDC*);
};

class CGrid : public CPlotObject
{
public:
    CGridFrame* m_pFrame;
    CGridLines* m_pLines;
    CAxes* m_pAxes;
    int m_nFontPointSize;
    char* m_szFontName;

	CGrid ();
	~CGrid ();
	void SetTics (int, int);
	void GetTics (int*, int*);
    void SetLines (bool, bool);
    void SetAxes (bool);

	virtual void Labels (CDC*) = 0;
	virtual void Draw (CDC*) = 0;
};

class CCartesianGrid : public CGrid
{
public:
    void Labels (CDC*);
    void Draw (CDC*);
};

#endif

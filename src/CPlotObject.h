// CPlotObject.h
//
// Header file for class CPlotObject
//
// Copyright 1995-1998 Creative Consulting for Research & Education
//
// Last Revised: 12-14-98


#ifndef __CPLOTOBJECT_H__
#define __CPLOTOBJECT_H__

#include "CTransform.h"

// abstract base class for plotting objects

class CPlotObject
{
protected:
	int m_nStyle;
	int m_nWidth;
	COLORREF m_nColor;
	static CTransform* m_pT;      // transform shared by all plot objects
public:
	CPlotObject (void);
	void SetTransform (CTransform* pCt) {m_pT = pCt;}
	void SetAttributes (int, int, COLORREF);
	void SetColor (COLORREF cr) { m_nColor = cr;}
	COLORREF GetColor (void) {return m_nColor;}
	int GetLineStyle () {return m_nStyle;}
	int GetLineWidth () {return m_nWidth;}
	virtual void Draw (CDC*) = 0;
//	virtual void Write ();
};


#endif
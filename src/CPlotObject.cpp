// CPlotObject.cpp
//
// Copyright 1995--1998 Creative Consulting for Research & Education
//
// Last Revised: 12-14-98

#include "stdafx.h"
#include <vector>
#include "CTransform.h"
#include "CPlotObject.h"

CTransform* CPlotObject::m_pT = NULL;

//---------------------------------------------------------------

CPlotObject::CPlotObject()
{
	m_nStyle = PS_SOLID;
	m_nWidth = 1;
	m_nColor = RGB(0, 0, 0);
}
//---------------------------------------------------------------

void CPlotObject::SetAttributes (int PenStyle, int Width, COLORREF Color)
{
	m_nStyle = PenStyle;
	m_nWidth = Width;
	m_nColor = Color;
}

// CWorkspace41.h
//
// Header file for class CWorkspace41
//
// Copyright 1995--1998 Creative Consulting for Research & Education
//
// Last Revised: 12-20-1998

#ifndef __CWORKSPACE41_H__
#define __CWORKSPACE41_H__

#include "xyp41.h"
#include <fstream.h>

class CWorkspace41 {
public:
	char* m_szHeader;
	struct Database db;
	struct Display41 di;
	struct Dataset* ds;
	struct Plot* pv;
	float * x;
	float * y;
	CWorkspace41 ();
	~CWorkspace41 ();
	int ReadXsp (char*);
	int WriteXsp (char*);
};

#endif

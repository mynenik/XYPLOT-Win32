// CWorkspace41.h
//
// Header file for class CWorkspace41
//
// Copyright 1995--1998 Krishna Myneni
//
// Provided under the terms of the GNU Affro General Public License
// (AGPL) v 3.0 or later.

#ifndef __CWORKSPACE41_H__
#define __CWORKSPACE41_H__

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

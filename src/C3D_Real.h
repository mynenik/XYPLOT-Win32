// C3D_Real.h
//
// Header file for class C3D_Real
//
// Copyright 1995--1997 Creative Consulting for Research & Education
//
// Last Revised: 9-11-97

#ifndef __C3DREAL_H__
#define __C3DREAL_H__

#include "CDataset.h"

class C3D_Real : public CDataset {
public:
	C3D_Real (int, char*, char*);
	void CopyData (void *, int);
	void CopyFromXY (void * x, void * y, int n) {};
    void Extrema();
    CDataset* Duplicate();
	void * operator[] (int);
	int Execute (void*, void*);
	void * Find (void*);
	// void * IncPtr (void *);
	// void * DecPtr (void *);
};

#endif
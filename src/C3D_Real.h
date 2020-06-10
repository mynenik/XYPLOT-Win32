// C3D_Real.h
//
// Header file for class C3D_Real
//
// Copyright 1995--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#ifndef __C3DREAL_H__
#define __C3DREAL_H__

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

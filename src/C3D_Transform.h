// C3D_Transform.h
//
// Header file for class C3D_Transform
//
// Copyright 1995--1997 Creative Consulting for Research & Education
//
// Last Revised: 6-25-97
//

#ifndef __C3DTRANSFORM_H__
#define __C3DTRANSFORM_H__

#include "CTransform.h"

class C3D_Transform : public CTransform {
	double Theta;		// view elevation angle
    double Phi;         // view azimuthal angle
    double R_Matrix [3] [3];
public:
	C3D_Transform();
	void SetView (float*);
	void SetLogical (float*);
	void GetLogical (float*);
	void Logical (CPoint, float*);
	void Logical (CRect, float*);
	CPoint Physical (float*);
	CPoint Physical (double*);
};

#endif
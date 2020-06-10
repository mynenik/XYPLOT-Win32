// C3D_Transform.h
//
// Header file for class C3D_Transform
//
// Copyright 1995--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#ifndef __C3DTRANSFORM_H__
#define __C3DTRANSFORM_H__

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

// C3D_Transform.cpp
//
// Copyright 1995--1997 Creative Consulting for Research & Education
//
// Last Revised: 7-19-97

#include <math.h>
#include "stdafx.h"
#include "CTransform.h"
#include "C3D_Transform.h"

//---------------------------------------------------------------

C3D_Transform::C3D_Transform()
{
	float angles[2] = {45.f, 45.f};
	SetView(angles);	// initialize view angles
}
//---------------------------------------------------------------

void C3D_Transform::SetView (float* angles)
{
	Theta = M_PI*angles[0]/180.f;
	Phi = M_PI*angles[1]/180.f;

    double SinTheta, CosTheta;
    double SinPhi, CosPhi;

	CosTheta = cos(Theta);
	SinTheta = sin(Theta);
	CosPhi = cos(Phi);
	SinPhi = sin(Phi);

	// Set up the rotation matrix
/*
	R_Matrix [0] [0] = CosPhi*CosTheta;
	R_Matrix [0] [1] = -SinPhi;
	R_Matrix [0] [2] = CosPhi*SinTheta;
	R_Matrix [1] [0] = SinPhi*CosTheta;
	R_Matrix [1] [1] = CosPhi;
	R_Matrix [1] [2] = SinPhi*SinTheta;
	R_Matrix [2] [0] = -SinTheta;
	R_Matrix [2] [1] = 0.;
	R_Matrix [2] [2] = CosTheta;
*/
	R_Matrix [0] [0] = CosPhi*CosTheta;
	R_Matrix [0] [1] = -CosTheta*SinPhi;
	R_Matrix [0] [2] = SinTheta;
	R_Matrix [1] [0] = SinPhi;
	R_Matrix [1] [1] = CosPhi;
	R_Matrix [1] [2] = 0.;
	R_Matrix [2] [0] = -SinTheta*CosPhi;
	R_Matrix [2] [1] = -SinTheta*SinPhi;
	R_Matrix [2] [2] = CosTheta;

}
//---------------------------------------------------------------

CPoint C3D_Transform::Physical (float* x)
{
    // Compute physical coordinates of point

    CPoint p(0,0);
    float x_prime [3];
    double sum, rho, mu, phi, mu_prime, z, z_prime;
    int i, j;

    // Multiply the coordinates by the rotation matrix to
    //   obtain the transformed coordinates
/*
    for (i = 0; i < 3; i++)
    {
        sum = 0.;
        for (j = 0; j < 3; j++)
        {
            sum += x[j]*R_Matrix[i][j];
        }
        x_prime[i] = sum;
    }

*/
    if (x[0])
        phi = atan2(x[1], x[0]);
    else
        phi = 0.;
/*
    rho = sqrt(x[0]*x[0] + x[1]*x[1]);
    mu = rho*cos(Phi - phi);
    z = x[3];

    mu_prime = mu*cos(Theta) - z*sin(Theta);
    z_prime = mu*sin(Theta) + z*cos(Theta);


    x_prime[0] = mu_prime*cos(Phi);
    x_prime[1] = mu_prime*sin(Phi);
    x_prime[2] = z_prime;
*/
    x_prime[0] = x[0];
    x_prime[1] = x[1];
    x_prime[2] = x[2];

    p.x = m_nPRect.TopLeft().x +
      (int)((x_prime[0] - m_pEx[0])*(m_nPRect.Width())/m_pDelta[0]);
    p.y = m_nPRect.BottomRight().y -
      (int)((x_prime[1] - m_pEx[2])*(m_nPRect.Height())/m_pDelta[1]);

    return p;

}
//---------------------------------------------------------------

CPoint C3D_Transform::Physical (double* x)
{
    float xx[3] = {x[0], x[1], x[2]};
    return Physical (xx);
}
//---------------------------------------------------------------

void C3D_Transform::SetLogical (float* x)
{
    float rmax = sqrt(x[1]*x[1] + x[3]*x[3]);
	m_pEx[0] = -rmax;
	m_pEx[1] = rmax;
	m_pEx[2] = -rmax;
	m_pEx[3] = rmax;
	m_pDelta[0] = m_pEx[1] - m_pEx[0];
	m_pDelta[1] = m_pEx[3] - m_pEx[2];
}
//---------------------------------------------------------------

void C3D_Transform::GetLogical (float* x)
{
    memcpy (x, m_pEx, 6*sizeof(float));
}
//---------------------------------------------------------------

void C3D_Transform::Logical (CPoint p, float* x)
{
	// Compute logical coordinates of point p


	x[0] = m_pEx[0] + (float)(p.x - m_nPRect.TopLeft().x)*
	  m_pDelta[0]/m_nPRect.Width();
	x[1] = m_pEx[2] + (float)(m_nPRect.BottomRight().y - p.y)*
	  m_pDelta[1]/m_nPRect.Height();

}
//---------------------------------------------------------------

void C3D_Transform::Logical (CRect rect, float* x)
{
	// Compute logical coordinates of rectangle

    float xx[2], temp;

	Logical (rect.BottomRight(), xx);	// xmax, ymin
	Logical (rect.TopLeft(), x);		// xmin, ymax
	temp = x[1];
	x[1] = xx[0];
	x[2] = xx[1];
	x[3] = temp;
}



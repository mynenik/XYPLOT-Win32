// C3D_Real.cpp
//
// Copyright 1995--1997 Creative Consulting for Research & Education
//
// Last Revised: 9-14-97

#include <string.h>
#include "ExpressionCompiler.h"
#include "CDataset.h"
#include "C3D_Real.h"

C3D_Real::C3D_Real(int npts, char* name, char* hdr)
	: CDataset((long) 3*npts*sizeof(float), name, hdr)
{
	if (m_pData == NULL) return;
	m_nNpts = npts;
	m_pEx = new float[6];
}
//---------------------------------------------------------------

void * C3D_Real::operator[] (int i)
{
	// return pointer to the ith element

	float* data = m_pData;
	if (i > 0) data += 3*i;
	return data;
}
//---------------------------------------------------------------

void* C3D_Real::Find (void* pX)
{
    return m_pData; // dummy function for now
}
//---------------------------------------------------------------

CDataset* C3D_Real::Duplicate()
{
    // Create a new 3d real dataset, duplicate its contents
    //  and return a ptr to it

    C3D_Real* ds = new C3D_Real (m_nNpts, m_szName, m_szHeader);
    ds->CopyData (m_pData, m_nNpts);
    memcpy (ds->m_pEx, m_pEx, 6*sizeof(float));
    return ((CDataset*) ds);
}
//---------------------------------------------------------------

void C3D_Real::CopyData(void* data, int npts)
{

	memcpy (m_pData, data, npts*3*sizeof(float));
}
//-------------------------------------------------------------------------

void C3D_Real::Extrema()
{
	// Determine extrema for data set;

	int i;
	float* data = m_pData;
	float* e = m_pEx;

	e[0] = *data;
	e[1] = *data;
	++data;
	e[2] = *data;
	e[3] = *data;
	++data;
	e[4] = *data;
	e[5] = *data;

	for (i = 1; i < m_nNpts; ++i) {
		++data;
		if (*data < e[0]) e[0] = *data;
		if (*data > e[1]) e[1] = *data;
		++data;
		if (*data < e[2]) e[2] = *data;
		if (*data > e[3]) e[3] = *data;
		++data;
	}

}
//-------------------------------------------------------------------------

int C3D_Real::Execute (void* op_c, void* op_v)
{
// Execute compiled expression; return error code
//   0 = no error;

    int ecode, nOffset;
    int* ArgMap = new int [6];

    ArgMap [0] = 0; // index of X in datum
    ArgMap [1] = 1; // index of Y in datum
    ArgMap [2] = 2; // index of Z in datum
    nOffset = 3;    // pointer offset to next datum

    ecode = ExecuteCode ((unsigned char*) op_c, (float*) op_v,
        (float*) m_pData, nOffset, m_nNpts, ArgMap);

    Extrema();
    return ecode;

}

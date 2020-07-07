// CDataset.h
//
// Header file for abstract base class CDataset
//
// Copyright (c) 1995--2020 Krishna Myneni
//
// This software is provided under the terms of the
// GNU Affero General Public License (AGPL) v 3.0 or later.
//

#ifndef __CDATASET_H__
#define __CDATASET_H__

const int HEADER_LENGTH = 4096;
const int NAME_LENGTH = 256;

class CDataset : public vector<float>
{
protected:
	vector<float> m_vEx;		// the extrema vector
	int m_nDatumLength;
	int m_nOrdering;
	int m_nEquallySpaced;
public:
	char* m_szName;
	char* m_szHeader;
	CDataset (int, int, char*, char*);
	~CDataset ();
	int NumberOfPoints() {return size()/m_nDatumLength;}
	int SizeOfDatum() {return m_nDatumLength;}
	void SetExtrema();
	vector<float> GetExtrema() {return m_vEx;}
	void CopyData (float*, int);
//	void CopyData (vector<float>::iterator, int);
	void CopyData (float*, int, int);
	void CopyFromXY (float*, float*, int);
	void CopyFromXY (float*, float*, int, int);
	void CopyToBuffer (float* );
	bool AppendToHeader (char* );
	vector<float>::iterator Find(float);
	int IsOverlapping (float, float);
	vector<vector<float>::iterator> Limits (float, float, int&);
	vector<int> IndexLimits (float, float, int&);
	virtual CDataset* Duplicate()  = 0;
// void Sort();
// void Prune();
};

class CReal : public CDataset
{
public:
    CReal (int, int, char*, char*);
    CDataset* Duplicate();
};

class CComplex : public CDataset {
public:
    CComplex (int, int, char*, char*);
    CDataset* Duplicate();
};

#endif

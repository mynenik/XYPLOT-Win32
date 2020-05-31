// CXyFile.h
//
// Header file for class CXyFile
//
// Copyright 1995--1998 Creative Consulting for Research & Education
//
// Last Revised: 12-11-1998

#ifndef __CXYFILE_H__
#define __CXYFILE_H__

#define HEADER_BEGIN    "\057*"
#define HEADER_END      "*/"
#define MAX_LINE_LENGTH 4096
#define MAX_DATA_COLUMNS 64

class CXyFile {
	char* m_szName;
	char* m_szFirstLine;
	SaveOptions m_nSave;
	ifstream* m_pInFile;
	ofstream* m_pOutFile;
	int m_nFail;
	int m_nCols;
	int m_bReadData;
	float* m_pValues;
public:
	char* m_szHeader;

	CXyFile (char*, int);
	~CXyFile (void);
	void ReadHeader (void);
	void WriteHeader (char*);
	int Columns (void);
	int ReadData (float*, int, int []);
	int WriteData (float*, int, int);
	int Fail () ;
	void SetSaveOptions (SaveOptions s) {m_nSave = s;}
};

extern int BlankLine (char*);
extern int NumberParse (float*, char*);
#endif

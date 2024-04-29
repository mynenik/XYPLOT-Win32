// CXyFile.h
//
// Header file for class CXyFile
//
// Copyright 1995--2024 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public
// License (AGPL) v 3.0 or later.

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
        void WriteHeaderLines(char*, char*, char*);
	int Columns (void);
	int ReadData (float*, int, int []);
	int WriteData (float*, int, int);
	int Fail () ;
	void SetSaveOptions (SaveOptions s) {m_nSave = s;}
        void GetFormatStrings(char*, char*, char*, char*);
};

extern int BlankLine (char*);
extern int NumberParse (float*, char*);
#endif

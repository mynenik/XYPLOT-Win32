// CDatabase.h
//
// Header file for class CDatabase
//
// Copyright 1995-2020 Krishna Myneni
//
// This software is provided under the terms of the GNU Affero General
// Public License (AGPL) v 3.0 or later.
//
#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#define byte unsigned char

class CDatabase
{
  int m_bMem;
  vector<CDataset*> m_nSets;
  SaveOptions m_nSave;
public:
  CDatabase ();
  ~CDatabase (void);
  int Nsets() {return m_nSets.size();}
  void AddSet (CDataset* d) {m_nSets.push_back(d);}
  void RemoveSet (CDataset*);
  int IndexOf (CDataset*);
  CDataset* FindInList (char*);
  int MakeListOfNames (char**);
  void Clear();           // clear the database
  CDataset* MakeDataset (vector<float*>, int, int, int, char*, char*);
  CDataset* MakeDataset (CWorkspace41*, int);
  CDataset* MakeDataset (int*);
  int GetColumnCount (char*);
  int LoadDataset (CDataset**, char*, vector<int>, int);
  int LoadWorkspace (CWorkspace41**, char*);
  int SaveDataset (CDataset*, vector<int>, char*);
  SaveOptions GetSaveOptions () {return m_nSave;}
  void SetSaveOptions (SaveOptions s) {m_nSave = s;}
  CDataset* operator[] (const int i) {return m_nSets[i];}
  int Write (ofstream&);
};

#endif








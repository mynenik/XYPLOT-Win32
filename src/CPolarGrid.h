// CPolarGrid.h
//
// Header file for class CPolarGrid
//
// Copyright 1996 Creative Consulting for Research & Education
//
// Last Revised: 6-29-96

#ifndef __CPOLARGRID_H__
#define __CPOLARGRID_H__

#include "CGrid.h"

class CPolarGrid : public CGrid {
public:
	CPolarGrid ();
	void Labels (CDC*);
	void Draw (CDC*);
};

#endif
// CPolarGrid.h
//
// Header file for class CPolarGrid
//
// Copyright 1996--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#ifndef __CPOLARGRID_H__
#define __CPOLARGRID_H__

class CPolarGrid : public CGrid {
public:
	CPolarGrid ();
	void Labels (CDC*);
	void Draw (CDC*);
};

#endif

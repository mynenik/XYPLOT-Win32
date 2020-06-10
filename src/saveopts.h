// saveopts.h
//
// The save options structure
//
// Copyright (c) 1996--2020 Krishna Myneni
//
// Provided under the terms of the GNU Affero General Public License
// (AGPL) v 3.0 or later.

#ifndef __SAVEOPTS_H__
#define __SAVEOPTS_H__


struct SaveOptions
{
    int HeaderType;
    int Delimiter;
    int NumberFormat;
    int CrLf;
    char UserPrefix [16];
};

#endif

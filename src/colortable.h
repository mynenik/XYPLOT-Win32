// colortable.h
//
// default colors for xyplot
//
// Created: 10-17-97
// Last Revised: 6-29-2003
//

#ifndef __COLORTABLE_H__
#define __COLORTABLE_H__

#include<windows.h>

#define MAX_COLORS 32

COLORREF color_table[] =
{
    RGB(0,0,255),
    RGB(255,0,0),
    RGB(0,255,128),
    RGB(128,0,255),
    RGB(128,255,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(0,0,0),
    RGB(255,255,255)
};

char* color_list[] =
  {"blue", "red", "forestgreen", "turquoise",
   "magenta", "sienna", "gold", "black",
   "orange", "seagreen", "darkslategray", "coral",
   "plum", "aquamarine", "steelblue", "orchid",
   "salmon", "midnightblue", "darkgreen", "cyan",
   "navyblue", "thistle", "yellow", "blueviolet",
   "brown", "maroon", "yellowgreen", "indianred",
   "skyblue", "darkgrey", "wheat", "white"};


#endif
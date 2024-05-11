/*
xyplot.h

Header file for XYPLOT (C++/Windows 95 version)

Copyright (c) 1995--2024 Krishna Myneni

Provided under the terms of the GNU Affero General Public
License (AGPL) v 3.0 or later.
*/

#define ANNOTATION_PREFIX "==XYPLOT==> "

// The application class

class CXyplotApp : public CWinApp {
public:
        BOOL InitInstance();
};

/* Function prototypes: */

int BlankLine (char*);
int NumberParse (float*, char*);
void SortRect (CRect*);
char* LabelFormat (float, float, char);
char* DisplayFormat (float x1, float x2);
int AddToHeader (char*, char*, bool);
void LoadInitializationFile();
void InitForthInterface ();
int LoadForthFile(char*);
int ExecuteForthExpression (char*, ostringstream*, long int*);
COLORREF LookupColor(char*);

struct IfcFuncTemplate
{
   const void* Function;
   const char* constantName;
};

// The following functions provide the interface between
// the C++ functions and the Forth environment. They
// should not be called by C, C++, or assembler routines.
//
int get_color_map();
int get_active_set();
int get_operand_set();
int get_active_plot();
int get_operand_plot();
int set_active_plot();
int set_operand_plot();
int get_ds();
int set_ds_extrema();
int make_ds();
int get_plot();
int drop_plot();
int make_plot();
int set_plot_symbol();
int set_plot_color();
int set_plot_rgbcolor();
int draw_plot();
int get_grid();
int set_grid_tics();
int set_grid_lines();
int get_window_title();
int set_window_title();
int clear_window();
int draw_window();
int reset_window();
int get_window_limits();
int set_window_limits();
int add_menu_item();
int set_background();
int make_menu();
int make_submenu();
int set_foreground();
int get_input();
int message_box();
int file_open_dialog();
int file_save_dialog();
int set_save_options();

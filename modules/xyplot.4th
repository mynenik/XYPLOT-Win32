\ xyplot.4th
\
\ Forth interface to xyplot
\
\ Copyright (c) 1999--2003 Krishna Myneni
\ Creative Consulting for Research and Education
\
\ This software is provided under the terms of the GNU General
\ Public License.
\
\ Last Revised: 8-12-2003
\
\ XYPLOT defines Forth constants which contain pointers to
\ C++ functions that interface with the Forth environment.
\
\ Defined below are words to execute the C++ interface functions:
\
\  get_active_set ( -- n ) Return the set number for the active plot.
\  get_operand_set ( -- n ) Return the set number for the operand plot.
\  ?active ( -- n )	synonymous with get_active_set.
\  ?operand ( -- n ) 	synonymous with get_operand_set.
\  get_active_plot ( -- n ) Return the index of the active plot.
\  get_operand_plot ( -- n ) Return the index of the operand plot.
\  set_active_plot ( n -- ) Set the active plot to plot n.
\  set_operand_plot ( n -- ) Set the operand plot to plot n.
\  get_ds ( i addr -- n ) Return info on dataset i into a structure.
\  set_ds_extrema ( i -- ) Recompute the extrema for dataset i.
\  make_ds ( addr -- )	Make a dataset according to info in structure.
\  get_plot ( i addr -- n ) Return info on plot i into a structure.
\  make_plot ( addr -- ) Make a plot according to info in structure.
\  set_plot_symbol ( symbolname -- ) set active plot symbol.
\  set_plot_color ( colorname -- ) set active plot color.
\  draw_plot ( flag -- ) draw the active plot.
\  set_grid_tics ( nx ny -- ) set number of tics for x and y axes
\  set_grid_lines ( flagx flagy -- ) set grid lines on/off for x and y axes
\  clear_window ( -- )	Clear the plot window.
\  draw_window ( -- ) 	Draw the plot window.
\  reset_window ( -- ) 	Reset the plot window.
\  get_input ( ^prompt -- ^resp flag ) Provide dialog for text input.
\  get_window_limits ( -- fxmin fymin fxmax fymax ) Return window extrema.
\  set_window_limits ( fxmin fymin fxmax fymax -- ) Set window extrema.
\  add_menu_item ( menu name command -- ) Add a new menu item
\  set_background ( colorname -- ) Set plot window background color.
\  set_foreground ( colorname -- ) Set plot window foreground color.

: get_active_set ( -- n | return the active dataset number )
	\ n less than zero indicates an error.
	FN_GET_ACTIVE_SET call ;

: get_operand_set ( -- n | return the operand dataset number )
	\ n less than zero indicates an error.
	FN_GET_OPERAND_SET call ;

: ?active ( -- n) get_active_set ;
: ?operand ( -- n) get_operand_set ;

: get_active_plot ( -- n | return the active plot number )
	FN_GET_ACTIVE_PLOT call ;

: get_operand_plot ( -- n | return the operand plot number )
	FN_GET_OPERAND_PLOT call ;

: set_active_plot ( n -- | set plot n as the active plot )
	FN_SET_ACTIVE_PLOT call ;

: set_operand_plot ( n -- | set plot n as the operand plot )
	FN_SET_OPERAND_PLOT call ;

: get_ds ( i addr -- n | return info on dataset i into a structure at addr )
	\ n less than zero indicates an error.
	FN_GET_DS call ;

: set_ds_extrema ( i -- | recompute the extrema for dataset i )
	FN_SET_DS_EXTREMA call ;

: make_ds ( addr -- | make a dataset in xyplot )
	FN_MAKE_DS call ;

: get_plot ( i addr -- n | return info on plot i into a structure at addr )
	\ n less than zero indicates an error.
	FN_GET_PLOT call ;

: make_plot ( addr -- | make a plot in xyplot )
	FN_MAKE_PLOT call ;

: set_plot_symbol ( symbolname -- | set active plot symbol )
	FN_SET_PLOT_SYMBOL call ;

: set_plot_color ( colorname -- | set active plot color )
	FN_SET_PLOT_COLOR call ;

: draw_plot ( flag -- | draw active plot, if flag = false use bkg color )
	FN_DRAW_PLOT call ;

: set_grid_tics ( nx ny -- | set number of tics on x and y axes )
	FN_SET_GRID_TICS call ;

: set_grid_lines ( flagx flagy -- | set grid lines on/off on x and y axes )
	FN_SET_GRID_LINES call ;

: clear_window ( -- | clear the plot window )
	FN_CLEAR_WINDOW call ;

: draw_window ( -- | draw the plot window )
	FN_DRAW_WINDOW call ;

: reset_window ( -- | reset the plot window )
	FN_RESET_WINDOW call ;

: get_window_limits ( -- fx1 fy1 fx2 fy2 | obtain the plot window limits )
	FN_GET_WINDOW_LIMITS call ;

: set_window_limits ( fx1 fy1 fx2 fy2 -- | set the plot window limits )
	FN_SET_WINDOW_LIMITS call ;

: add_menu_item ( menu name command -- | add new menu item )
	FN_ADD_MENU_ITEM call ;

: set_background ( colorname -- | set plot window background color )
	FN_SET_BACKGROUND call ;

: set_foreground ( colorname -- | set plot window foreground color )
	FN_SET_FOREGROUND call ;

: get_input ( ^prompt -- ^resp flag | get text input from dialog box )
	FN_GET_INPUT call ;

4 constant SFSIZE	\ size in bytes of single precision float

\ Some useful constants for accessing elements inside a dataset
\ info structure

0 constant DNAME		\ pointer to name string
DNAME 4 + constant DHEADER	\ pointer to header string
DHEADER 4 + constant DTYPE	\ dataset type
DTYPE 4 + constant DNPTS	\ number of points in set
DNPTS 4 + constant DSIZE	\ datum dimension
DSIZE 4 + constant DDATA	\ pointer to data

: DatasetInfo ( -- | defining word for a dataset info structure )
	create 6 cells allot ;

: ->name ( dsaddr -- a | fetch address of name string )
	DNAME + a@ ;
: ->header ( dsaddr -- a | fetch address of header string )
	DHEADER + a@ ;
: ->type ( dsaddr -- n | fetch data type )
	DTYPE + @ ;
: ->npts ( dsaddr -- n | fetch number of points from dsinfo )
	DNPTS + @ ;
: ->size ( dsaddr -- n | fetch datum size from dsinfo )
	DSIZE + @ ;
: ->data ( dsaddr -- a | fetch address of data buffer from dsinfo )
	DDATA + a@ ;


\ Some usefule constants for accessing elements inside a plot
\ info structure

0 constant PDSET		\ dataset number for plot
PDSET 4 + constant PTYPE	\ plot type
PTYPE 4 + constant PSYM		\ plot symbol
PSYM 4 + constant PCOLOR	\ plot color

: PlotInfo ( -- | defining word for a plot info structure )
	create 4 cells allot ;

: ->set ( pladdr -- n | fetch dataset number )
	PDSET + @ ;
: ->plot-type ( pladdr -- n | fetch plot type )
	PTYPE + @ ;
: ->symbol ( pladdr -- n | fetch plot symbol )
	PSYM + @ ;
: ->color ( pladdr -- n | fetch plot color )
	PCOLOR + @ ;

\ Useful words to fetch and store the i^th x, y pair from/to
\ a dataset are given below. Note that addr is the address of
\ the dataset info structure and the index i starts at 0 for
\ the first point.
\
\ If you want to perform computations efficiently, it's better to
\ work with the pointer to the data directly and increment it
\ to avoid all of the computations for each index. Both methods
\ -- one using the @xy and !xy words, and one that works
\ directly with the data pointer are illustrated in the words
\ test1 and test2, contained in test.4th.

: @xy ( i addr -- fx fy | retrieve the i^th x, y pair )
	dup ->size SFSIZE * rot * swap ->data
	swap + dup >r sf@ r> SFSIZE + sf@ ;

: !xy ( fx fy i addr -- | store the i^th x, y pair )
	dup ->size SFSIZE * rot * swap ->data
	swap + dup >r SFSIZE + sf! r> sf! ;


\ see the file test.4th for examples of usage

\ Load all of the forth files that provide basic functions
\ and menu items

include strings
include matrix
include xutils

include xypolyfit
include arithmetic
include xyhistogram
include abs
include smooth
include lpf
include fft
include xyfft
include ylog
include derivative
include xyderiv
include polyfit
include peak_search
include xypeaks

\ You can put some initialization stuff here, for example
\  to set the background and foreground colors.

" white" set_background
" black" set_foreground
false false set_grid_lines

\ end of xyplot.4th







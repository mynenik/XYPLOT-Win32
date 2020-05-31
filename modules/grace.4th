\ grace.4th
\
\ Generate a Grace project file for grace version 5.1.10
\   from within xyplot.
\
\ Grace is a program for generating publication quality scientific 
\ graphs. It is distributed under the GNU General Public License. 
\ For information on Grace, see
\
\	http://plasma-gate.weizmann.ac.il/Grace/
\
\ This file is Copyright (c) 2003 Krishna Myneni
\ Provided under the GNU General Public License
\
\ Revisions:
\
\	2003-3-27  created  km
\

9 constant MAXPLOTS

variable gr_fid

: tdstring ( -- a u | return a date and time string )
	time&date
	s"  "
	rot 0 <# [char] - hold # # # # #> strcat
	rot 0 <# [char] - hold # # #>     strcat
	rot 0 <# bl hold # # #>           strcat
	rot 0 <# [char] : hold # # #>     strcat
	rot 0 <# [char] : hold # # #>     strcat
	rot 0 <# # # #>                   strcat ;


: rep(',") ( a u -- a u | replace every single quote with double quote in string)
	2dup 0 ?do dup c@ [char] ' = if [char] " over c! then 1+ loop drop ;
	  
: >grfile ( a u -- | write string to grace file )
	gr_fid @ write-line drop ;

PlotInfo pl1
DatasetInfo ds1

create set_specifier 12 allot
s" @     s?    " set_specifier swap cmove 
variable flag_symbol
variable flag_line
fvariable xmin
fvariable xmax
fvariable ymin
fvariable ymax

: write_grace_file ( -- )
	s" # Grace project file (xyplot generated)" >grfile
	s" # " tdstring strcat >grfile
	s" @version 50110" >grfile
	s" @page size 792, 612" >grfile
	s" @background color 0" >grfile
	s" @page background fill on" >grfile

	s" @map color 0 to (255, 255, 255), 'white'" 	rep(',")  >grfile
	s" @map color 1 to (0, 0, 0), 'black'"       	rep(',")  >grfile
	s" @map color 2 to (0, 0, 255), 'blue'"      	rep(',")  >grfile
	s" @map color 3 to (255, 0, 0), 'red'"	     	rep(',")  >grfile
	s" @map color 4 to (34, 139, 34), 'forestgreen'" rep(',")  >grfile
	s" @map color 5 to (64, 224, 208), 'turquoise'" rep(',")  >grfile
	s" @map color 6 to (255, 0, 255), 'magenta'"   	rep(',")  >grfile
	s" @map color 7 to (160, 82, 45), 'sienna'"    	rep(',")  >grfile
	s" @map color 8 to (255, 215, 0), 'gold'"	rep(',")  >grfile
	s" @map color 9 to (255, 165,0), 'orange'"  	rep(',")  >grfile
	s" @map color 10 to (0, 0, 0), 'black'"  	rep(',")  >grfile

	s" @g0 on" >grfile
	s" @g0 type XY" >grfile
	s" @with g0" >grfile
	
	get_window_limits ymax f! xmax f! ymin f! xmin f!

	xmin f@ 6 f>string count  s" @     world xmin " 2swap strcat >grfile
	xmax f@ 6 f>string count  s" @     world xmax " 2swap strcat >grfile
	ymin f@ 6 f>string count  s" @     world ymin " 2swap strcat >grfile
	ymax f@ 6 f>string count  s" @     world ymax " 2swap strcat >grfile

	s" @     xaxis on" >grfile
	s" @     xaxis tick on" >grfile 
	s" @     xaxis tick major " 
	xmax f@ xmin f@ f- 10e f/ 6 f>string count strcat >grfile
	s" @     xaxis ticklabel on" >grfile
	s" @     yaxis on" >grfile
	s" @     yaxis tick on" >grfile
	s" @     yaxis tick major " 
	ymax f@ ymin f@ f- 10e f/ 6 f>string count strcat >grfile
	s" @     yaxis ticklabel on" >grfile

	s" @     frame type 0" >grfile

\ Write info on all visible sets

	MAXPLOTS 0 do
	  i pl1 get_plot 0< if leave then
	  pl1 ->set ds1 get_ds drop
	  pl1 ->set [char] 0 + set_specifier 7 + c!
	  set_specifier 9 s" comment '" strcat
	  ds1 ->name dup strlen strcat s" '" strcat rep(',") >grfile
	  set_specifier 9 s" hidden false" strcat >grfile
	  set_specifier 9 s" type xy" strcat >grfile

	  set_specifier 9 s" symbol " strcat	  
	  pl1 ->symbol dup 2 = swap 4 = or dup flag_symbol ! 
	  abs u>string count strcat >grfile

	  flag_symbol @ if
	    set_specifier 9 s" symbol size 0.2" strcat >grfile
	    set_specifier 9 s" symbol color " strcat 
	    pl1 ->color 2+ u>string count strcat >grfile
	    set_specifier 9 s" symbol fill color " strcat
	    pl1 ->color 2+ u>string count strcat >grfile
	    set_specifier 9 s" symbol pattern 1" strcat >grfile
	    set_specifier 9 s" symbol fill pattern 1" strcat >grfile
	  then

	  set_specifier 9 s" line type " strcat
	  pl1 ->symbol dup 0= swap 4 = or dup flag_line ! 	  
	  abs u>string count strcat >grfile

	  flag_line @ if
	    set_specifier 9 s" line color " strcat
	    pl1 ->color 2+ u>string count strcat >grfile
	    set_specifier 9 s" linewidth 1.0" strcat >grfile
	  then
	loop


\ Now write the visible set data

	MAXPLOTS 0 do
	  i pl1 get_plot 0< if leave then
	  pl1 ->set ds1 get_ds drop
	  s" @target G0.S" pl1 ->set u>string count strcat >grfile
	  s" @type xy" >grfile
	  ds1 ->npts 0 ?do
	    i ds1 @xy 2>r 
	    8 f>string count s"  " strcat 2r> 8 f>string count strcat >grfile
	  loop
	  s" &" >grfile
	loop

\ Close the output file

	gr_fid @ close-file drop
;


: xygrace ( -- | prompt user for filename and write grace file )
	c" Enter the Grace (.agr) filename:" get_input
	if
	  count W/O create-file 0 < if  ." Unable to open output file!" 
	  else gr_fid ! write_grace_file then
	then ;

\ add "Save Grace File" to the File Menu

MN_FILE  c" Save Grace File"   c" xygrace"  add_menu_item


	


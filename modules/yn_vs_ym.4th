\ yn_vs_ym.4th
\
\ xyplot module for making an x,y dataset from the
\   y values of one set versus the y values from another set.
\
\ Copyright (c) 2001--2005 K. Myneni
\
\ This software is released under the terms of the GNU
\ General Public License
\ 
\ Revisions: 
\     1-14-2005  updated use of DatasetInfo structure
\
\ Requires:
\
\	matrix.4th
\	xyplot.4th
\	xutils.4th

DatasetInfo ds3

8192 constant MAX_YNMPTS
MAX_YNMPTS 2 fmatrix ynm

variable ynm_pts

: yn_vs_ym ( -- )
	?active dup
	0 >= if
	  ds1 get_ds		\ get active set info
	  0 >= if
	    ?operand dup
	    0 >= if
	      ds2 get_ds	\ get operand set info
	      0 >= if
	        ds1 DatasetInfo->Npts @
	        ds2 DatasetInfo->Npts @
	        min dup ynm_pts !	\ use minimum value for npts
	        MAX_YNMPTS > if
	          ." Too many points. Change MAX_YNMPTS." cr	
	          exit
	        then

		\ Create the new x, y pairs in the fmatrix ynm

	        ynm_pts @ 0 do
	          i ds1 @xy
	          i ds2 @xy
	          fswap fdrop
	          2 i 1+ ynm frow!
	          fdrop
	        loop

	        \ Make the new dataset in xyplot

	        c" Yn_vs_Ym" 1+ ds3 DatasetInfo->Name !
	        c"  " 1+ ds3 DatasetInfo->Header !
	        256 ds3 DatasetInfo->Type !
	        ynm_pts @ ds3 DatasetInfo->Npts !
	        2 ds3 DatasetInfo->Size !
	        ynm cell+ cell+ ds3 DatasetInfo->Data !
		ds3 make_ds drop

	      then
	    else
	      drop
	    then
	  then
	else
	  drop
	then	  
;



\ add xyplot math menu item

MN_MATH c" Y_m vs Y_n"	c" yn_vs_ym" add_menu_item


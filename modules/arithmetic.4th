\ arithmetic.4th
\
\ xyplot module for dataset arithmetic functions
\
\ (c) 1999--2020 Krishna Myneni
\
\ This software is released under the terms of the GNU
\ General Public License
\ 
\ Requires:
\
\	matrix.4th
\	xyplot.4th
\	xutils.4th

DatasetInfo ds3

2 16384 fmatrix result

: make_result ( ^name  -- n | make dataset resulting from an arithmetic operation )
	
	\ Set up the info structure for a new dataset

	dup 1+                ds3 DatasetInfo->Name !
	count s"  of " strcat
	ds1 DatasetInfo->Name a@ dup strlen strcat
	s"  and " strcat
	ds2 DatasetInfo->Name a@ dup strlen strcat
	strpck 1+ 	      ds3 DatasetInfo->Header !

	REAL_DOUBLE           ds3 DatasetInfo->Type !
	result mat_size@ drop ds3 DatasetInfo->Npts !
	2                     ds3 DatasetInfo->Size !
	result cell+ cell+    ds3 DatasetInfo->Data !
		 	      
	ds3 make_ds ;

PlotInfo pl3
: plot_result ( n -- | make plot of result using new dataset number)
    pl3  PlotInfo->Set !
    0 pl3 PlotInfo->Type !
    sym_LINE pl3 PlotInfo->Symbol !
    2 pl3 PlotInfo->Color !

    pl3 make_plot
;

: ?interpolate ( fx1 idx -- flag | return TRUE if interpolation is needed )
    ds2 @xy fdrop f= invert ;

fvariable xs
fvariable x1
fvariable x2
fvariable y1
fvariable y2
fvariable slope
variable ordering2  \ true indicates ds2 is in ascending order; false otherwise
true ordering2 !

: interpolate_y2 ( fx1 idx -- fy2i | linear interpolation )
    >r xs f!
    r@ ds2 @xy y1 f! x1 f!

    r@ 0= IF
      r@ 1+
    ELSE
      r@ ds2 DatasetInfo->Npts @ 1- = IF
        r@ 1-
      ELSE
        r@
        ordering2 @ IF
          x1 f@ xs f@ f< IF  1+  ELSE  1-  THEN
        ELSE
          x1 f@ xs f@ f< IF  1-  ELSE  1+  THEN
        THEN
      THEN
    THEN
    ds2 @xy y2 f! x2 f!
    r> drop
    x1 f@ x2 f@ f> IF
      x1 f@ x2 f@  x1 f! x2 f!
      y1 f@ y2 f@  y1 f! y2 f!
    THEN
    y2 f@ y1 f@ f- x2 f@ x1 f@ f- f/ slope f!
    y2 f@ x2 f@ xs f@ f- slope f@ f* f- 
;

variable rcntr
variable ar_operator

: do_arithmetic ( xt -- flag | apply operator over index range )
	ar_operator !
	1 rcntr !
	index_range
	2dup <>
	IF
	  0 ds2 @xy fdrop  1 ds2 @xy fdrop f< ordering2 !
	  swap
	  2dup - 2 result mat_size!	\ set size of result matrix	
	  DO
	    i ds1 @xy               \ -- fx1 fy1
	    fover                   \ -- fx1 fy1 fx1
	    \ find index of point in ds2 with nearest abscissa
	    fdup ds2 findx          \ -- fx1 fy1 fx1 index
	    dup >r                  \ r: idx 
	    ?interpolate 0= IF
	      r> ds2 @xy            \ -- fx1 fy1 fx2 fy2
	      fswap fdrop           \ -- fx1 fy1 fy2
	    ELSE
	      fover r>              \ -- fx1 fy1 fx1 idx
	      interpolate_y2        \ -- fx1 fy1 fy2i
	    THEN
	    ar_operator a@ execute  \ -- fx1 operator(fy1, fy2)
	    2 rcntr @ result frow!
	    1 rcntr +!
	  LOOP
	  true
	ELSE
	  2drop
	  false
	THEN ;

 
: add_ds ( -- | add the operand set and the active set )
	['] f+ do_arithmetic
	IF c" Sum" make_result plot_result THEN ;

   
: sub_ds ( -- | add the operand set and the active set )
	['] f- do_arithmetic
	IF c" Difference" make_result plot_result THEN ;
   	   
     	        
: mul_ds ( -- | add the operand set and the active set )
	['] f* do_arithmetic
	IF c" Product" make_result plot_result THEN ;

   
: div_ds ( -- | add the operand set and the active set )
	['] f/ do_arithmetic
	IF c" Quotient" make_result plot_result THEN ;
   

\ add xyplot math menu items

MN_MATH c" Add"		c" add_ds reset_window" add_menu_item
MN_MATH c" Subtract"	c" sub_ds reset_window" add_menu_item
MN_MATH c" Multiply"	c" mul_ds reset_window" add_menu_item
MN_MATH c" Divide"	c" div_ds reset_window" add_menu_item


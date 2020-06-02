\ arithmetic.4th
\
\ xyplot module for dataset arithmetic functions
\
\ (c) 1999--2020 Krishna Myneni
\
\ This software is released under the terms of the GNU
\ General Public License
\ 
\ Revisions: 
\	1-18-2000
\	6-21-2000 updated for new features of kforth  KM
\ 	5-29-2020 implement linear interpolation when abscissas not equal
\                 in overlap region between two datasets KM 
\
\ Requires:
\
\	matrix.4th
\	xyplot.4th
\	xutils.4th

DatasetInfo ds3

2 8000 fmatrix result


: make_result ( ^name  -- n | make dataset resulting from an arithmetic operation )
	
	\ Set up the info structure for a new dataset

	1+                    ds3 DatasetInfo->Name !
	c"  " 1+              ds3 DatasetInfo->Header !
	REAL_DOUBLE           ds3 DatasetInfo->Type !
	result mat_size@ drop ds3 DatasetInfo->Npts !
	2                     ds3 DatasetInfo->Size !
	result cell+ cell+    ds3 DatasetInfo->Data !
		 	      
	ds3 make_ds ;


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

: do_arithmetic ( n1 n2 addr -- flag | loop over index range n1 to n2 and apply operator )
	ar_operator !			\ store execution address of operator 
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
	if c" Sum" make_result drop then ;

   
: sub_ds ( -- | add the operand set and the active set )
	['] f- do_arithmetic
	if c" Difference" make_result drop then ;
   	   
     	        
: mul_ds ( -- | add the operand set and the active set )
	['] f* do_arithmetic
	if c" Product" make_result drop then ;

   
: div_ds ( -- | add the operand set and the active set )
	['] f/ do_arithmetic
	if c" Quotient" make_result drop then ;
   

\ add xyplot math menu items

MN_MATH c" Add"		c" add_ds" add_menu_item
MN_MATH c" Subtract"	c" sub_ds" add_menu_item
MN_MATH c" Multiply"	c" mul_ds" add_menu_item
MN_MATH c" Divide"	c" div_ds" add_menu_item


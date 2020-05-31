\ arithmetic.4th
\
\ xyplot module for dataset arithmetic functions
\
\ (c) 1999--2000 Krishna Myneni
\
\ This software is released under the terms of the GNU
\ General Public License
\ 
\ Revisions: 
\	1-18-2000
\	6-21-2000 updated for new features of kforth  KM
\
\ Requires:
\
\	matrix.4th
\	xyplot.4th
\	xutils.4th

DatasetInfo ds3

2 8000 fmatrix result


: make_result ( ^name  -- | make dataset resulting from an arithmetic operation )
	
	\ Set up the info structure for a new dataset

	1+ ds3 DNAME + !
	"  " 1+ ds3 DHEADER + !
	256 ds3 DTYPE + !
	result mat_size@ drop ds3 DNPTS + !
	2 ds3 DSIZE + !
	result cell+ cell+ ds3 DDATA + !
		 	      
	ds3 make_ds ;

variable rcntr
variable ar_operator 

: do_arithmetic ( n1 n2 addr -- flag | loop over index range n1 to n2 and apply operator )
	ar_operator !			\ store execution address of operator
	1 rcntr !
	index_range
	2dup <>
	if
	  swap
	  2dup - 2 result mat_size!	\ set size of result matrix	
	  do
	    i ds1 @xy
	    fover ds2 findx		\ find nearest point in ds2
	    ds2 @xy
	    fswap fdrop
	    ar_operator a@ execute
	    2 rcntr @ result frow!
	    1 rcntr +!
	  loop
	  true
	else
	  2drop
	  false
	then ;

 
: add_ds ( -- | add the operand set and the active set )
	['] f+ do_arithmetic
	if " Sum" make_result then ;

   
: sub_ds ( -- | add the operand set and the active set )
	['] f- do_arithmetic
	if " Difference" make_result then ;
   	   
     	        
: mul_ds ( -- | add the operand set and the active set )
	['] f* do_arithmetic
	if " Product" make_result then ;

   
: div_ds ( -- | add the operand set and the active set )
	['] f/ do_arithmetic
	if " Quotient" make_result then ;
   

\ add xyplot math menu items

MN_MATH " Add"		" add_ds" add_menu_item
MN_MATH " Subtract"	" sub_ds" add_menu_item
MN_MATH " Multiply"	" mul_ds" add_menu_item
MN_MATH " Divide"	" div_ds" add_menu_item






\ xyderiv.4th
\
\ xyplot module for derivative computation
\
\ Krishna Myneni, 3-30-2001
\
\ Requires:
\	matrix.4th
\	derivative.4th
\	xyplot.4th
\
\ Revisions: 
\       1-14-2005  updated for new DatasetInfo data structure  km
 
32768 constant MAX_DER_PTS

DatasetInfo dsder
MAX_DER_PTS 2 fmatrix der
der 2 cells + dsder DatasetInfo->Data !

: xyderiv ( -- )
	?active dup
	0 >= if
	  ds1 get_ds
	  0 >= if
	    ds1 DatasetInfo->Npts @ MAX_DER_PTS > if
	      ." Too many points. Resize the fmatrix der."
	      exit
	    then
	    ds1 DatasetInfo->Npts @ 2 der mat_size!	\ Resize the derivative matrix
	    
	    \ Copy dataset x,y values to the derivative matrix

	    ds1 DatasetInfo->Npts @ 1+ 1 do
	      i 1- ds1 @xy
	      i 2 der fmat!
	      i 1 der fmat!
	    loop

	    der derivative

	    if
	      ." Error computing derivative"
	      exit
	    then

	    \ Make a new dataset

	    c" Derivative" 1+ dsder DatasetInfo->Name !
	    c" Derivative" 1+ dsder DatasetInfo->Header !
	    REAL_DOUBLE dsder DatasetInfo->Type !	\ double precision fp type
	    ds1 DatasetInfo->Npts @ dsder DatasetInfo->Npts !
	    2 dsder DatasetInfo->Size !

	    dsder make_ds
	  then
	else
	  drop
	then ;
	       
MN_MATH c" Derivative"	c" xyderiv" add_menu_item

\ xyhistogram.4th
\
\ Bin series of y values from the active dataset to create a histogram
\
\ Copyright (c) 2001--2003 Krishna Myneni
\ This software is provided under the terms of the GNU General Public License
\
\ Requires:
\	matrix.4th
\
\ Revisions:
\	7-27-2001 modified histogram.4th for use with xyplot KM
\	6-18-2003 changed F>S to FROUND>S  km

1024 constant MAX_BINS
MAX_BINS 2 fmatrix hist

fvariable hmax 0e hmax f!
fvariable hmin 0e hmin f!

: find_min_max ( -- | determine min and max of y values of dataset)
	1 ds1 @xy 
	fdup hmin f! hmax f!
	fdrop
	ds1 ->npts 2 do
	  i ds1 @xy   
	  fdup hmin f@ f< if fdup hmin f! then
	  fdup hmax f@ f> if fdup hmax f! then
	  fdrop fdrop
	loop ;

: ?bins ( fwidth -- n | compute number of bins required )
	\ hmin and hmax must be set with prior execution of find_min_max
	hmax f@ hmin f@ f- fover f/ fround>s 1+ ;

fvariable bin_width
variable nbins
DatasetInfo dshist
	
: histogram ( f -- | create a histogram of the active set's y values )
	\ f is the bin width for the histogram
	bin_width f!
	?active dup 0 >= if
	  ds1 get_ds
	  0 >= if
	    find_min_max	\ determine min and max of y values
	    bin_width f@ ?bins nbins !	\ no. of histogram bins needed

	    nbins @ MAX_BINS > if
	      ." Too many bins. Change MAX_BINS in xyhistogram.4th" cr exit
	    then

	    nbins @ 2 hist mat_size!	\ set size of histogram matrix
	    hist fmat_zero		\ zero the histogram matrix

	    \ Set up abscissas for histogram

	    bin_width f@ 2e f/ hmin f@ f+

	    nbins @ 1+ 1 do
	      fdup i 1 hist fmat!
	      bin_width f@ f+
	    loop
	    fdrop
	      	
	    \ Create the histogram

	    ds1 ->npts 0 do
	      i ds1 @xy
	      hmin f@ f-
	      bin_width f@ f/ fround>s 1+  \ bin index for current y val
	      1 swap 2 hist fmat_addr +!  \ increment bin count (as integer)
	      fdrop
	    loop

	    \ Convert histogram counts to floating pt numbers

	    nbins @ 1+ 1 do
	      i 2 hist fmat_addr @ s>f
	      i 2 hist fmat!
	    loop

	    \ Create dataset in xyplot

	    "  " 1+ dshist DHEADER + !
	    " Histogram" 1+ dshist DNAME + !
	    256 dshist DTYPE + !		\ double precision fp type
	    hist mat_size@ drop dshist DNPTS + !
	    2 dshist DSIZE + !
	    hist 8 + dshist DDATA + !

	    dshist make_ds	
	    
	  then
	else
	  drop
	then ;
	  

: xyhistogram ( -- | prompt user to enter bin width and create histogram )
	?active 0 >=
	if
	  c" Enter the bin width (for y values):" get_input
	  if
	    string>f histogram
	  else
	    drop
	  then
	then ;


MN_MATH " Histogram" " xyhistogram" add_menu_item

	  

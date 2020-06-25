\ xyhistogram.4th
\
\ Bin series of y values from the active dataset to create a histogram
\
\ Copyright (c) 2001--2020 Krishna Myneni
\ This software is provided under the terms of the GNU General Public License
\
\ Requires:
\	matrix.4th
\

1024 constant MAX_BINS
MAX_BINS 2 fmatrix hist

fvariable hmax 0e hmax f!
fvariable hmin 0e hmin f!

0 ptr dsaddr

: find_min_max ( addr -- | determine min and max of y values of dataset)
	to dsaddr
	0 dsaddr @xy fdup hmin f! hmax f! fdrop
	dsaddr DatasetInfo->Npts @ 1 DO
	  i dsaddr @xy   
	  fdup hmin f@ f< IF fdup hmin f! THEN
	  fdup hmax f@ f> IF fdup hmax f! THEN
	  fdrop fdrop
	LOOP ;

: ?bins ( fwidth -- n | compute number of bins required )
	\ hmin and hmax must be set with prior execution of find_min_max
	hmax f@ hmin f@ f- fover f/ fround>s 1+ ;

fvariable bin_width
variable nbins
DatasetInfo dshist
PlotInfo    plhist

: histogram ( f -- | create a histogram of the active set's y values )
	\ f is the bin width for the histogram
	bin_width f!
	?active dup 0 >= IF
	  ds1 get_ds 0 >= IF
	    ds1 find_min_max	\ determine min and max of y values
	    bin_width f@ ?bins nbins !	\ no. of histogram bins needed

	    nbins @ MAX_BINS > IF
	      ." Too many bins. Change MAX_BINS in xyhistogram.4th" cr exit
	    THEN

	    nbins @ 2 hist mat_size!	\ set size of histogram matrix
	    hist fmat_zero		\ zero the histogram matrix

	    \ Set up abscissas for histogram

	    bin_width f@ 2e f/ hmin f@ f+

	    nbins @ 1+ 1 DO
	      fdup i 1 hist fmat!
	      bin_width f@ f+
	    LOOP  fdrop
	      	
	    \ Create the histogram

	    ds1 DatasetInfo->Npts @ 0 DO
	      i ds1 @xy  hmin f@ f-
	      bin_width f@ f/ fround>s 1+	\ bin index for current y val
	      1 swap 2 hist fmat_addr +!  \ increment bin count (as integer)
	      fdrop
	    LOOP

	    \ Convert histogram counts to floating pt numbers

	    nbins @ 1+ 1 DO
	      i 2 hist fmat_addr @ s>f
	      i 2 hist fmat!
	    LOOP

	    \ Create dataset in xyplot

	    s" Histogram of " ds1 DatasetInfo->Name a@ dup strlen
	    strcat strpck 1+ dshist DatasetInfo->Header !
	    c" Histogram" 1+ dshist DatasetInfo->Name !
	    REAL_DOUBLE dshist DatasetInfo->TYPE !	\ double precision fp type
	    hist mat_size@ drop dshist DatasetInfo->Npts !
	    2 dshist DatasetInfo->Size !
	    hist 2 cells + dshist DatasetInfo->Data !

	    dshist make_ds

            \ Replace plot of data with histogram if dataset creation succeeds
            dup 0> IF
              get_active_plot plhist get_plot drop
              plhist PlotInfo->Set !
              0 plhist PlotInfo->Type !
              sym_HISTOGRAM plhist PlotInfo->Symbol !
              drop_plot
              plhist make_plot
            ELSE
              drop  ." Error creating histogram plot"
            THEN

	  THEN
	ELSE  drop  THEN ;
	  

: xyhistogram ( -- | prompt user to enter bin width and create histogram )
	?active 0 >= IF
	  c" Enter the bin width (for y values):" get_input
	  IF  string>f histogram  ELSE  drop  THEN
	THEN ;


MN_MATH c" Histogram" c" xyhistogram reset_window" add_menu_item



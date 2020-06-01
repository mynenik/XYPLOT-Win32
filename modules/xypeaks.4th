\ xypeaks.4th
\
\ xyplot module for automated peak finding. The active dataset should be 
\   smoothed or filtered sufficiently to remove high frequency noise prior 
\   to calling the peak search function. Note that if you use low-pass
\   filtering on the data, it will shift the horizontal positions of the peaks.
\
\ Copyright (c) 2001--2005 Krishna Myneni
\ Provided under the GNU General Public License
\
\ Requires:
\	matrix.4th
\	derivative.4th
\	polyfit.4th
\	peak_search.4th
\	xyplot.4th
\
\ Revisions:
\	3-30-2001  created
\	1-14-2005  updated use of DatasetInfo structure  km
\
5 constant DEF_PEAK_WIDTH

DatasetInfo dspks
peaks 2 cells + dspks DDATA !

MAX_PEAKSEARCH_PTS 2 fmatrix input_data

: xypeaks ( -- )
	?active dup
	0 >= if
	  ds1 get_ds
	  0 >= if
	    ds1 ->npts MAX_PEAKSEARCH_PTS > if
	      ." Too many points. Resize the fmatrices input_data and pksin."
	      exit
	    then
	    ds1 ->npts 2 input_data mat_size!  \ Resize the input_data matrix
	    
	    \ Copy dataset x,y values to the input_data matrix

	    ds1 ->npts 1+ 1 do
	      i 1- ds1 @xy
	      i 2 input_data fmat!
	      i 1 input_data fmat!
	    loop

	    c" Enter the minimum peak width:" get_input
	    if
	      string>s
	    else
	      DEF_PEAK_WIDTH
	    then

	    input_data swap peak_search

	    case
	      1 of ." Error computing derivative of dataset!" exit endof
	      2 of ." No peaks found!" exit endof
	    endcase

	    \ Make a new dataset

	    npeaks @ 0 > if
	      c" Peaks" 1+ dspks DNAME !
	      c" Peaks" 1+ dspks DHEADER !
	      256 dspks DTYPE !		\ double precision fp type
	      npeaks @ dspks DNPTS !
	      2 dspks DSIZE !

	      dspks make_ds
	    then
	  then
	else
	  drop
	then ;
	       
MN_MATH " Find Peaks"	" xypeaks" add_menu_item

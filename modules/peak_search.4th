\ peak_search.4th
\
\  Purpose: Find all peak positions and heights in data contained
\             in x, y arrays. The argument ipw is used to
\             discriminate against minor fluctuations in data that
\             may be detected as a peak. ipw should be set to the
\             number of points over which the peak should maintain
\             a smooth shape.
\
\ Krishna Myneni, 04-01-2001
\
\ Requires:
\
\	matrix.4th
\	derivative.4th
\	polyfit.4th
\
\ Revisions:
\
1024 1024 * constant MAX_PEAKSEARCH_PTS
MAX_PEAKSEARCH_PTS 2 fmatrix pksin

32768 constant MAX_PEAKS
MAX_PEAKS 2 fmatrix peaks

32 constant MAX_WIDTH

variable input_m
variable input_npts
variable ipw
variable npeaks
variable ys1
variable ys2
variable ips
variable ipe

fvariable xpeak
fvariable ypeak
fvariable xpk2
fvariable xoffs

3 1 fmatrix xp
3 1 fmatrix yp
MAX_WIDTH 1 fmatrix xpp
MAX_WIDTH 1 fmatrix ypp
3 1 fmatrix pcoeffs

: peak_search ( fm ipw -- ierr)

	ipw !
	dup mat_size@ drop input_npts !
	input_m !

	\ Copy the input matrix into the pksin matrix

	input_m a@ pksin 
	over mat_size@ * dfloats cell+ cell+	\ size of input matrix in bytes
	cmove

	\ Compute first derivative of data.

	pksin derivative
	if 1 exit then

	\  Search for X axis crossings.

	0 npeaks !
	1 2 pksin fmat@ 0e f< ys1 !	\ is first value negative?

	input_npts @ 2 do
	  i 2 pksin fmat@ 0e f< ys2 !	
	  ys2 @ ys1 @ <> if
	    \  X axis crossing found. Is it a peak or a valley?
	    
	    ys1 @ 0 >= if
	      \ Went from positive to negative so we found a peak.
	      
	      npeaks @ 1+ dup MAX_PEAKS <= if npeaks ! then

	      \  Use linear interpolation to determine x value at crossing.

	      i 1 pksin fmat@		\ x(i)
	      i 1- 2 pksin fmat@	\ y'(i-1)
	      f*
	      i 1- 1 pksin fmat@	\ x(i-1)
	      i 2 pksin fmat@		\ y'(i)
	      f*
	      f-
	      i 1- 2 pksin fmat@	\ y'(i-1)
	      i 2 pksin fmat@		\ y'(i)
	      f- f/ xpeak f!

	      \  Use parabolic fit to original data to determine peak height.

	      i 1- 1 pksin fmat@ fnegate xoffs f!	\ -x(i-1)	
	      0e 
	      1 1 xp fmat!
	      i 1 pksin fmat@ xoffs f@ f+
	      2 1 xp fmat!
	      i 1+ 1 pksin fmat@ xoffs f@ f+
	      3 1 xp fmat!

	      i 1- 2 input_m a@ fmat@
	      1 1 yp fmat!
	      i 2 input_m a@ fmat@
	      2 1 yp fmat!
	      i 1+ 2 input_m a@ fmat@
	      3 1 yp fmat!

	      xp yp pcoeffs 2 polfit fdrop

	      xpeak f@ xoffs f@ f+ xpk2 f!
	      xpk2 f@ fdup f* 3 1 pcoeffs fmat@ f*
	      xpk2 f@ 2 1 pcoeffs fmat@ f* f+
	      1 1 pcoeffs fmat@ f+
	      ypeak f!

	      xpeak f@ npeaks @ 1 peaks fmat!	\ store peak position
	      ypeak f@ npeaks @ 2 peaks fmat!	\ store peak height

	      \  Test for minimum width

	      i ipw @ 2/ - ips !
	      i ipw @ 2/ + ipe !

	      ips @ 1 < ipe @ input_npts @ > or
	      if 
	        -1 npeaks +!	\ reject the peak
	      else
	        ips @ 1 pksin fmat@ xoffs f!

	        ipe @ 1+ ips @ do
	          i 1 pksin fmat@ xoffs f@ f-
	          i ips @ - 1+ 1 xpp fmat!
	          i 2 input_m a@ fmat@
	          i ips @ - 1+ 1 ypp fmat!
	        loop

	        xpp ypp pcoeffs 2 polfit fdrop
	        
	        3 1 pcoeffs fmat@ 0e f>=
	        if -1 npeaks +! then  \ reject the peak
	      then
	    then
	  then
	  ys2 @ ys1 !
	loop

	npeaks @ 0> if
	  npeaks @ 2 peaks mat_size!	\ set the matrix size
	   0		\ no error
	else 
	  2		\ error: no peaks found
	then ;

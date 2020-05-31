\ xyfft.4th
\
\ FFT module for xyplot
\
\ Copyright (c) 2000 Krishna Myneni
\ Provided under the GNU General Public License
\
\ Requires:
\
\	matrix.4th
\	fft.4th
\
\ Notes:
\
\	data should be uniformly spaced
\
create dsfft 8 4 * allot

512 1024 * constant FFT_SIZE		\ max number of points for FFT
FFT_SIZE 2* 1 fmatrix fftdata

\ determine nearest power of 2 >= n and set the fftdata buffer size accordingly
 
: adjust_fft_size ( n -- )
	1
	begin
	  2dup 1 swap lshift
	  >
	while
	  1+
	repeat
	nip 1 swap lshift
	dup FFT_SIZE > if
	  drop
	  ." Number of points too great for current FFT buffer size"
	  abort
	else
	  2* 1 fftdata mat_size!
	then ;



fvariable fft_dx
fvariable fft_df

:  power ( -- | take fft of data and obtain its power spectrum )
	?active dup 0 >=
	if
	  dsfft get_ds
	  0 >= if

		\ Set the size of the fft data buffer
		
		dsfft ->npts adjust_fft_size	\ ensure power of 2
		fftdata fmat_zero		\ zero the buffer		

		\ Determine frequency scale

		0 dsfft @xy fdrop
	        1 dsfft @xy fdrop
		f- fabs fft_dx f!
		1e fftdata mat_size@ drop s>f f/ fft_dx f@ f/
		fft_df f!
	     
		\ Transfer data from xyplot dataset to the fftdata buffer

		dsfft ->npts 0 do
		  i dsfft @xy
		  i 2* 1+ 1 fftdata fmat!
		  0e i 1+ 2* 1 fftdata fmat! \ set imaginary part to zero
		  fdrop
		loop
	    
		fftdata fft	\ perform the Fast Fourier Transform

		\ Replace the data with appropriate x and power values

		fftdata mat_size@ drop 1+ 1 do
		  i 1 fftdata fmat@ fdup f*
		  i 1+ 1 fftdata fmat@ fdup f* f+
		  i 1+ 1 fftdata fmat!		\ store y value
		  i s>f fft_df f@ f* 		\ determine x value
		  i 1 fftdata fmat!
		2 +loop
	    
		\ make a new xyplot dataset containing the power spectrum

		" PowerSpectrum" 1+ ds1 DNAME + !
		"  " 1+ ds1 DHEADER + !
		256 ds1 DTYPE + !
		fftdata mat_size@ drop ds1 DNPTS + !
		2 ds1 DSIZE + !
		fftdata cell+ cell+ ds1 DDATA + !		 	      
		ds1 make_ds
	  then
	else
	  drop
	then ;	

MN_MATH " Power Spectrum"  " power" add_menu_item
\ xyfft.4th
\
\ FFT module for xyplot
\
\ Copyright (c) 2000--2005 Krishna Myneni
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
\ Revisions:
\
\	2004-07-26  added Real(FT) and Imag(FT) functions  KM
\	2004-07-27  added Real(FT^-1) KM
\       2005-01-13  updated use of DatasetInfo structure  KM

DatasetInfo dsfft 

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
fvariable fft_dt

: real-ft ( -- | take Real[FT] )
	?active dup 0 >=
	if
	  dsfft get_ds
	  0 >= if

		\ Set the size of the fft data buffer
		
		dsfft DatasetInfo->Npts @ adjust_fft_size	\ ensure power of 2
		fftdata fmat_zero		\ zero the buffer		

		\ Determine frequency scale

		0 dsfft @xy fdrop
	        1 dsfft @xy fdrop
		f- fabs fft_dx f!
		1e fftdata mat_size@ drop s>f f/ fft_dx f@ f/
		fft_df f!
	     
		\ Transfer data from xyplot dataset to the fftdata buffer

		dsfft DatasetInfo->Npts @ 0 do
		  i dsfft @xy
		  i 2* 1+ 1 fftdata fmat!
		  0e i 1+ 2* 1 fftdata fmat! \ set imaginary part to zero
		  fdrop
		loop
	    
		fftdata fft	\ perform the Fast Fourier Transform

		\ Replace the data with appropriate x and Real(FT) values

		fftdata mat_size@ drop 1+ 1 do
		  i 1 fftdata fmat@
		  i 1+ 1 fftdata fmat!		\ store y value
		  i s>f fft_df f@ f* 		\ determine x value
		  i 1 fftdata fmat!
		2 +loop
	    
		\ make a new xyplot dataset

		c" Real(FT)" 1+ ds1 DatasetInfo->Name !
		c"  " 1+ ds1 DatasetInfo->Header !
		256 ds1 DatasetInfo->Type !
		fftdata mat_size@ drop ds1 DatasetInfo->Npts !
		2 ds1 DatasetInfo->Size !
		fftdata cell+ cell+ ds1 DatasetInfo->Data !		 	      
		ds1 make_ds drop
	  then
	else
	  drop
	then ;

: imag-ft ( -- | take Imag[FT] )
	?active dup 0 >=
	if
	  dsfft get_ds
	  0 >= if

		\ Set the size of the fft data buffer
		
		dsfft DatasetInfo->Npts @ adjust_fft_size  \ ensure power of 2
		fftdata fmat_zero		\ zero the buffer		

		\ Determine frequency scale

		0 dsfft @xy fdrop
	        1 dsfft @xy fdrop
		f- fabs fft_dx f!
		1e fftdata mat_size@ drop s>f f/ fft_dx f@ f/
		fft_df f!
	     
		\ Transfer data from xyplot dataset to the fftdata buffer

		dsfft DatasetInfo->Npts @ 0 do
		  i dsfft @xy
		  i 2* 1+ 1 fftdata fmat!
		  0e i 1+ 2* 1 fftdata fmat! \ set imaginary part to zero
		  fdrop
		loop
	    
		fftdata fft	\ perform the Fast Fourier Transform

		\ Replace the data with appropriate x and Imag(FT) values

		fftdata mat_size@ drop 1+ 1 do
		  i s>f fft_df f@ f* 		\ determine x value
		  i 1 fftdata fmat!
		2 +loop
	    
		\ make a new xyplot dataset

		c" Imag(FT)" 1+ ds1 DatasetInfo->Name !
		c"  " 1+ ds1 DatasetInfo->Name !
		256 ds1 DatasetInfo->Type !
		fftdata mat_size@ drop ds1 DatasetInfo->Npts !
		2 ds1 DatasetInfo->Size !
		fftdata cell+ cell+ ds1 DatasetInfo->Data !		 	      
		ds1 make_ds drop
	  then
	else
	  drop
	then ;

: real-ft^-1 ( -- | take Real[FT^-1] )
	?active dup 0 >=
	if
	  dsfft get_ds
	  0 >= if

		\ Set the size of the fft data buffer
		
		dsfft DatasetInfo->Npts @ adjust_fft_size	\ ensure power of 2
		fftdata fmat_zero		\ zero the buffer		

		\ Determine time scale

		0 dsfft @xy fdrop
		fabs 2e f* 1e fswap f/ fft_dt f!
	     
		\ Transfer data from xyplot dataset to the fftdata buffer

		dsfft DatasetInfo->Npts @ 0 do
		  i dsfft @xy
		  i 2* 1+ 1 fftdata fmat!
		  0e i 1+ 2* 1 fftdata fmat! \ set imaginary part to zero
		  fdrop
		loop
	    
		fftdata inv_fft	\ perform the Fast Fourier Transform

		\ Replace the data with appropriate x and Re(FT^-1) values

		fftdata mat_size@ drop 1+ 1 do
		  i 1 fftdata fmat@
		  i 1+ 1 fftdata fmat!		\ store y value
		  i 1- s>f fft_dt f@ f* 	\ determine x value
		  i 1 fftdata fmat!
		2 +loop
	    
		\ make a new xyplot dataset

		c" Real(FT^-1)" 1+ ds1 DatasetInfo->Name !
		c"  " 1+ ds1 DatasetInfo->Header !
		256 ds1 DatasetInfo->Type !
		fftdata mat_size@ drop ds1 DatasetInfo->Npts !
		2 ds1 DatasetInfo->Size !
		fftdata cell+ cell+ ds1 DatasetInfo->Data !	      
		ds1 make_ds drop
	  then
	else
	  drop
	then ;

:  power ( -- | take fft of data and obtain its power spectrum )
	?active dup 0 >=
	if
	  dsfft get_ds
	  0 >= if

		\ Set the size of the fft data buffer
		
		dsfft DatasetInfo->Npts @ adjust_fft_size	\ ensure power of 2
		fftdata fmat_zero		\ zero the buffer		

		\ Determine frequency scale

		0 dsfft @xy fdrop
	        1 dsfft @xy fdrop
		f- fabs fft_dx f!
		1e fftdata mat_size@ drop s>f f/ fft_dx f@ f/
		fft_df f!
	     
		\ Transfer data from xyplot dataset to the fftdata buffer

		dsfft DatasetInfo->Npts @ 0 do
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

		c" PowerSpectrum" 1+ ds1 DatasetInfo->Name !
		c"  " 1+ ds1 DatasetInfo->Header !
		256 ds1 DatasetInfo->Type !
		fftdata mat_size@ drop ds1 DatasetInfo->Npts !
		2 ds1 DatasetInfo->Size !
		fftdata cell+ cell+ ds1 DatasetInfo->Data !		 	      
		ds1 make_ds drop
	  then
	else
	  drop
	then ;	

MN_MATH c" Real(FT)"        c" real-ft" add_menu_item
MN_MATH c" Imag(FT)"        c" imag-ft" add_menu_item
MN_MATH c" Real(FT^-1)"     c" real-ft^-1" add_menu_item
MN_MATH c" Power Spectrum"  c" power"   add_menu_item

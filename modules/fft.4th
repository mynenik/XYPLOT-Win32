\ fft.4th
\
\ Compute Fourier transform of complex data
\
\ Notes:
\
\	The number of rows in the input matrix must be a power of 2.
\
\ Revisions:
\	8-16-2000 added inverse FFT routine  KM
\
variable nvals
variable fft_arr


: @a_n ( n -- f | fetch the n^th element in the matrix fft_arr )
	1 fft_arr a@ fmat@ ;

: !a_n ( f n -- | store f in the n^th element of matrix fft_arr )
 	1 fft_arr a@ fmat! ;

variable jj
variable mm
variable mmax
variable istep

fvariable wr
fvariable wi
fvariable wpr
fvariable wpi

: fft ( a -- | replace a with FFT )
	dup fft_arr !
	mat_size@ drop nvals !

	1 jj !

	nvals @ 1+ 1 do

	  jj @ i > if		\ exchange two complex numbers
	    jj @ @a_n
	    jj @ 1+ @a_n
	    i @a_n jj @ !a_n
	    i 1+ @a_n jj @ 1+ !a_n
	    i 1+ !a_n
	    i !a_n	
	  then

	  nvals @ 2/ mm !

	  begin
	    mm @ 2 >= jj @ mm @ > and
	  while
	    mm @ negate jj +!
	    mm @ 2/ mm !
	  repeat

	  mm @ jj +!
	2 +loop

	2 mmax !

	begin
	  nvals @ mmax @ >
	while
	  mmax @ dup 2* istep !
	  >r 6.28318530717959e0 r> s>f f/
	  fdup
	  0.5e f* fsin fdup f* -2e f* wpr f!
	  fsin wpi f!
	  1e wr f!
	  0e wi f!
	  
	  mmax @ 1+ 1 do
	    nvals @ 1+ i do
	      i mmax @ + jj !
	      jj @ @a_n wr f@ f* 
	      jj @ 1+ @a_n wi f@ f*
	      f-
	      jj @ 1+ @a_n wr f@ f*
	      jj @ @a_n wi f@ f*
	      f+			
	      fover
	      i @a_n fswap f- jj @ !a_n
	      i 1+ @a_n fover f- jj @ 1+ !a_n
	      i 1+ @a_n f+ i 1+ !a_n
	      i @a_n f+ i !a_n
	    istep @ +loop
	    
	    wr f@
	    fdup fdup wpr f@ f* f+ wi f@ wpi f@ f* f- wr f!
	    wpi f@ f* wi f@ wpr f@ f* f+ wi f@ f+ wi f!
	  2 +loop
	  
	  istep @ mmax !
	repeat
;


 

: inv_fft ( a -- | replace a with inverse FFT )
	dup fft_arr !
	mat_size@ drop nvals !

	1 jj !

	nvals @ 1+ 1 do

	  jj @ i > if		\ exchange two complex numbers
	    jj @ @a_n
	    jj @ 1+ @a_n
	    i @a_n jj @ !a_n
	    i 1+ @a_n jj @ 1+ !a_n
	    i 1+ !a_n
	    i !a_n	
	  then

	  nvals @ 2/ mm !

	  begin
	    mm @ 2 >= jj @ mm @ > and
	  while
	    mm @ negate jj +!
	    mm @ 2/ mm !
	  repeat

	  mm @ jj +!
	2 +loop

	2 mmax !

	begin
	  nvals @ mmax @ >
	while
	  mmax @ dup 2* istep !
	  >r 6.28318530717959e0 r> negate s>f f/
	  fdup
	  0.5e f* fsin fdup f* -2e f* wpr f!
	  fsin wpi f!
	  1e wr f!
	  0e wi f!
	  
	  mmax @ 1+ 1 do
	    nvals @ 1+ i do
	      i mmax @ + jj !
	      jj @ @a_n wr f@ f* 
	      jj @ 1+ @a_n wi f@ f*
	      f-
	      jj @ 1+ @a_n wr f@ f*
	      jj @ @a_n wi f@ f*
	      f+			
	      fover
	      i @a_n fswap f- jj @ !a_n
	      i 1+ @a_n fover f- jj @ 1+ !a_n
	      i 1+ @a_n f+ i 1+ !a_n
	      i @a_n f+ i !a_n
	    istep @ +loop
	    
	    wr f@
	    fdup fdup wpr f@ f* f+ wi f@ wpi f@ f* f- wr f!
	    wpi f@ f* wi f@ wpr f@ f* f+ wi f@ f+ wi f!
	  2 +loop
	  
	  istep @ mmax !
	repeat
;
 	  
	
\
\ derivative.4th
\
\ Compute the numerical derivative of a series of data values
\
\ Krishna Myneni, 30 Dec 1989
\ Revisions:
\	Translated from QuickBasic to VAX FORTRAN on 30 Aug 1990.
\       Modified for Microsoft Fortran 5.0 on 2 Oct 1991.
\	Translated to kForth on 3-30-2001.
\
\ Requires:
\	matrix.4th

\ der_m is the n x 2 input fmatrix with column 1 containing
\   x values and column 2 containing y values. The y values
\   are replaced with the corresponding value of the derivative
\

variable der_m
variable der_n
fvariable der_dx1
fvariable der_dx2
fvariable der_last_y

\ derivative returns an integer error code with the following meaning:
\	0 = no error, derivative computed successfully;
\	1 = two points have same x value, derivative not computed.

: derivative ( fm  -- ierr)

	dup mat_size@ drop der_n !
	der_m !

	\ Compute derivative at first pt with just two points.

	2 1 der_m a@ fmat@	\ x(2)
	1 1 der_m a@ fmat@	\ x(1)
	f- fdup der_dx1 f!
	f0= if 1 exit then	\ exit if dx = 0 between first two pts
	2 2 der_m a@ fmat@	\ y(2)
	1 2 der_m a@ fmat@	\ y(1)
	fdup der_last_y f!
	f- der_dx1 f@ f/
	1 2 der_m a@ fmat!

\  Calculate derivative with average of forward and backward slopes.

	der_n @ 2 do
	  i 1+ 1 der_m a@ fmat@		\ x(i+1)
	  i 1 der_m a@ fmat@		\ x(i)
	  f- der_dx1 f!
	  i 1 der_m a@ fmat@		\ x(i)
	  i 1- 1 der_m a@ fmat@		\ x(i-1)
	  f- der_dx2 f!

	  der_dx1 f@ f0= der_dx2 f@ f0= or
	  if 1 unloop exit then

	  der_last_y f@			\ y(i-1)
	  i 2 der_m a@ fmat@		\ y(i)
	  fdup der_last_y f!
	  fswap f- der_dx2 f@ f/
	  i 1+ 2 der_m a@ fmat@		\ y(i+1)
	  der_last_y f@			\ y(i)
	  f- der_dx1 f@ f/
	  f+ 2e f/
	  i 2 der_m a@ fmat!

	loop

	\ Compute derivate at last pt. using two pts.

	der_n @ 1 der_m a@ fmat@
	der_n @ 1- 1 der_m a@ fmat@ 
	f- fdup der_dx1 f!
	f0= if 1 exit then	\ exit if dx = 0 between last two pts

	der_n @ 2 der_m a@ fmat@
	der_last_y f@ f- der_dx1 f@ f/
	der_n @ 2 der_m a@ fmat!	

	\  Derivative computed successfully.

	0 ;


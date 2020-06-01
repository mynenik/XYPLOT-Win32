\ xypolyfit.4th   
\                                                               
\  Polynomial fitting routine for xyplot                          
\
\ Adapted from the routine polfit in P.R. Bevington,
\ "Data Reduction and Error Analysis for the Physical Sciences"
\
\ Requires the following source files:
\
\	matrix.4th
\
\ Revisions:
\              
\	10-2-98 Adapted for LabForth KM             
\	3-29-99 Adapted for kForth KM  
\	3-15-2000 Adapted for xyplot KM
\	4-15-2000 Added user query for order KM
\ 	12-12-2000 Indexing mods and use of fmatrix  KM
\        1-13-2005 use DatasetInfo structure creation word and update usage  KM

variable nterms     \ number of terms (order - 1)               
variable params     \ holds address of parameter matrix         
variable nmax                                                   
variable npts
                                                   
fvariable chisq                                                 
fvariable xterm                                                 
fvariable yterm                                                 
fvariable delta                                                 
                                                                
\ Floating point matrices used by polfit                        
                                                                
10 10 fmatrix apfit
19 1  fmatrix sumx                                    
10 1  fmatrix sumy
                    
\ ds = address of dataset info structure                    
\ a = address of fmatrix for receiving fitted parameters        
\ n = order of fitting polynomial                               
                                                                
: polfit ( ds a n -- chi-square | perform polynomial fit )     
    1+ nterms !                                                 
    params !                                                    
    nterms @ dup apfit mat_size!                               
    nterms @ 1 sumy mat_size!                                  
    nterms @ 1 params a@ mat_size!                              
    nterms @ 2* 1- nmax !                                       
    nmax @ 1 sumx mat_size!                                    
    dup DatasetInfo->Npts @ npts !                                  
    sumx fmat_zero                                              
    sumy fmat_zero                                              
    params a@ fmat_zero                                          
    0e chisq f!                                                 
    npts @ 0 do                                                 
      i over @xy	\ fetch the i^th point
      fswap		\ fy fx
      1e                                               
      nmax @ 1+ 1 do                                               
        fdup                                                
        i 1 sumx fmat_addr >r                                
        r@ f@ f+ r> f!                                          
        fover f*
      loop
      xterm f!
                                                      
      fover                                            
      nterms @ 1+ 1 do                                             
        fdup                                               
        i 1 sumy fmat_addr >r                                
        r@ f@ f+ r> f!                                          
        fover f*                                          
      loop
      yterm f!                                                      
      fover fdup f*
      chisq >r r@ f@ f+ r> f!                                   
      fdrop fdrop                                               
    loop                                                        
    drop	\ drop ds

    nterms @ 1+ 1 do                                               
      nterms @ 1+ 1 do                                             
        i j + 1- 1 sumx fmat@                                   
        j i apfit fmat!                                   
      loop                                                      
    loop                                                        
    apfit determ 
    fdup delta f!                                  
    0e f= if 0e exit then                                   
    nterms @ 1+ 1 do                                               
      nterms @ 1+ 1 do                                             
        nterms @ 1+ 1 do                                           
          i j + 1- 1 sumx fmat@                                 
          j i apfit fmat!                                 
        loop                                                    
        i 1 sumy fmat@                                       
        i j apfit fmat!                                   
      loop                                                      
      apfit determ
      delta f@ f/                                  
      i 1 params a@ fmat!                                     
    loop                                                        

\ Calculate chi-squared                                         

    nterms @ 1+ 1 do                                               
      chisq f@ i 1 params a@ fmat@                            
      i 1 sumy fmat@ f* 2e f*                                
      f- chisq f!                                               
      nterms @ 1+ 1 do                                             
        j i + 1- 1 sumx fmat@                                   
        i 1 params a@ fmat@ f*                                
        j 1 params a@ fmat@ f*                                
        chisq f@ f+ chisq f!                                    
      loop                                                      
    loop
                                      
    chisq f@                                                    
    npts @ nterms @ - s>f                                       
    f/ ;

                                                               
DatasetInfo  ds_pdata 
DatasetInfo  ds_pfit 

10 1  fmatrix  poly_params

variable norder

: xypoly ( n -- | fit a polynomial of order n to the active dataset )
	norder !
	?active dup 0 >=
	if
	  ds_pdata get_ds
	  0 >= if
	    ds_pdata poly_params norder @ polfit
	    ." Reduced chi-square = " f. cr 
	    nterms @ 0 do i dup . 2 spaces 1+ 1 poly_params fmat@ f. cr loop 
          then
	then ;

: xypolyfit ( -- | prompt user and fit the active data set )
	?active 0 >=
	if
	  c" Enter the order of the fitting polynomial:" get_input
	  if
	    string>s xypoly
	  else
	    drop
	  then
	then ;

\ add "Poly Fit" as an item in the math menu

MN_MATH c" Poly Fit" c" xypolyfit draw_window" add_menu_item


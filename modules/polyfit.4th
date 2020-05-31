\ polfit.4th   
\                                                               
\  Polynomial fitting routine in Forth                          
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
\	3-30-2001 used fmatrix to declare matrices KM
 
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
19 1 fmatrix sumx
10 1 fmatrix sumy


\ x = address of fmatrix containing x values                    
\ y = address of fmatrix containing y values                    
\ a = address of fmatrix for receiving fitted parameters        
\ n = order of fitting polynomial                               
                                                                
: polfit ( x y a n -- chi-square | perform polynomial fit )     
    1+ nterms !                                                 
    params !                                                    
    nterms @ dup apfit mat_size!                               
    nterms @ 1 sumy mat_size!                                  
    nterms @ 1 params a@ mat_size!                              
    nterms @ 2* 1- nmax !                                       
    nmax @ 1 sumx mat_size!                                    
    dup mat_size@ drop npts !                                  
    sumx fmat_zero                                              
    sumy fmat_zero                                              
    params a@ fmat_zero                                          
    0e chisq f!                                                 
    npts @ 0 do                                                 
      2dup                                                      
      i 1+ 1 rot fmat@                                          
      rot                                                       
      i 1+ 1 rot fmat@                                          
      1e xterm f!                                               
      nmax @ 0 do                                               
        xterm f@                                                
        i 1+ 1 sumx fmat_addr >r                                
        r@ f@ f+ r> f!                                          
        fdup xterm >r                                           
        r@ f@ f* r> f!                                          
      loop                                                      
      fover yterm f!                                            
      nterms @ 0 do                                             
        yterm f@                                                
        i 1+ 1 sumy fmat_addr >r                                
        r@ f@ f+ r> f!                                          
        fdup yterm >r                                           
        r@ f@ f* r> f!                                          
      loop                                                      
      fover fdup f*
      chisq >r r@ f@ f+ r> f!                                   
      fdrop fdrop                                               
    loop                                                        
    2drop                                                       
    nterms @ 0 do                                               
      nterms @ 0 do                                             
        i j + 1+ 1 sumx fmat@                                   
        j 1+ i 1+ apfit fmat!                                   
      loop                                                      
    loop                                                        
    apfit determ 
    fdup delta f!                                  
    0e f= if 0e exit then                                   
    nterms @ 0 do                                               
      nterms @ 0 do                                             
        nterms @ 0 do                                           
          i 1+ j + 1 sumx fmat@                                 
          j 1+ i 1+ apfit fmat!                                 
        loop                                                    
        i 1+ 1 sumy fmat@                                       
        i 1+ j 1+ apfit fmat!                                   
      loop                                                      
      apfit determ
      delta f@ f/                                  
      i 1+ 1 params a@ fmat!                                     
    loop                                                        

\ Calculate chi-squared                                         

    nterms @ 0 do                                               
      chisq f@ i 1+ 1 params a@ fmat@                            
      i 1+ 1 sumy fmat@ f* 2e f*                                
      f- chisq f!                                               
      nterms @ 0 do                                             
        j 1+ i + 1 sumx fmat@                                   
        i 1+ 1 params a@ fmat@ f*                                
        j 1+ 1 params a@ fmat@ f*                                
        chisq f@ f+ chisq f!                                    
      loop                                                      
    loop
                                      
    chisq f@                                                    
    npts @ nterms @ - s>f                                       
    f/ ;
                                                               
                                                                
                                                                
                                                                
                                                                
                                                                
                                                                
                                                                
                                                                
                                                                
                                                                

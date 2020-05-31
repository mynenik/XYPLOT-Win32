\ smooth.4th
\
\ Smooth function for xyplot
\
\ Copyright (c) 1999--2000 Krishna Myneni
\ Creative Consulting for Research and Education
\
\ Last Revised: 2-21-1999
\

DatasetInfo dsa 	\ active dataset info structure

fvariable ylast

: smooth ( -- | smooth the y values of the active dataset )
	?active dup 0 >=
	if
	  dsa get_ds
	  0 >= if
	    0 dsa @xy fdup ylast f! 3e f*
	    1 dsa @xy fswap fdrop f+ 4e f/
	    0 dsa !xy
	    dsa ->npts 1-
	    1 do
	      ylast f@
	      i dsa @xy
	      fdup ylast f! 2e f* frot f+	      
	      i 1+ dsa @xy fswap fdrop f+
	      4e f/ i dsa !xy
	    loop
	    dsa ->npts 1- dsa @xy 3e f* 
	    ylast f@ f+ 4e f/  
	    dsa ->npts 1- dsa !xy
	    ?active set_ds_extrema
	  then
	else
	  drop
	then ;

\ add "smooth" as an item in the math menu

MN_MATH " Smooth" " smooth draw_window" add_menu_item




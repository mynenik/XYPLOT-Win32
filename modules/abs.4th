\ abs.4th
\
\ Take absolute value of y values of the active dataset
\
\ Copyright (c) 2000 Krishna Myneni
\
\ Revisions:
\	6-21-2000 KM

DatasetInfo ds_info

: abs_ds ( -- )
	?active			\ get the active set number
	dup 0 >=		\ is it valid (non-negative)?
	if
	  ds_info get_ds	\ ok, get info about the dataset
	  0 >=			\ did get_ds return an error?
	  if
	    ds_info ->npts	\ ok, obtain the number of points
	    0 do	
	      i ds_info @xy	\ fetch the i^th x, y pair 
	      fabs  		\ take absolute value
	      i ds_info !xy	\ store the i^th x, y pair
	    loop
	    ?active
	    set_ds_extrema	\ reset the extrema for that dataset
	  then
	else
	  drop
	then ;

\ add this function to the math menu

MN_MATH " Magnitude" " abs_ds reset_window" add_menu_item
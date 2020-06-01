\ yln.4th
\
\ Take log base e of y values of a dataset
\
\ K. Myneni, 2003-06-27 (modified ylog.4th)
\
: yln
	?active dup 0 >=
	if
	  dsa get_ds
	  0 >= if
	    dsa DatasetInfo->Npts @ 0 do
	      i dsa @xy fdup 0e f>	\ test for y > 0
	      if fln else fdrop 0e then
	      i dsa !xy
	    loop
	    ?active set_ds_extrema
	  then
	else
	  drop
	then ;


\ add "Ln(Y)" as an item in the math menu

MN_MATH c" Ln(Y)" c" yln reset_window" add_menu_item

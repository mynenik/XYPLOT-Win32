\ ylog.4th
\
\ Take log base 10 of y values of a dataset
\
\ K. Myneni, 8-14-2000
\
: ylog
	?active dup 0 >=
	if
	  dsa get_ds
	  0 >= if
	    dsa ->npts 0 do
	      i dsa @xy fdup 0e f>	\ test for y > 0
	      if flog else fdrop 0e then
	      i dsa !xy
	    loop
	    ?active set_ds_extrema
	  then
	else
	  drop
	then ;


\ add "Log(Y)" as an item in the math menu

MN_MATH " Log(Y)" " ylog reset_window" add_menu_item

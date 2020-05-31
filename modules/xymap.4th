\ xymap.4th
\
\ xyplot module for creating maps on x and y
\
\ Krishna Myneni, 4-1-2000
\
\ Required files:
\	xyplot.4th
\	matrix.4th
\
\ Revisions:
\

DatasetInfo dsmap

16384 constant MAX_MAP_PTS
MAX_MAP_PTS 2 fmatrix map_data
map_data 8 + dsmap DDATA + !		\ store the data pointer

variable map_type

: map ( ntype -- | create a map )
  \ ntype = 
  \	0 for map of y_n+1 vs y_n
  \ 	1 for map of x_n+1 vs x_n
  \	2 for map of dy_n+1 vs dy_n
  \	3 for map of dx_n+1 vs dx_n
  \	4 for map of dy_n+1 vs dx_n


	map_type !
	?active dup
	0 >= if
	  ds1 get_ds
	  0 >= if

	    \ Create map

	    map_type @
	    case
	      0 of
	        ds1 ->npts 1 do
	          i 1- ds1 @xy fswap fdrop
	          i 1 map_data fmat!
	          i ds1 @xy fswap fdrop
	          i 2 map_data fmat!
	        loop
 		" Map of y_n+1 vs y_n"
	        ds1 ->npts 1-  
	      endof
	        
	      1 of
	        ds1 ->npts 1 do
	          i 1- ds1 @xy fdrop
	          i 1 map_data fmat!
	          i ds1 @xy fdrop
	          i 2 map_data fmat!
	        loop
 		" Map of x_n+1 vs x_n"
	        ds1 ->npts 1-  
	      endof

	      2 of
	        ds1 ->npts 1- 1 do
	          i ds1 @xy fswap fdrop i 1- ds1 @xy fswap fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fswap fdrop i ds1 @xy fswap fdrop f-
	          i 2 map_data fmat!
	        loop
	        " Map of dy_n+1 vs dy_n"
	        ds1 ->npts 2-
	      endof

	      3 of
	        ds1 ->npts 1- 1 do
	          i ds1 @xy fdrop i 1- ds1 @xy fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fdrop i ds1 @xy fdrop f-
	          i 2 map_data fmat!
	        loop
	        " Map of dx_n+1 vs dx_n"
	        ds1 ->npts 2-
	      endof

	      4 of
	        ds1 ->npts 1- 1 do
	          i ds1 @xy fdrop i 1- ds1 @xy fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fswap fdrop i ds1 @xy fswap fdrop f-
	          i 2 map_data fmat!
	        loop
	        " Map of dy_n+1 vs dx_n"
	        ds1 ->npts 2-
	      endof
	      "  " 0
	    endcase

	    dup 0= if 2drop ." Unrecognized map type." exit then

	    2 map_data mat_size!	\ set size of map_data

	    \ Create the map dataset in xyplot

	    1+ dsmap DHEADER + !
	    " Map" 1+ dsmap DNAME + !
	    256 dsmap DTYPE + !		\ double precision fp type
	    map_data mat_size@ drop dsmap DNPTS + !
	    2 dsmap DSIZE + !

	    dsmap make_ds
	  then
	else
	  drop
	then ;	    
	    
MN_MATH " Map: y_n+1 vs y_n" " 0 map" add_menu_item
MN_MATH " Map: x_n+1 vs x_n" " 1 map" add_menu_item
MN_MATH " Map: dy_n+1 vs dy_n" " 2 map" add_menu_item
MN_MATH " Map: dx_n+1 vs dx_n" " 3 map" add_menu_item
MN_MATH " Map: dy_n+1 vs dx_n" " 4 map" add_menu_item
 	 	
	

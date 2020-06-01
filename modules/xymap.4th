\ xymap.4th
\
\ xyplot module for creating 1-D maps on x or y values of a dataset. 
\  Maps are useful for analyzing chaotic sequences. For example,
\  a dataset can contain the sequence of peaks in a chaotic waveform, 
\  where the x values are the positions, and the y values are the peak
\  heights. A "return-map" on the peak heights or heights usually shows
\  a simple structure for low-dimensional chaotic systems. The peak 
\  searching module for xyplot (xypeaks.4th) may be used to 
\  extract peak information from waveform data.
\
\ Copyright (c) 2000--2005 Krishna Myneni
\ Provided under the GNU General Public License
\
\ Required files:
\	xyplot.4th
\	matrix.4th
\
\ Revisions:
\      4-1-2000   created
\      1-15-2005  updated use of DatasetInfo structure km

DatasetInfo dsmap

32768 4 * constant MAX_MAP_PTS
MAX_MAP_PTS 2 fmatrix map_data
map_data 2 cells + dsmap DDATA !	\ store the data pointer

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
	        ds1 DatasetInfo->Npts @ 1 do
	          i 1- ds1 @xy fswap fdrop
	          i 1 map_data fmat!
	          i ds1 @xy fswap fdrop
	          i 2 map_data fmat!
	        loop
 		c" Map of y_n+1 vs y_n"
	        ds1 DatasetInfo->Npts @ 1-  
	      endof
	        
	      1 of
	        ds1 DatasetInfo->Npts @ 1 do
	          i 1- ds1 @xy fdrop
	          i 1 map_data fmat!
	          i ds1 @xy fdrop
	          i 2 map_data fmat!
	        loop
 		c" Map of x_n+1 vs x_n"
	        ds1 DatasetInfo->Npts @ 1-  
	      endof

	      2 of
	        ds1 DatasetInfo->Npts @ 1- 1 do
	          i ds1 @xy fswap fdrop i 1- ds1 @xy fswap fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fswap fdrop i ds1 @xy fswap fdrop f-
	          i 2 map_data fmat!
	        loop
	        c" Map of dy_n+1 vs dy_n"
	        ds1 DatasetInfo->Npts @ 2-
	      endof

	      3 of
	        ds1 DatasetInfo->Npts @ 1- 1 do
	          i ds1 @xy fdrop i 1- ds1 @xy fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fdrop i ds1 @xy fdrop f-
	          i 2 map_data fmat!
	        loop
	        c" Map of dx_n+1 vs dx_n"
	        ds1 DatasetInfo->Npts @ 2-
	      endof

	      4 of
	        ds1 DatasetInfo->Npts @ 1- 1 do
	          i ds1 @xy fdrop i 1- ds1 @xy fdrop f-
	          i 1 map_data fmat!
	          i 1+ ds1 @xy fswap fdrop i ds1 @xy fswap fdrop f-
	          i 2 map_data fmat!
	        loop
	        c" Map of dy_n+1 vs dx_n"
	        ds1 DatasetInfo->Npts @ 2-
	      endof
	      "  " 0
	    endcase

	    dup 0= if 2drop ." Unrecognized map type." exit then

	    2 map_data mat_size!	\ set size of map_data

	    \ Create the map dataset in xyplot

	    1+ dsmap DatasetInfo->HEADER !
	    c" Map" 1+ dsmap DatasetInfo->Name !
	    256 dsmap DatasetInfo->Type !		\ double precision fp type
	    map_data mat_size@ drop dsmap DatasetInfo->Npts !
	    2 dsmap DatasetInfo->Size !

	    dsmap make_ds
	  then
	else
	  drop
	then ;	    
	    
MN_MATH  c" Map: y_n+1 vs y_n"    c" 0 map"  add_menu_item
MN_MATH  c" Map: x_n+1 vs x_n"    c" 1 map"  add_menu_item
MN_MATH  c" Map: dy_n+1 vs dy_n"  c" 2 map"  add_menu_item
MN_MATH  c" Map: dx_n+1 vs dx_n"  c" 3 map"  add_menu_item
MN_MATH  c" Map: dy_n+1 vs dx_n"  c" 4 map"  add_menu_item
 	 	
	

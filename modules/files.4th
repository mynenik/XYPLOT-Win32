\ files.4th
\
\ This code provides kForth with a subset of the optional 
\ file access word set, following the guidelines of the ANS 
\ specifications.
\
\ Note that kForth (as of Rls. 3-2-1999) has the built-in
\ low level file access words OPEN, LSEEK, CLOSE, READ, WRITE.
\ The definitions herein provide some of the ANS compatible
\ word set and useful constants.
\
\ Copyright (c) 1999--2003 Krishna Myneni
\ Creative Consulting for Research and Education
\
\ This software is provided under the terms of the GNU General
\ Public License.
\
\ Revisions:
\
\ 	3-2-1999  created
\	3-6-1999 
\	4-25-1999 added read-line KM
\	10-15-1999 added file-exists KM
\	12-20-1999 fixed create-file and open-file; now
\	           requires strings.4th  KM
\	11-24-2000 modified O_CREAT and O_APPEND for 
\                  Cygwin compatibility  KM
\       4-07-2002 fixed defn of reposition-file; added file-position
\		    and file-size  KM
\	8-13-2002 fixed defn of file-size to not modify file-position  KM
\	9-19-2002 fixed read-line to return success for file with
\	            single line, even if line has no EOL  KM
\	5-16-2003 added constants O_EXCL and O_TRUNC  km
base @
hex
0 constant R/O
1 constant W/O
2 constant R/W
A constant EOL
200 constant O_CREAT
800 constant O_EXCL
400 constant O_TRUNC
  8 constant O_APPEND
0 constant SEEK_SET
1 constant SEEK_CUR
2 constant SEEK_END
base !
create EOL_BUF 4 allot
EOL EOL_BUF c!
0 EOL_BUF 1+ c!

variable read_count

: create-file ( c-addr count fam -- fileid ior )
	>r strpck r> O_CREAT or open
	dup 0> invert ;

: open-file ( c-addr count fam -- fileid ior )
	>r strpck r> open
	dup 0> invert ;

: close-file ( fileid -- ior )
	close ;

: read-file ( c-addr u1 fileid -- u2 ior )
	-rot read dup -1 = ;
 	 
: write-file ( c-addr u fileid -- ior )
	-rot write ;

: file-position ( fileid -- ud ior )
	0 SEEK_CUR lseek dup -1 = >r s>d r> ;

: reposition-file ( ud fileid -- ior )
	-rot drop SEEK_SET lseek ;

: file-size ( fileid -- ud ior )
	dup >r r@ file-position drop 2>r
	0 SEEK_END lseek dup -1 = >r s>d r> 
	2r> r> reposition-file drop ;


: file-exists ( ^filename  -- flag | return true if file exists )
	count R/O open-file
	if drop false else close-file drop true then ;	

: read-line ( c-addr u1 fileid -- u2 flag ior )
	-rot 0 read_count !
	0 do
	  2dup 1 read
	  1 < if
	    2drop read_count @ dup
	    if true 0 else false -1 then unloop exit
	  then
	  dup c@ EOL =
	  if
	    2drop read_count @ true 0 unloop exit
	  then
	  1+
	  1 read_count +!
	loop
	2drop read_count @ true 0 ;

: write-line ( c-addr u fileid -- ior )
	dup >r write-file
	EOL_BUF 1 r> write-file
	or ;


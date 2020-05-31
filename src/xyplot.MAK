ORIGIN		= Symantec C++
ORIGIN_VER	= Version 7.22
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= xyplot
APPTYPE		= WIN32 EXE
PROJTYPE	= EXE

CC		= SC
CPP		= SPP
JAVAC		= sj
MAKE		= SMAKE
RC		= RCC
HC		= HC31
ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= D:\SC\include\ctype.h D:\SC\include\stdlib.h D:\SC\include\string.h  \
		D:\SC\include\fstream.h D:\SC\include\iostream.h D:\SC\include\stdio.h D:\SC\include\math.h  \
		D:\SC\mfc\include\afxwin.h D:\SC\mfc\include\32-bit\afxwin.h D:\SC\mfc\include\afx.h D:\SC\mfc\include\32-bit\afx.h  \
		D:\SC\mfc\include\afxver_.h D:\SC\mfc\include\32-bit\afxver_.h D:\SC\mfc\include\afxv_w32.h D:\SC\mfc\include\32-bit\afxv_w32.h  \
		D:\SC\include\windows.h D:\SC\include\win32\scdefs.h D:\SC\include\win32\WINDOWS.H D:\SC\include\excpt.h  \
		D:\SC\include\stdarg.h D:\SC\include\windef.h D:\SC\include\win32\WINDEF.H D:\SC\include\winnt.h  \
		D:\SC\include\win32\WINNT.H D:\SC\include\pshpack4.h D:\SC\include\win32\pshpack4.h D:\SC\include\poppack.h  \
		D:\SC\include\win32\poppack.h D:\SC\include\win32\pshpack2.h D:\SC\include\winbase.h D:\SC\include\win32\WINBASE.H  \
		D:\SC\include\winerror.h D:\SC\include\win32\WINERROR.H D:\SC\include\wingdi.h D:\SC\include\win32\WINGDI.H  \
		D:\SC\include\pshpack1.h D:\SC\include\win32\pshpack1.h D:\SC\include\pshpack2.h D:\SC\include\winuser.h  \
		D:\SC\include\win32\WINUSER.H D:\SC\include\winnls.h D:\SC\include\win32\WINNLS.H D:\SC\include\wincon.h  \
		D:\SC\include\win32\WINCON.H D:\SC\include\winver.h D:\SC\include\win32\WINVER.H D:\SC\include\winreg.h  \
		D:\SC\include\win32\WINREG.H D:\SC\include\winnetwk.h D:\SC\include\win32\WINNETWK.H D:\SC\include\winsvc.h  \
		D:\SC\include\win32\WINSVC.H D:\SC\include\mcx.h D:\SC\include\win32\MCX.H D:\SC\include\imm.h  \
		D:\SC\include\win32\IMM.H D:\SC\include\commctrl.h D:\SC\include\win32\COMMCTRL.H D:\SC\include\prsht.h  \
		D:\SC\include\win32\PRSHT.H D:\SC\include\tchar.h D:\SC\include\mbstring.h D:\SC\include\mbctype.h  \
		D:\SC\include\time.h D:\SC\include\limits.h D:\SC\include\stddef.h D:\SC\include\crtdbg.h  \
		D:\SC\mfc\include\afxcoll.h D:\SC\mfc\include\32-bit\afxcoll.h D:\SC\mfc\include\afxstat_.h D:\SC\mfc\include\32-bit\afxstat_.h  \
		D:\SC\mfc\include\afxtls_.h D:\SC\mfc\include\32-bit\afxtls_.h D:\SC\include\shellapi.h D:\SC\include\win32\shellapi.h  \
		D:\SC\mfc\include\afxres.h D:\SC\mfc\include\32-bit\afxres.h D:\SC\mfc\include\afxmsg_.h D:\SC\mfc\include\32-bit\afxmsg_.h  \
		D:\SC\mfc\include\afxdd_.h D:\SC\mfc\include\32-bit\afxdd_.h D:\SC\include\strstrea.h D:\SC\mfc\include\afxdlgs.h  \
		D:\SC\mfc\include\32-bit\afxdlgs.h D:\SC\include\commdlg.h D:\SC\include\win32\COMMDLG.H D:\SC\include\richedit.h  \
		D:\SC\include\win32\RICHEDIT.H D:\SC\mfc\include\afxext.h D:\SC\mfc\include\32-bit\afxext.h D:\SC\include\sys\types.h  \
		D:\SC\include\sys\stat.h D:\SC\include\io.h D:\SC\include\conio.h D:\SC\include\fcntl.h  \
		D:\sc\include\stl\vector.h D:\sc\include\stl\function.h D:\sc\include\stl\bool.h D:\sc\include\stl\algobase.h  \
		D:\sc\include\stl\pair.h D:\sc\include\stl\iterator.h D:\sc\include\stl\defalloc.h D:\sc\include\new.h  \
		D:\sc\include\stl\stack.h D:\sc\include\stl\heap.h D:\sc\include\stl\deque.h D:\sc\include\sys\time.h  \
		D:\sc\include\sys\timeb.h D:\sc\include\termios.h D:\sc\include\sys\termio.h D:\sc\include\unistd.h  \
		..\..\sc\include\stl\vector.h ..\..\sc\include\stl\function.h ..\..\sc\include\stl\bool.h ..\..\sc\include\stl\algobase.h  \
		..\..\sc\include\stl\pair.h ..\..\sc\include\stl\iterator.h ..\..\sc\include\stddef.h ..\..\sc\include\iostream.h  \
		..\..\sc\include\string.h ..\..\sc\include\stl\defalloc.h ..\..\sc\include\new.h ..\..\sc\include\stdlib.h  \
		..\..\sc\include\limits.h ..\..\sc\include\stl\stack.h ..\..\sc\include\stl\heap.h ..\..\sc\include\stl\deque.h  \
		..\..\sc\include\ctype.h fbc.h consts.h CDatabase.h  \
		CWorkspace41.h xyp41.h ..\..\sc\include\fstream.h CDataset.h  \
		CXyFile.h ..\..\sc\include\stdio.h saveopts.h ..\..\sc\include\math.h  \
		CGrid.h CPlotObject.h CTransform.h ..\..\SC\mfc\include\afxwin.h  \
		..\..\SC\mfc\include\32-bit\afxwin.h ..\..\SC\mfc\include\afx.h ..\..\SC\mfc\include\32-bit\afx.h ..\..\SC\mfc\include\afxver_.h  \
		..\..\SC\mfc\include\32-bit\afxver_.h ..\..\SC\mfc\include\afxv_w32.h ..\..\SC\mfc\include\32-bit\afxv_w32.h ..\..\sc\include\windows.h  \
		..\..\sc\include\win32\scdefs.h ..\..\sc\include\win32\WINDOWS.H ..\..\sc\include\excpt.h ..\..\sc\include\stdarg.h  \
		..\..\sc\include\windef.h ..\..\sc\include\win32\WINDEF.H ..\..\sc\include\winnt.h ..\..\sc\include\win32\WINNT.H  \
		..\..\sc\include\pshpack4.h ..\..\sc\include\win32\pshpack4.h ..\..\sc\include\poppack.h ..\..\sc\include\win32\poppack.h  \
		..\..\sc\include\win32\pshpack2.h ..\..\sc\include\winbase.h ..\..\sc\include\win32\WINBASE.H ..\..\sc\include\winerror.h  \
		..\..\sc\include\win32\WINERROR.H ..\..\sc\include\wingdi.h ..\..\sc\include\win32\WINGDI.H ..\..\sc\include\pshpack1.h  \
		..\..\sc\include\win32\pshpack1.h ..\..\sc\include\pshpack2.h ..\..\sc\include\winuser.h ..\..\sc\include\win32\WINUSER.H  \
		..\..\sc\include\winnls.h ..\..\sc\include\win32\WINNLS.H ..\..\sc\include\wincon.h ..\..\sc\include\win32\WINCON.H  \
		..\..\sc\include\winver.h ..\..\sc\include\win32\WINVER.H ..\..\sc\include\winreg.h ..\..\sc\include\win32\WINREG.H  \
		..\..\sc\include\winnetwk.h ..\..\sc\include\win32\WINNETWK.H ..\..\sc\include\winsvc.h ..\..\sc\include\win32\WINSVC.H  \
		..\..\sc\include\mcx.h ..\..\sc\include\win32\MCX.H ..\..\sc\include\imm.h ..\..\sc\include\win32\IMM.H  \
		..\..\sc\include\commctrl.h ..\..\sc\include\win32\COMMCTRL.H ..\..\sc\include\prsht.h ..\..\sc\include\win32\PRSHT.H  \
		..\..\sc\include\tchar.h ..\..\sc\include\mbstring.h ..\..\sc\include\mbctype.h ..\..\sc\include\time.h  \
		..\..\sc\include\crtdbg.h ..\..\SC\mfc\include\afxcoll.h ..\..\SC\mfc\include\32-bit\afxcoll.h ..\..\SC\mfc\include\afxstat_.h  \
		..\..\SC\mfc\include\32-bit\afxstat_.h ..\..\SC\mfc\include\afxtls_.h ..\..\SC\mfc\include\32-bit\afxtls_.h ..\..\SC\mfc\include\32-bit\afxres.h  \
		..\..\SC\mfc\include\afxres.h xyplot.h ForthVM.h ForthCompiler.h  \
		CSaveOptionsDialog.h CVerifyDialog.h CHeaderDialog.h CGridDialog.h  \
		CPlotWindow.h ColorTable.h CXyPlot.h CPolarGrid.h  \
		CPlot.h CPlotList.h CPlotDisplay.h CPickDataDialog.h  \
		resource.h CInputDialog.h ..\..\sc\include\shellapi.h ..\..\sc\include\win32\shellapi.h  \
		..\..\SC\mfc\include\afxmsg_.h ..\..\SC\mfc\include\32-bit\afxmsg_.h ..\..\SC\mfc\include\afxdd_.h ..\..\SC\mfc\include\32-bit\afxdd_.h  \
		..\..\sc\include\strstrea.h ..\..\SC\mfc\include\afxdlgs.h ..\..\SC\mfc\include\32-bit\afxdlgs.h ..\..\sc\include\commdlg.h  \
		..\..\sc\include\win32\COMMDLG.H ..\..\sc\include\richedit.h ..\..\sc\include\win32\RICHEDIT.H ..\..\SC\mfc\include\afxext.h  \
		..\..\SC\mfc\include\32-bit\afxext.h ..\..\sc\include\sys\types.h ..\..\sc\include\sys\time.h ..\..\sc\include\sys\timeb.h  \
		..\..\sc\include\sys\stat.h ..\..\sc\include\termios.h ..\..\sc\include\sys\termio.h ..\..\sc\include\unistd.h  \
		..\..\sc\include\fcntl.h ..\..\sc\include\typeinfo.h ..\..\SC\mfc\include\32-bit\winres.h ..\..\sc\include\win32\objidl.h  \
		..\..\sc\include\win32\unknwn.h ..\..\sc\include\win32\wtypes.h ..\..\sc\include\win32\ole2.h ..\..\sc\include\win32\rpcndr.h  \
		..\..\sc\include\win32\rpc.h 

DEFFILE		= xyplot.DEF

!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= ..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib  \
		..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib  \
		..\..\Sc\LIB\winspool.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Ae -Ar -mn -C -WA -S -3 -a8 -c -gf 
LFLAGS		=  /CO /NOI /DE /NOPACKF /XN /NT /ENTRY:WinMainCRTStartup /BAS:4194304 /A:512 /RC   :XYPLOT.RES
DEFINES		= -D_DEBUG=1 -D_X86_=1 -D_MT=1 -D_MBCS=1
!ELSE
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= ..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib  \
		..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib  \
		..\..\Sc\LIB\winspool.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 

CFLAGS		=  -Ae -Ar -mn -WA -3 -a8 -c 
LFLAGS		=  /M /NOI /DE /DET /PACKF /XN /NT /ENTRY:WinMainCRTStartup /BAS:4194304 /A:512 /RC   :XYPLOT.RES
DEFINES		= -D_X86_=1 -D_MT=1 -D_MBCS=1
!ENDIF

HFLAGS		= $(CFLAGS) 
MFLAGS		= MASTERPROJ=$(PROJ) 
LIBFLAGS	=  /C 
RESFLAGS	=  -32 
DEBUGGERFLAGS	=  
AFLAGS		= $(CFLAGS) 
HELPFLAGS	= 

MODEL		= N

PAR		= PROJS BATS OBJS

RCDEFINES	= 

INCLUDES	= -IC:\sc\include -IC:\sc\include\stl

INCLUDEDOBJS	=  VM.OBJ 

OBJS		= CDatabase.OBJ CDataset.OBJ CGrid.OBJ  \
		CInputDialog.OBJ CPickDataDialog.OBJ CPlotDisplay.OBJ CPlotList.OBJ  \
		CPlotObject.OBJ CPlotWindow.OBJ CPolarGrid.OBJ CWorkspace41.OBJ  \
		CXyFile.OBJ CXyPlot.OBJ XYPLOT.OBJ CGridDialog.OBJ  \
		CHeaderDialog.OBJ CTransform.OBJ AlgebraCompiler.OBJ CVerifyDialog.OBJ  \
		CSaveOptionsDialog.OBJ vmc.OBJ ForthCompiler.OBJ ForthVM.OBJ 

RCFILES		=  XYPLOT.RES 

RESFILES	= 

HELPFILES	= 

BATS		= 

.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 

.C.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.c

.CPP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cpp

.CXX.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cxx

.CC.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cc

.CP.OBJ:
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.cp

.H.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.h

.HPP.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hpp

.HXX.SYM:
	$(CC) $(HFLAGS) $(DEFINES) $(INCLUDES) -HF -o$(*B).sym $*.hxx

.C.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)   $*.c   -o$*.lst

.CPP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cpp -o$*.lst

.CXX.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.cxx -o$*.lst

.CP.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cp  -o$*.lst

.CC.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES)  $*.cc  -o$*.lst

.ASM.EXP:
	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

.OBJ.COD:
	$(DISASM) $*.OBJ -c

!IF $(DEBUG)
.OBJ.EXE:
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
NUL
..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ELSE
.OBJ.EXE:
		$(LNK) $(LFLAGS) @$(PROJ).LNK<<
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
xyplot.MAP
C:\sc\lib\ ..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\MFC\LIB\nafxcw.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ENDIF

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

.ASM.OBJ:
	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

.RC.RES: 
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $*.rc -o$*.res

.DLG.RES:
	echo ^#include "windows.h" >$$$*.rc
	echo ^IF EXIST "$*.h" >>$$$*.rc
	echo ^#include "$*.h" >>$$$*.rc
	echo ^#include "$*.dlg" >>$$$*.rc
	$(RC) $(RCDEFINES) $(RESFLAGS) $(INCLUDES) $$$*.rc
	-del $*.res
	-ren $$$*.res $*.res



all:	noteout createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done


all2:	createdir $(PRECOMPILE) $(SYMS) $(OBJS) $(INCLUDEDOBJS) $(POSTCOMPILE) $(TARGETDIR)\$(PROJ).$(PROJTYPE) $(POSTLINK) _done

noteout:
	REM Output to $(OUTPUTDIR)

createdir:
	$(CREATEOUTPUTDIR)
	$(CREATETARGETDIR)
	
$(TARGETDIR)\$(PROJ).$(PROJTYPE): $(OBJS) $(INCLUDEDOBJS) $(RCFILES) $(RESFILES) $(HELPFILES) $(DEFFILE)
			-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
NUL
..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
xyplot.MAP
C:\sc\lib\ ..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\MFC\LIB\nafxcw.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ENDIF

			-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)
			-echo $(TARGETDIR)\$(PROJ).$(PROJTYPE) built

_done:
		REM  Project is up to date

buildall:	clean	all


clean:
		-del $(TARGETDIR)\$$SCW$$.$(PROJTYPE)
		-del $(PROJ).CLE
		-del $(OUTPUTDIR)\SCPH.SYM
		-del xyplot.dpd
		-del $(OBJS)
		-del XYPLOT.RES

cleanres:
		-del XYPLOT.RES

res:		cleanres $(RCFILES) all


link:
!IF $(DEBUG)
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
NUL
..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ELSE
		$(LNK) $(LFLAGS) @<<$(PROJ).LNK
CDatabase.OBJ+
CDataset.OBJ+
CGrid.OBJ+
CInputDialog.OBJ+
CPickDataDialog.OBJ+
CPlotDisplay.OBJ+
CPlotList.OBJ+
CPlotObject.OBJ+
CPlotWindow.OBJ+
CPolarGrid.OBJ+
CWorkspace41.OBJ+
CXyFile.OBJ+
CXyPlot.OBJ+
XYPLOT.OBJ+
CGridDialog.OBJ+
CHeaderDialog.OBJ+
CTransform.OBJ+
AlgebraCompiler.OBJ+
CVerifyDialog.OBJ+
CSaveOptionsDialog.OBJ+
VM.OBJ+
vmc.OBJ+
ForthCompiler.OBJ+
ForthVM.OBJ
$$SCW$$.EXE
xyplot.MAP
C:\sc\lib\ ..\..\Sc\LIB\gdi32.lib ..\..\Sc\LIB\kernel32.lib ..\..\Sc\LIB\user32.lib ..\..\Sc\LIB\advapi32.lib ..\..\Sc\LIB\comctl32.lib ..\..\Sc\LIB\comdlg32.lib ..\..\Sc\LIB\shell32.lib ..\..\Sc\LIB\winspool.lib ..\..\Sc\MFC\LIB\nafxcw.lib ..\..\Sc\LIB\snn.lib KERNEL32.LIB GDI32.LIB USER32.LIB 
xyplot.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (xyplot.dpd)
!INCLUDE xyplot.dpd
!ENDIF



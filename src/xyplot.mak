ORIGIN		= Digital Mars C++
ORIGIN_VER	= Version 8.57
VERSION		= RELEASE

!IFDEF SUB_DEBUG
DEBUG		= $(SUB_DEBUG)
NDEBUG		= !$(SUB_DEBUG)
!ELSE
DEBUG		= 0
NDEBUG		= 1
!ENDIF

PROJ		= xypw32
APPTYPE		= WIN32 EXE
PROJTYPE	= exe

CC		= SC
CPP		= SPP
JAVAC		= sj
MAKE		= SMAKE
RC		= RCC
HC		= HC31
# ASM		= SC
DISASM		= OBJ2ASM
LNK		= LINK
DLLS		= 

HEADERS		= fbc.h \
		consts.h \
		CDatabase.h  \
		CWorkspace41.h \
		xyp41.h \
		CDataset.h  \
		CXyFile.h \
		saveopts.h \
		CGrid.h \
		CPlotObject.h \
		CTransform.h \
		xyplot.h \
		ForthVM.h \
		ForthCompiler.h  \
		CSaveOptionsDialog.h \
		CVerifyDialog.h \
		CHeaderDialog.h \
		CGridDialog.h  \
		CPlotWindow.h \
		ColorTable.h \
		CXyPlot.h \
		CPolarGrid.h  \
		CPlot.h \
		CPlotList.h \
		CPlotDisplay.h \
		CPickDataDialog.h  \
		resource.h \
		CInputDialog.h


!IF $(DEBUG)
OUTPUTDIR	= .
CREATEOUTPUTDIR	=
TARGETDIR	= .
CREATETARGETDIR	=

SYMROOT		= 
SYMS		= 
LIBS		= gdi32.lib kernel32.lib user32.lib  \
		advapi32.lib comctl32.lib comdlg32.lib \
		shell32.lib  \
		winspool.lib \
		snn.lib 

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
LIBS		= gdi32.lib \
		  kernel32.lib \
		  user32.lib  \
		  advapi32.lib \
		  comctl32.lib \
		  comdlg32.lib \
		  shell32.lib  \
		  winspool.lib \
		  snn.lib 

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

INCLUDES	= -ID:\dm\stlport\stlport 

INCLUDEDOBJS	=  VM.OBJ 

OBJS		= CDatabase.OBJ \
		CDataset.OBJ \
		CGrid.OBJ \
		CPolarGrid.OBJ \
		CTransform.OBJ \
		CPlotObject.OBJ \
		CPlotList.OBJ \
		CPlotDisplay.OBJ \
		CPlotWindow.OBJ \
		CXyPlot.OBJ \
		xyplot.OBJ \
		CInputDialog.OBJ \
		CPickDataDialog.OBJ \
		CGridDialog.OBJ \
		CWorkspace41.OBJ  \
		CXyFile.OBJ \
		CHeaderDialog.OBJ \
		AlgebraCompiler.OBJ \
		CVerifyDialog.OBJ  \
		CSaveOptionsDialog.OBJ \
		vmc.OBJ \
		ForthCompiler.OBJ \
		ForthVM.OBJ 

RCFILES		=  XYPLOT.RES 

RESFILES	= 

HELPFILES	= 

BATS		= 

# .SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .ASM .RES .RC .OBJ 
.SUFFIXES: .C .CP .CPP .CXX .CC .H .HPP .HXX .COM .EXE .DLL .LIB .RTF .DLG .RES .RC .OBJ 


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

# .ASM.EXP:
#	$(CPP) $(CFLAGS) $(DEFINES) $(INCLUDES) $*.asm -o$*.lst

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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.winspool.lib snn.lib 
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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.lib winspool.lib nafxcw.lib snn.lib 
xyplot.DEF;
<<
!ENDIF

.RTF.HLP:
	$(HC) $(HELPFLAGS) $*.HPJ

# .ASM.OBJ:
#	$(ASM) $(AFLAGS) $(DEFINES) $(INCLUDES) -o$*.obj $*.asm

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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.lib winspool.lib snn.lib 
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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.lib winspool.lib nafxcw.lib snn.lib
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
		-del $(OBJS)
		-del xyplot.res
		-del xyplot.map

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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.lib winspool.lib snn.lib 
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
gdi32.lib kernel32.lib user32.lib advapi32.lib comctl32.lib comdlg32.lib shell32.lib winspool.lib nafxcw.lib snn.lib
xyplot.DEF;
<<
!ENDIF

		-del $(TARGETDIR)\$(PROJ).$(PROJTYPE)
		-ren $(TARGETDIR)\$$SCW$$.$(PROJTYPE) $(PROJ).$(PROJTYPE)




!IF EXIST (xyplot.dpd)
!INCLUDE xyplot.dpd
!ENDIF



# Microsoft Developer Studio Generated NMAKE File, Based on Server.dsp
!IF "$(CFG)" == ""
CFG=Server - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Server - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Server - Win32 Release" && "$(CFG)" != "Server - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Server.mak" CFG="Server - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Server - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Server - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Server - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

!IF "$(RECURSE)" == "0" 

ALL : ".\server.exe"

!ELSE 

ALL : ".\server.exe"

!ENDIF 

CLEAN :
	-@erase "$(OUTDIR)\server.map"
	-@erase ".\Debug\Clntmgr.obj"
	-@erase ".\Debug\Idgen.obj"
	-@erase ".\Debug\MFInterface.obj"
	-@erase ".\Debug\MFTransaction.obj"
	-@erase ".\Debug\Server.obj"
	-@erase ".\Debug\TIPTransaction.obj"
	-@erase ".\Debug\vc50.idb"
	-@erase ".\Debug\vc50.pdb"
	-@erase ".\Debug\WFChild.obj"
	-@erase ".\Debug\Wfcswin.obj"
	-@erase ".\Debug\Wfsocket.obj"
	-@erase ".\server.exe"
	-@erase ".\server.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MT /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE"\
 /D "_MBCS" /Fo"Debug/" /Fd"Debug/" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\server.pdb" /map:"$(INTDIR)\server.map" /machine:I386\
 /out:"C:\cdeen\server\server.exe" 
LINK32_OBJS= \
	".\Debug\Clntmgr.obj" \
	".\Debug\Idgen.obj" \
	".\Debug\MFInterface.obj" \
	".\Debug\MFTransaction.obj" \
	".\Debug\Server.obj" \
	".\Debug\TIPTransaction.obj" \
	".\Debug\WFChild.obj" \
	".\Debug\Wfcswin.obj" \
	".\Debug\Wfsocket.obj"

".\server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Server - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

!IF "$(RECURSE)" == "0" 

ALL : "h:\server.exe"

!ELSE 

ALL : "h:\server.exe"

!ENDIF 

CLEAN :
	-@erase "$(INTDIR)\Clntmgr.obj"
	-@erase "$(INTDIR)\Idgen.obj"
	-@erase "$(INTDIR)\MFInterface.obj"
	-@erase "$(INTDIR)\MFTransaction.obj"
	-@erase "$(INTDIR)\Server.obj"
	-@erase "$(INTDIR)\TIPTransaction.obj"
	-@erase "$(INTDIR)\vc50.idb"
	-@erase "$(INTDIR)\vc50.pdb"
	-@erase "$(INTDIR)\WFChild.obj"
	-@erase "$(INTDIR)\Wfcswin.obj"
	-@erase "$(INTDIR)\Wfsocket.obj"
	-@erase "$(OUTDIR)\server.pdb"
	-@erase "h:\server.exe"
	-@erase "h:\server.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MTd /W3 /Gm /GX /Zi /Od /I "..\Common" /D "WIN32" /D "_DEBUG"\
 /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\Server.pch" /YX /Fo"$(INTDIR)\\"\
 /Fd"$(INTDIR)\\" /FD /c 
CPP_OBJS=.\Debug/
CPP_SBRS=.

.c{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_OBJS)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(CPP_SBRS)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Server.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=wsock32.lib /nologo /subsystem:console /incremental:yes\
 /pdb:"$(OUTDIR)\server.pdb" /debug /machine:I386 /out:"h:\server.exe"\
 /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\Clntmgr.obj" \
	"$(INTDIR)\Idgen.obj" \
	"$(INTDIR)\MFInterface.obj" \
	"$(INTDIR)\MFTransaction.obj" \
	"$(INTDIR)\Server.obj" \
	"$(INTDIR)\TIPTransaction.obj" \
	"$(INTDIR)\WFChild.obj" \
	"$(INTDIR)\Wfcswin.obj" \
	"$(INTDIR)\Wfsocket.obj"

"h:\server.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(CFG)" == "Server - Win32 Release" || "$(CFG)" == "Server - Win32 Debug"
SOURCE=.\Clntmgr.cpp
DEP_CPP_CLNTM=\
	".\Clntmgr.hpp"\
	".\critical.h"\
	".\defs.h"\
	".\errors.h"\
	".\Idgen.hpp"\
	".\MFInterface.h"\
	".\portable.h"\
	".\TInterface.h"\
	".\Wafermap.h"\
	".\waferobj.hpp"\
	".\Wfclient.hpp"\
	".\Wfcswin.h"\
	".\wfsocket.hpp"\
	".\wip.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\Clntmgr.obj" : $(SOURCE) $(DEP_CPP_CLNTM) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Clntmgr.obj" : $(SOURCE) $(DEP_CPP_CLNTM) "$(INTDIR)"


!ENDIF 

SOURCE=.\Idgen.cpp
DEP_CPP_IDGEN=\
	".\Idgen.hpp"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\Idgen.obj" : $(SOURCE) $(DEP_CPP_IDGEN) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Idgen.obj" : $(SOURCE) $(DEP_CPP_IDGEN) "$(INTDIR)"


!ENDIF 

SOURCE=.\MFInterface.cpp
DEP_CPP_MFINT=\
	".\Config.h"\
	".\defs.h"\
	".\Idgen.hpp"\
	".\MFInterface.h"\
	".\portable.h"\
	".\TInterface.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\MFInterface.obj" : $(SOURCE) $(DEP_CPP_MFINT) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MFInterface.obj" : $(SOURCE) $(DEP_CPP_MFINT) "$(INTDIR)"


!ENDIF 

SOURCE=.\MFTransaction.cpp
DEP_CPP_MFTRA=\
	".\defs.h"\
	".\errors.h"\
	".\Idgen.hpp"\
	".\MFInterface.h"\
	".\MFTransaction.h"\
	".\portable.h"\
	".\TInterface.h"\
	".\Transaction.h"\
	".\wip.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\MFTransaction.obj" : $(SOURCE) $(DEP_CPP_MFTRA) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\MFTransaction.obj" : $(SOURCE) $(DEP_CPP_MFTRA) "$(INTDIR)"


!ENDIF 

SOURCE=.\Server.cpp
DEP_CPP_SERVE=\
	".\Clntmgr.hpp"\
	".\Config.h"\
	".\critical.h"\
	".\defs.h"\
	".\errors.h"\
	".\Idgen.hpp"\
	".\MFInterface.h"\
	".\portable.h"\
	".\Server.hpp"\
	".\TInterface.h"\
	".\Wafermap.h"\
	".\waferobj.hpp"\
	".\Wfchild.hpp"\
	".\Wfclient.hpp"\
	".\wfsocket.hpp"\
	".\wip.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\Server.obj" : $(SOURCE) $(DEP_CPP_SERVE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Server.obj" : $(SOURCE) $(DEP_CPP_SERVE) "$(INTDIR)"


!ENDIF 

SOURCE=.\TIPTransaction.cpp
DEP_CPP_TIPTR=\
	".\Config.h"\
	".\defs.h"\
	".\errors.h"\
	".\portable.h"\
	".\tip.h"\
	".\TIPTransaction.h"\
	".\Transaction.h"\
	".\waferobj.hpp"\
	".\wfsocket.hpp"\
	".\wip.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\TIPTransaction.obj" : $(SOURCE) $(DEP_CPP_TIPTR) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\TIPTransaction.obj" : $(SOURCE) $(DEP_CPP_TIPTR) "$(INTDIR)"


!ENDIF 

SOURCE=.\WFChild.cpp
DEP_CPP_WFCHI=\
	".\defs.h"\
	".\errors.h"\
	".\Idgen.hpp"\
	".\MFInterface.h"\
	".\MFTransaction.h"\
	".\portable.h"\
	".\TInterface.h"\
	".\Transaction.h"\
	".\waferobj.hpp"\
	".\Wfchild.hpp"\
	".\Wfclient.hpp"\
	".\wfsocket.hpp"\
	".\wip.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\WFChild.obj" : $(SOURCE) $(DEP_CPP_WFCHI) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\WFChild.obj" : $(SOURCE) $(DEP_CPP_WFCHI) "$(INTDIR)"


!ENDIF 

SOURCE=.\Wfcswin.cpp
DEP_CPP_WFCSW=\
	".\critical.h"\
	".\Wfcswin.h"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\Wfcswin.obj" : $(SOURCE) $(DEP_CPP_WFCSW) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Wfcswin.obj" : $(SOURCE) $(DEP_CPP_WFCSW) "$(INTDIR)"


!ENDIF 

SOURCE=.\Wfsocket.cpp
DEP_CPP_WFSOC=\
	".\portable.h"\
	".\waferobj.hpp"\
	".\wfsocket.hpp"\
	

!IF  "$(CFG)" == "Server - Win32 Release"


".\Debug\Wfsocket.obj" : $(SOURCE) $(DEP_CPP_WFSOC) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Server - Win32 Debug"


"$(INTDIR)\Wfsocket.obj" : $(SOURCE) $(DEP_CPP_WFSOC) "$(INTDIR)"


!ENDIF 


!ENDIF 


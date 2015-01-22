# Microsoft Developer Studio Project File - Name="pair" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=pair - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pair.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pair.mak" CFG="pair - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pair - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "pair - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pair - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /O2 /I "../" /I "../../" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"pairc.exe"

!ELSEIF  "$(CFG)" == "pair - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "../" /I "../../" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"paircd.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "pair - Win32 Release"
# Name "pair - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\vm\functions\abstractfunction.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\functions\ask.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\callcompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\code.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\compilervmtarget.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\compilervmutil.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\compileutil.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\context.cpp
# End Source File
# Begin Source File

SOURCE=..\io\files.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\foreachcompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\frame.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\functioncompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\functionhandle.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\functiontable.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\ifcompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\invokecompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\letcompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\scanner\lispscanner.cpp
# End Source File
# Begin Source File

SOURCE=.\base\listconstructor.cpp
# End Source File
# Begin Source File

SOURCE=..\io\log.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\opcode.cpp
# End Source File
# Begin Source File

SOURCE=.\base\pair.cpp
# End Source File
# Begin Source File

SOURCE=.\pairc.cpp
# End Source File
# Begin Source File

SOURCE=.\base\pairiterator.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\procedurecompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\quotecompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\base\reader.cpp
# End Source File
# Begin Source File

SOURCE=.\base\readerutil.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\functions\runtimecompile.cpp
# End Source File
# Begin Source File

SOURCE=.\script.cpp
# End Source File
# Begin Source File

SOURCE=.\script.h
# End Source File
# Begin Source File

SOURCE=.\compiler\scriptcompiler.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\scriptvm.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\singleopcodecompilervm.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\singleopcodelengthcompilervm.cpp
# End Source File
# Begin Source File

SOURCE=..\strings\strings.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\functions\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\vm\util.cpp
# End Source File
# Begin Source File

SOURCE=.\compiler\vm\whilecompilervm.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project

# Microsoft Developer Studio Project File - Name="RayTrace" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RayTrace - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "RayTrace.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "RayTrace.mak" CFG="RayTrace - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "RayTrace - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "RayTrace - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RayTrace - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib dxerr9.lib /nologo /subsystem:windows /profile /machine:I386

!ELSEIF  "$(CFG)" == "RayTrace - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fr /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 d3d9.lib d3dx9.lib winmm.lib dxerr9.lib /nologo /subsystem:windows /profile /debug /machine:IX86

!ENDIF 

# Begin Target

# Name "RayTrace - Win32 Release"
# Name "RayTrace - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\cone.cpp
# End Source File
# Begin Source File

SOURCE=.\cube.cpp
# End Source File
# Begin Source File

SOURCE=.\cylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTorus.cpp
# End Source File
# Begin Source File

SOURCE=.\expand.cpp
# End Source File
# Begin Source File

SOURCE=.\gathering.cpp
# End Source File
# Begin Source File

SOURCE=.\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\minus.cpp
# End Source File
# Begin Source File

SOURCE=.\move.cpp
# End Source File
# Begin Source File

SOURCE=.\multiple.cpp
# End Source File
# Begin Source File

SOURCE=.\node.cpp
# End Source File
# Begin Source File

SOURCE=.\NodeTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\plus.cpp
# End Source File
# Begin Source File

SOURCE=.\polygon2.cpp
# End Source File
# Begin Source File

SOURCE=.\RayTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\RayTrace.rc
# End Source File
# Begin Source File

SOURCE=.\RayTraceDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RayTraceView.cpp
# End Source File
# Begin Source File

SOURCE=.\rotate.cpp
# End Source File
# Begin Source File

SOURCE=.\rotate_x.cpp
# End Source File
# Begin Source File

SOURCE=.\rotate_y.cpp
# End Source File
# Begin Source File

SOURCE=.\rotate_z.cpp
# End Source File
# Begin Source File

SOURCE=.\ScnTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Solve_Polynomial.cpp
# End Source File
# Begin Source File

SOURCE=.\sp.cpp
# End Source File
# Begin Source File

SOURCE=.\sphere.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\teapot.cpp
# End Source File
# Begin Source File

SOURCE=.\Torus.cpp
# End Source File
# Begin Source File

SOURCE=.\Viewport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\cone.h
# End Source File
# Begin Source File

SOURCE=.\cube.h
# End Source File
# Begin Source File

SOURCE=.\cylinder.h
# End Source File
# Begin Source File

SOURCE=.\DlgMaterial.h
# End Source File
# Begin Source File

SOURCE=.\DlgMatrix.h
# End Source File
# Begin Source File

SOURCE=.\DlgPolygon.h
# End Source File
# Begin Source File

SOURCE=.\DlgTorus.h
# End Source File
# Begin Source File

SOURCE=.\expand.h
# End Source File
# Begin Source File

SOURCE=.\gathering.h
# End Source File
# Begin Source File

SOURCE=.\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\info.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\matrix.h
# End Source File
# Begin Source File

SOURCE=.\minus.h
# End Source File
# Begin Source File

SOURCE=.\move.h
# End Source File
# Begin Source File

SOURCE=.\multiple.h
# End Source File
# Begin Source File

SOURCE=.\node.h
# End Source File
# Begin Source File

SOURCE=.\NodeTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\plane.h
# End Source File
# Begin Source File

SOURCE=.\plus.h
# End Source File
# Begin Source File

SOURCE=.\Polygon2.h
# End Source File
# Begin Source File

SOURCE=.\RayTrace.h
# End Source File
# Begin Source File

SOURCE=.\RayTraceDoc.h
# End Source File
# Begin Source File

SOURCE=.\RayTraceView.h
# End Source File
# Begin Source File

SOURCE=.\rotate.h
# End Source File
# Begin Source File

SOURCE=.\rotate_x.h
# End Source File
# Begin Source File

SOURCE=.\rotate_y.h
# End Source File
# Begin Source File

SOURCE=.\rotate_z.h
# End Source File
# Begin Source File

SOURCE=.\ScnTreeView.h
# End Source File
# Begin Source File

SOURCE=.\sp.h
# End Source File
# Begin Source File

SOURCE=.\sphere.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\teapot.h
# End Source File
# Begin Source File

SOURCE=.\Torus.h
# End Source File
# Begin Source File

SOURCE=.\trans.h
# End Source File
# Begin Source File

SOURCE=.\Viewport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap_n.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_s.bmp
# End Source File
# Begin Source File

SOURCE=.\RES\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\RayTrace.ico
# End Source File
# Begin Source File

SOURCE=.\res\RayTrace.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RayTraceDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=".\青いﾚｰｽ編み 16.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\RayTrace.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project

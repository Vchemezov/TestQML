!define Name "TestQML"
Name "${Name}"
Outfile "${Name} setup.exe"
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
InstallDir "$ProgramFiles\${Name}"

!include LogicLib.nsh
!include MUI.nsh

Function .onInit
SetShellVarContext all
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
    MessageBox mb_iconstop "Administrator rights required!"
    SetErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
    Quit
${EndIf}
FunctionEnd

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"


Section
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\Uninstall.exe"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."   "DisplayName" "${Name}"
WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."   "UninstallString" "$INSTDIR\Uninstall.exe"
File "..\testQML\cmake_build\Release\TestQML.exe"
File "..\testQML\cmake_build\Release\Qt5Core.dll"
File "..\testQML\cmake_build\Release\Qt5Widgets.dll"
File "..\testQML\cmake_build\Release\Qt5Qml.dll"
File "..\testQML\cmake_build\Release\Qt5Quick.dll"
File "..\testQML\cmake_build\Release\Qt5WinMain.dll"
CreateDirectory "$INSTDIR\qml"
File "..\testQML\cmake_build\qml\OpenFileDialog.qml"
File "..\testQML\cmake_build\qml\mainWindow.qml"
CreateShortCut "$SMPROGRAMS\${Name}.lnk" "$INSTDIR\${Name}.exe"
SectionEnd

Section "Uninstall"
Delete "$SMPROGRAMS\${Name}.lnk"
DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."
Delete "$INSTDIR\TestQML.exe"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\Qt5Qml.dll"
Delete "$INSTDIR\Qt5Quick.dll"
Delete "$INSTDIR\Qt5WinMain.dll"
Delete "$INSTDIR\qml\mainWindow.qml"
Delete "$INSTDIR\qml\OpenFileDialog.qml"
RMDir "$INSTDIR\qml"
Delete "$INSTDIR\Uninstall.exe"
RMDir "$INSTDIR"
SectionEnd
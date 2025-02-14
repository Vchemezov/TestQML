!define Name "TestQML"
Name "${Name}"
Outfile "${Name}_installer.exe"
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
InstallDir "$ProgramFiles64\${Name}"

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
;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."   "DisplayName" "${Name}"
;WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."   "UninstallString" "$INSTDIR\Uninstall.exe"
File "TestQML.exe"
CreateDirectory "$INSTDIR\qml"
SetOutPath "$INSTDIR\qml"
File "qml\OpenFileDialog.qml"
File "qml\mainWindow.qml"
CreateDirectory "$INSTDIR\test_data"
SetOutPath "$INSTDIR\test_data"
File /r "test_data\*.*"

CreateShortCut "$SMPROGRAMS\${Name}.lnk" "$INSTDIR\${Name}.exe"
SectionEnd

Section "Uninstall"
Delete "$SMPROGRAMS\${Name}.lnk"
;DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Test."
Delete "$INSTDIR\TestQML.exe"
Delete "$INSTDIR\qml\*.*"
Delete "$INSTDIR\test_data\*.*"
RMDir "$INSTDIR\qml"
RMDir "$INSTDIR\test_data"
Delete "$INSTDIR\Uninstall.exe"
RMDir "$INSTDIR"
SectionEnd
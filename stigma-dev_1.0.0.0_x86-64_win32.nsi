SetCompressor /FINAL lzma

!include "MUI2.nsh"
Unicode True
!define PRODUCT "STIGMA"
Name "STIGMA"
Outfile "C:\stigma-dev_1.0.0.0_x86-64_win32.exe"
InstallDir "C:\${PRODUCT}"
RequestExecutionLevel admin
ShowInstDetails "show"
ShowUninstDetails "show"

InstallDirRegKey HKLM "Software\${PRODUCT}" ""

!define MUI_ABORTWARNING
!define MUI_ICON "C:\stigma-dev-Windows-x86_64\Resources\icon.ico"

!include WinMessages.nsh
!define /IfNDef EM_SHOWBALLOONTIP 0x1503
!define /IfNDef EM_HIDEBALLOONTIP 0x1504

!define DIRPAGE_CHANGETEXT ; Remove this line to disable the text change
!define DIRPAGE_BALLOON    ; Remove this line to disable the balloon
Function .onVerifyInstDir
    FindWindow $9 "#32770" "" $HWNDPARENT
!ifdef DIRPAGE_CHANGETEXT
    GetDlgItem $3 $9 1006 ; IDC_INTROTEXT
    LockWindow on
!endif
    StrCpy $1 0
    loop:
        StrCpy $2 $InstDir 1 $1
        StrCmp $2 '' valid ; End of string
        StrCmp $2 ' ' found_space
        IntOp $1 $1 + 1
        Goto loop
valid:
!ifdef DIRPAGE_CHANGETEXT
    SetCtlColors $3 SYSCLR:18 SYSCLR:15
    SendMessage $3 ${WM_SETTEXT} "" "STR:$(^DirText)"
    LockWindow off
!endif
!ifdef DIRPAGE_BALLOON
    GetDlgItem $3 $9 1019
    SendMessage $3 ${EM_HIDEBALLOONTIP} "" "" ; Not required?
!endif
    Return
found_space:
    StrCpy $2 "$InstDir\" $1
!ifdef DIRPAGE_CHANGETEXT
    SetCtlColors $3 ff0000 transparent
    SendMessage $3 ${WM_SETTEXT} "" "STR:Paths with spaces are not allowed!"
    LockWindow off
!endif
!ifdef DIRPAGE_BALLOON
    GetDlgItem $3 $9 1019
    System::Call '*(&l${NSIS_PTR_SIZE},w "Bad path!", w "Spaces not allowed in path!",p 3)p.r2'
    SendMessage $3 ${EM_SHOWBALLOONTIP} "" $2 ; This will only work on XP and later (and you must use "XPStyle on")
    System::Free $2
!endif
    Abort
FunctionEnd

XPStyle on

!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

Section "STIGMA Sources"
  SectionIn RO # Just means if in component mode this is locked

  ;Set output path to the installation directory.
  SetOutPath $INSTDIR
  File /r C:\stigma-dev-Windows-x86_64

  ;Store installation folder in registry
  WriteRegStr HKLM "Software\${PRODUCT}" "" $INSTDIR

  ;Registry information for add/remove programs
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "DisplayName" "${PRODUCT}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "UninstallString" '"$INSTDIR\${PRODUCT}-Uninstaller-x86_64.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}" "NoRepair" 1

  ;Create optional start menu shortcut for Uninstaller and Main component
  CreateDirectory "$SMPROGRAMS\${PRODUCT}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\STIGMA Development Environment (x64).lnk" "$INSTDIR\stigma-dev-Windows-x86_64\STIGMA.exe"  0
  CreateShortCut "$SMPROGRAMS\${PRODUCT}\Uninstall STIGMA Development Environment (x64).lnk" "$INSTDIR\${PRODUCT}-Uninstaller-x86_64.exe" "" "$INSTDIR\${PRODUCT}-Uninstaller-x86_64.exe" 0

  ;Create Uninstaller
  WriteUninstaller "${PRODUCT}-Uninstaller-x86_64.exe"

SectionEnd

Section "MSYS2"

  SetOutPath $INSTDIR
  File /r C:\msys64

SectionEnd

Section "Uninstall"

  ;Remove all registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT}"
  DeleteRegKey HKLM "Software\${PRODUCT}"

  ;Delete the installation directory + all files in it
  ;Add 'RMDir /r "$INSTDIR\folder\*.*"' for every folder you have added additionaly
  RMDir /r "$INSTDIR\*.*"
  RMDir "$INSTDIR"

  ;Delete Start Menu Shortcuts
  Delete "$SMPROGRAMS\${PRODUCT}\*.*"
  RmDir  "$SMPROGRAMS\${PRODUCT}"

SectionEnd

!define WriteEnvStr_RegKey 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
!include "LogicLib.nsh"
!include "WinMessages.nsh"

Function IsNT
  Push $0
  ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentVersion"
  StrCmp $0 "" 0 IsNT_yes
  Pop $0
  Push 0
  Return
  IsNT_yes:
    Pop $0
    Push 1
FunctionEnd

Function StrStr
  Exch $R0 ; needle
  Exch
  Exch $R1 ; haystack
  Push $R2
  Push $R3
  Push $R4
  Push $R5
  StrLen $R3 $R0
  StrCpy $R4 0
  StrCpy $R5 $R1
  StrStr_loop:
    StrCpy $R2 $R5 $R3
    StrCmp $R2 $R0 StrStr_done
    StrCmp $R5 "" StrStr_done
    StrCpy $R5 $R5 "" 1
    Goto StrStr_loop
  StrStr_done:
    StrCpy $R0 $R5
    Pop $R5
    Pop $R4
    Pop $R3
    Pop $R2
    Exch $R1
    Exch
    Exch $R0
FunctionEnd

Function Trim
  Exch $0
  Push $1
  Push $2
  StrCpy $1 $0
  StrCpy $2 0
  Trim_Loop1:
    StrCpy $1 $0 1 $2
    StrCmp $1 " " 0 Trim_Done1
    IntOp $2 $2 + 1
    Goto Trim_Loop1
  Trim_Done1:
    StrCpy $0 $0 "" $2
    StrLen $2 $0
    IntOp $2 $2 - 1
    Trim_Loop2:
      StrCpy $1 $0 1 $2
      StrCmp $1 " " 0 Trim_Done2
      IntOp $2 $2 - 1
      Goto Trim_Loop2
  Trim_Done2:
    IntOp $2 $2 + 1
    StrCpy $0 $0 $2
    Pop $2
    Pop $1
    Exch $0
FunctionEnd

Function AddToPath
  Exch $0
  Push $1
  Push $2
  Push $3
 
  # don't add if the path doesn't exist
  IfFileExists "$0\*.*" "" AddToPath_done
 
  ReadEnvStr $1 PATH
  Push "$1;"
  Push "$0;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$0\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  GetFullPathName /SHORT $3 $0
  Push "$1;"
  Push "$3;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$3\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
 
  Call IsNT
  Pop $1
  StrCmp $1 1 AddToPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" a
    FileSeek $1 -1 END
    FileReadByte $1 $2
    IntCmp $2 26 0 +2 +2 # DOS EOF
      FileSeek $1 -1 END # write over EOF
    FileWrite $1 "$\r$\nSET PATH=%PATH%;$3$\r$\n"
    FileClose $1
    SetRebootFlag true
    Goto AddToPath_done
 
  AddToPath_NT:
    ReadRegStr $1 ${WriteEnvStr_RegKey} "PATH"
    StrCmp $1 "" AddToPath_NTdoIt
      Push $1
      Call Trim
      Pop $1
      StrCpy $0 "$1;$0"
    AddToPath_NTdoIt:
      WriteRegExpandStr ${WriteEnvStr_RegKey} "PATH" $0
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
 
  AddToPath_done:
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

Function un.IsNT
  Push $0
  ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" "CurrentVersion"
  StrCmp $0 "" 0 IsNT_yes
  ; 不是 NT
  Pop $0
  Push 0
  Return
  IsNT_yes:
    Pop $0
    Push 1
FunctionEnd

Function un.StrStr
  Exch $R0 ; needle
  Exch
  Exch $R1 ; haystack
  Push $R2
  Push $R3
  Push $R4
  Push $R5
  StrLen $R3 $R0
  StrCpy $R4 0
  StrCpy $R5 $R1
  unStrStr_loop:
    StrCpy $R2 $R5 $R3        ; 取 haystack 的前 $R3 个字符
    StrCmp $R2 $R0 unStrStr_done  ; 如果匹配，跳转到 done
    StrCmp $R5 "" unStrStr_done   ; 如果 haystack 为空，结束
    StrCpy $R5 $R5 "" 1        ; 移除第一个字符，继续向后移动
    Goto unStrStr_loop
  unStrStr_done:
    ; $R5 现在是从匹配位置开始的剩余部分
    StrCpy $R0 $R5             ; 将结果存入 $R0
    Pop $R5
    Pop $R4
    Pop $R3
    Pop $R2
    Exch $R1                   ; 恢复原来的栈顺序
    Exch
    Exch $R0
FunctionEnd

Function un.RemoveFromPath
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  Push $5
  Push $6
  IntFmt $6 "%c" 26 # DOS EOF
  Call un.IsNT
  Pop $1
  StrCmp $1 1 unRemoveFromPath_NT
  ; Not on NT
  StrCpy $1 $WINDIR 2
  FileOpen $1 "$1\autoexec.bat" r
  GetTempFileName $4
  FileOpen $2 $4 w
  GetFullPathName /SHORT $0 $0
  StrCpy $0 "SET PATH=%PATH%;$0"
  Goto unRemoveFromPath_dosLoop
  unRemoveFromPath_dosLoop:
    FileRead $1 $3
    StrCpy $5 $3 1 -1 # read last char
    StrCmp $5 $6 0 +2 # if DOS EOF
    StrCpy $3 $3 -1 # remove DOS EOF so we can compare
    StrCmp $3 "$0$\r$\n" unRemoveFromPath_dosLoopRemoveLine
    StrCmp $3 "$0$\n" unRemoveFromPath_dosLoopRemoveLine
    StrCmp $3 "$0" unRemoveFromPath_dosLoopRemoveLine
    StrCmp $3 "" unRemoveFromPath_dosLoopEnd
    FileWrite $2 $3
    Goto unRemoveFromPath_dosLoop
  unRemoveFromPath_dosLoopRemoveLine:
    SetRebootFlag true
    Goto unRemoveFromPath_dosLoop
  unRemoveFromPath_dosLoopEnd:
    FileClose $2
    FileClose $1
    StrCpy $1 $WINDIR 2
    Delete "$1\autoexec.bat"
    CopyFiles /SILENT $4 "$1\autoexec.bat"
    Delete $4
    Goto unRemoveFromPath_done
  unRemoveFromPath_NT:
    ReadRegStr $1 ${WriteEnvStr_RegKey} "PATH"
    StrCpy $5 $1 1 -1 # copy last char
    StrCmp $5 ";" +2 # if last char != ;
    StrCpy $1 "$1;" # append ;
    Push $1
    Push "$0;"
    Call un.StrStr ; Find `$0;` in $1
    Pop $2 ; pos of our dir
    StrCmp $2 "" unRemoveFromPath_done
    ; else, it is in path
    # $0 - path to add
    # $1 - path var
    StrLen $3 "$0;"
    StrLen $4 $2
    StrCpy $5 $1 -$4 # $5 is now the part before the path to remove
    StrCpy $6 $2 "" $3 # $6 is now the part after the path to remove
    StrCpy $3 $5$6
    StrCpy $5 $3 1 -1 # copy last char
    StrCmp $5 ";" 0 +2 # if last char == ;
    StrCpy $3 $3 -1 # remove last char
    WriteRegExpandStr ${WriteEnvStr_RegKey} "PATH" $3
    SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
  unRemoveFromPath_done:
    Pop $6
    Pop $5
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

# NativeLinux

Runs Linux in Native Windows Mode (chkdsk screen). Supports Windows NT 4.0 — Windows 11, also ReactOS.

> [!IMPORTANT]
> Press Shift + ESC to return to the Win32 (GUI) Mode.
> <br /> NativeLinux Version 1.0 does **NOT** have an Exit Shortcut nor a Exit Command. Use NativeLinux 1.1 
> <br /> NativeLinux does not have a Working Keyboard in some Builds of Windows 11, ex. 22621. Test in a Virtual Machine!

> [!CAUTION]
> DO **NOT** DOWNLOAD NATIVELINUX **FROM ANY OTHER WEBSITES/REPOSITORIES**
> <br /> NativeLinux does **NOT** have **GUI**
> <br /> Legitimate NativeLinux, when running as a normal EXE application, will report that it is **CANNOT RUN IN WIN32**




# How to Build NativeLinux?
Requires Windows Driver Kit 7.1.0

After Installing. Search for x86 Free Build Enviroment. Open it.

Go to the Source Code of NativeLinux and Type the following Command: `build /g /W` or let a batch do this for you by just typing `b`.

> [!NOTE]
> Some files had to be changed due to multiple compilation errors, including mini-rv32ima.h; disable some warnings.
> <br />Portable stdint.h was added because it was not included in WDK 7, which resulted in many compilation errors.
> <br />The keyboard has only an English-US layout.



# Installation

1. Copy nativelinux.exe to the %SystemRoot%\System32 (C:\Windows\System32) folder.
2. Open the install.reg from the `Files` Directory.

From a Release Build: Just unzip downloaded archive open the install.cmd **AS ADMIN** which will do everything for you.



# Manual Uninstallation in case of not working keyboard

1. Hard reboot PC several times until you get the "Preparing Automatic Repair" screen. Then after waiting a bit, you will get the "Automatic Repair" screen. **Click "Advanced options"**, not "Reboot"
  ![image](https://i.imgur.com/367ZWlG.png)

2. Go to Troubleshoot > Advanced Options > Command Prompt
  ![image](https://i.imgur.com/hI1tMDs.png)

3. In Command Prompt type: `cd /d C:\Windows\System32`, then `del nativelinux.exe`
4. Type: `regedit`
5. In the Registry Editor click HKEY_LOCAL_MACHINE. Then File > Load Hive > choose C:\Windows\System32\config\SYSTEM > Enter any name (Example: mainsystem)
6. Go to HKEY_LOCAL_MACHINE\<your_hive_name>\ControlSet001\Control\Session Manager and then in right column find "BootExecute" key, double-click it
  ![image](https://i.imgur.com/O47nUYW.png?1)

7. Edit BootExecute from this: 
  ```
  autocheck autochk *
  nativelinux

  ```
  
- To this:
  ```
  autocheck autochk *
  
  ```
  ![image](https://i.imgur.com/E6h9FpM.png)

7. Reboot and Enjoy!

> [!NOTE]
> remove.reg and install.bat doesn't work in Windows PE. Don't even try it.



# Limited Features

Backspace **NOT** working

**No ANSI sequences**, because it is unsupported in Native Mode.



# Screenshots of NativeLinux.
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d9f60038-0219-43ae-aa55-87d9fb8258ee)
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d4324a79-c9e8-450a-a458-8f183d0a0c0c)



# To Do
  - [X] Get Keyboard to work on Windows 2000 and Newer Windows 11 builds (except Windows 11 build 22631)
  - [ ] Get backspace to work
  - [ ] Fix more crashes during some commands on NativeLinux



# Credits.
[mini-rv32ima](https://github.com/cnlohr/mini-rv32ima) for the RISC-V emulator.

[BugCheck2Linux](https://github.com/NSG650/BugCheck2Linux) for the BootImage.h file.

[ReactOS Project](https://github.com/reactos/reactos) for keyboard support files.

[Native Shell](https://github.com/amdf/NativeShell) for some great functions needed for this to work.

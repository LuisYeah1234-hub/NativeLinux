# NativeLinux

Runs Linux in the Windows Native Mode (chkdsk screen). Supports Windows XP to Windows 11 and also Works on the Open Source Project ReactOS.

> [!IMPORTANT]
> Shift + ESC to return to the GUI.
> <br /> NativeLinux Version 1.0 Does **NOT** have an Exit Shortcut nor a Exit Command. Use NativeLinux 1.1 
> <br /> NativeLinux does not have a Working Keyboard in some Builds of Windows 11. Test in a Virtual Machine!

> [!CAUTION]
> I've seen some unknown Websites such as miku.place and GitOwl talking about this Project.
> <br /> Let it be Legit or not. I still warn you
> <br /> Do **NOT** download NativeLinux from anywhere else! **NativeLinux is only Available in this Repository.**
> <br /> NativeLinux does **NOT** run in the GUI. **If it runs in the GUI. It's an Fake Program.**
> <br /> The Normal NativeLinux will give an error: **"Cannot run NativeLinux in Win32 Mode".**

# How to Build NativeLinux?
Requires Windows Driver Kit 7.1.0

After Installing. Search for x86 Free Build Enviroment. Open it.

Go to the Source Code of NativeLinux and Type the following Command: `build /g /W` or let a batch do this for you by just typing `b`.

> [!NOTE]
> Some Files had to be modified due to many Compilation Errors that includes mini-rv32ima.h
> <br /> Including disabling some Warnings.
> <br /> An Portable stdint.h had to be added due to of not included in WDK 7. Resulting in many Compilation Errors.
> <br /> The Keyboard is US Layout only.

# Installation

Copy nativelinux.exe to the %SystemRoot%\System32 (C:\Windows\System32) Folder.

Open the install.reg from the `Files` Directory.

From a Release Build. Just open the install.cmd which will do everything for you. **`Requires Admin Privileges (UAC).`**

# Manual Uninstallation in case of Keyboard not working

- Reboot the Computer Several Times until you get the "Preparing Automatic Repair" Screen. Then after waiting a bit you get the "Automatic Repair" Screen. **Do NOT click on Reboot!** Click "Advanced Options"

- After Clicking "Advanced Options" Go to Troubleshoot > Advanced Options > Command Prompt

- In The Command Prompt Type: `cd /d C:\Windows\System32` then `rm nativelinux.exe`


- After doing these steps. Type in the Command Prompt: `regedit`

- In the Registry Editor click HKEY_LOCAL_MACHINE. Then File > Load Hive > Choose C:\Windows\System32\config\SYSTEM > Enter any name (Example: mainsystem) > Go to ControlSet001\Control\Session Manager

- Edit BootExecute change it from this: 
  ```
  autocheck autochk *
  nativelinux
  ```
  
- To this:
  ```
  autocheck autochk *
  ```
  

- Reboot and Enjoy!

> [!NOTE]
> remove.reg and install.bat doesn't work in Windows PE. Don't even try it.

# Limited Features

Backspace has been Disabled. Due to it being Broken.

No ANSI sequences due to it being unsupported in Native Mode.

# Some Screenshots of NativeLinux running.
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d9f60038-0219-43ae-aa55-87d9fb8258ee)
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d4324a79-c9e8-450a-a458-8f183d0a0c0c)

# To do
  - [ ] Get Keyboard to work on Windows 2000 and Newer Windows 11 builds
  - [ ] Get backspace to work
  - [ ] Fix more crashes during some commands on NativeLinux
  - [ ] Fix CTRL outputting ⌃@ sometimes when pressed

# Credits.
[mini-rv32ima](https://github.com/cnlohr/mini-rv32ima) for the RISC V emulator.

[BugCheck2Linux](https://github.com/NSG650/BugCheck2Linux) for the BootImage.h file.

[ReactOS Project](https://github.com/reactos/reactos) For keyboard support files.

[Native Shell](https://github.com/amdf/NativeShell) for some great functions needed for this to work.

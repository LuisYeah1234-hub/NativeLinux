# NativeLinux
Runs Linux in the Windows Native Mode (chkdsk screen).
Supported starting from Windows XP to Windows 11, Also works on the Open Source Windows project ReactOS.

> [!WARNING]
> You cannot go to the Windows GUI After it has been ran!
> 
> Unless it has been Removed from the Registry, Or Removed from System32 Folder
> 
> I recommend you to test this on a Virtual Machine!!
> 

# How to build?
Requires Windows Driver Kit 7.1.0

After installed search for x86 Free Build Environment. Open it

Go to the source code of NativeLinux and type the following command:

`build /g /W` or let a batch do this for you by typing just `b`.

# NOTE
Some files had to be modified due to many compilation errors that includes mini-rv32ima.h 

Including disabling some warnings

An portable stdint.h had to be added due of not being included in WDK 7. Resulting in many compilation errors

The keyboard is US Layout only.

# Screenshots
Some Screenshots of NativeLinux Running.
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d9f60038-0219-43ae-aa55-87d9fb8258ee)
![image](https://github.com/LuisYeah1234-hub/NativeLinux/assets/64372171/d4324a79-c9e8-450a-a458-8f183d0a0c0c)

# Installation
Copy nativelinux.exe to %SystemRoot%\System32 (C:\Windows\System32\)

Open the install.reg from the `files` directory.

From a release build just open the install.cmd which will do everything for you, Requires Admin privileges (UAC)

# Limited features
Backspace has been disabled. Due to it being broken

No ANSI sequences due of it being unsupported in Native mode.

# To do
  - [ ] Get keyboard to work on Windows 2000
  - [ ] Get backspace to work

# Credits.
[mini-rv32ima](https://github.com/cnlohr/mini-rv32ima) for the RISC V emulator.

[BugCheck2Linux](https://github.com/NSG650/BugCheck2Linux) for the BootImage.h file.

[ReactOS Project](https://github.com/reactos/reactos) For keyboard support files.

[Native Shell](https://github.com/amdf/NativeShell) for some great functions needed for this to work.

@echo off
net session >nul 2>&1
if %errorLevel% == 0 (
    goto Script
) else (
    echo You do not have administrative privileges. This script requires administrator privileges to continue.
    pause
    exit /b 0
)

:Script
cd %~dp0
echo ------------------------------------------------------------------------------------------------------------------------
echo       WARNING: You cannot exit from the Linux shell to go to Windows GUI. After installed 
echo       It can't be uninstalled unless you do the steps on the bottom
echo ------------------------------------------------------------------------------------------------------------------------
echo      We recommend not to use this on your Host PC. But on a virtual machine.
echo      But if you still like to use it on your Host. Do it but we won't be responsible for anything          
echo      To remove NativeLinux from this Computer you need To do the following steps.
echo       1. Cause the pc to go to the recovery mode. aka Automatic repair
echo       2. In recovery mode go to command prompt option
echo       3. Type regedit
echo       4. Click HKEY_LOCAL_MACHINE then On the top bar click file, Load hive
echo       5. Go to your Windows drive letter ex. C:\ then Windows\System32\config and open SYSTEM file
echo       6. Call it whatever you want SYS, etc doesnt matter
echo       7. Open the folder (key) with the name you specified in step 6 then
echo       Go to ControlSet001\Control\Session Manager
echo       8. Inside Session Manager open BootExecute and remove nativelinux string here.
echo       DO NOT REMOVE THE AUTOCHECK AUTOCHK * STRING AS IT CAUSES CHKDSK TO NO LONGER WORK
echo       Press any key if you understand whatever it's been said here and if you want to continue.
echo       Or Press X on the titlebar to cancel the installation
echo ------------------------------------------------------------------------------------------------------------------------
pause
regedit /s install.reg
copy nativelinux.exe %SystemRoot%\System32
echo Installation Complete.
pause
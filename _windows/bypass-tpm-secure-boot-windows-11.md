# Bypass TPM and Secure Boot checks

<https://www.windowslatest.com/2021/06/28/youll-be-able-to-bypass-windows-11-tpm-2-0-requirement/>

[Download](https://www.microsoft.com/en-us/software-download/windows11) Windows 11 ISO. Copy its contents to an NTFS formatted USB Flash drive using Explorer. Boot from that drive and start Windows 11 installation.

At some point it will tell you that your PC doesn't meet the requirements. Press `SHIFT + F10`, that will open `cmd` console. Start Windows Registry from there (`regedit`).

Go to `HKEY_LOCAL_MACHINE/SYSTEM/Setup`. Create a new node/key `LabConfig`. Inside it create the following DWORD 32-bit values:

- `BypassTPMCheck` and set it to `1`
- `BypassSecureBootCheck` and set it to `1` (*might not need that one*)
- `BypassRAMCheck` and set it to `1` (*might not need that one*)

Close Registry, close `cmd` console. Click on back arrow and try to proceed with the installation again.

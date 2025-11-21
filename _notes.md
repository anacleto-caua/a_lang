# A Note for Future ME

The code and presets shall now 100% cross-platform. The only thing that isn't is the libs/slint folder itself.

When you eventually move this project to a Linux machine, you just need to:

    - Delete the contents of libs/slint (the .dlls).

    - Paste in the Linux version of Slint (the .sos).

    - Open VS Code. It will auto-detect the "Linux (Ninja)" preset and work immediately.
# GarHal_CSGO
A project that demonstrates how to screw with CSGO from Kernel Space.
Project comes with updated offsets, my aim was to create a project with more features achieved using a driver.
I tend to hook DirectX, and implement Kernel-level drawing into this project.

Side note: No, this project has nothing to do with csgo's released source code or whatever.

## Features
* Driver Hiding (Kill Patchguard before use) @JKornev
* Process Hiding (Kill Patchguard before use) @landhb
* Simple glow Wallhack
* Highlights defuser
* Aimbot
* Smooth aimbot
* Anti-Aimbot (TODO: Fix up ugly code, test, use VirtualAlloc, and OpenHandle from Kernel Sphere)
* Bhop

## Patchguard?
Simple explanation: It's something that doesn't like kernel manipulation, and triggers a BSOD If detects It.
Use something like [EfiGuard](https://github.com/Mattiwatti/EfiGuard) or [UPGDSED](https://github.com/hfiref0x/UPGDSED) or [Shark](https://github.com/9176324/Shark) . I prefer the two latter.
Doing so will give you sort of full "invisibility", but It's unnecessary (imo).

## Compilation
You will need Visual Studio 2017 atleast, and amongs that the WDK. If you don't know how to compile a Kernel Driver, you will need to google a bit.

## Usage
1. Start the Driver.
2. Start CSGO.
3. Place the default config where GarhalController.exe is.
4. Modify config to your needs.
5. Start GarhalController.exe.
6. Play the game.
7. Beware of Overwatch.

## Starting Driver
Open up CMD as admin mode:
```Bash
  # Enable testmode and reboot.
  bcdedit /set testsigning on

  # Create service and start it.
  sc create garhal type= kernel binpath="C:\Users...\Desktop\garhal.sys"
  sc start garhal
```

## Preview
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/ADj12ykKq-o/0.jpg)](https://www.youtube.com/watch?v=ADj12ykKq-o)

## Credits
* @Life45 for explaining me the basic work, pushing me to do this.
* @ZeroMemory for providing videos for basic stuff.
* @hrt for a (not so much working) aimbot structure.
* drew79

## Note
The project is for research purposes only and is not suitable for production environments, misusage, or commercial use.

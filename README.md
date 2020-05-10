# GarHal_CSGO
A project that demonstrates how to screw with CSGO from Kernel Space.
Project comes with updated offsets, my aim was to create a project with more features achieved using a driver.
I tend to hook DirectX, and implement Kernel-level drawing into this project.

## Features
* Driver Hiding (Kill Patchguard before use) @JKornev
* Process Hiding (Kill Patchguard before use) @landhb
* Simple glow Wallhack
* Highlights defuser
* Aimbot
* Smooth aimbot
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
```Bash
  # Enable testmode and reboot.
  bcdedit /set testsigning on

  # Create service and start it.
  sc create garhal type= kernel binpath="C:\Users...\Desktop\garhal.sys"
  sc start garhal
```

## Credits
@Life45 for explaining me the basic work, pushing me to do this.
@ZeroMemory for providing videos for basic stuff.

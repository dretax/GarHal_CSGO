# GarHal_CSGO
A project that demonstrates how to screw with CSGO from Kernel Space.
Project comes with updated offsets, my aim was to create a project with more features achieved using a driver.
I tend to hook DirectX, and implement Kernel-level drawing into this project. This has been done by a couple of people already, I mainly wanted to do one myself from "scratch" because I haven't seen completed Driver hacks for CSGO out there. It was something that helped me understand more of how Kernels work in particular, make a clean project for myself, and go forward. 

Side note: 
No, this project has nothing to do with csgo's released source code or whatever.
This project probably has mistakes. Feel free to correct me through PRs.

## Note
The project is for research purposes only and is not suitable for production environments, misusage, or commercial use.

## Features
* Driver Hiding (Kill Patchguard before use) @JKornev
* Process Hiding (Kill Patchguard before use) @landhb
* Simple glow Wallhack
* Highlights defuser
* Aimbot
* Smooth aimbot
* Anti-Aimbot (TODO: Fix up ugly code, test, use VirtualAlloc, and OpenHandle from Kernel Sphere)
* Bhop
* OBRegisterCallback to limit access to the GarhalController.exe (Read VAC section)

## Patchguard?
Simple explanation: It's something that doesn't like kernel manipulation, and triggers a BSOD If detects It.
Use something like [EfiGuard](https://github.com/Mattiwatti/EfiGuard) or [UPGDSED](https://github.com/hfiref0x/UPGDSED) or [Shark](https://github.com/9176324/Shark) . I prefer the two latter.
Doing so will give you sort of full "invisibility", but It's unnecessary (imo).

## VAC
As long as you do not open a handle to the game you should be fine, but of course there are ways to detect Read/Write memories.
It's just harder without a driver, and hence they haven't really done this since 2017 I hardly think they are going to.
Here is some information from 2013, and 2016:
* [Analysis from 2013 by forcon](https://www.unknowncheats.me/wiki/Valve_Anti-Cheat:VAC_external_tool_detection_(and_more))
* [Write up from Mambda](https://guidedhacking.com/threads/how-to-bypass-vac-valve-anti-cheat-info.8125/post-42854)
Both sources talk about that VAC opens handle to processes and searches all volumes with FindFirstVolumeW, FindNextVolumeW and closes handles with FindVolumeClose.

It opens every handle running with Query_Information, and VM_READ.
Denying access with ObRegisterCallbacks to protect the controller would be more than enough to get rid of this scan.

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
* @drew79

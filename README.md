[![IMAGE ALT TEXT HERE](https://i.imgur.com/RCDePZO.png)](https://i.imgur.com/RCDePZO.png)
# GarHal_CSGO
A project that demonstrates how to screw with CSGO from Kernel Space.
Project comes with updated offsets, my aim was to create a project with more features achieved using a driver.
I tend to hook DirectX, and implement Kernel-level drawing into this project. This has been done by a couple of people already, I mainly wanted to do one myself. The project has a UserMode application which uses IOCTL to control the Kernel Driver. This was something that helped me understand more of how Kernel Drivers work in particular, make a clean project for myself, and go forward. 

Side note: 
No, this project has nothing to do with csgo's released source code or whatever.
This project probably has mistakes. Feel free to correct me through PRs.

#### Project discussion: https://guidedhacking.com/threads/csgo-kernel-driver-multihack.15130/

### WARNING: The project is for research purposes only and is not suitable for production environments, misusage, or commercial use. See [License](https://github.com/dretax/GarHal_CSGO/blob/master/LICENSE.md).

## Credits & Thanks
* [Life45](https://github.com/Life45) for explaining me the basic work, pushing me to do this. Best guy eva.
* @ZeroMemory for providing videos for basic stuff.
* @hrt for a (not so much working) aimbot structure.

## Features
* Driver Hiding (Kill Patchguard before use) @JKornev
* Process Hiding (Kill Patchguard before use) @landhb
* Simple glow Wallhack
* Highlights defuser
* No flash
* Aimbot
* Smooth aimbot
* TriggerBot - Supports randomized delay.
* Another simple program to Display Ranks (Work in progress)
* Displays enemies on the Radar
* Bhop
* OBRegisterCallback to limit access to the GarhalController.exe & GarhalRankDisplayer.exe (Read VAC section)

## Changelog
* **Changes on 2020.07.31**: The driver now detects process termination. Should have coded It more faster than being lazy. The controller, and rankreader sends an IOCTL code to the driver once they have been launched, and loaded. If we would do this sooner, the applications would not start up. We are now completely re-writing the access flags, and the usermode applications should be untouchable by VAC for now. The usermode applications now wait until the addresses are not zero at startup. The plan to make an anti-aimbot was dropped. A kernel driver should be all about being non-straightforward, and used carefully to draw less attention. Please continue testing, and let me know if there are any issues.

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

--
After some emails I have decided to clear some things up. The project doesn't open a handle, and obviously doesn't use usermode RPM/WPM. Therefore the only detectable parts should be memory writing It self, and by VAC searching for the driver / usermode application.
As stated above you can grant yourself "invisibility" (from VAC) atleast by using the implemented driver / process hiding.
The driver also protects the usermode application from VAC's process scanning. (Stated in the VAC section)
The project files could be flagged, and can get you banned If VAC detects you are running this certain program (Checking for name / hash, but It's a really bad approach from any anti-cheat)
```C
if (EnumDeviceDrivers(drivers, sizeof(drivers), &cbNeeded) && cbNeeded < sizeof(drivers))
```

## A little Analysis of OverWatch
I have purchased CSGO around April-May on a separate account, which you see in the sample videos.
For 80+ hours I have been cheating on that account, for about a month or so. Revisited the game in August last time. I have used wallhack, triggerbot, aimbot without holding back and received countless reports from competetive matches. At this time being the account is still not banned for CSGO.

## Compilation
You will need Visual Studio 2017 atleast, and amongs that the WDK. If you don't know how to compile a Kernel Driver, you will need to google a bit, or just [click here](https://guidedhacking.com/threads/windows-kernel-mode-driver-tutorial.15201/).

## Updating offsets after a game update
Don't be a loser, dump with [GH Dumper](https://guidedhacking.com/resources/guided-hacking-offset-dumper-gh-offset-dumper.51/)

Use -insecure, and dump after.

~~Visit [hazedumper](https://github.com/frk1/hazedumper) for easy usage.~~

## Supported Windows Versions
If you don't need the csrss.exe check just remove It (memory.c, but not recommended), or update the offsets manually
for different windows versions. 

Click [Here](https://www.vergiliusproject.com/kernels/x64/Windows%2010%20%7C%202016/1909%2019H2%20(November%202019%20Update)/_EPROCESS) to view the EPROCESS offsets.
Click [Here](https://docs.microsoft.com/en-us/windows/release-information/) to view the release information site.
* Windows 10 1903
* Windows 10 1909
* Windows 10 2004

## Usage
1. Start the Driver.
2. Start CSGO.
3. Place the default config where GarhalController.exe is.
4. Modify config to your needs.
5. Start GarhalController.exe.
6. Optional: If you are playing ranked, use GarhalRankDisplayer.exe to view ranks.
7. Play the game.
8. Beware of Overwatch.

## Starting Driver
Open up CMD as admin mode:
```Bash
  # Enable testmode and reboot.
  bcdedit /set testsigning on

  # Create service and start it.
  sc create garhal type= kernel binpath="C:\Users...\Desktop\garhal.sys"
  sc start garhal
  
  # Stopping the service, bohoho...
  sc stop garhal
  
  # Disabling testmode...
  bcdedit /set testsigning off
```

## Manual mapping the driver instead of testmode
1. Uncomment the commented DriverEntry function in garhal.c.
2. Rename the current one to DriverInitialize.
3. Compile the driver with /GS- (Disable the security check).
[![IMAGE ALT TEXT HERE](https://i.imgur.com/iASLlHG.png)](https://i.imgur.com/iASLlHG.png)
4. Use kdmapper or something else to load up your driver.
5. **Optional (and recommended if you are interested in kernel, rather than being a crappy user):** Clear the MmUnloadedDrivers and PIBCache yourself, get the signature of these using WinDbg for example, don't use CreateDriver, hook an IOCTL function with a trick, or do some other type of communication. I'm not releasing practices for this for now, there is plenty available. If you manage to do these tricks you are free to attack any realistic anti-cheats with a bit of more work.
6. Reboot your PC to clear out the driver entirely from the memory if you want to play different games.

## Preview
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/ADj12ykKq-o/0.jpg)](https://www.youtube.com/watch?v=ADj12ykKq-o)
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/lEE0W2o7WDA/0.jpg)](https://youtu.be/lEE0W2o7WDA)

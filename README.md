# ckLua
Lua scripting for Corsair's RGB keyboards for Windows. *Note that this program only works for firmware version lower than 1.20*

## How it works
By using the Windows API messages sent from the keyboard are intercepted and read. Keys are mapped to strings (optional) and lighting codes. This information is passed directly to loaded Lua script.

## Information
Only Windows is supported at the moment and I have no intention of porting it to any other operating system. However, the code is structured with the idea in mind that it should be as easy as possible to port. So feel free to fork my project and port it to whatever Lua runs on!

I have supplied example scripts to show how both scripts and key map files are structured. It should be relatively easy to create new key maps. If you create one, let me know and I will include it in my project (with proper credits of course).

## Dependencies
This program uses the library from https://github.com/Palmr/LibCorsairRGB, and therefore libusb. Other than this you need Lua installed on your system to build.

## How to use
The program uses C++/CLI. So you need to compile with /clr and /SUBSYSTEM:WINDOWS flags. You also need to include libusb, LibCorsairRGB and Lua.

As mentioned by Palmr, you may have to install drivers for libusb to get this program to work. See here for information on how to set up the libusb drivers for windows: https://github.com/libusb/libusb/wiki/Windows#Driver_Installation.

A pre-built binary can be found [here](https://github.com/Jeflux/ckLua/blob/master/_misc/ckLuaBin.zip).

## Disclaimer
I am not responsible for any third party scripts used in conjunction with this application. By using this application with scripts from untrusted sources you are potentially running the risk of having a keylogger on your system, as with any application that reads hardware input.

Before using this application I strongly suggest you read through my source code and any third party script you are planning to use with application. This to reassure yourself that your personal information is safe. Keep in mind that I have no intentions of creating malicious applications and therefore will keep this project open source.

Any executables supplied by me corresponds to the source code. I still however recommend you to build the application on your own if you have the possibility.

## Special Thanks

Figuring out the USB protocol is non of my work. I have used pre existing code by others to manage connection betwen input and lighting.

Credits goes to:
* https://github.com/Palmr/LibCorsairRGB
* https://github.com/CalcProgrammer1/CorsairKeyboardSpectrograph/

For supplying libraries and reverse engineering the communication to the keyboard.

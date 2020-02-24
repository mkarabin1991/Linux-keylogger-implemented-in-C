# Linux-keylogger-implemented-in-C

This is a simple keylogger implemented in C.

Makefile commands:
-make all:              compile the keylogger
-make find_keyboard:    prints the keyboards event (must be hooked manualy inside the code)
-make run_terminal:     runs the keylogger and prints in terminal
-make run_file:         runs the keylogger and prints in file <output.txt> same dir.
-make run_background:   runs the keylogger as a background process

press <ESC> key to kill the keylogger!

Daemonizing the procees to run in the background.
An other simple way to run the task ath the background is to put & at the end of the command.
(it will still let you kill it with <Esc>)

More:
All types of keyloggers (Hardware/Software/Kernel/driver) require something to be installed in your computer. For hardware keyloggers the attacker must have physical access to the machine he wants to monitor, so it is crusial to protect your machine from anyone who might have physical access to it. Software keyloggers which are the most common tool, also requires root privileges in order to monitor the keyboard. This is mostly done with social engineering attacks. So we need to be carefull of what we install. Even if a keylogger runs in our machine, we might be able to track it and stop it before it obtains critical info (passwords/ bank credentials..) with a detection software.


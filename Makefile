all: keylogger.c
	gcc -Wall -o keylogger keylogger.c

find_keyboard:
	grep "keyboard" /proc/bus/input/devices -A 10 | grep Handlers | tr " " "\n" |grep event

run_terminal: keylogger
	sudo ./keylogger  -s

run_file: keylogger
	sudo ./keylogger -f output.txt

run_background: keylogger
	sudo ./keylogger -f output.txt &

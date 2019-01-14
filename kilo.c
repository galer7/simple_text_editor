/* includes */
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// STDIN_FILENO e din unistd.h

/* data */

struct termios orig_termios;


/* terminal */

void die(const char *s){
  perror(s);
  exit(1);
}

void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}

void enableRawMode() {
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
	atexit(disableRawMode);

	struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);// IXON = soft. flow control, ICRNL = input Carriage Return, New Line. 
  // BRKINT = break condition.
  // INPCK = parity check 
  // ISTRIP = causes the 8th bit to be strippepd
  // CS8 = not a flag, a bit maask. sets char size to 8 bits per byte.
 
  raw.c_oflag &= ~(OPOST);// Output Post-Processing.
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);// astia sunt toti flags
  raw.c_cc[VMIN] =  0;
  raw.c_cc[VTIME] =  1;

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}


/* init */

int main() {
	enableRawMode();
	
  while(1) {
    char c = '\0';
    // citeste un beat de data in variabila c, paana cand EOF sau 'q' pressed
    // read() e din unistd.h
    if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");
    if (iscntrl(c)) {
      printf("%d\r\n", c);
    } else {
        printf("%d ('%c')\r\n", c, c);
    }
    if (c == 'q') {
      break;
    }
  }
	return 0;
}

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

// STDIN_FILENO e din unistd.h

struct termios orig_termios;

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);// IXON = soft. flow control, ICRNL = input Carriage Return, New Line. 
  // BRKINT = break condition.
  // INPCK = parity check 
  // ISTRIP = causes the 8th bit to be strippepd
  // CS8 = not a flag, a bit maask. sets char size to 8 bits per byte.
 
  raw.c_oflag &= ~(OPOST);// Output Post-Processing.
  raw.c_cflag |= (CS8);
  raw.co_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);// astia sunt toti flags
  raw.c_cc[VMIN] =  0;
  raw.c_cc[VTIME] =  1;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();
	
	char c = '\0';
	// citeste un beat de data in variabila c, paana cand EOF sau 'q' pressed
  // read() e din unistd.h
  read(STDOM_FILENO, &c, 1);
  if (iscntfl(c)) {
          printf("%d\r\n", c);
  } else {
          printf("%d ('%c')\r\n", c, c);
  }
  if (c == 'q') break;

	return 0;
}

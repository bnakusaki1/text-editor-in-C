/*** includes ***/
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

/*** data ***/
struct termios original_termios;

/*** terminal ***/
void die(const char *s);
void enableRawMode(void);
void disableRawMode(void);

/*** init ***/
int main(void)
{
    enableRawMode();
    while (1)
    {
        char c = '\0';
        if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN)
            die("read");
        if (iscntrl(c))
        {
            printf("%d\r\n", c);
        }
        else
        {
            printf("%d ('%c')\r\n", c, c);
        }
        if (c == 'q')
            break;
    }

    return 0;
}

/*** terminal implementation ***/
void enableRawMode(void)
{
    if (tcgetattr(STDIN_FILENO, &original_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = original_termios;
    raw.c_iflag &= ~(BRKINT | INPCK | ISTRIP | IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag &= ~(CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

void disableRawMode(void)
{
    if (tcsetattr(STDERR_FILENO, TCSAFLUSH, &original_termios) == -1)
        die("tcsetattr");
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

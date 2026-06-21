#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

#define MAX_PASSWORD_LEN 255

void password(void)
{
    char pwd[MAX_PASSWORD_LEN];
    const char code[] = "Anshi";

    struct termios oldt, newt;
    int i = 0;
    char ch;

    // Save current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable canonical mode and echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply settings immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    printf("Enter your password: ");
    fflush(stdout);

    while (1)
    {
        if (read(STDIN_FILENO, &ch, 1) != 1)
            continue;

        // Enter key (\n or \r depending on terminal)
        if (ch == '\n' || ch == '\r')
        {
            break;
        }
        // Backspace (127 on macOS terminals)
        else if ((ch == 127 || ch == 8) && i > 0)
        {
            i--;
            printf("\b \b");
            fflush(stdout);
        }
        else if (i < MAX_PASSWORD_LEN - 1)
        {
            pwd[i++] = ch;
            printf("*");
            fflush(stdout);
        }
    }

    pwd[i] = '\0';

    // Restore terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    printf("\n");

    if (strcmp(pwd, code) == 0)
        printf("Correct Password!\n");
    else
        printf("Invalid Password!\n");
}

int main(void)
{
    password();
    return 0;
}
#include <stdio.h>
#include <ncurses.h>

/* setting up typing function */
int nr_echo(int start_y, int start_x, int w, int h){
    int ch, y, x;
    ch = 0;
    y = start_y;
    x = start_x;
    /* main loop, F1 quits */
    while (ch != KEY_F(1)){
        ch = getch();
        /* backspace when not at the start of a line */
        if (ch == KEY_BACKSPACE && x > start_x){
            x--;
            mvdelch(y, x);
        }
        /* backspace when at the start of a line */
        else if (ch == KEY_BACKSPACE && x == start_x && y > start_y){
            y--;
            x = w;
            mvdelch(y, x);
        }
        /* return if enter is pressed and not at bottom of field*/
        else if (ch == '\n' && y <= h){
            x = start_x;
            y++;
            move(y,x);
        }
        /* auto return if not at bottom of field */
        else if (x >= w && y <= h ){
            x = start_x;
            y++;
            mvaddch(y, x, ch);
            x++;
        }

        /* main print */
        else {
            mvaddch(y, x, ch);
            x++;
        }
            }
    return 0;
}

int main()
{
    initscr();              /* initializes the screen */
    raw();                  /* get raw input */             
    keypad(stdscr, TRUE);   /* get special keys */
    noecho();               /* don't echo while we do getch() */
    curs_set(2);            /* sets the cursor to a box */
    

    nr_echo(0, 0, 20, 10);

    endwin();
    return 0;
}

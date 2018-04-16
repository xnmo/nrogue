#include <ncurses.h>

/* draws a nice box around the text field */
void textbox(int y1, int y2, int x1, int x2){
    int i, drawy, drawx;
    for (drawy = y1; drawy < y2; drawy++){
        mvaddch(drawy, x1, '|');
        mvaddch(drawy, x2, '|');
    }
    for (drawx = x1; drawx < x2; drawx++){
        mvaddch(y1, drawx, '-');
        mvaddch(y2, drawx, '-');
    }
    mvaddch(y1, x1, '+');
    mvaddch(y2, x1, '+');
    mvaddch(y1, x2, '+');
    mvaddch(y2, x2, '+');
    /* fill box with blank spaces */
    for (drawy = y1+1; drawy <= y2-1; drawy++){
        for (drawx = x1+1; drawx <= x2-1; drawx++)
            mvaddch(drawy, drawx, ' ');
    }
    move(y1+1, x1+1);
}

int main(){
    
    initscr();              /* initializes the screen */
    raw();                  /* get raw input */             
    keypad(stdscr, TRUE);   /* get special keys */
    noecho();               /* we will echo manually */
    curs_set(2);            /* sets the cursor to a solid rectangle */
    
    textbox(2,40,2,80);
    getch();
    textbox(20, 22, 60, 85);
    getch();

    
    endwin();
    return 0;
}

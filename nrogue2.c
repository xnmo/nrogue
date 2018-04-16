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


/* echo and keystrokes */
void textfield(int y1, int y2, int x1, int x2){
    int ch, writey, writex;
    writey = y1+1;
    writex = x1+1;
    ch = 0;

    while (ch != KEY_F(4) ){
        ch = getch();
        /* backspace when not at start of line */
        if (ch == KEY_BACKSPACE && writex > x1+1){
            writex--;
            mvaddch(writey, writex, ' ');
            move(writey, writex);
        }
        /* backspace when at start of line */
        else if (ch == KEY_BACKSPACE && writex == x1+1 && writey > y1+1){
            writey--;
            writex = x2-1;
            mvdelch(writey, writex);
        }
        /* return if enter is pressed and not at bottom of field */
        else if (ch == '\n' && writey < y2-1){
            writey++;
            writex = x1+1;
            move(writey, writex);
        }
        /* auto return if not at bottom of field */
        else if (writey < y2-1 && writex > x2-1){
            writey++;
            writex = x1+1;
            mvaddch(writey, writex, ch);
            writex++;
        }
        /* main print */
        else if (writey < y2 && writex < x2){
            mvaddch(writey, writex, ch);
            writex++;
        }

    }
}

int main(){
   

    initscr();              /* initializes the screen */
    raw();                  /* get raw input */             
    keypad(stdscr, TRUE);   /* get special keys */
    noecho();               /* we will echo manually */
    curs_set(2);            /* sets the cursor to a solid rectangle */
    
    /* print a nice header */
    textbox(1, 5, 2, 82);
    mvaddstr(3,36, "- NROGUE -");
    /* main textbox field */
    textbox(6, 40, 2, 82);
    textfield(6, 40, 2, 82);
    /* various other boxes for fun*/
    textbox(20, 23, 60, 85);
    textfield(20, 23, 60, 85);

    textbox(4, 19, 16, 29);
    textfield(4, 19, 16, 29);
    textbox(10, 15, 12, 65);
    textfield(10, 15, 12, 65);
    
    endwin();
    return 0;
}

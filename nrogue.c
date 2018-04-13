#include <stdio.h>
#include <ncurses.h>

/* setting up typing function */
int nr_echo( int start_y, int start_x, int w, int h ){
    
    int ch, y, x, end_y, end_x;
    ch = 0;
    y = start_y;
    x = start_x;
    end_y = start_y + h;
    end_x = start_x + w;
    move( start_y, start_x );

    /* main loop, F1 quits */
    while (ch != KEY_F(1) ){
        ch = getch();
        
        /* backspace when not at the start of a line */
        if ( ch == KEY_BACKSPACE && x > start_x ){
            x--;
            mvdelch( y, x );
        }
        /* backspace when at the start of a line */
        else if ( ch == KEY_BACKSPACE && x == start_x && y > start_y ){
            y--;
            x = end_x;
            mvdelch( y, x );
        }
        /* return if enter is pressed and not at bottom of field*/
        else if ( ch == '\n' && y <= end_y ){
            x = start_x;
            mvaddch( y, x, ch );
            y++;
        }
        /* auto return if not at bottom of field */
        else if ( x >= end_x && y < end_y ){
            x = start_x;
            y++;
            mvaddch( y, x, ch );
            x++;
        }

        /* main print */
        else{
            mvaddch( y, x, ch );
            x++;
        }
    }
    return 0;
}

int main(){
    initscr();              /* initializes the screen */
    raw();                  /* get raw input */             
    keypad(stdscr, TRUE);   /* get special keys */
    noecho();               /* we will echo manually */
    curs_set(2);            /* sets the cursor to a solid rectangle */
    
    /* draws a nice header */
    /* doing this manually because i'm a savage */
    mvaddstr( 1, 37, "- NROGUE -" );
    mvaddstr( 2, 2, "F1: exit    F2: save" );
    move( 3, 2 );
    /* makes a line diving header and body */
    {
        int n;
        for ( n = 0; n < 80; n++ )
            addch( '_' );
    }
    
    /* primary function call */
    nr_echo( 5, 2, 20, 2 );
    mvaddstr( 0, 2, "save file as:" );
    nr_echo( 0, 16, 16, 1 );
    endwin();
    return 0;
}

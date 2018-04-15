#include <stdlib.h>
#include <ncurses.h>

/* setting up typing function */
char * nr_echo( int start_y, int start_x, int h, int w, int *out_count ){
    int ch, y, x, end_y, end_x, ch_count ; 
   
    /* create pointer stored_line */
    char *stored_line;
   
    /* aim that pointer at a bunch of memory from malloc */ 
    stored_line = malloc(100000);
    
    ch = ch_count = 0;
    y = start_y;
    x = start_x;
    end_y = start_y + h - 1;
    end_x = start_x + w;
    
    /* move to the starting location */
    move( start_y, start_x );

    /* main loop, F1 quits */
    while (ch != KEY_F(1) ){
        ch = getch();
        
        /* backspace when not at the start of a line */
        if ( ch == KEY_BACKSPACE && x > start_x ){
            x--;
            mvdelch( y, x );
            ch_count--;

        }
        /* backspace when at the start of a line */
        else if ( ch == KEY_BACKSPACE && x == start_x && y > start_y ){
            y--;
            x = end_x;
            mvdelch( y, x );
            ch_count--;
        }
        /* return if enter is pressed and not at bottom of field*/
        else if ( ch == '\n' && y <= end_y ){
            x = start_x;
            y++;
            move( y, x );
            stored_line[ch_count] = '\n';
            ch_count++;
        }
        /* auto return if not at bottom of field */
        else if ( x >= end_x && y < end_y ){
            x = start_x;
            y++;
            stored_line[ch_count] = ch;
            mvaddch( y, x, stored_line[ch_count] );
            x++;
            ch_count++;
        }

        /* main print */
        else if ( x <= end_x && y <= end_y ){
            stored_line[ch_count] = ch;
            mvaddch( y, x, stored_line[ch_count] );
            x++;
            ch_count++;
        }
    }
    /* print the array */
    out_count[0] = ch_count;
    return stored_line;
}



int main(){
    FILE *fp;
    char *save_string, *title_string;
    int save_count;
    int text_count, title_count;

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
    /* starting y, starting x, height, width */
    save_string = nr_echo( 5, 2, 40, 80, &text_count );

    /* ask for file name */
    mvaddstr( 0, 2, "save file as:" );
    title_string = nr_echo( 0, 16, 1, 16, &title_count ); 


    fp = fopen("testfile", "w+" );
    for (save_count = 0; save_count < text_count; save_count ++)
        fputc( save_string[save_count], fp);
    fclose(fp);
    free(save_string);
    endwin();
    return 0;
}

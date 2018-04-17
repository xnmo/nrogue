#include <ncurses.h>
#include <stdlib.h>

/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/

/* echo keystrokes and store input */
char *textfield(int y1, int y2, int x1, int x2){
    int ch, writey, writex, ch_count;
    char *stored_line;
    stored_line = malloc(100000);
    writey = y1+1;
    writex = x1+1;
    ch = ch_count = 0;

    while (ch != KEY_F(4) ){
        ch = getch();
        /* backspace when not at start of line */
        if (ch == KEY_BACKSPACE && writex > x1+1){
            writex--;
            mvaddch(writey, writex, ' ');
            move(writey, writex);
            ch_count--;
        }
        /* backspace when at start of line */
        else if (ch == KEY_BACKSPACE && writex == x1+1 && writey > y1+1){
            writey--;
            writex = x2-1;
            mvdelch(writey, writex);
            ch_count--;
        }
        /* return if enter is pressed and not at bottom of field */
        else if (ch == '\n' && writey < y2-1){
            writey++;
            writex = x1+1;
            move(writey, writex);
            stored_line[ch_count] = ch;
            ch_count++;
        }
        /* auto return if not at bottom of field */
        else if (writey < y2-1 && writex > x2-1){
            writey++;
            writex = x1+1;
            mvaddch(writey, writex, ch);
            writex++;
            stored_line[ch_count] = ch;
            ch_count++;
        }
        /* main print */
        else if (writey < y2 && writex < x2){
            mvaddch(writey, writex, ch);
            writex++; 
            stored_line[ch_count] = ch;
            ch_count++;

        }

    }
    stored_line[ch_count - 1] = '\0';
    return stored_line;
}

/*----------------------------------------------------------------------------*/

int main(){
    FILE *fp;
    int i;
    char *save_string, *title_string;   

    initscr();              /* initializes the screen */
    raw();                  /* get raw input */             
    keypad(stdscr, TRUE);   /* get special keys */
    noecho();               /* we will echo manually */
    curs_set(2);            /* sets the cursor to a solid rectangle */
    
/*----------------------------------------------------------------------------*/

    /* print a nice header */
    textbox(1, 5, 2, 82);
    mvaddstr(3,36, "- NROGUE -");

    /* main textbox field */
    textbox(6, 40, 2, 82);
    save_string = textfield(6, 40, 2, 82);

    /* save as box */
    textbox(20, 23, 30, 60);
    mvaddstr(21,38," - SAVE AS - ");
    move(22, 31);
    title_string = textfield(21, 23, 30, 60);

/*----------------------------------------------------------------------------*/

    /* save the main sting as a file, one char at a time */
    fp = fopen(title_string, "w+");
    for (i = 0; save_string[i] != '\0'; i++)
        fputc(save_string[i], fp);
    fclose(fp);
    free(save_string);
    
/*----------------------------------------------------------------------------*/

    endwin();
    return 0;
}

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
        if (ch == KEY_BACKSPACE){
            if (writex > x1+1){
                /* just move left one */
                writex--;
                mvaddch(writey, writex, ' ');
                move(writey, writex);
                ch_count--;
            }
            else if (writex == x1+1 && writey > y1+1){
                int i;
                /* set cursor at end of previous line and update ch_count*/
                writey--;
                ch_count--;
                writex = x2-1;
                /* moving left until we find a char */
                /* i suspect mvinch is the issue here */
                for (i = mvinch(writey, writex); i != ' '; writex--){
                    mvaddch(writey, writex, ' ');
                }   
            }
        }
        /* return if enter is pressed and not at bottom of field */
        else if (ch == '\n' && writey < y2-1){
            /* add newline char at end of current line */
            move(writey, writex);
            stored_line[ch_count] = '\n';
            /* move cursor to next line and increment ch_count */
            writey++;
            writex = x1+1;
            move(writey, writex);
            ch_count++;
        }
        /* print 4 spaces if tab is pressed */
        else if (ch == '\t'){
            /* check if there is room */
            if ( writex <= x2-4){
                int i;
                for (i=0; i < 4; i++){
                    mvaddch(writey, writex, ' ');
                    stored_line[ch_count] = ' ';
                    writex++;
                    ch_count++;
                }
            }
            /* when there is not enough room go to next line and tab */
            else {
                int i;
                writey++;
                writex = x1+1;
                move(writey, writex);
                /* a little crude to write this twice don't you think */
                /* don't think it is worthy of a function tho */
                for (i=0; i < 4; i++){
                    mvaddch(writey, writex, ' ');
                    stored_line[ch_count] = ' ';
                    writex++;
                    ch_count++;

                } 
            }
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
    /* overwrite last char (F4) with a null termination */
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
    free(title_string);

/*----------------------------------------------------------------------------*/

    endwin();
    return 0;
}

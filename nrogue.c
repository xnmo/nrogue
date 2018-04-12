#include <stdio.h>
#include <ncurses.h>

#define WIDTH 80
#define BUFFER 600000
int main()
{

/*----------------------------------------------------------*/
//definitions
    
    int ch, cursy, cursx, ch_count;
    int stored_line[BUFFER];

    FILE* fp;

/*----------------------------------------------------------*/
//setup screen

    initscr();              //initializes the screen
    raw();                               
    keypad(stdscr, TRUE);   //get special keys
    noecho();               //don't echo while we do getch()
    curs_set(2);            //sets the cursor to a box

/*----------------------------------------------------------*/
//setup text editor

    //draws a nice header
    mvaddstr(1,(WIDTH/2) - 5,"- NROGUE -");
    mvaddstr(2,0,"press F1 to exit    press F2 to save");
    move(4,0);
    //makes a line diving header and body
    int n;
    for (n = 0; n <WIDTH; n++)
        addch('-');

    //start the program at the appropriate place
    cursy = 5;
    cursx = 0;
    ch_count = 0;
/*----------------------------------------------------------*/
//main loop

    while (ch != KEY_F(1)){
        // if the cursor is at the 40th line
        // automatically move to the next line
        if ( cursx == WIDTH ){
            cursy ++;
            cursx = 0;
        }
        //set cursor position and wait for chars
        move(cursy, cursx);
        ch = getch();
        
        //when backspace is pressed...
        if (ch == KEY_BACKSPACE && cursx > 0){
            
            //jumps the cursor back one space
            //and deletes that character
            cursx --;
            ch_count --;
            move(cursy, cursx);
            delch();
        }
            //jumps to prior line if you are
            //at the start of the current line
            //and not at the start of the body
        else if (ch == KEY_BACKSPACE && cursx == 0 && cursy >5){
            cursy --;
            cursx = WIDTH;
            ch_count --;
        }
        //start a new line if enter is pressed
        else if (ch == 10){
            cursy++;
            cursx = 0;
            stored_line[ch_count] = 10;
            ch_count ++;
        }
/*----------------------------------------------------------*/
        //print out a copy of stored_line[1024]
        //tests to see how file output will appear
        else if (ch == KEY_F(2)){
            int ch_print_count, ch_print_x;
            ch_print_x = 0;
            mvaddstr(0,0, "save file as?:");
           // TODO : Make this actually work 
            fp=fopen("testfile","w+");  //open the file
            for (ch_print_count = 0; ch_print_count < ch_count; ch_print_count ++){
                fputc( stored_line[ch_print_count], fp); //put ch into file
                ch_print_x ++;
            }
            getch();
            break;
/*----------------------------------------------------------*/
        }
        //draw a character in the next position
        else {
           // addch(ch);
            stored_line[ch_count] = ch;
            mvaddch(cursy,cursx,stored_line[ch_count]);
            cursx ++;
            ch_count ++;
            //refresh();
        }
    
    }

/*----------------------------------------------------------*/

    endwin();
    return 0;
}

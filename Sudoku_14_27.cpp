#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>
using namespace std;

//sudoku variables and functions
int grid[9][9];
bool find_unassigned_location(int &row, int &col);
bool is_safe(int row, int col, int num);
bool solve_sudoku();
bool find_unassigned_location(int &row, int &col);
bool used_in_row(int row, int num);
bool used_in_col(int col, int num);
bool used_in_box(int box_row, int box_col, int num);
void sudoku();


//display variables and functions
struct player{
    char name[50];
    int score;
}players[3][4];
char identity[50], ch;
bool rematch, done;
char display_grid[9][9];
bool initial_grid[9][9];
int digits, num_left, command;
int TIME, rest;
int active_box;
void welcome();
void start_game();
void get_name();
void set_level();
void show_rank_list();
void game();
void solver();
void solve_given();
void preset();
void change_active_box_game();
bool check_click();
bool get_input(char ch);
bool game_over();
void result();
void permanent_display();
void display_layout();
void update_number_game();
void update_number_solver();
void thank_you();
void reset_all();
void instructions();
int show_timer(time_t beginning);

//functions for rank-list
void load_database();
int sort_rank();
void show_rank_list();
void update_rank_list();
void print_solved();

int main(void)
{

    while(1)
    {
        welcome();
        switch (command)
        {
            case 1 : game();
                break;
            case 2 : show_rank_list();
                break;
            case 3 : solver();
                break;
            case 4 : instructions();
                break;
            case 5 : return 0;
                break;
        }
        reset_all();
    }
    return 0;
}


//sudoku variables and functions
bool solve_sudoku()
{
    int row, col;
    if (!find_unassigned_location(row, col)) return true;
    for (int num = 1; num <= 9; num++)
        if (is_safe(row, col, num))
        {
            grid[row][col] = num;
            if (solve_sudoku())  return true;
            grid[row][col] = 0;
        }
    return false;
}

bool find_unassigned_location(int &row, int &col)
{
    for (row = 0; row < 9; row++)
        for (col = 0; col < 9; col++)
            if (grid[row][col] == 0) return true;
    return false;
}

bool used_in_row(int row, int num)
{
    for (int col = 0; col < 9; col++)
        if (grid[row][col] == num)  return true;
    return false;
}

bool used_in_col(int col, int num)
{
    for (int row = 0; row < 9; row++)
        if (grid[row][col] == num)  return true;
    return false;
}

bool used_in_box(int box_row, int box_col, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (grid[row+box_row][col+box_col] == num)  return true;
    return false;
}

bool is_safe(int row, int col, int num)
{
    return !used_in_row(row, num) &&
           !used_in_col(col, num) &&
           !used_in_box(row - row%3, col - col%3, num);
}

void sudoku()
{
    srand(time(0));
    int v[9]= {1,2,3,4,5,6,7,8,9};
    random_shuffle(v,v+9);
    srand (time(0));
    int x = rand()%9;
    for(int i=0; i<9; i++)  grid[x][i]=v[i] ;
    solve_sudoku();
    return;
}


//display variables and functions
void welcome()
{
    if(rematch)
    {
        command = 1;
        return;
    }
    initwindow(1300, 700);
    int x,y;
    readimagefile("sudoku1.jpg",0,0,1300,700);
    while(1)
    {
        if(mousex()>202 && mousex()<380 && mousey()>405 && mousey()<434)
        {
            readimagefile("sudoku2.jpg",0,0,1300,700);
        }
        else if(mousex()>202 && mousex()<477 && mousey()>475 && mousey()<503)
        {
            readimagefile("sudoku3.jpg",0,0,1300,700);
        }
        else if(mousex()>202 && mousex()<465 && mousey()>545 && mousey()<573)
        {
            readimagefile("sudoku4.jpg",0,0,1300,700);
        }
        else if(mousex()>202 && mousex()<432 && mousey()>620 && mousey()<644)
        {
            readimagefile("sudoku5.jpg",0,0,1300,700);
        }
        else if(mousex()>1068 && mousex()<1150 && mousey()>656 && mousey()<679)
        {
            readimagefile("sudoku6.jpg",0,0,1300,700);
        }
        else
        {
            readimagefile("sudoku1.jpg",0,0,1300,700);
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>202 && x<380 && y>405 && y<434) break;
            if(x>202 && x<477 && y>475 && y<503) break;
            if(x>202 && x<465 && y>545 && y<573) break;
            if(x>202 && x<432 && y>620 && y<644) break;
            if(x>1068 && x<1150 && y>656 && y<679) break;
        }
        clearmouseclick(WM_LBUTTONDOWN);
    }
    if(y>405 && y<434) command = 1;
    if(y>475 && y<503) command = 2;
    if(y>545 && y<573) command = 3;
    if(y>620 && y<644) command = 4;
    if(y>656 && y<679) command = 5;
    closegraph();
    return;
}

void game()
{
    initwindow(1300, 700);
    if(!rematch)
    {
        start_game();
    }
    sudoku();
    preset();
    cleardevice();
    readimagefile("background1.jpg",0,0,1300,700);
    permanent_display();
    bool updated = true;
    time_t beginning;
    time(&beginning);
    TIME = 1800;
    while(TIME<=1800)
    {
        TIME = show_timer(beginning);
        if(updated)
        {
            update_number_game();
            updated = false;
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            change_active_box_game();
        }
        if(kbhit() && active_box!=-1)
        {
            ch = getch();
            updated = get_input(ch);
        }
        clearmouseclick(WM_LBUTTONDOWN);
        if(done) break;
    }
    result();
    getch();
    cleardevice();
    thank_you();
    closegraph();
    return;
}


void start_game()
{
    get_name();
    set_level();
    int x,y;
    while(1)
    {
        if(mousex()>970 && mousex()<1231 && mousey()>601 && mousey()<636)
        {
            readimagefile("play.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        else
        {
            readimagefile("new_game.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        if(kbhit())
        {
            ch = getch();
            if(ch==13) break;
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>798 && x<1085 && y>600 && y<645) break;
        }
        clearmouseclick(WM_LBUTTONDOWN);
    }
    return;
}




void get_name()
{
    int i;
    for(i=0; i<26; i++) identity[i] = ' ';
    identity[i] = '\0';
    i = 0;
    readimagefile("new_game.jpg",0,0,1300,700);
    settextstyle(1,HORIZ_DIR,4);
    setbkcolor(WHITE);
    setcolor(BLACK);
    while(1)
    {
        if(kbhit())
        {
            ch = (char)getch();
            if(ch==13)
            {
                if(identity[0]!=' ') break;
            }
            else if(ch == 8)
            {
                if(i) identity[--i] = ' ';
            }
            else if(i<24) identity[i++] = ch;
            outtextxy(160, 140, identity);
        }
    }
    return;
}


void set_level()
{
    int x, y;
    setbkcolor(WHITE);
    settextstyle(1,HORIZ_DIR,4);
    setcolor(BLACK);
    readimagefile("new_game.jpg",0,0,1300,700);
    outtextxy(160, 140, identity);
    while(1)
    {
        if(mousex()>605 && mousex()<718 && mousey()>285 && mousey()<321)
        {
            readimagefile("easy.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        else if(mousex()>585 && mousex()<735 && mousey()>382 && mousey()<412)
        {
            readimagefile("medium.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        else if(mousex()>606 && mousex()<716 && mousey()>473 && mousey()<505)
        {
            readimagefile("hard.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        else
        {
            readimagefile("new_game.jpg",0,0,1300,700);
            outtextxy(160, 140, identity);
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(x>605 && x<718 && y>285 && y<321) break;
            if(x>585 && x<735 && y>382 && y<412) break;
            if(x>606 && x<716 && y>473 && y<505) break;
        }
        clearmouseclick(WM_LBUTTONDOWN);
    }
    if(y>285 && y<321) digits = 38;
    else if(y>382 && y<412) digits = 32;
    else if(y>473 && y<505) digits = 26;
    return;
}


void preset()
{
    bool mark[81];
    fill(mark,mark+81,0);
    srand(time(0));
    for (int i=0; i<digits; )
    {
        int random = rand() % 81;
        int x = random / 9;
        int y = random % 9;
        if (mark[random] == false && display_grid[x][y]!='0')
        {
            display_grid[x][y] = '0';
            mark[random] = true;
            i++;
        }
    }

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(display_grid[i][j]=='0')
            {
                display_grid[i][j] = grid[i][j]+'0';
                initial_grid[i][j] = 1;
            }
        }
    }
    return;
}


void permanent_display()
{
    int num_left, right, top = 100, bottom = 140;
    for(int i=0; i<9; i++)
    {
        num_left = 300, right = 340;
        for(int j=0; j<9; j++)
        {
            rectangle(num_left, top, right, bottom);
            num_left += 40;
            right += 40;
        }
        top += 40;
        bottom += 40;
    }
    for(int i=300; i<670; i+=120)
    {
        line(i-1,100,i-1,460);
        line(i+1,100,i+1,460);
        line(300,i-199,660,i-199);
        line(300,i-201,660,i-201);
    }
    settextstyle(8, HORIZ_DIR, 4);
    setcolor(RED);
    int x = 305, y = 105;
    for(int i=0; i<9; i++)
    {
        x = 305;
        for(int j=0; j<9; j++)
        {
            bgiout << display_grid[i][j];
            outstreamxy(x, y);
            x += 40;
        }
        y += 40;
    }
    return;
}


int show_timer(time_t beginning)
{
    time_t now;
    time(&now);
    TIME = difftime(now,beginning);
    rest = 1800-TIME;
    setcolor(RED);
    if(rest/60<10) bgiout << '0' << rest/60 << ':';
    else bgiout << rest/60 << ':';
    if(rest%60<10) bgiout << '0' << rest%60 << endl;
    else bgiout <<  rest%60 << endl;
    outstreamxy(1000, 300);
    return TIME;
}


void change_active_box_game()
{
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>1030 && x<1160 && y>432 && y<487)
    {
        done = true;
    }

    if(x>300 && x<660 && y>100 && y<460)
    {
        x-=300;
        y-=100;
        if(x%40!=0 && y%40!=0)
        {
            x/=40;
            y/=40;
            if(initial_grid[y][x]==0)
            {
                active_box = (9*y)+x;
                return;
            }
        }
    }
    active_box = -1;
    return;
}


bool get_input(char ch)
{
    int x, y;
    x = (active_box) / 9;
    y = (active_box) % 9;
    if(ch==13)
    {
        active_box = -1;
        return false;
    }
    if(ch==8)
    {
        num_left+=(display_grid[x][y]!=32);
        display_grid[x][y] = ' ';
        return true;
    }
    if(ch>='1' && ch<='9')
    {
        num_left+=(display_grid[x][y]==32);
        display_grid[x][y] = ch;
        return true;
    }
    return false;
}


void update_number_game()
{
    settextstyle(8, HORIZ_DIR, 4);
    setbkcolor(WHITE);
    setcolor(BLACK);
    int x = 305, y = 105;
    for(int i=0; i<9; i++)
    {
        x = 305;
        for(int j=0; j<9; j++)
        {
            if(initial_grid[i][j]==0)
            {
                bgiout << display_grid[i][j];
                outstreamxy(x, y);
            }
            x += 40;
        }
        y += 40;
    }
    return;
}



void result()
{
    bool won = true;;
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(display_grid[i][j]!=grid[i][j]+'0')
            {
                won = false;
            }
        }
    }
    if(won)
    {
        readimagefile("congrats.jpg",0,0,1300,700);
    }
    else
    {
        readimagefile("failed.jpg",0,0,1300,700);
    }
    getch();
    return;
}


void thank_you()
{
    settextstyle(3, HORIZ_DIR, 5);
    readimagefile("thanks.jpg",0,0,1300,700);
    int x,y;
    while(1)
    {
        if(kbhit())
        {
            ch = (char)getch();
            if(ch==13)
            {
                rematch = true;
                return;
            }
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if(((x>339 && x<464) || (x>843 && x<938)) && (y>386 && y<462)) break;
        }
        clearmouseclick(WM_LBUTTONDOWN);
    }
    if(x>339 && x<464) rematch = true;
    else rematch = false;
    command = 1;
    return;
}


void load_database()
{
	FILE *input;
	input = fopen("rank_list.txt","r");
    for(int i=0; i<3; i++)
	{
	    for(int j=0; j<3; j++)
        {
            fscanf(input," %[^\n]",players[i][j].name);
            fscanf(input,"%d",&players[i][j].score);
        }
    }
    fclose(input);
    return;
}

int sort_rank()
{
    int i, j;
    if(digits==38) i = 0;
    else if(digits==32) i = 1;
    if(digits==26) i = 2;
    for(j=2; j>0; j--)
    {
        if(players[i][j].score>players[i][j-1].score)
        {
            swap(players[i][j], players[i][j-1]);
        }
        else break;
    }
    return (j+1);
}


void show_rank_list()
{
    load_database();
    initwindow(1300, 700);
    readimagefile("rank_list.jpg",0,0,1300,700);
    settextstyle(8, HORIZ_DIR,4);
    bgiout  << players[0][0].name;
    outstreamxy(185,135);
    bgiout  << players[0][1].name;
    outstreamxy(185,175);
    bgiout  << players[0][2].name;
    outstreamxy(185,215);
    bgiout  << players[1][0].name;
    outstreamxy(185,330);
    bgiout  << players[1][1].name;
    outstreamxy(185,370);
    bgiout  << players[1][2].name;
    outstreamxy(185,410);
    bgiout  << players[2][0].name;
    outstreamxy(185,525);
    bgiout  << players[2][1].name;
    outstreamxy(185,565);
    bgiout  << players[2][2].name;
    outstreamxy(185,605);
    bgiout << players[0][0].score;
    outstreamxy(1000,135);
    bgiout << players[0][1].score;
    outstreamxy(1000,175);
    bgiout << players[0][2].score;
    outstreamxy(1000,215);
    bgiout << players[1][0].score;
    outstreamxy(1000,330);
    bgiout << players[1][1].score;
    outstreamxy(1000,370);
    bgiout << players[1][2].score;
    outstreamxy(1000,410);
    bgiout << players[2][0].score;
    outstreamxy(1000,525);
    bgiout << players[2][1].score;
    outstreamxy(1000,565);
    bgiout << players[2][2].score;
    outstreamxy(1000,605);
    getch();
    closegraph();
    return;
}


void update_rank_list()
{
    FILE *input;
	input = fopen("rank_list","w");
	for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            fprintf(input,"%s\n%d\n", players[i][j].name, players[i][j].score);
        }
    }
    fclose(input);
    return;
}


void solver()
{
    initwindow(1300, 700);
    setbkcolor(WHITE);
    setcolor(BLACK);
    bool updated = true;
    bool done = false;
    readimagefile("background2.jpg",0,0,1300,700);
    display_layout();
    settextstyle(8,HORIZ_DIR,6);
    outtextxy(900,600,"Solve");
    while(!done)
    {
        if(updated)
        {
            update_number_solver();
            updated = false;
        }
        if(ismouseclick(WM_LBUTTONDOWN))
        {
            done = check_click();
        }
        if(kbhit() && active_box!=-1)
        {
            ch = getch();
            if(ch==13) break;
            updated = get_input(ch);
        }
        clearmouseclick(WM_LBUTTONDOWN);
    }
    cleardevice();
    readimagefile("background2.jpg",0,0,1300,700);
    solve_given();
    return;
}


void display_layout()
{
    int num_left, right, top = 100, bottom = 140;
    for(int i=0; i<9; i++)
    {
        num_left = 300, right = 340;
        for(int j=0; j<9; j++)
        {
            rectangle(num_left, top, right, bottom);
            num_left += 40;
            right += 40;
        }
        top += 40;
        bottom += 40;
    }
    for(int i=300; i<670; i+=120)
    {
        line(i-1,100,i-1,460);
        line(i+1,100,i+1,460);
        line(300,i-199,660,i-199);
        line(300,i-201,660,i-201);
    }
    return;
}


bool check_click()
{
    int x,y;
    getmouseclick(WM_LBUTTONDOWN, x, y);
    if(x>300 && x<660 && y>100 && y<460)
    {
        x-=300;
        y-=100;
        if(x%40!=0 && y%40!=0)
        {
            x/=40;
            y/=40;
            active_box = (9*y)+x;
        }
        else active_box = -1;
        return false;
    }
    else if(x>900 && x<1062 && y>600 && y<642) return true;
    active_box = -1;
    return false;
}


void update_number_solver()
{
    settextstyle(8, HORIZ_DIR, 4);
    setcolor(BLACK);
    int x = 305, y = 105;
    for(int i=0; i<9; i++)
    {
        x = 305;
        for(int j=0; j<9; j++)
        {
            bgiout << display_grid[i][j];
            outstreamxy(x, y);
            x += 40;
        }
        y += 40;
    }
    return;
}


void solve_given()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            if(display_grid[i][j]>='1' && display_grid[i][j]<='9')
                grid[i][j] = display_grid[i][j] - '0';
            else grid[i][j] = 0;
        }
    }
    bool solved = solve_sudoku();
    if(solved)
    {
        print_solved();
    }
    if(!solved)
    {
        settextstyle(8, HORIZ_DIR, 4);
        bgiout << "No solution exists for this input\n";
        outstreamxy(100, 100);
    }
    getch();
    return;
}


void print_solved()
{
    display_layout();
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            display_grid[i][j] = grid[i][j]+'0';
        }
    }
    update_number_solver();
    return;
}


void instructions()
{
    initwindow(1300, 700);
    readimagefile("instructions.jpg",0,0,1300,700);
    getch();
    closegraph();
    return;
}


void reset_all()
{
    for(int i=0; i<9; i++)
    {
        for(int j=0; j<9; j++)
        {
            grid[i][j] = 0;
            display_grid[i][j] = 0;
            initial_grid[i][j] = 0;
        }
    }
    active_box = -1;
    command = 0;
    rematch = 0;
    done = 0;
    return;
}

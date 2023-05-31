
#include <stdio.h>
#include <stdlib.h>

/*=========================================================================
  Constants and definitions:
==========================================================================*/

/*#defines and typedefs here*/
#define N 25
#define BOARD_ELEMENT '#'
#define PERCENT 0
#define FIRST_PLAYER PERCENT
#define PERCENT_HEAD '%'
#define PERCENT_body '*'
#define SHTRODEL 1
#define SHTRODEL_HEAD '@'
#define SHTRODEL_body '+'
#define ILLEGAL_MOVE 0
#define food_place 'F'
#define down 2
#define up 8
#define right 6
#define left 4
#define first_length 3
#define eaten 9
#define moved 99
#define error 1
#define min_size 4
#define full -1
#define MAXSTEPS 2
#define TWO_INPUTS 2


/*-------------------------------------------------------------------------
*   Function declaration
*--------------------------------------------------------------------------*/

void print_introduction_msg();                        //print welcome message , and introduction message to the game
void print_board_size_msg();                          //print message to enter board size
void print_steps_without_food();                      //print message to enter maximum number of steps can snake do without food
void print_food_location_msg();                       //print message to insert place for next food
void help_print_board(int size);                      //printing static part of the board
void print_board(char board[N][N], int size);          //print the current board for the user
void print_player_message(int player);                //print message when we switch between users
void print_insert_direction();			              //print message asking the user to insert his move
void print_finsih_msg(int player,int finish_reason ); //get the player who lose and the reason of losing and print message state
void print_error_input();                             //print message - there is error in the input
void print_invalid_input();			                  //print message - re-insert valid input
void print_full_board();		                      //print message - full board - no winner


int proper_input(int i,int j,int row,int cul,char place,char board[N][N]);
int where_to_add_culumns(int direction);
int where_to_add_rows(int direction);
int snake_moved(char head,char board[N][N],int step,int maxstep,int size);
int can_move(int i,int j,int row,int cul,char board[N][N],char head);
int ate(int i,int j,int row,int cul,char board[N][N],char head,int size);
int play(char board[N][N],int size,int maxstep,int p[N][N],int sh[N][N]);
int get_direction();
int get_max_steps();
int get_board_size();
int check_direction(int direction);
int is_full_matrix(char board[N][N], int size);
int get_food_location(char board[N][N], int size);
int check_food(int food_row,int food_culumn,int size,
               char board[N][N]);
void fill_snake_matrix(int matrix[N][N], int turn, int size);
void fill_matrix0(char board[N][N], int size);
char get_body(char head);
int get_turn(char head);
int check_go(char b[N][N],int m[N][N],int nck,
             int tl,char head,int go,int stp);
int is_error(int check);
void change_snake(int matrix[N][N],char board[N][N],int part,char base);
void print_start_turn(char board[N][N], int size,int turn);
int is_properly_moved (int i,int j,int add_rows,
  int add_culumns,char board[N][N],int size,
  int step , int maxstep,char head);


/*the main function getts the first values of
food_location ,size, max_steps and calls for
the play function that runs the game*/
int main()
{
    print_introduction_msg();
    int size= get_board_size();
    if(is_error(size)==error){return 0 ;}

    char board[N][N];
    fill_matrix0(board, size);

    int p[N][N], sh[N][N];
    fill_snake_matrix(p,  PERCENT,  size);
    fill_snake_matrix(sh,  SHTRODEL,  size);


    int max_steps=get_max_steps();
    if(is_error(max_steps)==error){return 0 ;}

   if (is_error(get_food_location(board, size))== error){return 0;}

   if(is_error(play(board, size, max_steps,p,sh))==error){return 0;}
   return 0;


}

//////////////////////////////////////
/* the function check if the input is not legal*/
int is_error(int check)
{
    if(check==error)
        {   print_error_input();
            return error;
        }
            return (!error);

}

/////////////////////////////////////
/* the finction fills the board matrix for the first time */
void fill_matrix0(char board[N][N], int size)
{
    for(int i=0 ; i<size ; i++)
    {
        for(int j=0 ; j<size ; j++)
        {
            board[i][j]=' ';
        }
    }

    board[0][0]= PERCENT_HEAD;
    board[size-1][0]= SHTRODEL_HEAD ;

    for(int j=1; j<first_length ; j++)
    {
        board[0][j]= PERCENT_body;
        board[size-1][j]= SHTRODEL_body ;
    }

}
/////////////////////////////////////
/* the function fills the snake_matrix for the first time*/
void fill_snake_matrix(int matrix[N][N], int turn, int size)
{
    for(int i=0 ; i<N; i++)
    {
        for(int j=0 ; j<N ; j++)
        {
            matrix[i][j]=0;
        }
    }

    int length_num= first_length ;
    int i =0 ;
    if(turn==SHTRODEL)
    {
        i= size-1 ;
    }
    for(int j=0; j<first_length ; j++)
    {
        matrix[i][j]= length_num-- ;
    }

}

////////////////////////////////////
/*the function checks if the food place is legal
 and if the place is empty */
int check_food(int food_row,int food_culumn,int size,
               char board[N][N])
{

    if(food_row<size&&food_row>=0)
        if (food_culumn<size&&food_culumn>=0)
            if(board[food_row][food_culumn]==' ')
            {
                return 1;
            }

    return 0 ;


}
////////////////////////////////////////
/*the function gets values for the food place
and put them in the matrix ifter checking them */
int get_food_location(char board[N][N], int size)
{
    if(is_full_matrix(board,size)){return full;}

    int food_row=0, food_culumn=0 ;
    print_food_location_msg();
   int check= scanf("%d %d", &food_row, &food_culumn);
while((!check_food(food_row, food_culumn, size, board))&&check==TWO_INPUTS)
    {
        print_invalid_input();
        check=scanf("%d %d", &food_row, &food_culumn);
    }
    if(check!=TWO_INPUTS){return error;}
    board[food_row][food_culumn]=food_place ;
    return eaten ;

}
////////////////////////////////////////
/*the function checks if the matrix is full */
int is_full_matrix(char board[N][N], int size)
{
    for(int i=0; i<size ; i++)
    {
        for(int j=0; j<size ; j++)
        {
            if(board[i][j]==' ')
            {
                return 0;
            }
        }
    }
     print_board(board,size);
    print_full_board();
    return 1;
}

////////////////////////////////////////
/*the function checks if the direction is proper */
int check_direction(int direction)
{
    if(direction!=up)
        if(direction!=down)
            if(direction!=left)
                if(direction!=right)
                    return 0 ;

    return 1;
}
////////////////////////////////////////
/* the function gets the size of the matrix
 and checks if it's proper*/
int get_board_size()
{
    int size=0;
    int check=0;
    print_board_size_msg();
    check =scanf("%d", &size);
    while(!(size>=min_size&&size<=N)&& check==1)
    {
        print_invalid_input();
        check=scanf("%d", &size);
    }
    if (check!=1)
    {
        return error;
    }
    return size;
}
///////////////////////////////////////
/* the function gets the max_steps that a
 snake can move and checks if the value is proper*/
int get_max_steps()
{
    int check =0;
    int max_steps=0;
    print_steps_without_food();
    check =scanf("%d", &max_steps);
    while(max_steps<MAXSTEPS&&check==1)
    {
        print_invalid_input();
        check=scanf("%d", &max_steps);
    }
    if (check!=1)
    {
        return error;
    }
    return max_steps ;
}

////////////////////////////////////////
/*the function adds "neck" to the snake matrix so
 we can stay updated of the place of the head , or ,
 the function erases the tail of the snake
in the main matrix and the helping matrix
so we can stay updated of it's place */
void change_snake(int matrix[N][N],char board[N][N],int part,char base)
{
     int stop=0;
    for ( int i=0 ; (i<N) && (!stop); i++)
        for ( int j=0 ; (j<N) && (!stop); j++)
        {
             if((base==SHTRODEL_HEAD)||(base==PERCENT_HEAD))
             {
                if (board[i][j]==base)
            {
                matrix[i][j]=part;
                stop=1;
            }
             }
             else
              {
            if (matrix[i][j]==part)

            {
                matrix[i][j]=0;
                board[i][j]=base;
                stop=1;
            }
              }

        }
}

///////////////////////////////////////
/*the function gets direction and
 checks if the values are proper and legal */
int get_direction()
{
    int direction =0 ; int check=0;
    print_insert_direction();
    check=scanf("%d",&direction);
    while(!check_direction(direction)&&check==1)
    {
        print_invalid_input();
        check=scanf("%d",&direction);
    }
    if(check!=1){return error;}
    return direction ;
}
////////////////////////////////////////////////////////////
/*go is a value that we get when calling another
 function and checks if the result
 is legal or if the snake moved or ate the food*/
int check_go(char board[N][N],int matrix[N][N],int neck,int tail,
             char head,int go,int stp)
{
    char space =' ';
    if(go==eaten)
    {
        stp=0;
    change_snake(matrix,board,neck,head);

    }
    else if (go==moved)
    {
    change_snake(matrix,board,neck,head);
    change_snake(matrix,board,tail,space);
    }
    return stp ;
}

////////////////////////////////////////////////////////////
/*the main function that runs the game and works
 until the main matrix is full ,
 this function calls for another function to
  move the snakes and update the values */
int play(char board[N][N],int size,int maxstep,int p[N][N],int sh[N][N])
{

    int turn =PERCENT,p_steps=0,sh_steps=0,go=1,check=0;
    int ptail=1,shtail =1,p_neck = first_length,sh_neck =first_length;

    while (go!=0&&go!=full)
    {

print_start_turn(board,size,turn);
if(turn==SHTRODEL)
        {
go= snake_moved(SHTRODEL_HEAD,board,++sh_steps,maxstep,size);
sh_steps=check_go(board,sh,++sh_neck,shtail,SHTRODEL_HEAD,go,sh_steps);
        }
else {go= snake_moved(PERCENT_HEAD,board,++p_steps,maxstep,size);
p_steps=check_go(board,p,++p_neck,ptail,PERCENT_HEAD,go,p_steps);
        }

if (go==error){return error ;}
check=(go==moved?(turn==SHTRODEL?(shtail++):(ptail++)):check);
turn=turn==SHTRODEL?PERCENT:SHTRODEL;
    }
                       return (!error);
}
//////////////////////////////////////
void print_start_turn(char board[N][N], int size,int turn)
{
            print_board(board,size);
        print_player_message(turn);
}
//////////////////////////////////////
/*the function checks if the player ate
the food or not and changes values accordingly */
int ate(int i,int j,int row,int cul,char board[N][N],char head,int size)
{
    char  body = get_body(head); int check=0;
    if(proper_input(i,j,row,cul,food_place,board))
    {
        board[i][j]=body;
        board[i+row][j+cul]=head;

       check =get_food_location(board, size);
       return check!=error?(check==full?full:eaten):error;

    }
    return 0 ;

}
//////////////////////////////////////
/*this function checks if the player made a proper move  */
int can_move(int i,int j,int row,int cul,char board[N][N],char head)
{
    char body=get_body(head);
    char space=' ';
    if(proper_input(i,j,row,cul,space,board))
    {
        board[i+row][j+cul]=head;
        board[i][j]=body;
        return 1;

    }
    return 0;
}
////////////////////////////////////////////////
/*this function returns the player's turn according to his head */
int get_turn(char head)
{
    if(head==PERCENT_HEAD)
    {
        return PERCENT;
    }
    {
        return SHTRODEL;
    }
}
//////////////////////////////////////////////////
/*this function returns the player's body according to his head */
char get_body(char head)
{

    if(head==PERCENT_HEAD)
    {
        return PERCENT_body;
    }
    {
        return SHTRODEL_body;
    }

}
///////////////////////////////////////////////
/*this function gets the updated directions and food place and moves
 the snakes by sending the collected values
  to function that returns the result*/
int snake_moved(char head,char board[N][N],int step,int maxstep,int size)
{
    int direction = get_direction();  int result =0;
    if(direction==error){return error;}
    int add_rows = where_to_add_rows(direction);
    int add_culumns = where_to_add_culumns(direction);

    for(int i=0 ; i<size ; i++)
        for(int j=0 ; j<size; j++)
        {
            if (board[i][j]== head)
            {
           result = is_properly_moved (i,j,add_rows,
           add_culumns,board,size,step ,maxstep,head);
           return result;
           }

        }

    return 0;

}
//////////////////////////////////////
/*this function gets the directions and food place and moves
 the snakes accordingly ' the function at the end returns the
 result if the snake moved properly or if after
  moving the board became full or illegal move  */

int is_properly_moved (int i,int j,int add_rows,
  int add_culumns,char board[N][N],int size,
  int step , int maxstep,char head)
  {
      int check=0;
            {
    check=ate(i,j,add_rows,add_culumns,board,head,size);
    if(check!=0){return check!=error?(check==full?full:eaten):error;}

    if (step==maxstep){ print_finsih_msg(get_turn(head),!ILLEGAL_MOVE);
                    return 0 ;
                                  }
    if(can_move(i,j,add_rows,add_culumns,board,head)){  return moved; }


    {print_finsih_msg(get_turn(head),ILLEGAL_MOVE);}
                 return 0;

            }
  }

//////////////////////////////////////
/*the function checks where the player
 should move in the rows according to the direction*/
int where_to_add_rows(int direction)
{
    if (direction==down)
    {
        return 1;
    }
    if (direction==up)
    {
        return -1;
    }

    {
        return 0;
    }

}
//////////////////////////////////////
/*the function checks where the player
 should move in the culumns according to the direction*/
int where_to_add_culumns(int direction)
{
    if (direction==right)
    {
        return 1;
    }
    if (direction==left)
    {
        return -1;
    }

    {
        return 0;
    }

}
//////////////////////////////////////
/*the function checks if the place the head of the
 snake is moving to is empty or
  within the limits of the board  */
int proper_input(int i,int j,int row,int cul,char place,char board[N][N])
{
    if(i+row<N&&i+row>=0)
        if( j+cul<N&&j+cul>=0)
            if((board[i+row][j+cul]==place))
                return 1 ;

    return 0 ;
}
/////////////////////////////////////

void print_introduction_msg()
{
    printf("Welcome to multi-snake  game, The game have 2 player Percent and Shtrodel.\n"
           "The game starts with PERCENT player.\n");
    printf("You have to choose 4 moves :Up,Down,Right and Left "
           "The Percent player is the snake that starts in (0,0)\n\n");
}

void print_board_size_msg()
{
    printf("Insert board size between 4 and 25:\n");
}

void print_steps_without_food()
{
    printf("Insert the maximum number of steps the snake can do without food:\n");
}

void print_food_location_msg()
{
    printf("in order to continue the game please Insert a row then column numbers to place the next food: \n");
}

void help_print_board(int size)
{
    int i=0;
    printf("%c",BOARD_ELEMENT);
    for(i=0; i<size; i++)
    {
        printf("%c",BOARD_ELEMENT);
    }
    printf("%c\n",BOARD_ELEMENT);
}

void print_board(char board[N][N], int size)
{
    int i=0,j=0;
    help_print_board(size);
    for(i=0; i<size; i++)
    {
        printf("%c",BOARD_ELEMENT);
        for(j=0; j<size; j++)
        {
            printf("%c",board[i][j]);
        }
        printf("%c\n",BOARD_ELEMENT);
    }
    help_print_board(size);
    printf("\n\n");
}

void print_player_message(int player )
{
    printf("******* Turn of %c ***********\n",player==PERCENT?PERCENT_HEAD:SHTRODEL_HEAD);
}

void print_insert_direction()
{
    printf("Enter the direction to move your snake: (2-down,4-left,6-right,8-up):\n");
}

void print_finsih_msg(int player,int finish_reason)
{
    printf("\nThe game finish and the winner is:\n");
    printf("*** %s player ****.\n",player==PERCENT?"SHTRODEL":"PERCENT");
    printf("The reason is %s of the ",finish_reason==ILLEGAL_MOVE? "Illegal move" : "Snake die");
    printf("%s player.\n\n",player==PERCENT?"PERCENT":"SHTRODEL");
}

void print_invalid_input()
{
    printf("Please re-Inset valid input:\n");
}

void print_full_board()
{
    printf("Full Board. The game over with no Winner.\n");
}
void print_error_input()
{
    printf("Error with the input.\n");
}



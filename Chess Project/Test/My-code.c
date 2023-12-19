#include <stdio.h>
#include <math.h>
int turn=0;
int board[8][8]={500,300,310,900,10000,310,300,500,
                  100,100,100,100,100,100,100,100,
                   0,  0,  0,  0,  0,  0,  0,  0,
                   0,  0,  0,  0,  0,  0,  0,  0,
                   0,  0,  0,  0,  0,  0,  0,  0,
                   0,  0,  0,  0,  0,  0,  0,  0,
                  101,101,101,101,101,101,101,101,
                 501,301,311,901,10001,311,301,501};
int testboard[8][8]={500,300,310,900,10000,310,300,500,
                      100,100,100,100,100,100,100,100,
                       0,  0,  0,  0,  0,  0,  0,  0,
                       0,  0,  0,  0,  0,  0,  0,  0,
                       0,  0,  0,  0,  0,  0,  0,  0,
                       0,  0,  0,  0,  0,  0,  0,  0,
                      101,101,101,101,101,101,101,101,
                     501,301,311,901,10001,311,301,501};
int castle[2][3]={0,0,0,
                  0,0,0};
int undocastle[2][3]={0,0,0,
                      0,0,0};;
int enpassant[2][8]={0,0,0,0,0,0,0,0,
                     0,0,0,0,0,0,0,0};
int undoenpassant[2][8]={0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0};;
int king[2]={40,47};
int checkmate=0;
void showboard()
{
    for(int i=16;i>=0;i--)
    {
        if(i%2==1) printf("%d  ",(i+1)/2);
        else printf("   ");
        for(int j=0;j<8;j++)
        {
            if(i%2==0&&i>0)
            {
                printf("+----+----+----+----+----+----+----+----+");
                break;
            }
            else if(i==0)
            {
                printf("+----+----+----+----+----+----+----+----+\n     1    2    3    4    5    6    7    8");
                break;
            }
            else
            {   
                if(board[(i-1)/2][j]==10000)printf("| wk ");
                else if(board[(i-1)/2][j]==900)printf("| wq ");
                else if(board[(i-1)/2][j]==500)printf("| wr ");
                else if(board[(i-1)/2][j]==310)printf("| wb ");
                else if(board[(i-1)/2][j]==300)printf("| wn ");
                else if(board[(i-1)/2][j]==100)printf("| wp ");
                else if(board[(i-1)/2][j]==10001)printf("| bk ");
                else if(board[(i-1)/2][j]==901)printf("| bq ");
                else if(board[(i-1)/2][j]==501)printf("| br ");
                else if(board[(i-1)/2][j]==311)printf("| bb ");
                else if(board[(i-1)/2][j]==301)printf("| bn ");
                else if(board[(i-1)/2][j]==101)printf("| bp ");
                else printf("|    ");
            }
        }
        if(i%2==0) printf("\n");
        else printf("|\n");
    }
}
void promote(int num)
{
    int x;
    printf("Choose a piece to promote to:\n1. Queen\n2. Rook\n3. Bishop\n4. Knight\n");
    promotion:
    scanf("%d",&x);
    switch(x)
    {
        case 1: board[num%10][num/10]+=800; break;
        case 2: board[num%10][num/10]+=400; break;
        case 3: board[num%10][num/10]+=210; break;
        case 4: board[num%10][num/10]+=200; break;
        default: printf("Invalid input! Enter a number between 1 to 4: "); goto promotion;
    }
}
int move(int from,int to)
{
    int count=0,pvalue=(board[from%10][from/10])/10;                                    //pvalue is value of a piece
    if((board[to%10][to/10]==0)||(board[to%10][to/10]+board[from%10][from/10])%2==1)    //checks if moving position is empty or an enemy piece
    {
        switch(pvalue)                                                                  //check what piece you selected
        {
            case 1000: 
                        if((abs((from/10)-(to/10))==0||abs((from/10)-(to/10))==1)&&(abs((from%10)-(to%10))==0||abs((from%10)-(to%10))==1))
                        {                                                               //if diff of x is 0 or 1 and if diff of y is 0 or 1
                            count=1;
                            if((board[from%10][from/10]%2)==0) king[0]=to;
                            else king[1]=to;
                        }
                        else if(from==40&&castle[0][1]==0&&board[from%10][from/10]%2==0)        //castle w king
                        {
                            if(to==20)                                                          //q side
                            {
                                if(board[0][1]==0&&board[0][2]==0&&board[0][3]==0&&castle[0][0]==0)
                                count=1;
                                castle[0][0]=2;
                            }
                            else if(to==60)                                                     //k side
                            {
                                if(board[0][5]==0&&board[0][6]==0&&castle[0][2]==0)
                                count=1;
                                castle[0][2]=2;
                            }
                            else count=0;
                            if(count==1) king[0]=to;
                        }
                        else if(from==47&&castle[1][1]==0&&board[from%10][from/10]%2==1)        //castle b king
                        {
                            if(to==27)                                                          //q side
                            {
                                if(board[7][1]==0&&board[7][2]==0&&board[7][3]==0&&castle[1][0]==0)
                                count=1;
                                castle[1][0]=2;
                            }
                            else if(to==67)                                                     //k side
                            {
                                if(board[7][5]==0&&board[7][6]==0&&castle[1][2]==0)
                                count=1;
                                castle[1][2]=2;
                            }
                            else count=0;
                            if(count==1) king[1]=to;
                        }
                        else count=0;
                        if(count==1&&board[from%10][from/10]%2==0) castle[0][1]=1;      //Castle no longer available for w king
                        else if(count==1) castle[1][1]=1;                               //castle no longer available for b king
                        break;
            case 90:    
                        if(((to%10)-(from%10))==((to/10)-(from/10)))                    //slope  1
                        {
                            count=1;
                            if((to/10)>(from/10))
                            {
                                for(int i=(from/10)+1,j=(from%10)+1;i<to/10;i++,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to/10)<(from/10))
                            {
                                for(int i=(to/10)+1,j=(to%10)+1;i<from/10;i++,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else if(((to%10)-(from%10))==-((to/10)-(from/10)))              //slope -1
                        {
                            count=1;
                            if((to%10)>(from%10))
                            {
                                for(int i=(from/10)-1,j=(from%10)+1;i>to/10;i--,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to%10)<(from%10))
                            {
                                for(int i=(to/10)-1,j=(to%10)+1;i>from/10;i--,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else if(from/10==to/10)                                         //vertical movement
                        {
                            count=1;
                            if((to%10)>(from%10))
                            {
                                for(int i=from%10+1;to%10-i-1>=0;i++)
                                {
                                    if(board[i][from/10]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to%10)<(from%10))
                            {
                                for(int i=to%10+1;from%10-i-1>=0;i++)
                                {
                                    if(board[i][from/10]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else if(from%10==to%10)                                         //horizontal movement
                        {
                            count=1;
                            if((to/10)>(from/10))
                            {
                                for(int i=from/10+1;to/10-i-1>=0;i++)
                                {
                                    if(board[from%10][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to/10)<(from/10))
                            {
                                for(int i=to/10+1;from/10-i-1>=0;i++)
                                {
                                    if(board[from%10][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else count=0;
                        break;
            case 50:    
                        if(from/10==to/10)                                              //vertical movement
                        {
                            count=1;
                            if((to%10)>(from%10))
                            {
                                for(int i=from%10+1;to%10-i-1>=0;i++)
                                {
                                    if(board[i][from/10]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to%10)<(from%10))
                            {
                                for(int i=to%10+1;from%10-i-1>=0;i++)
                                {
                                    if(board[i][from/10]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else if(from%10==to%10)                                         //horizontal movement
                        {
                            count=1;
                            if((to/10)>(from/10))
                            {
                                for(int i=from/10+1;to/10-i-1>=0;i++)
                                {
                                    if(board[from%10][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to/10)<(from/10))
                            {
                                for(int i=to/10+1;from/10-i-1>=0;i++)
                                {
                                    if(board[from%10][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else count=0;
                        if(count==1)
                        {
                                 if((from%10==0)&&(from/10==0)) castle[0][0]=1;        //castle not possible with w rook q side
                            else if((from%10==0)&&(from/10==7)) castle[0][2]=1;        //castle not possible with w rook k side
                            else if((from%10==7)&&(from/10==0)) castle[1][0]=1;        //castle not possible with b rook q side
                            else if((from%10==7)&&(from/10==7)) castle[1][2]=1;        //castle not possible with b rook k side
                        }
                        break;
            case 31:    
                        if(((to%10)-(from%10))==((to/10)-(from/10)))                    //slope  1
                        {
                            count=1;
                            if((to/10)>(from/10))
                            {
                                for(int i=(from/10)+1,j=(from%10)+1;i<to/10;i++,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to/10)<(from/10))
                            {
                                for(int i=(to/10)+1,j=(to%10)+1;i<from/10;i++,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else if(((to%10)-(from%10))==-((to/10)-(from/10)))              //slope -1
                        {
                            count=1;
                            if((to%10)>(from%10))
                            {
                                for(int i=(from/10)-1,j=(from%10)+1;i>to/10;i--,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else if((to%10)<(from%10))
                            {
                                for(int i=(to/10)-1,j=(to%10)+1;i>from/10;i--,j++)
                                {
                                    if(board[j][i]!=0) return 0;
                                    else count=1;
                                }
                            }
                            else count=0;
                        }
                        else count=0;
                        break;
            case 30:    
                        if((to%10)!=(from%10)&&(to/10)!=(from/10)&&(abs((to%10)-(from%10))+abs((to/10)-(from/10)))==3) count=1;
                        else count=0;                                                   //y1,y1 not same and x1,x2 not same
                        break;                                                          // and sum of (diff of x) and (diff of y) coords= 3
            case 10:    
                        if(board[from%10][from/10]==100&&from%10==1)
                        {                                                                                                   //white
                            if((to%10==2)&&(from/10==to/10)&&board[to%10][to/10]==0) count=1;                               //first foreward single
                            else if((to%10==3)&&(from/10==to/10)&&board[2][to/10]==0&&board[3][to/10]==0)                   //foreward double
                            {
                                count=1;
                                enpassant[0][to/10]=1;
                            }
                            else if((to%10==2)&&abs(from/10-to/10)==1&&board[to%10][to/10]!=0) count=1;                     //first capture
                            else count=0;
                        }
                        else if(board[from%10][from/10]==100&&(to%10)-(from%10)==1)
                        {
                            if(board[to%10][to/10]==0&&(from/10)==(to/10)) count=1;                                         //foreward
                            else if(abs(from/10-to/10)==1&&board[to%10][to/10]!=0) count=1;                                 //capture
                            else if(enpassant[1][to/10]==2&&(from%10)==4&&(to%10)==5&&abs((from/10)-(to/10))==1&&board[5][to/10]==0&&board[4][to/10]==101)
                            {                                                                                               //en passant
                                count=1;
                                enpassant[1][to/10]=4;
                            }
                            else count=0;
                        }
                        else if(board[from%10][from/10]==101&&from%10==6)
                        {                                                                                                   //black
                            if((to%10==5)&&(from/10==to/10)&&board[to%10][to/10]==0) count=1;                               //first foreward single
                            else if((to%10==4)&&(from/10==to/10)&&board[5][to/10]==0&&board[4][to/10]==0)                   //foreward double
                            {
                                count=1;
                                enpassant[1][to/10]=1;
                            }
                            else if((to%10==5)&&abs(from/10-to/10)==1&&board[to%10][to/10]!=0) count=1;                     //first capture
                            else count=0;
                        }
                        else if(board[from%10][from/10]==101&&(to%10)-(from%10)==-1)
                        {
                            if(board[to%10][to/10]==0&&(from/10)==(to/10)) count=1;                                         //foreward
                            else if(abs(from/10-to/10)==1&&board[to%10][to/10]!=0) count=1;                                 //capture
                            else if(enpassant[0][to/10]==2&&(from%10)==3&&(to%10)==2&&abs((from/10)-(to/10))==1&&board[2][to/10]==0&&board[3][to/10]==100)
                            {                                                                                               //en passant
                                count=1;
                                enpassant[0][to/10]=4;
                            }
                            else count=0;
                        }
                        else count=0;
                        if((count==1)&&(to%10==0||to%10==7)) promote(from);                                                 //promotion
                        break;
            default:    count=0;
                        break;
        }
        if(count==1) return 1;
        else return 0;
    }
    else return 0;
}
int check(int to)
{
    int from,chk=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            from=10*j+i;
            if(move(from,to)==1)
            chk=1;
        }
    }
    return chk;
}
//board[y][x] from%10=y1 from/10=x1  move(x1y1,x2y1)  i=row (y), j=column (x)
int main()
{
    showboard();
    int from,to;
    printf("\nInput syntax: x1y1,x2y2\nHere x1y1 are initial co-ordinates and x2y2 are final co-ordinates\n\n");
    for(;;)
    {
        if(check(king[1])==1)
        {
            checkmate=1;
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    int a=10*j+i;
                    if(board[i][j]%2==1)
                    {
                        for(int k=0;k<8;k++)
                        {
                            for(int l=0;l<8;l++)
                            {
                                int b=10*l+k;
                                if(move(a,b)==1)
                                {
                                    move(a,b);
                                    board[b%10][b/10]=board[a%10][a/10];                                        //move
                                    board[a%10][a/10]=0;                                                        //clear
                                    if(castle[0][0]==2)
                                    {
                                        castle[0][0]=1;
                                        board[0][3]=board[0][0];
                                        board[0][0]=0;
                                    }
                                    else if(castle[0][2]==2)
                                    {
                                        castle[0][2]=1;
                                        board[0][5]=board[0][7];
                                        board[0][7]=0;
                                    }
                                    else if(castle[1][0]==2)
                                    {
                                        castle[1][0]=1;
                                        board[7][3]=board[7][0];
                                        board[7][0]=0;
                                    }
                                    else if(castle[1][2]==2)
                                    {
                                        castle[1][2]=1;
                                        board[7][5]=board[7][7];
                                        board[7][7]=0;
                                    }
                                    for(int i=0;i<2;i++)
                                    {
                                        for(int j=0;j<8;j++)
                                        {
                                            if(enpassant[i][j]>0&&enpassant[i][j]<3)
                                            enpassant[i][j]++;
                                        }
                                    }
                                    for(int j=0;j<8;j++)
                                    {
                                        if(enpassant[1][to/10]==4)
                                        {
                                            enpassant[1][to/10]=3;
                                            board[4][to/10]=0;
                                        }
                                        if(enpassant[0][to/10]==4)
                                        {
                                            enpassant[0][to/10]=3;
                                            board[3][to/10]=0;
                                        }
                                    }
                                }
                                if(check(king[1])==0)
                                {
                                    printf("(%d,%d)",a,b);
                                    checkmate=0;
                                }
                                for(int i=0;i<8;i++)
                                {
                                    for(int j=0;j<8;j++)
                                    {
                                        board[i][j]=testboard[i][j];
                                    }
                                }
                                for(int i=0;i<2;i++)
                                {
                                    for(int j=0;j<3;j++)
                                    {
                                        castle[i][j]=undocastle[i][j];
                                    }
                                }
                                for(int i=0;i<2;i++)
                                {
                                    for(int j=0;j<8;j++)
                                    {
                                        enpassant[i][j]=undoenpassant[i][j];
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(checkmate==1)
            {
                printf("Checkmate! White wins!");
                return 0;
            }
            else if(checkmate==0)
            printf("Black's King in Check!\n");
        }
        else if(check(king[0])==1)
        {
            checkmate=2;
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    int a=10*j+i;
                    if(board[i][j]%2==0)
                    {
                        for(int k=0;k<8;k++)
                        {
                            for(int l=0;l<8;l++)
                            {
                                int b=10*l+k;
                                if(move(a,b)==1)
                                {
                                    move(a,b);
                                    board[b%10][b/10]=board[a%10][a/10];                                        //move
                                    board[a%10][a/10]=0;                                                        //clear
                                    if(castle[0][0]==2)
                                    {
                                        castle[0][0]=1;
                                        board[0][3]=board[0][0];
                                        board[0][0]=0;
                                    }
                                    else if(castle[0][2]==2)
                                    {
                                        castle[0][2]=1;
                                        board[0][5]=board[0][7];
                                        board[0][7]=0;
                                    }
                                    else if(castle[1][0]==2)
                                    {
                                        castle[1][0]=1;
                                        board[7][3]=board[7][0];
                                        board[7][0]=0;
                                    }
                                    else if(castle[1][2]==2)
                                    {
                                        castle[1][2]=1;
                                        board[7][5]=board[7][7];
                                        board[7][7]=0;
                                    }
                                    for(int i=0;i<2;i++)
                                    {
                                        for(int j=0;j<8;j++)
                                        {
                                            if(enpassant[i][j]>0&&enpassant[i][j]<3)
                                            enpassant[i][j]++;
                                        }
                                    }
                                    for(int j=0;j<8;j++)
                                    {
                                        if(enpassant[1][to/10]==4)
                                        {
                                            enpassant[1][to/10]=3;
                                            board[4][to/10]=0;
                                        }
                                        if(enpassant[0][to/10]==4)
                                        {
                                            enpassant[0][to/10]=3;
                                            board[3][to/10]=0;
                                        }
                                    }
                                }
                                if(check(king[0])==0)
                                {
                                    checkmate=0;
                                }
                                for(int i=0;i<8;i++)
                                {
                                    for(int j=0;j<8;j++)
                                    {
                                        board[i][j]=testboard[i][j];
                                    }
                                }
                                for(int i=0;i<2;i++)
                                {
                                    for(int j=0;j<3;j++)
                                    {
                                        castle[i][j]=undocastle[i][j];
                                    }
                                }
                                for(int i=0;i<2;i++)
                                {
                                    for(int j=0;j<8;j++)
                                    {
                                        enpassant[i][j]=undoenpassant[i][j];
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if(checkmate==2)
            {
                printf("Checkmate! Black wins!");
                return 0;
            }
            else if(checkmate==0)
            printf("White's King in Check!\n");
        }
        illegalmove:
        wrongpiece:
        scanf("%d,%d",&from,&to);
        from-=11; to-=11;
        if((board[from%10][from/10])%2!=turn)
        {
            if(turn==0)
            printf("Its white's turn to move! Move a white piece.\n");
            else printf("Its black's turn to move. Move a black piece.\n");
            goto wrongpiece;
        }
        if(move(from,to)==0)
        {
            printf("Illegal Move! Try again.\n");
            if(castle[0][0]==2)
            {
                castle[0][0]=0;
            }
            else if(castle[0][2]==2)
            {
                castle[0][2]=0;
            }
            else if(castle[1][0]==2)
            {
                castle[1][0]=0;
            }
            else if(castle[1][2]==2)
            {
                castle[1][2]=0;
            }
            goto illegalmove;
        }
        else
        {
            move(from,to);
            board[to%10][to/10]=board[from%10][from/10];                                //move
            board[from%10][from/10]=0;                                                  //clear
            if(castle[0][0]==2)
            {
                castle[0][0]=1;
                board[0][3]=board[0][0];
                board[0][0]=0;
            }
            else if(castle[0][2]==2)
            {
                castle[0][2]=1;
                board[0][5]=board[0][7];
                board[0][7]=0;
            }
            else if(castle[1][0]==2)
            {
                castle[1][0]=1;
                board[7][3]=board[7][0];
                board[7][0]=0;
            }
            else if(castle[1][2]==2)
            {
                castle[1][2]=1;
                board[7][5]=board[7][7];
                board[7][7]=0;
            }
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<8;j++)
                {
                    if(enpassant[i][j]>0&&enpassant[i][j]<3)
                    enpassant[i][j]++;
                }
            }
            for(int j=0;j<8;j++)
            {
                if(enpassant[1][to/10]==4)
                {
                    enpassant[1][to/10]=3;
                    board[4][to/10]=0;
                }
                if(enpassant[0][to/10]==4)
                {
                    enpassant[0][to/10]=3;
                    board[3][to/10]=0;
                }
            }
            turn++;
            turn%=2;
        }
        if((turn==1 && check(king[0])==1) || (turn==0 && check(king[1])==1))
        {
            printf("Illegal Move. \n");
            turn+=1;
            turn%=2;
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    board[i][j]=testboard[i][j];
                }
            }
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<3;j++)
                {
                    castle[i][j]=undocastle[i][j];
                }
            }
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<8;j++)
                {
                    enpassant[i][j]=undoenpassant[i][j];
                }
            }
        }
        else
        {
            for(int i=0;i<8;i++)
            {
                for(int j=0;j<8;j++)
                {
                    testboard[i][j]=board[i][j];
                }
            }
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<3;j++)
                {
                    undocastle[i][j]=castle[i][j];
                }
            }
            for(int i=0;i<2;i++)
            {
                for(int j=0;j<8;j++)
                {
                    undoenpassant[i][j]=enpassant[i][j];
                }
            }
        }
        printf("\n");
        showboard();
    }
}
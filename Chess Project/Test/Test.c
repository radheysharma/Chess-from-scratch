#include <stdio.h>
#include <math.h>
int turn;
int board[8][8];
int testboard[8][8];
int castle[2][3];
int undocastle[2][3];
int enpassant[2][8];
int undoenpassant[2][8];
int king[2];
int undoking[2];
int checkmate;
int move50;
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
void goback()
{
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
    king[0]=undoking[0];
    king[1]=undoking[1];
}
int check(int to)
{
    int from,chk=0;
    for(int m=0;m<8;m++)
    {
        for(int n=0;n<8;n++)
        {
            from=10*n+m;
            if(move(from,to)==1)
            chk=1;
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
            king[0]=undoking[0];
            king[1]=undoking[1];
        }
    }
    return chk;
}
//board[y][x] from%10=y1 from/10=x1  move(x1y1,x2y1)  i=row (y), j=column (x)
int chess(int from,int to)
{
    int x;
    from-=11; to-=11;
    if((board[from%10][from/10])%2!=turn)
    {
            if(turn==0)
            printf("Its white's turn to move! Move a white piece.\n");
            else printf("Its black's turn to move. Move a black piece.\n");
            return 0;
    }
    x=move(from,to);
    if(x==0)
    {
            printf("Illegal Move! Try again.\n");
            goback();
            move50--;
            return 0;
    }
    else
    {
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
            goback();
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
            undoking[0]=king[0];
            undoking[1]=king[1];
    }
    printf("\n");
    showboard();
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
                                x=move(a,b);
                                if(x==1)
                                {
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
                                    king[1]=b;
                                }
                                if(check(king[1])==0)
                                {
                                    checkmate=0;
                                }
                                goback();
                            }
                        }
                    }
                }
            }
        if(check(king[1])==1)
        {
            if(checkmate==1)
            {
                printf("Checkmate! White wins!");
                return 1;
            }
            else if(checkmate==0)
            printf("Black's King in Check!\n");
        }
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
                                x=move(a,b);
                                if(x==1)
                                {
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
                                    king[0]=b;
                                }
                                if(check(king[0])==0)
                                {
                                    checkmate=0;
                                }
                                goback();
                            }
                        }
                    }
                }
            }
        if(check(king[0])==1)
        {
            if(checkmate==2)
            {
                printf("Checkmate! Black wins!");
                return 1;
            }
            else if(checkmate==0)
            printf("White's King in Check!\n");
        }
    }
    return 0;
}
int main()
{
    int n,side,from,to,x,game;
    play:
    turn=0;
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            enpassant[i][j]=0;
            undoenpassant[i][j]=0;
            if(j<6)
            {
                castle[i][j]=0;
                undocastle[i][j]=0;
            }
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            board[i][j]=0;
            if(i==1||i==6) board[i][j]=100;
            if(i==0||i==7)
            {
                if(j==0||j==7) board[i][j]=500;
                if(j==1||j==6) board[i][j]=300;
                if(j==2||j==5) board[i][j]=310;
                if(j==3) board[i][j]=900;
                if(j==4) board[i][j]=10000;
            }
            if(i==6||i==7) board[i][j]+=1;
            testboard[i][j]=board[i][j];
        }
    }
    king[0]=40;
    king[1]=47;
    undoking[0]=40;
    undoking[1]=47;
    checkmate=0;
    move50=0;
    x=0;
    printf("\n\nSelect Gamemode:\n\n1. Singleplayer\t2. Multiplayer\n");
    gamemode:
    scanf("%d",&n);
    if(n==1)
    {
        goto play;
        printf("Choose your side:\n1.White 2.Black\n");
        color:
        scanf("%d",&side);
        if(side==1||side==2)
        {
            side-=1;
            for(;;)
            {
                showboard();
                printf("\nInput syntax: x1y1 x2y2\nHere x1y1 are initial co-ordinates and x2y2 are final co-ordinates\n\n");
                for(;;)
                {
                    if(turn==side)
                    {
                        scanf("%d%*c",&from);
                        failsafe1:
                        if(from==0)
                        {
                            printf("Are you sure you want to quit?\n1.Yes 2.No\n");
                            scanf("%d",&from);
                            if(from==1) goto newgame;
                            else printf("Continuing game...\n");
                            goto failsafe1;
                        }
                        scanf("%d",&to);
                        if(from%10>0&&from%10<9&&from/10>0&&from/10<9&&to%10>0&&to%10<9&&to/10>0&&to/10<9)
                        {
                            x=chess(from,to);
                            if(x==1)
                            {
                                goto newgame;
                            }
                        }
                        else printf("Invalid move.\n");
                    }
                    else
                    {
                      /*x=chess();
                        if(x==1)
                        {
                            printf("\nDo you want to play again?\n1.Yes 2.No\n");
                            scanf("%d",&game);
                            if(game==1) goto play;
                            else return 0;
                        }*/
                    }
                }
            }
        }
        else
        {
            printf("Enter 1 or 2\n");
            goto color;
        }
    }
    else if(n==2)
    {
        showboard();
        printf("\nInput syntax: x1y1 x2y2\nHere x1y1 are initial co-ordinates and x2y2 are final co-ordinates\n\n");
        for(;;)
        {
            failsafe2:
            scanf("%d%*c",&from);
            if(from==0)
            {
                printf("Are you sure you want to quit?\n1.Yes 2.No\n");
                scanf("%d",&from);
                if(from==1) goto newgame;
                else printf("Continuing game...\n");
                goto failsafe2;
            }
            scanf("%d",&to);
            if(from%10>0&&from%10<9&&from/10>0&&from/10<9&&to%10>0&&to%10<9&&to/10>0&&to/10<9)
            {
                if((board[to%10-1][to/10-1]+board[from%10-1][from/10-1])%2==1&&board[to%10-1][to/10-1]>0&&board[from%10-1][from/10-1]>0) move50+=100;
                if((board[from%10-1][from/10-1]/10)==10) move50+=100;
                x=chess(from,to);
                if(x==1)
                {
                    newgame:
                    printf("\nDo you want to play again?\n1.Yes 2.No\n");
                    scanf("%d",&game);
                    if(game==1) goto play;
                    else return 0;
                }
                if(x==0)
                {
                    move50++;
                    if(move50>50) move50=0;
                    if(move50==50)
                    {
                        printf("It's a Draw.(50-move rule)\n");
                        goto newgame;
                    }
                }
            }
            else printf("Invalid move.\n");
        }
    }
    else
    {
        printf("Enter 1 or 2\n");
        goto gamemode;
    }
}
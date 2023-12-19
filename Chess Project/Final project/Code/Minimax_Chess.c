#include <stdio.h>
#include <math.h>
int board[8][8],undoboard[8][8],castle[2][2],undocastle[2][2],king[2],undoking[2],enpassant[2][8],undoenpassant[2][8],posmov[512][2];
int turn,nummov,prmte,undoprmte,stalemate,move50,undomove50,side;
void reset()
{
    for(int i=7;i>-1;i--)
    {
        for(int j=0;j<8;j++)
        {
            if(i==6||i==1) board[i][j]=100;
            else if(i==7||i==0)
            {
                if(j==0||j==7) board[i][j]=500;
                else if(j==1||j==6) board[i][j]=300;
                else if(j==2||j==5) board[i][j]=310;
                else if(j==3) board[i][j]=900;
                else board[i][j]=10000;
            }
            else board[i][j]=0;
            if(i==6||i==7) board[i][j]++;
            undoboard[i][j]=board[i][j];
        }
    }
    castle[0][0]=0;  //wk qs
    castle[0][1]=0;  //wk ks
    castle[1][0]=0;  //bk qs
    castle[1][1]=0;  //bk ks
    undocastle[0][0]=0;
    undocastle[0][1]=0;
    undocastle[1][0]=0;
    undocastle[1][1]=0;
    king[0]=40;
    king[1]=47;
    undoking[0]=40;
    undoking[1]=47;
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            enpassant[i][j]=0;
            undoenpassant[i][j]=0;
        }
    }
    turn=0;
    prmte=-1;
    undoprmte=-1;
    stalemate=0;
    move50=0;
    undomove50=0;
    side=0;
    nummov=0;
}
void undo()
{
    castle[0][0]=undocastle[0][0];
    castle[0][1]=undocastle[0][1];
    castle[1][0]=undocastle[1][0];
    castle[1][1]=undocastle[1][1];
    king[0]=undoking[0];
    king[1]=undoking[1];
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            enpassant[i][j]=undoenpassant[i][j];
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            board[i][j]=undoboard[i][j];
        }
    }
    prmte=undoprmte;
    move50=undomove50;
}
void set()
{
    undocastle[0][0]=castle[0][0];
    undocastle[0][1]=castle[0][1];
    undocastle[1][0]=castle[1][0];
    undocastle[1][1]=castle[1][1];
    undoking[0]=king[0];
    undoking[1]=king[1];
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            undoenpassant[i][j]=enpassant[i][j];
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            undoboard[i][j]=board[i][j];
        }
    }
    undoprmte=prmte;
    undomove50=move50;
}
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
int check(int to)
{
    int chk=0,p=board[to%10][to/10];
    if(p>0)
    {
        p%=10;
        for(int i=1+to%10,j=1+to/10;i>=0&&i<8&&j>=0&&j<8;i++,j++)
        {
            if((board[i][j]==901-p)||(board[i][j]==311-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=1+to%10,j=-1+to/10;i>=0&&i<8&&j>=0&&j<8;i++,j--)
        {
            if((board[i][j]==901-p)||(board[i][j]==311-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=1+to/10;i>=0&&i<8&&j>=0&&j<8;i--,j++)
        {
            if((board[i][j]==901-p)||(board[i][j]==311-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=-1+to/10;i>=0&&i<8&&j>=0&&j<8;i--,j--)
        {
            if((board[i][j]==901-p)||(board[i][j]==311-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=1+to%10,j=to/10;i>=0&&i<8;i++)
        {
            if((board[i][j]==901-p)||(board[i][j]==501-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=to/10;i>=0&&i<8;i--)
        {
            if((board[i][j]==901-p)||(board[i][j]==501-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=to%10,j=1+to/10;j>=0&&j<8;j++)
        {
            if((board[i][j]==901-p)||(board[i][j]==501-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=to%10,j=-1+to/10;j>=0&&j<8;j--)
        {
            if((board[i][j]==901-p)||(board[i][j]==501-p)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1;i<2;i++)
        {
            for(int j=-1;j<2;j++)
            {
                if(to%10+i<8&&to%10+i>=0&&to/10+j<8&&to/10+j>=0)
                {
                    if(board[to%10+i][to/10+j]==10001-p) chk=1;
                }
            }
        }
        if(p==0&&to%10+1>=0&&to/10+1>=0&&to%10+1<8&&to/10+1<8&&board[to%10+1][to/10+1]==101-p) chk=1;
        if(p==0&&to%10+1>=0&&to/10-1>=0&&to%10+1<8&&to/10-1<8&&board[to%10+1][to/10-1]==101-p) chk=1;
        if(p==1&&to%10-1>=0&&to/10+1>=0&&to%10-1<8&&to/10+1<8&&board[to%10-1][to/10+1]==101-p) chk=1;
        if(p==1&&to%10-1>=0&&to/10-1>=0&&to%10-1<8&&to/10-1<8&&board[to%10-1][to/10-1]==101-p) chk=1;
    }
    else
    {
       for(int i=1+to%10,j=1+to/10;i>=0&&i<8&&j>=0&&j<8;i++,j++)
        {
            if((board[i][j]==901-turn)||(board[i][j]==311-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=1+to%10,j=-1+to/10;i>=0&&i<8&&j>=0&&j<8;i++,j--)
        {
            if((board[i][j]==901-turn)||(board[i][j]==311-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=1+to/10;i>=0&&i<8&&j>=0&&j<8;i--,j++)
        {
            if((board[i][j]==901-turn)||(board[i][j]==311-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=-1+to/10;i>=0&&i<8&&j>=0&&j<8;i--,j--)
        {
            if((board[i][j]==901-turn)||(board[i][j]==311-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=1+to%10,j=to/10;i>=0&&i<8;i++)
        {
            if((board[i][j]==901-turn)||(board[i][j]==501-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1+to%10,j=to/10;i>=0&&i<8;i--)
        {
            if((board[i][j]==901-turn)||(board[i][j]==501-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=to%10,j=1+to/10;j>=0&&j<8;j++)
        {
            if((board[i][j]==901-turn)||(board[i][j]==501-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=to%10,j=-1+to/10;j>=0&&j<8;j--)
        {
            if((board[i][j]==901-turn)||(board[i][j]==501-turn)) chk=1;
            else if(board[i][j]>0) break;
        }
        for(int i=-1;i<2;i++)
        {
            for(int j=-1;j<2;j++)
            {
                if(to%10+i<8&&to%10+i>=0&&to/10+j<8&&to/10+j>=0)
                {
                    if(board[to%10+i][to/10+j]==10001-turn) chk=1;
                }
            }
        }
        if(turn==0&&to%10+1>=0&&to/10+1>=0&&to%10+1<8&&to/10+1<8&&board[to%10+1][to/10+1]==101-turn) chk=1;
        if(turn==0&&to%10+1>=0&&to/10-1>=0&&to%10+1<8&&to/10-1<8&&board[to%10+1][to/10-1]==101-turn) chk=1;
        if(turn==1&&to%10-1>=0&&to/10+1>=0&&to%10-1<8&&to/10+1<8&&board[to%10-1][to/10+1]==101-turn) chk=1;
        if(turn==1&&to%10-1>=0&&to/10-1>=0&&to%10-1<8&&to/10-1<8&&board[to%10-1][to/10-1]==101-turn) chk=1; 
    }
    return chk;
}
void move(int from,int to)
{
    if((board[from%10][from/10]/10==10)||((board[from%10][from/10]+board[to%10][to/10])%2==1&&(board[from%10][from/10]>0&&board[to%10][to/10]>0))) move50=0;
    else move50++;
    board[to%10][to/10]=board[from%10][from/10];
    board[from%10][from/10]=0;
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
            if(castle[i][j]==1)
            {
                castle[i][j]=-2;
                board[i*7][2*j+3]=500+i;
                board[7*i][7*j]=0;
            }
        }
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(enpassant[i][j]==-2)
            {
                printf("H");
                enpassant[i][j]=-1;
                board[i+3][to/10]=0;
            }
        }
    }
    if(board[to%10][to/10]==10000) king[0]=to;
    if(board[to%10][to/10]==10001) king[1]=to;
    if(board[to%10][to/10]==101&&to%10==0) prmte=to;
    if(board[to%10][to/10]==100&&to%10==7) prmte=to;
}
int islegal(int from, int to)
{
    int legal=1;
    move(from,to);
    if(board[to%10][to/10]<10000&&check(king[turn])==1)
    {
        legal=0;
    }
    else if(board[to%10][to/10]>9999&&check(to)==1)
    {
        legal=0;
    }
    undo();
    if(legal==0) return 0;
    legal=0;
    int pval=board[from%10][from/10];
    if((pval%10==turn)&&(from!=to)&&(pval!=0)&&((board[to%10][to/10]==0)||((pval+board[to%10][to/10])%2==1)))
    {
        switch (pval/10)
        {
            case 1000:
                        {
                            if(check(to)==0)
                            {
                                //Normal move
                                if(abs(from%10-to%10)<2&&abs(from/10-to/10)<2)
                                {
                                    castle[pval%10][0]=-1;
                                    castle[pval%10][1]=-1;
                                    legal=1;
                                }
                                //Castle qs
                                else if(castle[pval%10][0]==0&&((from==40)&&(to==20)||(from==47)&&(to==27))&&check(king[pval%10])==0&&check(king[pval%10]-10)==0&&check(king[pval%10]-20)==0)
                                {
                                    if(board[7*(pval%10)][3]==0&&board[7*(pval%10)][2]==0&&board[7*(pval%10)][1]==0&&board[7*(pval%10)][0]==(500+pval%10))
                                    {
                                        legal=1;
                                        castle[pval%10][0]=1;
                                    }
                                }
                                //Castle ks
                                else if(castle[pval%10][1]==0&&((from==40)&&(to==60)||(from==47)&&(to==67))&&check(king[pval%10])==0&&check(king[pval%10]+10)==0&&check(king[pval%10]+20)==0)
                                {
                                    if(board[7*(pval%10)][5]==0&&board[7*(pval%10)][6]==0&&board[7*(pval%10)][7]==(500+pval%10))
                                    {
                                        legal=1;
                                        castle[pval%10][1]=1;
                                    }
                                }
                            }
                            break;
                        }
            case   90:
                        {
                            if(from%10-to%10==from/10-to/10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+to%10,j=1+to/10;i<from%10;i++,j++)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+from%10,j=1+from/10;i<to%10;i++,j++)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                            }
                            else if(from%10-to%10==to/10-from/10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+from%10,j=-1+from/10;i<to%10;i++,j--)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+to%10,j=-1+to/10;i<from%10;i++,j--)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                            }
                            else if(from%10==to%10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+to/10;i<from/10;i++)
                                    {
                                        if(board[from%10][i]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+from/10;i<to/10;i++)
                                    {
                                        if(board[from%10][i]!=0) legal=0;
                                    }
                                }
                            }
                            else if(from/10==to/10)
                            {
                                legal=1;
                                if(from%10>to%10)
                                {
                                    for(int i=1+to%10;i<from%10;i++)
                                    {
                                        if(board[i][from/10]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+from%10;i<to%10;i++)
                                    {
                                        if(board[i][from/10]!=0) legal=0;
                                    }
                                }
                            }
                            break;
                        }
            case   50:
                        {
                            if(from%10==to%10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+to/10;i<from/10;i++)
                                    {
                                        if(board[from%10][i]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+from/10;i<to/10;i++)
                                    {
                                        if(board[from%10][i]!=0) legal=0;
                                    }
                                }
                            }
                            else if(from/10==to/10)
                            {
                                legal=1;
                                if(from%10>to%10)
                                {
                                    for(int i=1+to%10;i<from%10;i++)
                                    {
                                        if(board[i][from/10]!=0) legal=0;
                                    }
                                }
                            else
                                {
                                    for(int i=1+from%10;i<to%10;i++)
                                    {
                                        if(board[i][from/10]!=0) legal=0;
                                    }
                                }       
                            }
                            if(legal==1)
                            {
                                if(pval%10==0)
                                {
                                    if(from==00) castle[0][0]=-1;
                                    if(from==70) castle[0][1]=-1;
                                }
                                else
                                {
                                    if(from==07) castle[1][0]=-1;
                                    if(from==77) castle[1][1]=-1;
                                }
                            }
                            break;
                        }
            case   31:
                        {
                            if(from%10-to%10==from/10-to/10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+to%10,j=1+to/10;i<from%10;i++,j++)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                    }
                            else
                                {
                                    for(int i=1+from%10,j=1+from/10;i<to%10;i++,j++)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                            }
                            else if(from%10-to%10==to/10-from/10)
                            {
                                legal=1;
                                if(from/10>to/10)
                                {
                                    for(int i=1+from%10,j=-1+from/10;i<to%10;i++,j--)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                                else
                                {
                                    for(int i=1+to%10,j=-1+to/10;i<from%10;i++,j--)
                                    {
                                        if(board[i][j]!=0) legal=0;
                                    }
                                }
                            }
                            break;
                        }
            case   30:
                        {
                            if(((abs(from%10-to%10)+abs(from/10-to/10))==3)&&from%10!=to%10&&from/10!=to/10)
                            legal=1;
                            break;
                        }
            case   10:
                        {
                            //firstmove
                            if(from%10==1||from%10==6)
                            {
                                if(pval==101)
                                {
                                    if(from/10==to/10&&board[to%10][to/10]==0&&board[5][to/10]==0)
                                    {
                                        if(from%10-to%10==2)
                                        {
                                            enpassant[pval%10][from/10]=1;
                                            legal=1;
                                        }
                                        else if(from%10-to%10==1)
                                        {
                                            enpassant[pval%10][from/10]=-1;
                                            legal=1;
                                        }
                                        else legal=0;
                                    }
                                    else if((abs(from/10-to/10)==1)&&(from%10-to%10==1)&&board[to%10][to/10]>0&&board[to%10][to/10]%2==0)
                                    {
                                        enpassant[pval%10][from/10]=-1;
                                        legal=1;
                                    }
                                }
                                else if(pval==100)
                                {
                                    if(from/10==to/10&&board[to%10][to/10]==0&&board[2][to/10]==0)
                                    {
                                        if(to%10-from%10==2)
                                        {
                                            enpassant[pval%10][from/10]=1;
                                            legal=1;
                                        }
                                        else if(to%10-from%10==1)
                                        {
                                            enpassant[pval%10][from/10]=-1;
                                            legal=1;
                                        }
                                        else legal=0;
                                    }
                                    else if((abs(from/10-to/10)==1)&&(to%10-from%10==1)&&board[to%10][to/10]>0&&board[to%10][to/10]%2==1)
                                    {
                                        enpassant[pval%10][from/10]=-1;
                                        legal=1;
                                    }
                                }
                            }
                            else
                            {
                                if(pval==101)
                                {
                                    if(from/10==to/10&&board[to%10][to/10]==0)
                                    {
                                        if(from%10-to%10==1)
                                        {
                                            legal=1;
                                        }
                                        else legal=0;
                                    }
                                    else if((abs(from/10-to/10)==1)&&(from%10-to%10==1)&&board[to%10][to/10]>0&&board[to%10][to/10]%2==0)
                                    {
                                        legal=1;
                                    }
                                    else if(from%10==3&&to%10==2&&board[to%10+1][to/10]==100&&enpassant[0][to/10]==2)
                                    {
                                        enpassant[0][to/10]=-2;
                                        legal=1;
                                    }
                                }
                                else if(pval==100)
                                {
                                    if(from/10==to/10&&board[to%10][to/10]==0)
                                    {
                                        if(to%10-from%10==1)
                                        {
                                            legal=1;
                                        }
                                        else legal=0;
                                    }
                                    else if((abs(from/10-to/10)==1)&&(to%10-from%10==1)&&board[to%10][to/10]>0&&board[to%10][to/10]%2==1)
                                    {
                                        legal=1;
                                    }
                                    else if(from%10==4&&to%10==5&&board[to%10-1][to/10]==101&&enpassant[1][to/10]==2)
                                    {
                                        enpassant[1][to/10]=-2;
                                        legal=1;
                                    }
                                }
                            }
                            
                            break;
                        }
            default: break;
        }
    }
    return legal;
}
void promote(int num)
{
    int x;
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
void copy(int m, int n, int copyto[m][n], int copyfrom[m][n])
{
    for(int i=0;i<m;i++)
    {
        for(int j=0;j<n;j++)
        {
            copyto[i][j]=copyfrom[i][j];
        }
    }
}
int chess(int from,int to)
{
    move(from,to);
    if(prmte!=-1)
    {
        printf("Choose a piece to promote to:\n1. Queen\n2. Rook\n3. Bishop\n4. Knight\n");
        promote(prmte);
        prmte=-1;
    }
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(enpassant[i][j]>0) enpassant[i][j]++;
            if(enpassant[i][j]>2) enpassant[i][j]=-1;
        }
    }
    set();
    return 0;
}
int possiblemoves()
{
    int count=0,x;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]%10==turn&&board[i][j]>0)
            {
                for(int k=0;k<8;k++)
                {
                    for(int l=0;l<8;l++)
                    {
                        x=islegal(10*j+i,10*l+k);
                        undo();
                        if(x==1)
                        {
                            posmov[count][0]=10*j+i+11;
                            posmov[count][1]=10*l+k+11;
                            count++;
                            posmov[count][0]=0;
                            posmov[count][1]=0;
                        }
                    }
                }
            }
        }
    }
    return count;
}
int score()
{
    int val=0;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j]%10==1) val-=board[i][j]/10;
            else val+=board[i][j]/10;
        }
    }
    return val;
}
int minimax()
{
    int fromto[2][2]={0,0,-1,-1},tstb1[8][8],tstb2[8][8],tstb3[8][8],tstc1[2][2],tstc2[2][2],tstc3[2][2],k1[2],k2[2],k3[2],te1[2][8],te2[2][8],te3[2][8],x,temp,solution;
    for(int i=0;i<possiblemoves()-1;i++)
    {
        x=score();
        copy(8,8,tstb1,board);
        copy(2,2,tstc1,castle);
        copy(2,8,te1,enpassant);
        k1[0]=king[0];
        k1[1]=king[1];
        fromto[0][0]=posmov[i][0]-11;
        fromto[0][1]=posmov[i][1]-11;
        move(posmov[i][0]-11,posmov[i][1]-11);
        temp=score();
        if((turn==0&&temp>x)||(turn==1&&temp<x))
        {
            x=temp;
            for(int j=0;j<possiblemoves()-1;j++)
            {
                copy(8,8,tstb2,board);
                copy(2,2,tstc2,castle);
                copy(2,8,te2,enpassant);
                k2[0]=king[0];
                k2[1]=king[1];
                fromto[0][0]=posmov[j][0]-11;
                fromto[0][1]=posmov[j][1]-11;
                move(posmov[j][0]-11,posmov[j][1]-11);
                temp=score();
                if((turn==0&&temp<x)||(turn==1&&temp>x))
                {
                    x=temp;
                    for(int k=0;k<possiblemoves()-1;k++)
                    {
                        copy(8,8,tstb3,board);
                        copy(2,2,tstc3,castle);
                        copy(2,8,te3,enpassant);
                        k3[0]=king[0];
                        k3[1]=king[1];
                        fromto[0][0]=posmov[k][0]-11;
                        fromto[0][1]=posmov[k][1]-11;
                        move(posmov[k][0]-11,posmov[k][1]-11);
                        temp=score();
                        if((turn==0&&temp>x)||(turn==1&&temp<x))
                        {
                            fromto[1][0]=fromto[0][0];
                            fromto[1][1]=fromto[0][1];
                        }
                        copy(8,8,board,tstb3);
                        copy(2,2,castle,tstc3);
                        copy(2,8,enpassant,te3);
                        king[0]=k3[0];
                        king[1]=k3[1];
                    }
                }
                copy(8,8,board,tstb2);
                copy(2,2,castle,tstc2);
                copy(2,8,enpassant,te2);
                king[0]=k2[0];
                king[1]=k2[1];
            }
        }
        copy(8,8,board,tstb1);
        copy(2,2,castle,tstc1);
        copy(2,8,enpassant,te1);
        king[0]=k1[0];
        king[1]=k1[1];
    }
    undo();
    if(fromto[1][0]==-1) solution=100*fromto[0][0]+fromto[0][1];
    else solution=100*fromto[1][0]+fromto[1][1];
    return solution;
}
int main()
{
    int from,to,x,y,z;
    printf("\n\n\t\tWelcome to Chess!\n\n");
    play:
    printf("Select Gamemode:\n1.Singleplayer 2.Multiplayer\n");
    modeselect:
    scanf("%d",&from);
    if(from==1)
    {
        reset();
        printf("Choose side:\n1.White 2. Black\n");
        schoose:
        scanf("%d",&side);
        if(side==1||side==2) side--;
        else
        {
            printf("Enter 1 or 2: ");
            goto schoose;
        }
        showboard();
        printf("Input Syntax: x1y1,x2y2. Enter 1 to show all possible moves. Enter 0 anytime to quit.\nHere x1y1 are initial coordinates and x2y2 are final coordinates.\nGood Luck!\n");
        for(;;)
        {
            {
                if(turn==side)
                {
                    cont1:
                    scanf("%d%*c",&from);
                    if(from==0)
                    {
                        printf("Are you sure you want to quit the game?\n1.Yes 2.No\n");
                        failsafe1:
                        scanf("%d",&from);
                        if(from==1) goto newgame;
                        else if(from==2)
                        {
                            printf("Continuing the game...\n");
                            goto cont1;
                        }
                        else
                        {
                            printf("Enter 1 or 2: ");
                            goto failsafe1;
                        }
                    }
                    else if(from==1)
                    {
                        x=possiblemoves();
                        printf("List of all possible moves:\n");
                        for(int i=0;i<x;i++)
                        {
                            printf("%d,%d\n",posmov[i][0],posmov[i][1]);
                        }
                        goto cont1;
                        }
                    scanf("%d",&to);
                    from-=11; to-=11;
                }
                else
                {
                    y=minimax();
                    from=y/100;
                    to=y%100;
                }
                if(from%10>=0&&to%10>=0&&from%10<8&&to%10<8&&from/10>=0&&to/10>=0&&from/10<8&&to/10<8)
                {
                    if(islegal(from,to)==1)
                    {
                        chess(from,to);
                        showboard();
                        if(check(king[0])==1||check(king[1])==1) printf("Check!\n");
                        turn++; turn%=2; nummov++;
                    }
                    else printf("Illegal move\n");

                }
                else printf("Invalid input\n");
                stalemate=1;
                x=possiblemoves();
                if(x>0) stalemate=0;
                if(stalemate==1&&check(king[0])==1)
                {
                    printf("Checkmate! Black wins.\n");
                    goto newgame;
                }
                else if(stalemate==1&&check(king[1])==1)
                {
                    printf("Checkmate! White wins.\n");
                    goto newgame;
                }
                else if(stalemate==1)
                {
                    printf("Stalemate! ");
                }
                if(move50==50)
                {
                    printf("50-move rule. ");
                }
                if(stalemate==1||move50==50)
                {
                    printf("Its a Draw.\n");
                    goto newgame;
                }
            }
        }
    }
    else if(from==2)
    {
        reset();
        showboard();
        printf("Input Syntax: x1y1,x2y2. Enter 1 to show all possible moves. Enter 0 anytime to quit.\nHere x1y1 are initial coordinates and x2y2 are final coordinates.\nGood Luck!\n");
        for(;;)
        {
            cont:
            scanf("%d%*c",&from);
            if(from==0)
            {
                printf("Are you sure you want to quit the game?\n1.Yes 2.No\n");
                failsafe:
                scanf("%d",&from);
                if(from==1) goto newgame;
                else if(from==2)
                {
                    printf("Continuing the game...\n");
                    goto cont;
                }
                else
                {
                    printf("Enter 1 or 2: ");
                    goto failsafe;
                }
            }
            else if(from==1)
            {
                x=possiblemoves();
                printf("List of all possible moves:\n");
                for(int i=0;i<x;i++)
                {
                    printf("%d,%d\n",posmov[i][0],posmov[i][1]);
                }
                goto cont;
            }
            scanf("%d",&to);
            from-=11; to-=11;
            if(from%10>=0&&to%10>=0&&from%10<8&&to%10<8&&from/10>=0&&to/10>=0&&from/10<8&&to/10<8)
            {
                if(islegal(from,to)==1)
                {
                    chess(from,to);
                    showboard();
                    if(check(king[0])==1||check(king[1])==1) printf("Check!\n");
                    turn++; turn%=2;
                }
                else printf("Illegal move\n");
            }
            else printf("Invalid input\n");
            stalemate=1;
            x=possiblemoves();
            if(x>0) stalemate=0;
            if(stalemate==1&&check(king[0])==1)
            {
                printf("Checkmate! Black wins.\n");
                goto newgame;
            }
            else if(stalemate==1&&check(king[1])==1)
            {
                printf("Checkmate! White wins.\n");
                goto newgame;
            }
            else if(stalemate==1)
            {
                printf("Stalemate! ");
            }
            if(move50==50)
            {
                printf("50-move rule. ");
            }
            if(stalemate==1||move50==50)
            {
                printf("Its a Draw.\n");
                goto newgame;
            }
        }
    }
    else
    {
        printf("Enter 1 or 2: ");
        goto modeselect;
    }
    newgame:
    printf("Do you want to play another game?\n1.Yes 2.No\n");
    new:
    scanf("%d",&from);
    if(from==1) goto play;
    else if(from==2) return 0;
    else
    {
        printf("Enter 1 or 2: ");
        goto new;
    }
}
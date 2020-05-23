#include<graphics.h>
//#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include<malloc.h>

#define WIDTH      45
#define HIGH       40
#define SNAKELG    30
#define BOXSIZE    10

//物体
#define WALL       	10
#define FOOD       	3
#define SNAKE      	4
#define EMPTY		0
#define SHEAD		5
#define STAIL		1
#define FRUIT		2
//定义物体颜色
#define COLOR_WALL EGERGB(100, 100, 100)
#define COLOR_FOOD EGERGB(255 ,99, 71)
#define COLOR_BACK EGERGB(225, 225, 225)
#define COLOR_HEAD EGERGB(144 ,238 ,144)
#define COLOR_FRUIT EGERGB(	205, 190 ,112)
//方向
#define Up 0
#define Down 2
#define Left 3
#define Right 1
int snake_direct;
int score;
int direct[4][2]={{-1,0},{0,1},{1,0},{0,-1}};
int speed = 60;
int state = 5;
int colorlib[10]={0xDC143C,0xC71585,0x8B008B,0x4B0082,0x483D8B,0x4682B4,0x008B8B,0x556B2F,0xBDB76B,0xD2691E};
int COLOR_SNAKE = colorlib[9];
int AI;


typedef struct point{
	int m_nX;
	int m_nY;
	struct point *next;
	struct point *last;
}Point;

Point *head=NULL,*tail;
Point *food = (Point*)malloc(sizeof(Point));
Point *fruit = (Point*)malloc(sizeof(Point));

char sMap[HIGH][WIDTH];//地图 

int rungame=0;

//void samey();
//void samex();
int Sx=0,Sy=0;
int Fx=0,Fy=0;
int Tx=0,Ty=0;

void menu();
void help_menu(){
	cleardevice();
    outtextrect(280, 120,250,30, "使用↑↓←→进行方向操作");
    
    setfillcolor(COLOR_FRUIT); 
    fillellipse(280, 160,BOXSIZE/2,BOXSIZE/2);
    outtextxy(310, 150, "吃到这个可以使身长减二");
	setfillcolor(COLOR_FOOD); 
	fillellipse(280, 190,BOXSIZE/2,BOXSIZE/2);
    outtextxy(310, 180, "吃到这个身体长长一格");
    
	outtextrect(300, 240,150,30, "单机鼠标以继续操作");
    
	mouse_msg msg = {0};
	while (1){
	msg = getmouse();
		if(msg.is_left()&& msg.is_up())	break;
	}
	menu();
}

void menu(){
	initgraph(700,400);
	setbkcolor(WHITE);
	setcolor(BLACK);
//	
//	setbkmode(TRANSPARENT);
	setfont(28, 0, "宋体");
	outtextxy(260, 50, "无穷贪吃蛇");
	
	setfont(18, 0, "宋体");
	setfontbkcolor(YELLOW);
	outtextrect(280, 120,150,30, "开 始 游 戏");
	outtextrect(280, 180,150,30, "   帮助    ");
	outtextrect(280, 240,150,30, "退 出 游 戏");
	
	
    mouse_msg msg = {0};
	while (1){
		
        msg = getmouse();
        if(msg.is_left() && msg.is_up()){
        	
        	if(msg.x >= 280 && msg.x <= 430){
        		if(msg.y >= 120 && msg.y <= 150){
					rungame = 1;
					break;
				}
				else if(msg.y >= 180 && msg.y <= 210){
					help_menu();
					break;
				}
				else if(msg.y >= 240 && msg.y <= 270) {
					rungame = 0;
					break;
				}
			}
        	
		} 
    }
 
	cleardevice();
}

void DrawBox(Point *lpPos){
	Point thePos;
    thePos.m_nX = lpPos->m_nX * BOXSIZE;
    thePos.m_nY = lpPos->m_nY * BOXSIZE;
	if(sMap[lpPos->m_nX][lpPos->m_nY]==FOOD||sMap[lpPos->m_nX][lpPos->m_nY]==FRUIT)
		fillellipse(thePos.m_nY+BOXSIZE/2, thePos.m_nX+BOXSIZE/2,BOXSIZE/2,BOXSIZE/2);
		
    else
    	bar(thePos.m_nY, thePos.m_nX, thePos.m_nY+BOXSIZE, thePos.m_nX+BOXSIZE);
}

void initWall(){
    int nX = 0, nY = 0;

    for (nY = 0; nY < WIDTH; nY++)         //left
        sMap[nX][nY] = WALL;
    nY--;

    for (nX = 0; nX < HIGH; nX++)        //bottom
        sMap[nX][nY] = WALL;
    nX--;

    for (nY; nY > -1; nY--)               //right
        sMap[nX][nY] = WALL;
	nY++;
    for (nX; nX > -1; nX--)               //top
        sMap[nX][nY] = WALL;

    setfillcolor(COLOR_WALL);             //设置墙颜色
    bar(0, 0, BOXSIZE, BOXSIZE*HIGH);
    bar(0, 0, BOXSIZE*WIDTH, BOXSIZE);
    bar(BOXSIZE*(WIDTH-1), 0, BOXSIZE*(WIDTH), BOXSIZE*HIGH);
    bar(0, BOXSIZE*(HIGH-1), BOXSIZE*(WIDTH), BOXSIZE*HIGH);
}

int foodmap[700];

void CreateFood(int F){
	Point *f;
	
	if(F==FOOD)	f = food;
	else if(F==FRUIT)	f=fruit;
	
	f->m_nX = rand() % (HIGH-2)+1;
    f->m_nY = rand() % (WIDTH-2)+1;
	while(sMap[f->m_nX][f->m_nY]!=EMPTY){
		f->m_nX = rand() % (HIGH-2)+1;
    	f->m_nY = rand() % (WIDTH-2)+1;
	}

	
    //绘画食物
    if(F==FOOD){
    	sMap[f->m_nX][f->m_nY] = FOOD;
    	setfillcolor(COLOR_FOOD); 
	}
	else if(F==FRUIT){
		sMap[f->m_nX][f->m_nY] = FRUIT;
    	setfillcolor(COLOR_FRUIT); 
	}
	
    DrawBox(f);
}



void CreateNewSnake(){
	Point *snake1 = (Point*)malloc(sizeof(Point));
	Point *snake2 = (Point*)malloc(sizeof(Point));
	Point *snake3 = (Point*)malloc(sizeof(Point));
    snake1->m_nX = HIGH/2;
    snake1->m_nY = WIDTH/2;
    
    snake1->next = snake2;
    snake2->last = snake1;
    
	snake2->m_nX = HIGH/2+1;
    snake2->m_nY = WIDTH/2;
    
    snake2->next = snake3;
    snake3->last = snake2;
    
    snake3->m_nX = HIGH/2+2;
    snake3->m_nY = WIDTH/2;
    
    head = snake1;
    tail = snake3;
    
    sMap[snake1->m_nX][snake1->m_nY] = SHEAD;
    sMap[snake2->m_nX][snake2->m_nY] = SNAKE;
    sMap[snake3->m_nX][snake3->m_nY] = STAIL;
    //绘画蛇头 
    setfillcolor(COLOR_HEAD);
    DrawBox(snake1);
    //画蛇身
	setfillcolor(COLOR_HEAD);      
    DrawBox(snake2);
    DrawBox(snake3);
}

void init(){
	outtextxy(470, 30,"当前得分：");
	outtextxy(470, 70,"贪吃蛇可以穿墙而过");
	outtextrect(470, 100,300,100,"每吃掉五个食物\n贪吃蛇可以进行一次蜕皮");
	outtextxy(470, 140,"使用方向键上下左右进行操作");
	outtextxy(470, 170,"单击空格开启托管");
	
	setfillcolor(COLOR_FRUIT);
    fillellipse(460, 210,BOXSIZE/2,BOXSIZE/2);
    outtextxy(470, 200, "身长-2");
	setfillcolor(COLOR_FOOD); 
	fillellipse(460, 240,BOXSIZE/2,BOXSIZE/2);
    outtextxy(470, 230, "身长+1");
    
    outtextxy(470, 260,"按下Esc退出游戏");
    
}
void StartGame(){
	memset(sMap,0,sizeof(sMap));
    initgraph(700, 400);
    setbkcolor(WHITE);
    AI = 0;
    speed = 60;
    score = 0;
    state = 5;
    snake_direct = 0;
    initWall();
    CreateNewSnake();
    CreateFood(FOOD);
    CreateFood(FRUIT);
	init();
}
int OnkeyMsg(char cKey){ 
    switch (cKey){
    	case VK_UP:if(snake_direct!=Up&&snake_direct!=Down)snake_direct=Up;break;
    	case VK_DOWN:if(snake_direct!=Up&&snake_direct!=Down)snake_direct=Down;break;
    	case VK_LEFT:if(snake_direct!=Left&&snake_direct!=Right)snake_direct=Left;break;
    	case VK_RIGHT:if(snake_direct!=Left&&snake_direct!=Right)snake_direct=Right;break;
    	case VK_SPACE:AI=!AI;break;
    	case VK_F1:while(!getch());break;
    	case VK_F2:speed-=10;break;
    	case VK_F3:speed+=10;break;
    	case VK_ESCAPE:rungame = 0;break;
    }
	return 1;
}

void EraseBox(Point *thePos){
    color_t color = getbkcolor();
    setfillcolor(color);
    DrawBox(thePos);
    sMap[thePos->m_nX][thePos->m_nY] = EMPTY;
}
void edgeCheck (void){
	if (head->m_nX < 1)
		head->m_nX = HIGH - 2;
	else if (head->m_nX > HIGH - 2)
		head->m_nX = 1;
	else if (head->m_nY < 1)
		head->m_nY = WIDTH - 2;
	else if (head->m_nY > WIDTH - 2)
		head->m_nY = 1;
}

void reachState(){
	state+=5;
	int i=rand()%9;
	COLOR_SNAKE = colorlib[i];
	int t=colorlib[i];
	colorlib[i] = colorlib[9];
	colorlib[9] = t;
}

void SnakeMove(){
	Point *NewHead = (Point*)malloc(sizeof(Point));
	NewHead->m_nX=head->m_nX+direct[snake_direct][0];
	NewHead->m_nY=head->m_nY+direct[snake_direct][1];
	NewHead->next = head;
	sMap[head->m_nX][head->m_nY] = SNAKE;
	head->last = NewHead;
	head = NewHead;
	
	int t=1;
	
	edgeCheck ();//到达边界循环
	
	
	
	setfillcolor(COLOR_SNAKE);
	DrawBox(NewHead->next);
	
	//吃到食物 
	if(sMap[NewHead->m_nX][NewHead->m_nY]==FOOD){
		
		t=0;
		
		if(score+1>=state){//变颜色 
			reachState();
		}
			
		char s[3];//输出得分 
		++score;
		sprintf(s, "%d", score);
		outtextxy(550, 30,s);
		
		CreateFood(FOOD);
		
		if(sMap[fruit->m_nX][fruit->m_nY]!=FRUIT){
			int x = rand()%10;
			if(x==9)
				CreateFood(FRUIT);
		}
		
	}
	else if(sMap[NewHead->m_nX][NewHead->m_nY]==FRUIT)	t=3;
	else if(sMap[NewHead->m_nX][NewHead->m_nY]==SNAKE)//游戏结束 
	{
		MessageBox(NULL, "Game Over!", "sorry", MB_OK);
		rungame = false;
	}
	
	
	for(int i=0;i<t;++i){//去掉尾部一格
		Point *TailPos = tail;
		EraseBox(TailPos);
		sMap[TailPos->m_nX][TailPos->m_nY] = EMPTY;
		tail = tail->last;
		free(TailPos);
		tail->next=NULL;
	}
	//绘制新头部
	setfillcolor(COLOR_HEAD);
	DrawBox(NewHead);
	
	sMap[NewHead->m_nX][NewHead->m_nY] = SHEAD;
	sMap[tail->m_nX][tail->m_nY] = STAIL;
}
void EndGame(){
	//清除链表 
//	getch();
	while(head->next){
		Point *p=head;
		head=head->next;
		free(p);
	}
	free(food);
    cleardevice();
    
    setfont(30, 0, "宋体");
    outtextxy(300, 170,"WASTED");
    getch();
	closegraph();
//    initgraph(700,400);
//	setbkcolor(WHITE);
//	setfont(18, 0, "宋体");
//
//	outtextxy(200, 170,"按下R键重玩，按下其它键退出游戏");
//	char c=getch();
//	if(c=='r'||c=='R')	rungame=1;//需要重玩吗 
menu();
}
void astar();

int main(){
	srand((int)time(0));
	setinitmode(0);
	menu();
	while(rungame){
    	StartGame();
		key_msg theMag;
	
		while (rungame){
        	if (kbhit()){
            	theMag = getkey();
		    	OnkeyMsg(theMag.key);
        	}
        if(AI)astar();
        SnakeMove();
        Sleep(speed);
		}
    	EndGame();
	} 
    return 0;
}


typedef struct node{
	int x;
	int y;
}Node;
Node Path[2000];


char smap[HIGH][WIDTH];
int checkmove(int d){
	if(sMap[Sx+direct[d][0]][Sy+direct[d][1]]==SNAKE)
		return 0;
	return 1;
}
void avoid(){
	if(!checkmove(snake_direct)){
		if(checkmove((snake_direct+1)%4))	snake_direct=(snake_direct+1)%4;
		else snake_direct=(snake_direct+3)%4;
	}
}
int a=0,b=0;
int a1[]={0,2,1};

void bfs(){
	int l=-1,r=0;
	for(int k=0;k<3;++k){
		int i=a1[k];
		int xx=Sx+direct[(snake_direct+3+i)%4][0];
		int yy=Sy+direct[(snake_direct+3+i)%4][1];
		if(xx>0&&xx<39&&yy>0&&yy<44&&smap[xx][yy]<=FOOD){
			++l;
			Path[l].x = xx;
			Path[l].y = yy;
			smap[Path[l].x][Path[l].y]=11+i;
			++r;
		}
	}
	l=0;
	a=0,b=0;
	while(l<r){
		if(Path[l].x==Fx&&Path[l].y==Fy)
			a=smap[Fx][Fy];
		else if(Path[l].x==Tx&&Path[l].y==Ty)
			b=smap[Tx][Ty];
		if(a&&b)return ;
		for(int i=0;i<4;++i){
			int xx=Path[l].x+direct[i][0];
			int yy=Path[l].y+direct[i][1];
			if(xx>0&&xx<39&&yy>0&&yy<44&&smap[xx][yy]<=FOOD){
				Path[r].x=xx;
				Path[r].y=yy;
				smap[xx][yy]=smap[Path[l].x][Path[l].y];
				++r;
			}
		}
		++l;
	}
	return;
	
}

int minibfs(int sx,int sy,int tx,int ty){
	Path[0].x=sx;
	Path[0].y=sy;
	int l=0,r=1;
	while(l<r){
		smap[Path[l].x][Path[l].y]=SNAKE;
		if(Path[l].x==tx&&Path[l].y==ty)
			return 1;
		for(int i=0;i<4;++i){
			int xx=Path[l].x+direct[i][0];
			int yy=Path[l].y+direct[i][1];
			if(xx>0&&xx<39&&yy>0&&yy<44&&smap[xx][yy]<=FOOD){
				Path[r].x=xx;
				Path[r].y=yy;
				smap[xx][yy]=smap[Path[l].x][Path[l].y];
				++r;
			}
		}
		++l;
	}
	return 0;
}

int f(int d){
	
	
	int Dx=Sx+direct[(snake_direct+3+d)%4][0];
	int Dy=Sy+direct[(snake_direct+3+d)%4][1];
	
	for(int i=0;i<3;++i){
		if(i!=d){
			int xx=Sx+direct[(snake_direct+3+i)%4][0];
			int yy=Sy+direct[(snake_direct+3+i)%4][1];
			memcpy(smap,sMap,sizeof(sMap));
			if(smap[xx][yy]<=FOOD){
				if(minibfs(xx,yy,Dx,Dy))
				{
					return i;
				}
			}
			
		}
	}
	return d;
}


void astar(){
	Sx=head->m_nX,Sy=head->m_nY;
	Fx=food->m_nX,Fy=food->m_nY;
	Tx=tail->m_nX,Ty=tail->m_nY;
	memcpy(smap,sMap,sizeof(sMap));
	bfs();
//	printf("%d\n",a);
	
	memcpy(smap,sMap,sizeof(sMap));
	int plana = a, planb = b;
	if(plana){
		plana-=11;
		int t = snake_direct;
		snake_direct=(snake_direct+3+plana)%4;
		
		Sx=Sx+direct[snake_direct][0];
		Sy=Sy+direct[snake_direct][1];
		smap[Sx][Sy] = SNAKE;
		
		bfs();
		snake_direct = t;
		if(!b){
			planb-=11;
			snake_direct=(snake_direct+3+planb)%4;
//			printf("bunengchi\n");
//			int c=f(plana);
//			snake_direct=(snake_direct+3+c)%4;
		}
		else snake_direct=(snake_direct+3+plana)%4;
		
	}
	else if(planb){
		planb-=11;
//		int t = snake_direct;
//		snake_direct=(snake_direct+3+planb)%4;
//		printf("zhaobudao\n");
		int c=f(planb);
		snake_direct=(snake_direct+3+c)%4;
	}

}

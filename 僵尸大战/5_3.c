#include <windows.h>
#include "5_3.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE hinst;
static int wave = 0; //第几波
double sx = 975, sy = 825;//人物位置
double bx = 525, by = 450;//显示区域在整个地图中的位置
int sz = 0;
static int dir = 7;
static num[105];//记录小怪数目
static flag = 1;//判断是否刷新怪
int res;
int fenshu = 0;
int tiandan; //判断是否装弹
int playSucc;
int fail;
double mx, my; //鼠标位置
static int fashetime = 0;
static int pause = 0;
static int patime = 0;
int tt;
struct guai
{
	double x, y;
	int blood;//血量
	int direction;//方向
	int id;
	int sz;//使用位图是用到的参数
	double vx, vy;//速度
	int attack;//判断是否要攻击
	int tim;//boss发弹的间隔
	int dead;//判断是否死亡
	int fra;//击杀每个怪的分数
	int timefen;//分数显示的时间
}smg[1000], boss[1000];//小怪和boss
struct peo
{
	double x, y;//位置
	int blood; // 血量
	int direction; //方向
	int save; //判断是否回血
	//int 
}Bob;//人物

struct vis
{
	double x, y;
}th[10005];

struct att //boss攻击
{
	double x, y; //位置
	double flag; //判断该弹是否存在
	double vx, vy;//速度
}att[10006];
int cirnum;//数量

struct gun
{
	
	double x, y;//子弹位置
	double flag;//判断是否存在
	double vx, vy;//速度
	int dir;//方向
	int time;//间隔
}zidan[10][10005]; //子弹

int zidanshu[10];
struct wahaha
{
	int keyong, dangqiang;//当前弹夹子弹数和后备子弹数
	int time;//
}wa[5];//弹夹

static int gunid = 1;
struct blood
{
	double x, y;
}xue[10005];
static xuenum = 0;
int waittime;

struct Box
{
	double x, y;
	int time;
}box[10006];
static boxnum = 0;
static int menu = 1;
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInst,LPSTR lpszCmdLine,int nCmdShow)
{
	HWND hwnd;
	MSG Msg;
	WNDCLASS wndclass; 
	
	char lpszClassName[] = "窗口";
	char lpszTitle[]= "My_Windows";
	
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground =(HBRUSH) GetStockObject(NULL);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = lpszClassName;
	hinst = hInstance;
	if(!RegisterClass(&wndclass))
	{
		MessageBeep(0) ;
		return FALSE ;
	}
	hwnd=CreateWindow(
		lpszClassName,
		"僵尸大战",
		WS_OVERLAPPED,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		1050,
		900,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	while(GetMessage(&Msg,NULL,0,0))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}

void chuli1() //小僵尸处理
{
	int i, j;
	double xx, yy;
	for(i = 1; i <= num[wave]; ++ i)
	{
		if(smg[i].blood > 0)//僵尸还没挂
		{
			//判断是否攻击
			if(fabs(sx - smg[i].x) < 80 && fabs(sy - smg[i].y) < 80)
			{
				smg[i].attack = 1;
				if(sx - smg[i].x < 80 && sx > smg[i].x + 60)
				{
					if(sy - smg[i].y < 80 && sy - smg[i].y > 60)
					{
						smg[i].direction = 6;
					}
					else if(smg[i].y - sy < 80 && smg[i].y - sy > 60)
					{
						smg[i].direction = 8;
						
					}
					else
					{
						smg[i].direction = 7;					
					}
				}
				else if(smg[i].x - sx < 80 && smg[i].x - sx > 60)
				{
					if(sy - smg[i].y < 80 && sy - smg[i].y > 60)
					{
						smg[i].direction = 4;
						
					}
					else if(smg[i].y - sy < 80 && smg[i].y - sy > 60)
					{
						smg[i].direction = 2;
					}
					else
					{
						smg[i].direction = 3;												
					}
				}
				else
				{
					if(sy - smg[i].y < 80)
					{
						smg[i].direction = 5;
					}
					else if(smg[i].y - sy < 80)
					{
						smg[i].direction = 1;
						
					}
				}
			}
			else
				smg[i].attack = 0;
			
			if(!smg[i].attack)
			{
				if(sx - smg[i].x >70)
				{
					if(sy - smg[i].y > 70)
					{
						smg[i].direction = 6;
						xx = smg[i].x + smg[i].vx;
						yy = smg[i].y + smg[i].vy;
					}
					else if(smg[i].y - sy > 70)
					{
						smg[i].direction = 8;
						xx = smg[i].x + smg[i].vx;
						yy = smg[i].y - smg[i].vy;
					}
					else
					{
						smg[i].direction = 7;
						xx = smg[i].x + smg[i].vx;
						yy = smg[i].y;
						
					}
				}
				else if(smg[i].x - sx > 70)
				{
					if(sy - smg[i].y > 70)
					{
						smg[i].direction = 4;
						xx = smg[i].x - smg[i].vx;
						yy = smg[i].y + smg[i].vy;
					}
					else if(smg[i].y - sy > 70)
					{
						smg[i].direction = 2;
						xx = smg[i].x - smg[i].vx;
						yy = smg[i].y - smg[i].vy;
					}
					else
					{
						smg[i].direction = 3;
						xx = smg[i].x - smg[i].vx;
						yy = smg[i].y;
						
					}
				}
				else
				{
					if(sy - smg[i].y > 70)
					{
						smg[i].direction = 5;
						xx = smg[i].x;
						yy = smg[i].y + smg[i].vy;
					}
					else if(smg[i].y - sy > 70)
					{
						smg[i].direction = 1;
						xx = smg[i].x;
						yy =smg[i].y - smg[i].vy;
					}
				}
				
				
				
				smg[i].x = xx;
				smg[i].y = yy;
				
				
			}
			
			}
		}
}
void chuli2()
{
	int i, j;
	double xx, yy;
	for(i = 1; i <= wave; ++ i)
	{
		if(boss[i].blood > 0)//该僵尸还没挂
		{
			//判断是否攻击
			if(fabs(sx - boss[i].x) < 500 && fabs(sy - boss[i].y) < 500)
			{
				boss[i].attack = 1;
				if(sx - boss[i].x < 500 && sx > boss[i].x + 400)
				{
					if(sy - boss[i].y < 500 && sy - boss[i].y > 400)
					{
						boss[i].direction = 6;
					}
					else if(boss[i].y - sy < 500 && boss[i].y - sy > 400)
					{
						boss[i].direction = 8;
						
					}
					else
					{
						boss[i].direction = 7;						
						
					}
				}
				else if(boss[i].x - sx < 500 && boss[i].x - sx > 400)
				{
					if(sy - boss[i].y < 500 && sy - boss[i].y > 400)
					{
						boss[i].direction = 4;
						
					}
					else if(boss[i].y - sy < 500 && boss[i].y - sy > 400)
					{
						boss[i].direction = 2;
						
					}
					else
					{
						boss[i].direction = 3;
						
						
					}
				}
				else
				{
					if(sy - boss[i].y < 500)
					{
						boss[i].direction = 5;
					}
					else if(boss[i].y - sy < 500)
					{
						boss[i].direction = 1;
						
					}
				}
			}
			else
				boss[i].attack = 0;
			
			if(!boss[i].attack)
			{
				if(sx - boss[i].x >450)
				{
					if(sy - boss[i].y > 450)
					{
						boss[i].direction = 6;
						xx = boss[i].x + boss[i].vx;
						yy = boss[i].y + boss[i].vy;
					}
					else if(boss[i].y - sy > 450)
					{
						boss[i].direction = 8;
						xx = boss[i].x + boss[i].vx;
						yy = boss[i].y - boss[i].vy;
					}
					else
					{
						boss[i].direction = 7;
						xx = boss[i].x + boss[i].vx;
						yy = boss[i].y;
						
					}
				}
				else if(boss[i].x - sx > 450)
				{
					if(sy - boss[i].y > 450)
					{
						boss[i].direction = 4;
						xx = boss[i].x - boss[i].vx;
						yy = boss[i].y + boss[i].vy;
					}
					else if(boss[i].y - sy > 450)
					{
						boss[i].direction = 2;
						xx = boss[i].x - boss[i].vx;
						yy = boss[i].y - boss[i].vy;
					}
					else
					{
						boss[i].direction = 3;
						xx = boss[i].x - boss[i].vx;
						yy = boss[i].y;
						
					}
				}
				else
				{
					if(sy - boss[i].y > 450)
					{
						boss[i].direction = 5;
						xx = boss[i].x;
						yy = boss[i].y + boss[i].vy;
					}
					else if(boss[i].y - sy > 450)
					{
						boss[i].direction = 1;
						xx = boss[i].x;
						yy =boss[i].y - boss[i].vy;
					}
				}
				
				
				
				boss[i].x = xx;
				boss[i].y = yy;
				
				
			}
			
			}
		}
}

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM  wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HPEN hpen;
	HBRUSH hbr;
	HFONT hf;
	HCURSOR hcursor;
	static HDC hmemdc[600], hdcmem;
	static HBITMAP hbitmap[600], hbmp;
	static BITMAP bitmap[600], bmp;
	RECT rect;
	double vx = 14;
	double vy = 12;
	int i, j, k, p;
	double xx, yy;
	int ans, lucay, len;//分数
	char s1[] = "第";
	char s3[] = "波";
	char s5[] = "/";
	char s2[10], s4[10], s6[10], s[20];
	char s8[] = "你的分数为:";
	char s9[20];
	char buf[256];
	for(i = 1; i <= 100; ++ i)
	{
		num[i] = 5 + i * 5;
	}
	
	if(flag)//初始化每波
	{
		
		if(wave == 0)
		{
			Bob.blood = 100;
			fenshu = 0;
			Bob.save = 40;
			wa[1].keyong = 1000;
			wa[0].keyong = 10;
			wa[1].dangqiang = 6;
			wa[0].dangqiang = 2;
			wa[0].time = 0;
			wa[1].time = 0;
			fail = 0;
			boxnum = 0;
			xuenum = 0;
		}	
		tt = 0;
		for(i = 0 ; i <= 1; ++ i)
		{
			for(j = 0 ; j < zidanshu[i]; ++ j)
			{
				zidan[i][j].time = 0;
			}
		}
		for(i = 0 ; i < cirnum; ++ i)
		{
			att[i].flag = 0;
		}
		tiandan = 0;
		cirnum = 0;
		memset(zidanshu, 0, sizeof(zidanshu));
		srand( (unsigned)time( NULL ));
		wave++;
		
		for(i = 1; i <= num[wave]; ++ i)
		{
			smg[i].id = i;
			smg[i].x = rand() % 1995;
			smg[i].y = rand() % 1650;
			smg[i].blood = 20;
			smg[i].sz = 1;
			smg[i].vx = 3.5;
			smg[i].vy = 3;
			smg[i].attack = 0;
			smg[i].dead = 0;
			smg[i].fra = rand() % 100 + 300;
			smg[i].timefen = 0;
		}
		for(i = 1; i <= wave; ++ i)
		{
			boss[i].id = i;
			boss[i].x = rand() % 1995;
			boss[i].y = rand() % 1650;
			boss[i].blood = 50;
			boss[i].sz = 1;
			boss[i].vx = 3.5;
			boss[i].vy = 3;
			boss[i].attack = 0;
			boss[i].tim = 0;
			boss[i].fra = rand() % 500 + 600;
			boss[i].timefen = 0;
		}
		flag = 0;
		
		waittime = 0;
	}
	switch(message)
	{


	case WM_CREATE:
		
		hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rect);
		//加载双缓冲需要用的虚拟dc和位图
		hdcmem = CreateCompatibleDC(hdc);
		hbmp = CreateCompatibleBitmap(hdc, 2100, 1800);	
		SelectObject(hdcmem, hbmp);
		GetObject(hbmp, sizeof(BITMAP), &bmp);
		for(i = 301; i <= 311; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		for(i = 500; i <= 506; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		//1-16
		for(i = 1; i <= 48; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		for(i = 101; i <= 116; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		for(i = 151; i <= 154; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		for(i = 201; i <= 217; ++ i)
		{
			hmemdc[i] = CreateCompatibleDC(hdc);
			hbitmap[i] = LoadBitmap(hinst, MAKEINTRESOURCE(i));
			SelectObject(hmemdc[i], hbitmap[i]);
			GetObject(hbitmap[i], sizeof(BITMAP), &bitmap[i]);
		}
		ReleaseDC(hwnd, hdc);
		break;
	case WM_KEYDOWN:
		{
			if(Bob.blood <= 0)
				break;
			if(GetAsyncKeyState('P') && menu < 1)
			{
				if(pause == 1)
				{
					pause = 0;
				//	patime = 30;
				}
				else
				{
					//if(patime <= 0)
						pause = 1;
				}
				break;
			}
			
			if(pause == 1)
				break;
			if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_LEFT))
			{
				sy+=vy;
				sx-=vx;
				dir = 4;
				if(sx < 0)
					sx = 0;
				if(sy > 1650)
					sy = 1650;
				if(by <= 900 && sy - by >= 350)
					by += vy;
				if(by  > 900)
					by = 900;
				
				if(bx <= 1050 && sx - bx <= 400)
					bx -= vx;
				if(bx < 0)
					bx = 0;
				
				sz = !sz;
				
			}
			else if(GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_LEFT))
			{
				sy-=vy;
				sx-=vx;
				dir = 2;
				if(sx < 0)
					sx = 0;	
				if(sy < 0)
					sy = 0;
				if(by <= 900 && sy - by <= 350)
					by -=vy;
				if(by < 0)
					by = 0;
				
				if(bx <= 1050 && sx - bx <= 400)
					bx -= vx;
				if(bx < 0)
					bx = 0;
				
				sz = !sz;
			}
			else if(GetAsyncKeyState(VK_UP) && GetAsyncKeyState(VK_RIGHT))
			{
				sy-=vy;
				sx+=vx;
				if(sy < 0)
					sy = 0;
				if(sx > 1995)
					sx = 1995;
				dir = 8;
				if(by <= 900 && sy - by <= 350)
					by -=vy;
				if(by < 0)
					by = 0;			
				if(bx <= 1050 && sx - bx >= 400)
					bx += vx;		
				if(bx > 1050)
					bx = 1050;
				sz = !sz;
				
			}
			else if(GetAsyncKeyState(VK_DOWN) && GetAsyncKeyState(VK_RIGHT))
			{
				sy+=vy;
				sx+=vx;
				dir = 6;
				if(sy > 1650)
					sy = 1650;
				if(sx > 1995)
					sx = 1995;
				if(by <= 900 && sy - by >= 350)
					by += vy;
				if(by  > 900)
					by = 900;
				
				if(bx <= 1050 && sx - bx >= 400)
					bx += vx;
				
				if(bx > 1050)
					bx = 1050;
				sz = !sz;
				
			}
			else if(GetAsyncKeyState(VK_UP))
			{
				dir = 1;
				sy -=vy;
				if(sy < 0)
					sy = 0;
				if(by <= 900 && sy - by <= 350)
					by -=vy;
				if(by < 0)
					by = 0;
				
				sz = !sz;
			}	
			else if(GetAsyncKeyState(VK_DOWN))
			{
				dir = 5;
				sy +=vy;
				if(sy > 1650)
					sy = 1650;
				if(by <= 900 && sy - by >= 350)
					by += vy;
				if(by  > 900)
					by = 900;
				
				sz = !sz;
			}
			else if(GetAsyncKeyState(VK_LEFT))
			{
				sx -= vx;
				dir = 3;
				if(sx < 0)
					sx = 0;
				if(bx <= 1050 && sx - bx <= 400)
					bx -= vx;
				if(bx < 0)
					bx = 0;
				
				sz = !sz;
				
			}
			else if(GetAsyncKeyState(VK_RIGHT))
			{
				sx += vx;	
				if(sx > 1995)
					sx = 1995;
				if(bx <= 1050 && sx - bx >= 400)
					bx += vx;
				
				if(bx > 1050)
					bx = 1050;
				sz = !sz;
				dir = 7;
				
			}
			else if(GetAsyncKeyState(VK_SPACE))
			{
				if(wa[gunid].dangqiang > 0)
				{
					zidan[gunid][zidanshu[gunid]].flag = 0;
					if(Bob.direction == 1)
					{
						zidan[gunid][zidanshu[gunid]].x = sx + 25;
						zidan[gunid][zidanshu[gunid]].y = sy - 70;
						zidan[gunid][zidanshu[gunid]].vx = 0;
						zidan[gunid][zidanshu[gunid]].vy = -6;
					}
					else if(Bob.direction == 2)
					{
						zidan[gunid][zidanshu[gunid]].x = sx - 25;
						zidan[gunid][zidanshu[gunid]].y = sy - 70;
						zidan[gunid][zidanshu[gunid]].vx = -7;
						zidan[gunid][zidanshu[gunid]].vy = -6;
					}
					else if(Bob.direction == 3)
					{
						zidan[gunid][zidanshu[gunid]].x = sx - 45;
						zidan[gunid][zidanshu[gunid]].y = sy - 30;
						zidan[gunid][zidanshu[gunid]].vx = -7;
						zidan[gunid][zidanshu[gunid]].vy = 0;
					}
					else if(Bob.direction == 4)
					{
						zidan[gunid][zidanshu[gunid]].x = sx - 20;
						zidan[gunid][zidanshu[gunid]].y = sy - 20;
						zidan[gunid][zidanshu[gunid]].vx = -7;
						zidan[gunid][zidanshu[gunid]].vy = 6;
					}
					else if(Bob.direction == 5)
					{
						zidan[gunid][zidanshu[gunid]].x = sx - 30;
						zidan[gunid][zidanshu[gunid]].y = sy + 40;
						zidan[gunid][zidanshu[gunid]].vx = 0;
						zidan[gunid][zidanshu[gunid]].vy = 6;
					}
					else if(Bob.direction == 6)
					{
						zidan[gunid][zidanshu[gunid]].x = sx + 50;
						zidan[gunid][zidanshu[gunid]].y = sy;
						zidan[gunid][zidanshu[gunid]].vx = 7;
						zidan[gunid][zidanshu[gunid]].vy = 6;
					}
					else if(Bob.direction == 7)
					{
						zidan[gunid][zidanshu[gunid]].x = sx + 50;
						zidan[gunid][zidanshu[gunid]].y = sy - 30;
						zidan[gunid][zidanshu[gunid]].vx = 7;
						zidan[gunid][zidanshu[gunid]].vy = 0;
					}
					else if(Bob.direction == 8)
					{
						zidan[gunid][zidanshu[gunid]].x = sx + 10;
						zidan[gunid][zidanshu[gunid]].y = sy - 50;
						zidan[gunid][zidanshu[gunid]].vx = 7;
						zidan[gunid][zidanshu[gunid]].vy = -6;
					}
					zidan[gunid][zidanshu[gunid]].vx *= 4;
					zidan[gunid][zidanshu[gunid]].vy *= 4;
					zidan[gunid][zidanshu[gunid]].dir = Bob.direction;
					zidanshu[gunid] ++;
					wa[gunid].dangqiang --;
					fashetime = 15;
					if(gunid == 1)
					{
						PlaySound(TEXT("sound/gun1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
					else
					{
						PlaySound(TEXT("sound/gun0.wav"), NULL, SND_FILENAME | SND_ASYNC);
					}
				}
				
			}
			else if(GetAsyncKeyState(VK_NUMPAD0))//切换武器
			{
				fashetime = 15;
				gunid = !gunid;
				PlaySound(TEXT("sound/qieqiang.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}	
			else if(GetAsyncKeyState(VK_CONTROL)) //装填子弹
			{
				fashetime = 15;
				tiandan = 1;
				PlaySound(TEXT("sound/tiandan.wav"), NULL, SND_FILENAME | SND_ASYNC);
				
			}		
			break;
		}
	case WM_MOUSEMOVE:
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if(menu >= 1)
			{
				if(mx >= 350 && mx <= 715 && my >= 540 && my <= 695)
				{
					//	hcursor = LoadCursor(NULL, IDC_NO);
					hcursor = 	LoadCursorFromFile("image/hand.cur");
					SetCursor(hcursor);					
				}
				else if(mx >= 990 && mx <= 1050 && my >= 0 && my <= 50)
				{
					hcursor = 	LoadCursorFromFile("image/hand.cur");
					SetCursor(hcursor);	
				}
				else
				{
					hcursor = 	LoadCursorFromFile("image/hand2.cur");
					SetCursor(hcursor);	
				}
			}
			else
			{
				if(Bob.blood > 0)
				{
					if(mx >= 939 && mx <= 1050 && my >= 0 && my <= 34)
					{
						hcursor = 	LoadCursorFromFile("image/back.cur");
						SetCursor(hcursor);
					}
					else
					{
						hcursor = 	LoadCursorFromFile("image/wahaha.cur");
						SetCursor(hcursor);
					}
				}
				else
				{
					if(mx >= 380 && mx <= 486 && my >= 780 && my <= 840)
					{
						hcursor = 	LoadCursorFromFile("image/fire.cur");
						SetCursor(hcursor);
					}
					else if(mx >= 560 && mx <= 665 && my >= 780 && my <= 839)
					{
						hcursor = 	LoadCursorFromFile("image/fire.cur");
						SetCursor(hcursor);
					}
					else
					{
						hcursor = 	LoadCursorFromFile("image/wahaha.cur");
						SetCursor(hcursor);
					}
				}
			
			}
			break;
		}
		
	case WM_LBUTTONDOWN:
		{
			mx = LOWORD(lParam);
			my = HIWORD(lParam);
			if(pause == 1)
				break;
			if(menu >= 1)
			{
				if(mx >= 350 && mx <= 715 && my >= 540 && my <= 695)
				{
					
					menu = 0;					
				}
				else if(mx >= 990 && mx <= 1050 && my >= 0 && my <= 50)
				{
					DestroyWindow(hwnd);
				}
			}
			else
			{
				if(Bob.blood > 0)
				{
					if(mx >= 939 && mx <= 1050 && my >= 0 && my <= 34)
					{
						menu = 1;
						flag = 1;
						wave = 0;
					}
				}
				else
				{
					if(mx >= 380 && mx <= 486 && my >= 780 && my <= 840)
					{
						menu = 1;
						flag = 1;
						wave = 0;
					}
					else if(mx >= 560 && mx <= 665 && my >= 780 && my <= 839)
					{
						flag = 1;
						wave = 0;
					}
				}
			}
			break;
		}
	case WM_KEYUP:
		{
			if(pause == 0)
			{
				InvalidateRect(hwnd, NULL, 0);
				break;
			}
		}
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		fashetime --;
		if(menu >= 1)
		{
			BitBlt(hdcmem, bx, by,  bitmap[503].bmWidth, bitmap[503].bmHeight,hmemdc[503], 0, 0, SRCCOPY);
			if(menu == 1)
				PlaySound(TEXT("sound/menu.wav"), NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
			if(menu > 1000)
				menu = 100;
			menu ++;
			TransparentBlt(hdcmem, bx + 995 , by ,bitmap[505].bmWidth, bitmap[505].bmHeight,hmemdc[505], 0, 0,bitmap[505].bmWidth , bitmap[505].bmHeight, RGB(0, 0, 0));
		}
		else
		{
			if(Bob.blood > 0)
			{
				tt ++;
				if(tt % 20 == 0)
				{
					mciSendString("open sound/bk.wav type MPEGVideo alias mci",0,0,0); 
					mciSendString("play mci repeat",0,0,0);
				}
			}
			ans = 0;
			for(i = 1; i <= num[wave]; ++ i)
			{
				if(smg[i].blood > 0)
				{
					ans = 1;
					break;
				}
			}
			for(i = 1; i <= wave; ++ i)
			{
				if(boss[i].blood > 0)
				{
					ans = 1;
					break;
				}
			}
			if(!ans)
			{
				flag = 1;
			}
			if(tiandan > 0) //填子弹
			{
				tiandan ++;
				if(tiandan > 20)
				{
					tiandan = 0;
					if(wa[gunid].keyong > 0)
					{
						if(gunid == 0)
						{
							wa[gunid].keyong -= 2 - wa[gunid].dangqiang;
							wa[gunid].dangqiang = 2;
						}
						else
						{
							wa[gunid].keyong -= 6 - wa[gunid].dangqiang;
							wa[gunid].dangqiang = 6;
						}
					}
				}
			}
			
			Bob.direction = dir;
			Bob.save++;
			if(Bob.save > 35 && Bob.blood < 100)
			{
				Bob.blood ++;
			}
				
			BitBlt(hdcmem, 0, 0,  bitmap[501].bmWidth, bitmap[501].bmHeight,hmemdc[501], 0, 0, SRCCOPY);
			
			//血块
			for(i = 0; i < xuenum; ++ i)
			{
				TransparentBlt(hdcmem, xue[i].x , xue[i].y ,bitmap[206].bmWidth, bitmap[206].bmHeight,hmemdc[206], 0, 0,bitmap[206].bmWidth , bitmap[206].bmHeight, RGB(255, 255, 255));
				
			}
			//盒子
			
			for(i = 0 ; i < boxnum; ++ i)
			{
				if(box[i].time < 100)
				{
					if(fabs(sx - box[i].x) < 50 && fabs(sy - box[i].y) < 50)
					{
						box[i].time += 200;
						wa[0].keyong += 2;
						wa[1].keyong += 10;
						PlaySound(TEXT("sound/eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
						continue;
					}
					TransparentBlt(hdcmem, box[i].x , box[i].y ,bitmap[201].bmWidth, bitmap[201].bmHeight,hmemdc[201], 0, 0,bitmap[201].bmWidth , bitmap[201].bmHeight, RGB(255, 255, 255));
					box[i].time ++;
				}
			}
				//子弹
			for(i = 0; i <= gunid; ++ i)
			{
				for(j = 0; j < zidanshu[i]; ++ j)
				{
					if(i == 1)
					{
						if(zidan[i][j].flag < 50)
						{
							if(i == 1)
							{
								zidan[i][j].flag ++;
								for(k = 1; k <= num[wave]; ++ k)
								{
									if(fabs(zidan[i][j].x - smg[k].x) < 40 && fabs(zidan[i][j].y - smg[k].y) < 40 && smg[k].blood > 0)
									{
										zidan[i][j].flag += 100;
										smg[k].blood -= 10;
										
									}
								}
								for(k = 1; k <= wave; ++ k)
								{
									if(fabs(zidan[i][j].x - boss[k].x) < 40 && fabs(zidan[i][j].y - boss[k].y) < 40 && boss[k].blood > 0)
									{
										zidan[i][j].flag += 100;
										boss[k].blood -= 10;
									}
								}
								zidan[i][j].x += zidan[i][j].vx;
								zidan[i][j].y += zidan[i][j].vy;
								TransparentBlt(hdcmem, zidan[i][j].x , zidan[i][j].y ,bitmap[i + 300].bmWidth, bitmap[i + 300].bmHeight,hmemdc[i + 300], 0, 0,bitmap[i + 300].bmWidth , bitmap[i + 300].bmHeight, RGB(255, 255, 255));
							}
						}
					}
					else
					{
						if(zidan[i][j].flag < 50)
						{
							zidan[i][j].flag ++;
							ans = 0;
							for(k = 1; k <= num[wave]; ++ k)
							{
								if(fabs(zidan[i][j].x - smg[k].x) < 120 && fabs(zidan[i][j].y - smg[k].y) < 120 && smg[k].blood > 0)
								{
									ans = 1;
									smg[k].blood -= 20;
									
								}
							}
							for(k = 1; k <= wave; ++ k)
							{
								if(fabs(zidan[i][j].x - boss[k].x) < 120 && fabs(zidan[i][j].y - boss[k].y) < 120 && boss[k].blood > 0)
								{
									ans = 1;
									boss[k].blood -= 20;
									
								}
							}
							if(ans)
							{
								zidan[i][j].flag += 100;
							}
							zidan[i][j].x += zidan[i][j].vx;
							zidan[i][j].y += zidan[i][j].vy;
							TransparentBlt(hdcmem, zidan[i][j].x , zidan[i][j].y ,bitmap[zidan[i][j].dir + 301].bmWidth, bitmap[zidan[i][j].dir + 301].bmHeight,hmemdc[zidan[i][j].dir + 301], 0, 0,bitmap[zidan[i][j].dir + 301].bmWidth , bitmap[zidan[i][j].dir + 301].bmHeight, RGB(255, 255, 255));
							
						}
						else 
						{
							if(zidan[i][j].time <= 20)
							{
								
								TransparentBlt(hdcmem, zidan[i][j].x - 150, zidan[i][j].y - 150 ,bitmap[208 + zidan[i][j].time / 2].bmWidth, bitmap[208 + zidan[i][j].time / 2].bmHeight,hmemdc[208 + zidan[i][j].time / 2], 0, 0,bitmap[208 + zidan[i][j].time / 2].bmWidth , bitmap[208 + zidan[i][j].time / 2].bmHeight, RGB(255, 255, 255));
								zidan[i][j].time ++;	
							}
							if(zidan[i][j].time == 1)
							{
								PlaySound(TEXT("sound/boom.wav"), NULL, SND_FILENAME | SND_ASYNC);
								
							}
						}
						
					}
				}
				
			}
			//僵尸
			if(waittime < 30)
			{
				if(waittime == 1)
					PlaySound(TEXT("sound/come.wav"), NULL, SND_FILENAME | SND_ASYNC);
				waittime ++;
				hf = CreateFont(
					200,
					100,
					0,
					0,
					FW_HEAVY,
					0,
					0,
					0,
					GB2312_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH|FF_DONTCARE,
					"粗体字"
					);
				SelectObject(hdcmem, hf);
				SetTextColor(hdcmem, RGB(255, 0, 0));
				SetBkMode(hdcmem, TRANSPARENT);
				TextOut(hdcmem, bx + 300, by + 300, s1, strlen(s1));
				sprintf(s2, "%d", wave);
				TextOut(hdcmem, bx + 500, by + 300, s2, strlen(s2));
				TextOut(hdcmem, bx + 600, by + 300, s3, strlen(s3));
				DeleteObject(hf);
				
			}
			else
			{
				
				chuli1();
				for(i = 1; i <= num[wave]; ++ i)
				{
					if(smg[i].blood > 0)
					{
						if(!smg[i].attack)
						{
							TransparentBlt(hdcmem, smg[i].x , smg[i].y ,bitmap[smg[i].direction * 2 + 16 - smg[i].sz].bmWidth, bitmap[smg[i].direction * 2 + 16 - smg[i].sz].bmHeight,hmemdc[smg[i].direction * 2 + 16 - smg[i].sz], 0, 0,bitmap[smg[i].direction * 2 + 16 - smg[i].sz].bmWidth , bitmap[smg[i].direction * 2 + 16 - smg[i].sz].bmHeight, RGB(255, 255, 255));
							smg[i].sz = !smg[i].sz;
						}
						else
						{
							TransparentBlt(hdcmem, smg[i].x , smg[i].y ,bitmap[smg[i].direction * 2 + 100 - smg[i].sz].bmWidth, bitmap[smg[i].direction * 2 + 100 - smg[i].sz].bmHeight,hmemdc[smg[i].direction * 2 + 100 - smg[i].sz], 0, 0,bitmap[smg[i].direction * 2 + 100 - smg[i].sz].bmWidth , bitmap[smg[i].direction * 2 + 16 - smg[i].sz].bmHeight, RGB(255, 255, 255));
							smg[i].sz = !smg[i].sz;
							Bob.blood -=1;
							Bob.save = 0;
							if(Bob.blood > 0 && fashetime <= 0)
								PlaySound(TEXT("sound/attack.wav"), NULL, SND_FILENAME | SND_ASYNC);
						}
					}
					else
					{
						if(smg[i].dead < 20)
						{
							smg[i].dead ++;
							TransparentBlt(hdcmem, smg[i].x , smg[i].y ,bitmap[(smg[i].direction + 1) / 2 + 150].bmWidth, bitmap[(smg[i].direction + 1) / 2 + 150].bmHeight,hmemdc[(smg[i].direction + 1) / 2 + 150], 0, 0,bitmap[(smg[i].direction + 1) / 2 + 150].bmWidth , bitmap[(smg[i].direction + 1) / 2 + 150].bmHeight, RGB(255, 255, 255));
							if(smg[i].dead == 1)
							{
								xue[xuenum].x = smg[i].x;
								xue[xuenum++].y = smg[i].y;
								srand( (unsigned)time( NULL ));
								lucay = rand() % 100;
								
								fenshu += smg[i].fra;
								if(lucay < 30)
								{
									box[boxnum].x = smg[i].x;
									box[boxnum].y = smg[i].y;
									box[boxnum++].time = 0;
								}
							}
							hf = CreateFont(
								20,
								10,
								0,
								0,
								FW_HEAVY,
								0,
								0,
								0,
								GB2312_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								DEFAULT_PITCH|FF_DONTCARE,
								"粗体字"
								);
							SelectObject(hdcmem, hf);
							SetTextColor(hdcmem, RGB(255, 255, 255));
							SetBkMode(hdcmem, TRANSPARENT);
							sprintf(s, "+%d", smg[i].fra);
							TextOut(hdcmem, smg[i].x + 50, smg[i].y - 20, s, strlen(s));
							DeleteObject(hf);												
							
						}
					}
				}
				
					chuli2();
				for(i = 1; i <= wave; ++ i)
				{
					if(boss[i].blood > 0)
					{
						TransparentBlt(hdcmem, boss[i].x , boss[i].y ,bitmap[boss[i].direction * 2 + 32 - boss[i].sz].bmWidth, bitmap[boss[i].direction * 2 + 32 - boss[i].sz].bmHeight,hmemdc[boss[i].direction * 2 + 32 - boss[i].sz], 0, 0,bitmap[boss[i].direction * 2 + 32 - boss[i].sz].bmWidth , bitmap[boss[i].direction * 2 + 32 - boss[i].sz].bmHeight, RGB(255, 255, 255));
						if(!boss[i].attack)
						{
							boss[i].sz = !boss[i].sz;
						}
						else
						{
							if(boss[i].tim  % 15 == 0)
							{
								att[cirnum].flag = 1;
								att[cirnum].x = boss[i].x;
								att[cirnum].y = boss[i].y;
								att[cirnum].vx = (sx - boss[i].x) / 30;
								att[cirnum].vy = (sy - boss[i].y) / 30;
								cirnum++;
							}
							boss[i].tim ++;
							boss[i].tim %= 15;
						}
					}
					else
					{
						if(boss[i].timefen < 20)
						{
							if(boss[i].timefen == 1)
							{
								xue[xuenum].x = boss[i].x;
								xue[xuenum++].y = boss[i].y;
								box[boxnum].x = boss[i].x;
								box[boxnum].y = boss[i].y;
								box[boxnum++].time = 0;	
								fenshu += boss[i].fra;
							}		
						
							hf = CreateFont(
								20,
								10,
								0,
								0,
								FW_HEAVY,
								0,
								0,
								0,
								GB2312_CHARSET,
								OUT_DEFAULT_PRECIS,
								CLIP_DEFAULT_PRECIS,
								DEFAULT_QUALITY,
								DEFAULT_PITCH|FF_DONTCARE,
								"粗体字"
								);
							SelectObject(hdcmem, hf);
							SetTextColor(hdcmem, RGB(255, 255, 255));
							SetBkMode(hdcmem, TRANSPARENT);
							sprintf(s, "+%d", boss[i].fra);
							TextOut(hdcmem, boss[i].x + 50, boss[i].y - 20, s, strlen(s));
							DeleteObject(hf);
							boss[i].timefen ++;
						}
					}
					
				}
			
				for(i = 0; i < cirnum; ++ i)
				{
					if(att[i].flag < 40)
					{
						att[i].flag++;			
						if(fabs(att[i].x - sx) < 50 && fabs(att[i].y - sy) < 50)
						{
							att[i].flag += 100;
							Bob.blood -= 19;
							Bob.save = 0;
						}
						att[i].x += att[i].vx;
						att[i].y += att[i].vy;
						TransparentBlt(hdcmem, att[i].x , att[i].y ,bitmap[203].bmWidth, bitmap[203].bmHeight,hmemdc[203], 0, 0,bitmap[203].bmWidth , bitmap[203].bmHeight, RGB(255, 255, 255));
						
					}
				}
			}
			TransparentBlt(hdcmem, sx , sy ,bitmap[Bob.direction * 2 - sz].bmWidth, bitmap[Bob.direction * 2 - sz].bmHeight,hmemdc[Bob.direction * 2 - sz], 0, 0,bitmap[Bob.direction * 2 - sz].bmWidth , bitmap[Bob.direction * 2 - sz].bmHeight, RGB(255, 255, 255));
		
			//back menu
			BitBlt(hdcmem, bx + 939, by,  bitmap[504].bmWidth, bitmap[504].bmHeight,hmemdc[504], 0, 0, SRCCOPY);
			//枪
			TransparentBlt(hdcmem, bx , by ,bitmap[310 + gunid].bmWidth, bitmap[310 + gunid].bmHeight,hmemdc[310 + gunid], 0, 0,bitmap[310 + gunid].bmWidth , bitmap[310 + gunid].bmHeight, RGB(255, 255, 255));
			hf = CreateFont(
				20,
				10,
				0,
				0,
				FW_HEAVY,
				0,
				0,
				0,
				GB2312_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH|FF_DONTCARE,
				"粗体字"
				);
			SelectObject(hdcmem, hf);
			SetTextColor(hdcmem, RGB(255, 0, 0));
			SetBkColor(hdcmem, RGB(0, 255, 0));
			SetBkMode(hdcmem, OPAQUE);
			sprintf(s4, "%d", wa[gunid].dangqiang);
			sprintf(s6, "%d", wa[gunid].keyong);
			TextOut(hdcmem, bx + 50, by + 150, s4, strlen(s4));
			TextOut(hdcmem, bx + 62, by + 150, s5, strlen(s5));
			TextOut(hdcmem, bx + 72, by + 150, s6, strlen(s6));
			DeleteObject(hf);
			//血条
			if(Bob.blood < 0)
				Bob.blood = 0;
			hpen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			SelectObject(hdcmem, hpen);
			if(Bob.blood > 50)
				hbr = CreateSolidBrush(RGB(0, 255, 0));
			else
				hbr = CreateSolidBrush(RGB(255, 0, 0));
			
			SelectObject(hdcmem, hbr);
			
			Rectangle(hdcmem, sx + 10, sy, sx + 10 + 120 * Bob.blood / 100.0, sy + 15);
			DeleteObject( hbr);
			hbr = CreateSolidBrush(RGB(255, 255, 255));
			SelectObject(hdcmem, hbr);
			
			Rectangle(hdcmem, sx + 10 + 120 * Bob.blood / 100.0, sy, sx + 130, sy + 15);
			DeleteObject( hbr);
			DeleteObject( hpen);
			//分数
				hf = CreateFont(
				60,
				30,
				0,
				0,
				FW_HEAVY,
				0,
				0,
				0,
				GB2312_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH|FF_DONTCARE,
				"粗体字"
				);
			SelectObject(hdcmem, hf);
			SetTextColor(hdcmem, RGB(255, 255, 255));
			SetBkMode(hdcmem, TRANSPARENT);
			sprintf(s, "%012d", fenshu);
			TextOut(hdcmem, bx + 350, by + 20, s, strlen(s));
			DeleteObject(hf);
			if(Bob.blood <= 0)
			{
				mciSendString("stop mci", NULL, 0, NULL);
				fail ++;
				BitBlt(hdcmem, bx, by, 1050, 900,hmemdc[502], 0, 0, SRCCOPY);
				hf = CreateFont(
				40,
				20,
				0,
				0,
				FW_HEAVY,
				0,
				0,
				0,
				GB2312_CHARSET,
				OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH|FF_DONTCARE,
				"粗体字"
				);
				SelectObject(hdcmem, hf);
				SetTextColor(hdcmem, RGB(255, 255, 255));
				SetBkMode(hdcmem, TRANSPARENT);
				len = (log(fenshu * 1.0) / log(2.0)) + 1;
				sprintf(s, "%d", fenshu);
				sprintf(s9, "存活%d波", wave - 1);
				TextOut(hdcmem, bx + 420, by + 570, s8, strlen(s8));
				TextOut(hdcmem, bx + 525 - len * 4, by + 620, s, strlen(s));
				TextOut(hdcmem, bx + 450, by + 670, s9, strlen(s9));
				if(fail == 1)
					PlaySound(TEXT("sound/fail.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
		}
		if(pause == 1)
		{
			mciSendString("pause mci", NULL, 0, NULL);
			TransparentBlt(hdcmem, bx + 350 , by + 375 ,bitmap[506].bmWidth, bitmap[506].bmHeight,hmemdc[506], 0, 0,bitmap[506].bmWidth , bitmap[506].bmHeight, RGB(255, 255, 255));
		}
		BitBlt(hdc, 0, 0,  1050, 900,hdcmem, bx, by, SRCCOPY);
		EndPaint(hwnd, &ps);
		if(pause == 0)
		{
			//if(patime >= 0)
			{
				patime --;
			}
			
			Sleep(100);
			InvalidateRect(hwnd, NULL, 0);			
		}
		break;
	case WM_DESTROY:
		mciSendString("play mci fullscreen",0,0,0);
		PostQuitMessage(0);
	default:
		return DefWindowProc(hwnd,message,wParam,lParam);
	}
	return 0;
}

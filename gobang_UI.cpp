#include"gobang_UI.h"
#include <math.h>
int get_pri(IMAGE& board) {
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
	setfillcolor(WHITE);
	fillrectangle(0, 190, 530, 310);


	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;						// 设置字体高度为 25
	_tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿 
	settextstyle(&f);
	settextcolor(RGB(10, 10, 10));
	setbkcolor(WHITE);
	outtextxy(150, 200, _T("请选择先出棋的一方"));

	setfillcolor(WHITE);
	setlinecolor(RGB(150, 150, 150));
	int x = 18;
	int y = 250;
	setfillcolor(RGB(26, 173, 26));
	fillrectangle(x + 1 * 128, y, x + 1 * 128 + 110, y + 50);
	setbkcolor(RGB(26, 173, 26));
	outtextxy(x + 1 * 128 + 30, y + 13, _T("玩家"));
	setfillcolor(RGB(26, 173, 26));
	fillrectangle(x + 2 * 128, y, x + 2 * 128 + 110, y + 50);
	setbkcolor(RGB(26, 173, 26));
	outtextxy(x + 2 * 128 + 48, y + 13, _T("AI"));
	MOUSEMSG msg;
	while (true) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN) {
			if (msg.x >= x + 1 * 128 && msg.y >= y && msg.x <= x + 1 * 128 + 110 && msg.y <= y + 50) {
				putimage(0, 0, &board);
				return 1;
			}
			if (msg.x >= x + 2 * 128 && msg.y >= y && msg.x <= x + 2 * 128 + 110 && msg.y <= y + 50) {
				putimage(0, 0, &board);
				return -1;
			}
		}
	}

	return 0;
}


void init_UI(IMAGE& board)
{
	initgraph(530, 680);//找到的棋盘图片宽度为530
	//加载棋盘图片并打印出来
	setbkcolor(WHITE);
	cleardevice();
	loadimage(&board, _T(Path));
	putimage(0, 0, &board);
	//打印4个按键
	int init_x = 0 + 18, init_y = 530 + 80;	//用棋盘的左下角为起点计算四个按键的坐标
	setfillcolor(RGB(26, 173, 26));
	setlinecolor(RGB(26, 173, 26));
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
	fillrectangle(init_x + 0 * 128, init_y, init_x + 0 * 128 + 110, init_y + 50);	//按键横宽110，纵长50

	setfillcolor(RGB(236, 139, 137));
	setlinecolor(RGB(236, 139, 137));
	fillrectangle(init_x + 1 * 128, init_y, init_x + 1 * 128 + 110, init_y + 50);

	setfillcolor(RGB(59, 90, 223));
	setlinecolor(RGB(59, 90, 223));
	fillrectangle(init_x + 2 * 128, init_y, init_x + 2 * 128 + 110, init_y + 50);

	setfillcolor(RGB(200, 200, 70));
	setlinecolor(RGB(200, 200, 70));
	fillrectangle(init_x + 3 * 128, init_y, init_x + 3 * 128 + 110, init_y + 50);

	//按键文字
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;						// 设置字体高度为 25
	_tcscpy_s(f.lfFaceName, _T("黑体"));	// 设置字体为“黑体”(高版本 VC 推荐使用 _tcscpy_s 函数)
	f.lfQuality = ANTIALIASED_QUALITY;		// 设置输出效果为抗锯齿 
	settextstyle(&f);
	settextcolor(WHITE);
	setbkcolor(RGB(26, 173, 26));
	outtextxy(init_x + 0 * 128 + 30, init_y + 13, _T("开始"));
	setbkcolor(RGB(236, 139, 137));
	outtextxy(init_x + 1 * 128 + 30, init_y + 13, _T("认输"));
	setbkcolor(WHITE);


	setbkcolor(RGB(59, 90, 223));
	outtextxy(init_x + 2 * 128 + 30, init_y + 13, _T("悔棋"));
	setbkcolor(RGB(200, 200, 70));
	outtextxy(init_x + 3 * 128 + 30, init_y + 13, _T("重做"));
	//以上内容打印完之后，等待开始指令
	while (true) {
		int ord = get_order();
		if (ord == 1)
			break;
	}
}
/*
画棋盘和棋子
参数表：
map:棋盘
white:棋盘中代表白子的数
black:棋盘中代表黑子的数
none:棋盘中代表无子的数
clear:表示是否需要清空棋盘，如果不设置，什么情况都重新加载棋盘背景图片，会造成闪动
!需要清空棋盘的情况只有悔棋，其他情况不需要设置该参数
*/
void draw_pieces(vector<vector<int>> map, int white, int black, int none, IMAGE& board, bool clear)
{
	//初始化图形界面时已加载过图片到board变量中，直接使用
	if (clear == true)
		putimage(0, 0, &board);
	//打印棋子
	setlinestyle(3);
	setfillcolor(WHITE);
	//solidcircle(21, 23, 15);//(x,y)=(21,23)为棋盘左上角坐标,一格间距36
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (map[i][j] == white)
				setfillcolor(WHITE);
			else if (map[i][j] == black)
				setfillcolor(BLACK);
			if (map[i][j] != none) {
				solidcircle(21 + j * 35, 23 + i * 35, 14);
			}
		}
	}
	setbkcolor(WHITE);
	outtextxy(20, 560, "                                                                          ");
}


//获取鼠标信息
pos get_order(vector<vector<int>>& map)
{
	pos hit;
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		//表示点的是棋子
		if (msg.uMsg == WM_LBUTTONDOWN) {//如果按下左键
			//计算在哪一个点
			int x1 = msg.x, y1 = msg.y, x2, y2;
			for (int i = 0; i < 15; i++) {	//找点
				for (int j = 0; j < 15; j++) {
					x2 = 21 + j * 35;
					y2 = 23 + i * 35;
					int distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
					if (distance <= 15)
						return pos(i,j);
				}
			}


			//如果点的不是棋子，则看是点击了界面下方哪一个按键
				if (msg.x >= 18 + 0 * 128 && msg.x <= 18 + 0 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1,1);//点的是第一个
				if (msg.x >= 18 + 1 * 128 && msg.x <= 18 + 1 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 2);//点的是第二个

				if (msg.x >= 18 + 2 * 128 && msg.x <= 18 + 2 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 3);//点的是第三个

				if (msg.x >= 18 + 3 * 128 && msg.x <= 18 + 3 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 4);//点的是第四个
		}
	}
	return pos(-3,-3);
}


//获取鼠标信息
int get_order()
{
	MOUSEMSG msg;
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN) {//如果按下左键
		if (msg.x >= 18 + 0 * 128 && msg.x <= 18 + 0 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 1;//点的是第一个
		if (msg.x >= 18 + 1 * 128 && msg.x <= 18 + 1 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 2;//点的是第二个

		if (msg.x >= 18 + 2 * 128 && msg.x <= 18 + 2 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 3;//点的是第三个

		if (msg.x >= 18 + 3 * 128 && msg.x <= 18 + 3 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 4;//点的是第四个
	}
}


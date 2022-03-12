#include"gobang_UI.h"
#include <math.h>
int get_pri(IMAGE& board) {
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
	setfillcolor(WHITE);
	fillrectangle(0, 190, 530, 310);


	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;						// ��������߶�Ϊ 25
	_tcscpy_s(f.lfFaceName, _T("����"));	// ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ����� 
	settextstyle(&f);
	settextcolor(RGB(10, 10, 10));
	setbkcolor(WHITE);
	outtextxy(150, 200, _T("��ѡ���ȳ����һ��"));

	setfillcolor(WHITE);
	setlinecolor(RGB(150, 150, 150));
	int x = 18;
	int y = 250;
	setfillcolor(RGB(26, 173, 26));
	fillrectangle(x + 1 * 128, y, x + 1 * 128 + 110, y + 50);
	setbkcolor(RGB(26, 173, 26));
	outtextxy(x + 1 * 128 + 30, y + 13, _T("���"));
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
	initgraph(530, 680);//�ҵ�������ͼƬ���Ϊ530
	//��������ͼƬ����ӡ����
	setbkcolor(WHITE);
	cleardevice();
	loadimage(&board, _T(Path));
	putimage(0, 0, &board);
	//��ӡ4������
	int init_x = 0 + 18, init_y = 530 + 80;	//�����̵����½�Ϊ�������ĸ�����������
	setfillcolor(RGB(26, 173, 26));
	setlinecolor(RGB(26, 173, 26));
	setlinestyle(PS_SOLID | PS_JOIN_ROUND, 1);
	fillrectangle(init_x + 0 * 128, init_y, init_x + 0 * 128 + 110, init_y + 50);	//�������110���ݳ�50

	setfillcolor(RGB(236, 139, 137));
	setlinecolor(RGB(236, 139, 137));
	fillrectangle(init_x + 1 * 128, init_y, init_x + 1 * 128 + 110, init_y + 50);

	setfillcolor(RGB(59, 90, 223));
	setlinecolor(RGB(59, 90, 223));
	fillrectangle(init_x + 2 * 128, init_y, init_x + 2 * 128 + 110, init_y + 50);

	setfillcolor(RGB(200, 200, 70));
	setlinecolor(RGB(200, 200, 70));
	fillrectangle(init_x + 3 * 128, init_y, init_x + 3 * 128 + 110, init_y + 50);

	//��������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 25;						// ��������߶�Ϊ 25
	_tcscpy_s(f.lfFaceName, _T("����"));	// ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
	f.lfQuality = ANTIALIASED_QUALITY;		// �������Ч��Ϊ����� 
	settextstyle(&f);
	settextcolor(WHITE);
	setbkcolor(RGB(26, 173, 26));
	outtextxy(init_x + 0 * 128 + 30, init_y + 13, _T("��ʼ"));
	setbkcolor(RGB(236, 139, 137));
	outtextxy(init_x + 1 * 128 + 30, init_y + 13, _T("����"));
	setbkcolor(WHITE);


	setbkcolor(RGB(59, 90, 223));
	outtextxy(init_x + 2 * 128 + 30, init_y + 13, _T("����"));
	setbkcolor(RGB(200, 200, 70));
	outtextxy(init_x + 3 * 128 + 30, init_y + 13, _T("����"));
	//�������ݴ�ӡ��֮�󣬵ȴ���ʼָ��
	while (true) {
		int ord = get_order();
		if (ord == 1)
			break;
	}
}
/*
�����̺�����
������
map:����
white:�����д�����ӵ���
black:�����д�����ӵ���
none:�����д������ӵ���
clear:��ʾ�Ƿ���Ҫ������̣���������ã�ʲô��������¼������̱���ͼƬ�����������
!��Ҫ������̵����ֻ�л��壬�����������Ҫ���øò���
*/
void draw_pieces(vector<vector<int>> map, int white, int black, int none, IMAGE& board, bool clear)
{
	//��ʼ��ͼ�ν���ʱ�Ѽ��ع�ͼƬ��board�����У�ֱ��ʹ��
	if (clear == true)
		putimage(0, 0, &board);
	//��ӡ����
	setlinestyle(3);
	setfillcolor(WHITE);
	//solidcircle(21, 23, 15);//(x,y)=(21,23)Ϊ�������Ͻ�����,һ����36
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


//��ȡ�����Ϣ
pos get_order(vector<vector<int>>& map)
{
	pos hit;
	MOUSEMSG msg;
	while (1) {
		msg = GetMouseMsg();
		//��ʾ���������
		if (msg.uMsg == WM_LBUTTONDOWN) {//����������
			//��������һ����
			int x1 = msg.x, y1 = msg.y, x2, y2;
			for (int i = 0; i < 15; i++) {	//�ҵ�
				for (int j = 0; j < 15; j++) {
					x2 = 21 + j * 35;
					y2 = 23 + i * 35;
					int distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
					if (distance <= 15)
						return pos(i,j);
				}
			}


			//�����Ĳ������ӣ����ǵ���˽����·���һ������
				if (msg.x >= 18 + 0 * 128 && msg.x <= 18 + 0 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1,1);//����ǵ�һ��
				if (msg.x >= 18 + 1 * 128 && msg.x <= 18 + 1 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 2);//����ǵڶ���

				if (msg.x >= 18 + 2 * 128 && msg.x <= 18 + 2 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 3);//����ǵ�����

				if (msg.x >= 18 + 3 * 128 && msg.x <= 18 + 3 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
					return pos(-1, 4);//����ǵ��ĸ�
		}
	}
	return pos(-3,-3);
}


//��ȡ�����Ϣ
int get_order()
{
	MOUSEMSG msg;
	msg = GetMouseMsg();
	if (msg.uMsg == WM_LBUTTONDOWN) {//����������
		if (msg.x >= 18 + 0 * 128 && msg.x <= 18 + 0 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 1;//����ǵ�һ��
		if (msg.x >= 18 + 1 * 128 && msg.x <= 18 + 1 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 2;//����ǵڶ���

		if (msg.x >= 18 + 2 * 128 && msg.x <= 18 + 2 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 3;//����ǵ�����

		if (msg.x >= 18 + 3 * 128 && msg.x <= 18 + 3 * 128 + 110 && msg.y >= 580 + 15 && msg.y <= 580 + 15 + 70)
			return 4;//����ǵ��ĸ�
	}
}


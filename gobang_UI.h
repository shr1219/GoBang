#include <iostream>
#include <graphics.h>
#include<time.h>
#include <vector>


#define PI 3.14
#define Path "board.jpg"
using namespace std;
typedef pair<int, int>pos;
void init_UI(IMAGE& board);//��ʼ��
void draw_pieces(vector<vector<int>> map, int white, int black, int none, IMAGE& board, bool clear = false);//������ʾ

int get_pri(IMAGE& board);//ȷ������ѡ��,1Ϊ������ߣ�-1ΪAI����
pos get_order(vector<vector<int>>& map);//��ȡ�����Ϣ
int get_order();//��ȡ�����Ϣ����ʼ��ʱ��
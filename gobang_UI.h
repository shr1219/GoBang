#include <iostream>
#include <graphics.h>
#include<time.h>
#include <vector>


#define PI 3.14
#define Path "board.jpg"
using namespace std;
typedef pair<int, int>pos;
void init_UI(IMAGE& board);//初始化
void draw_pieces(vector<vector<int>> map, int white, int black, int none, IMAGE& board, bool clear = false);//界面显示

int get_pri(IMAGE& board);//确定优先选边,1为玩家先走，-1为AI先走
pos get_order(vector<vector<int>>& map);//获取鼠标信息
int get_order();//获取鼠标信息，初始化时用
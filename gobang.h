#include<vector>
#include<stack>
using namespace std;
typedef pair<int, int> pos;
#define PLAYER_TURN 0
#define AI_TURN 1
#define EMPTY_POS 0
#define PLAYER_POS 1
#define AI_POS 2


#define HORIZONTAL 0
#define VERTICAL 1
#define LEFT_CORNER 2
#define RIGHT_CORNER 3
#define ALL_DIRECTIONS 4


#define FIVE 10000000
#define FOUR 100000
#define BLOCK_FOUR 1000
#define THREE 1000
#define BLOCK_THREE 100
#define TWO 100
#define BLOCK_TWO 10
const double coe = 0.5;//自己棋形的系数
const int step = 4;
const vector<pos>directions = {
	{0,1},
	{1,0},
	{1,1},
	{1,-1}
};
class GoBang {
private:
	const int len = 15;
	vector<vector<int>>map;
	vector<vector<vector<int>>>player_score;
	vector<vector<vector<int>>>ai_score;
	int steps;
	bool turn;
	int p_val;
	int a_val;
	pos next_step;
	stack<pos>path;
public:
	GoBang();
	void get_move();
	void player_move();
	void ai_move();
	bool exist_neighbour(int a,int b,int r,int num);
	void search_pos(int a,int b,bool order,int direct);
	int evaluate_pos(int count, int blank, int block);
	void update_pos(int a, int b);
	int search(int depth, bool order, int alpha=INT_MIN, int beta=INT_MAX);//a-b剪枝
	bool game_over();
	void start();
	void undo();
	void restart();
};


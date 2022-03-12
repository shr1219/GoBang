#define _CRT_SECURE_NO_WARNINGS
#include"gobang.h"
#include"gobang_UI.h"
#include<iostream>
#include<time.h>
#include<stdlib.h>
#include<iomanip>
#include<queue>
using namespace std;

clock_t begin_clock;
clock_t end_clock;
int allnums=0;
//构造函数

GoBang::GoBang() {
	map = vector<vector<int>>(len, vector<int>(len, 0));
	player_score = vector<vector<vector<int>>>(len, vector<vector<int>>(len, vector<int>(5, 0)));
	ai_score = vector<vector<vector<int>>>(len, vector<vector<int>>(len, vector<int>(5, 0)));
	turn = PLAYER_TURN;
	p_val = a_val = 0;
	steps = 0;
}
//玩家落子
void GoBang::player_move() {
	while (1) {
		get_move();
		int x = next_step.first;
		int y = next_step.second;
		if (x == -1)
			break;
		if (x < 0 || x >= len || y < 0 || y>len)
			continue;
		if (map[x][y] == EMPTY_POS) {
			map[x][y] = PLAYER_POS;
			break;
		}
	}
}
//电脑落子
void GoBang::ai_move() {

	int x = next_step.first;
	int y = next_step.second;
	map[x][y] = AI_POS;//落子

}


bool GoBang::exist_neighbour(int a, int b, int r,int num) {
	int count = num;
	for (int i = -r; i <= r; ++i) {
		if (a + i < 0)
			continue;
		else if (a + i >= len)
			break;
		for (int j = -r; j <= r; ++j) {
			if (b + j < 0)
				continue;
			else if (b + j >= len)
				break;
			if (map[a + i][b + j] != EMPTY_POS)
				if(--count==0)
					return true;
		}
	}
	return false;

}



//获取玩家落子
void GoBang::get_move() {
	next_step = get_order(map);
}

//评估函数
void GoBang::search_pos(int a,int b,bool order,int direct) {
	//order：玩家或AI  direct方向 
	int target = (order == PLAYER_TURN) ? PLAYER_POS : AI_POS;
	if (direct == ALL_DIRECTIONS) {
		search_pos(a, b, order, HORIZONTAL);
		search_pos(a, b, order, VERTICAL);
		search_pos(a, b, order, LEFT_CORNER);
		search_pos(a, b, order, RIGHT_CORNER);
	}
	else {
		int count = 0;//已落子数
		int blank = -1;//空格位 初始-1代表没有读到空格
		int block = 0;//边界或对方落子数
		for (int i = 0; i < len; ++i) {
			int temp_a = a + i * directions[direct].first;
			int temp_b = b + i * directions[direct].second;
			if (temp_a < 0 || temp_a >= len || temp_b < 0 || temp_b >= len)
				break;//保证访问不越界

			if (map[temp_a][temp_b] == target) {
				++count;
				if (temp_a == 0 || temp_a == len - 1 || temp_b == 0 || temp_b == len - 1)//已在边界格上
					++block;
			}
			else if (map[temp_a][temp_b] == EMPTY_POS) {
				if (blank == -1) {//出现第一个空格，搜索该空格下一个格子
					int next_a = temp_a + directions[direct].first;
					int next_b = temp_b + directions[direct].second;
					if (next_a < 0 || next_a >= len || next_b < 0 || next_b >= len)
						break;//下一格不越界
					if (map[next_a][next_b] == target) {
						blank = count;//记录该空格位置
					}
					else
						break;
				}
				else
					break;
			}
			else {
				//此时该位置已有对方落子
				++block;
				break;
			}

		}
		for (int i = 1; i < len; ++i) {//从1开始
			int temp_a = a - i * directions[direct].first;
			int temp_b = b - i * directions[direct].second;
			if (temp_a < 0 || temp_a >= len || temp_b < 0 || temp_b >= len)
				break;//保证访问不越界

			if (map[temp_a][temp_b] == target) {
				++count;
				if (blank != -1)
					++blank;//blank随count同步改变位置
				if (temp_a == 0 || temp_a == len - 1 || temp_b == 0 || temp_b == len - 1)//已在边界格上
					++block;
			}
			else if (map[temp_a][temp_b] == EMPTY_POS) {
				if (blank == -1) {//出现第一个空格，搜索该空格下一个格子
					int next_a = temp_a - directions[direct].first;
					int next_b = temp_b - directions[direct].second;
					if (next_a < 0 || next_a >= len || next_b < 0 || next_b >= len)
						break;//下一格不越界
					if (map[next_a][next_b] == target) {
						blank = 0;//记录该空格位置
					}
					else
						break;
				}
				else
					break;
			}
			else {
				//此时该位置已有对方落子
				++block;
				break;
			}
		}

		if (order == PLAYER_TURN) {
			p_val -= player_score[a][b][direct];
			player_score[a][b][ALL_DIRECTIONS] -= player_score[a][b][direct];
			player_score[a][b][direct] = evaluate_pos(count, blank, block);
			player_score[a][b][ALL_DIRECTIONS] += player_score[a][b][direct];
			p_val += player_score[a][b][direct];
		}
		if (order == AI_TURN) {
			a_val -= ai_score[a][b][direct];
			ai_score[a][b][ALL_DIRECTIONS] -= ai_score[a][b][direct];
			ai_score[a][b][direct] = coe * evaluate_pos(count, blank, block);//乘以系数coe
			ai_score[a][b][ALL_DIRECTIONS] += ai_score[a][b][direct];
			a_val += ai_score[a][b][direct];
		}
	}
	
}

int GoBang::evaluate_pos(int count, int blank, int block) {
	if (blank <= 0) {
		if (count == 5)
			return FIVE;
		else if (block == 0) {
			if (count == 4)
				return FOUR;
			else if (count == 3)
				return THREE;
			else if (count == 2)
				return TWO;
			else
				return 0;
		}
		else if (block == 1) {
			if (count == 4)
				return BLOCK_FOUR;
			else if (count == 3)
				return BLOCK_THREE;
			else if (count == 2)
				return BLOCK_TWO;
			else
				return 0;
		}
		else
			return 0;//两边堵死
	}

	//blank大于1时，说明count的棋子被从中截断，根据截断位置和count确定棋形
	else if (blank ==  1 || blank == count - 1) {
		if (count >= 6) {
			return FIVE;//example:1011111
		}
		else if (block == 0) {
			switch (count) {
				case 2: return TWO;//101
				case 3: return THREE;//1011
				case 4: return BLOCK_FOUR;//10111
				case 5: return FOUR;//101111
			}
		}

		else if (block == 1) {
			switch (count) {
				case 2: return BLOCK_TWO;//1012
				case 3: return BLOCK_THREE;//10112
				case 4: return BLOCK_FOUR;//101112
				case 5: return BLOCK_FOUR;//1011112
			}
		}
	}
	else if (blank == 2 || blank == count - 2) {
		if (count >= 7) {
			return FIVE;//11011111
		}
		else if (block == 0) {
			switch (count) {
				case 3: return THREE;//1101
				case 4://11011
				case 5: return BLOCK_FOUR;//110111
				case 6: return FOUR;//1101111
			}
		}

		else if (block == 1) {
			switch (count) {
				case 3: return BLOCK_THREE;//11012
				case 4: return BLOCK_FOUR;//110112
				case 5: return BLOCK_FOUR;//1101112
				case 6: return FOUR;//21101111
			}
		}

		else if (block ==2) {
			switch (count) {
				case 4://2110112
				case 5://21101112
				case 6: return BLOCK_FOUR;//211011112
			}
		}
	}
	else if (blank == 3 || blank == count - 3) {
		if (count >= 8) {
			return FIVE;//111011111
		}
		if (block == 0) {
			switch (count) {
				case 4://11101
				case 5: return THREE;//111011
				case 6: return BLOCK_FOUR;//1110111
				case 7: return FOUR;//11101111
			}
		}

		if (block == 1) {
			switch (count) {
				case 4://211101
				case 5://2111011
				case 6: return BLOCK_FOUR;//21110111
				case 7: return FOUR;//211101111
			}
		}

		if (block == 2) {
			switch (count) {
				case 4://2111012
				case 5://21110112
				case 6://211101112
				case 7: return BLOCK_FOUR;//211101112
			}
		}
	}
	else if (blank == 4 || blank == count - 4) {
		if (count >= 9) {
			return FIVE;//1111011111
		}
		if (block == 0) {
			switch (count) {
				case 5://111101
				case 6://1111011
				case 7://11110111
				case 8: return FOUR;//111101111
			}
		}

		if (block ==1) {
			switch (count) {
				case 4://2111101
				case 5://21111011
				case 6://211110111
				case 7: return BLOCK_FOUR;//211110111
				case 8: return FOUR;//2111101111
			}
		}

		if (block == 2) {
			switch (count) {
				case 5://211110112
				case 6://2111101112
				case 7://2111101112
				case 8: return BLOCK_FOUR;//21111011112
			}
		}
	}
	else if (blank == 5 || blank == count - 5) {
		return FIVE;//111110...
	}
	return 0;//block=2的情况，两边堵死
}



void GoBang::update_pos(int a,int b) {
	const int dis = 4;
	for (int i = HORIZONTAL; i < ALL_DIRECTIONS; ++i) {
		for (int j = -dis; j <= dis; ++j) {
			int temp_a = a + j * directions[i].first;
			int temp_b = b + j * directions[i].second;

			if (temp_b < 0 || temp_a < 0)
				continue;
			else if (temp_a >= len || temp_b >= len)
				continue;
			search_pos(temp_a, temp_b, PLAYER_TURN, i);
			search_pos(temp_a, temp_b, AI_TURN, i);
		}
	}
}



//a-b剪枝
int GoBang::search(int depth, bool order,int alpha, int beta) {
	static int max_depth = 0;
	int count = 0;
	if (game_over()) {
		return p_val - a_val;
	}
	if (depth <= max_depth) {
		return p_val - a_val;
	}
	vector<pos>select_pos;
	for (int i = 0; i < len; ++i)
		for (int j = 0; j < len; ++j)
			if ((map[i][j] == EMPTY_POS) && exist_neighbour(i, j, 2, 1)) {
				select_pos.push_back(pos(i, j));
			}
	sort(select_pos.begin(), select_pos.end(), [&](const pos& a, const pos& b)->bool {
		return (player_score[a.first][a.second][ALL_DIRECTIONS] - ai_score[a.first][a.second][ALL_DIRECTIONS])
		> (player_score[b.first][b.second][ALL_DIRECTIONS] - ai_score[b.first][b.second][ALL_DIRECTIONS]);
		});//按照玩家分数-电脑分数排序


	int select_size = select_pos.size();
	for (int k = 0; k < select_pos.size(); ++k)//分数最高、最低的先探索
	{
		int i, j;
		if (k % 2 == 0) {
			i = select_pos[k / 2].first;
			j = select_pos[k / 2].second;
		} 
		else {
			i = select_pos[select_size - 1 - k / 2].first;
			j = select_pos[select_size - 1 - k / 2].second;
		}
		++allnums;

		int rec_p_val = p_val;
		int rec_a_val = a_val;
		map[i][j] = (order == PLAYER_TURN) ? PLAYER_POS : AI_POS;//模拟落子

		update_pos(i, j);
		++count;

		int ret = search(depth - 1, !order, alpha, beta);//进入下一层

		map[i][j] = EMPTY_POS;//取消落子
		update_pos(i, j);
		p_val = rec_p_val;
		a_val = rec_a_val;

		if (order == PLAYER_TURN) {//MAX层
			if (ret > alpha) {
				alpha = ret;
				if (depth == step)//没用 目前是min层
					next_step = pos(i, j);
			}
			if (alpha >= beta) {
				return (order == PLAYER_TURN) ? alpha : beta;//b剪枝
			}
		}
		else {//MIN层
			if (ret < beta) {
				beta = ret;
				if (depth == step)
					next_step = pos(i, j);
			}
			if (beta <= alpha) {

				return (order == PLAYER_TURN) ? alpha : beta;//a剪枝
			}
		}


	}
	if (count == 0) {
		max_depth = depth;
		return p_val-a_val;
	}

	return (order == PLAYER_TURN) ? alpha : beta;
}




//判断游戏状态
bool GoBang::game_over() {
	if (a_val >= coe * FIVE || p_val >= FIVE)
		return true;
	return false;
}



void GoBang::restart() {
	steps = 0;
	p_val = a_val = 0;
	while (!path.empty())
		path.pop();
	for(int i=0;i<len;++i)
		for (int j = 0; j < len; ++j) {
			map[i][j] = EMPTY_POS;
			for (int k = 0; k <= ALL_DIRECTIONS; ++k) {
				player_score[i][j][k] = 0;
				ai_score[i][j][k] = 0;
			}
		}
}



void GoBang::undo() {
	if (path.size()<=1)
		return;
	steps=steps-2;
	pos temp = path.top();
	path.pop();
	map[temp.first][temp.second] = EMPTY_POS;
	update_pos(temp.first, temp.second);
	
	temp = path.top();
	path.pop();
	map[temp.first][temp.second] = EMPTY_POS;
	update_pos(temp.first, temp.second);

}

//游戏开始
void GoBang::start() {	
	IMAGE board;
	init_UI(board);
	while (1){
		draw_pieces(map, AI_POS, PLAYER_POS, EMPTY_POS, board, true);
		int pri = get_pri(board);
		if (pri == 1)
			turn = PLAYER_TURN;
		else if (pri == -1)
			turn = AI_TURN;

		while (!game_over()) {
			if (turn == PLAYER_TURN) {
				player_move();//获取命令
				if (next_step.first == -1) {
					if (next_step.second == 1)
						continue;
					if (next_step.second == 2) {
						a_val = FIVE;
						break;
					}
					else if (next_step.second == 3) {
						undo();
						draw_pieces(map, AI_POS, PLAYER_POS, EMPTY_POS, board, true);
						continue;
					}
					else if (next_step.second == 4) {
						break;
					}
				}
			}
			else {
				if (steps == 0)
					next_step = pos(len / 2, len / 2);
				else {
					begin_clock = clock();
					search(step, turn);//搜索
					end_clock = clock();
				}
				ai_move();
			}

			draw_pieces(map, AI_POS, PLAYER_POS, EMPTY_POS, board);
			if (turn == AI_TURN) {
				setbkcolor(WHITE);
				outtextxy(20, 560, "                                                                          ");
				char mes[100];
				sprintf(mes, "%s%d%s%.3f%s", "共搜索了", allnums,
					"个节点数,用时", double(end_clock - begin_clock) / CLOCKS_PER_SEC, "s");
				outtextxy(20, 560, mes);
				allnums = 0;
			}
			update_pos(next_step.first, next_step.second);

			path.push(next_step);
			++steps;
			turn = !turn;
		}

		if (a_val >= coe*FIVE) {
			outtextxy(20, 560, "                                                                          ");
			outtextxy(20, 560, "AI WIN!点击重做重新开始!");
		}
		if (p_val >= FIVE) {
			outtextxy(20, 560, "                                                                          ");
			outtextxy(20, 560, "PLAYER WIN!点击重做重新开始!");
		}
		while (next_step.first!=-1|| next_step.second!=4){
			player_move();//获取
		}
		restart();
	}
	closegraph();
}

#include <cstdlib>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

const unsigned int Rows = 6;
const unsigned int Columns = 7;
string winstr;

struct State {
	char turn = 'P';
	
	char board[Rows][Columns];
	int heights[Columns] = {5,5,5,5,5,5,5};
	int moves_played = 0;
	int next_move_best = 0;

	State()
	{
		turn = 'P';
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Columns; j++)
				board[i][j] = '_';
	}

	//=============================================================================================GET POSSIBLE MOVES
	vector<State> possible_moves(State & s)
	{
		vector<State> result;
		for (int i = 0; i < Columns; i++) {
			if (s.is_valid(i)) {
				auto x = s.make_move(i);
				result.push_back(x);
				//cout << "MMM" << endl;
				//result[i].print_board();
			}
		}
		return result;
	}	

	//=================================================================================================Clones state
	State & clone(State & s) {
		State clone_st;
		clone_st.turn = s.turn;
		for (int i = 0; i < Columns; i++)
			clone_st.heights[i] = s.heights[i];
		for (int i = 0; i < Rows; i++)
			for (int j = 0; j < Columns; j++)
				clone_st.board[i][j] = s.board[i][j];
		clone_st.moves_played = s.moves_played;
		clone_st.next_move_best = s.next_move_best;
		return clone_st;
	}


	//==================================================================================================Print
	void print_board() {
		for (int i = 0; i < Columns; i++)
			cout << i << ' ';
		cout << endl << "______________" << endl;
		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++) {
				cout << board[i][j] << ' ';
			}
			cout << endl;
		}
		cout << endl;
		if (turn == 'C')
			cout << "It's computer's turn!" << endl;
		else
			cout << "It's your turn!" << endl;
	}

	//==================================================================================================Is valid
	bool is_valid(int c) {
		return heights[c] >=0 && c < 7;
	}

	//===============================================================================get person's move
	void get_move() {
		int c;
		cout << "Choose the row (0-6):";
		cin >> c;
		if (is_valid(c))
		{
			board[heights[c]][c] = turn;
			heights[c] -= 1;
			turn = (turn == 'C' ? 'P' : 'C');
			moves_played += 1;
		}
		else
		{
			cout << "Invalid column!" << endl;
			get_move();
		}
	}

	//=====================================================================================================MAKE MOVE
	State make_move(int c) {
		State s = clone(*this);
		s.board[heights[c]][c] = turn;
		s.turn = (turn == 'C' ? 'P' : 'C');
		s.moves_played+=1;
		s.heights[c] -= 1;
		return s;
	}


	bool check_vertical(char & win) {
		string col = "";
		for (int i = 0; i < Columns; i++) {
			for (int j = 0; j < Rows; j++) {
				col += board[j][i];
			}
			if (col.find("CCCC") != string::npos)
			{
				cout << "Computer wins!";
				win = 'C';
				return true;
			}
			else
				if (col.find("PPPP") != string::npos)
				{
					cout << "Person wins!";
					win = 'P';
					return true;
				}
			col = "";
		}
		return false;
	}

	bool check_horizontal(char & win) {
		string row = "";
		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++) {
				row += board[i][j];
			}
			if (row.find("CCCC") != string::npos)
			{
				cout << "Computer wins!";
				win = 'C';
				return true;
			}
			else
				if (row.find("PPPP") != string::npos)
				{
					cout << "Person wins!";
					win = 'P';
					return true;
				}
			row = "";
		}
		return false;
	}


	//=====================================================================================Is winning
	bool IsLinearMatch(int x, int y, int stepX, int stepY) {
		/* Get the value of the start position. */
		const int startValue = board[x][y];
		winstr = startValue;
		/* Confirm 1the two values after it match. */
		for (int i = 1; i < 4; ++i)  // X = 1 Y = 4
		{
			if (board[x + i * stepX][y + i * stepY] != startValue || (x + i * stepX) > Rows || (y + i * stepY) > Columns)
				return false;
			winstr += board[x + i * stepX][y + i * stepY];
		}
		/* If we got here, then they all match! */
		return true;
	}

	bool IsLineStartingAt(int x, int y) {
		return board[x][y]!='_' && 
			(IsLinearMatch(x, y, 1, 0) || // Horizontal
			IsLinearMatch(x, y, 0, 1) || // Vertical
			IsLinearMatch(x, y, 1, 1) || // Diagonal Down
			IsLinearMatch(x, y, 1, -1) ); // Diagonal Up
	}

	//===================================================================================================CHECK WIN
	bool check_win(char * winner) {
		for (int i = 0; i < Rows; i++) {
			for (int j = 0; j < Columns; j++)
				//if (heights[j] == 6)
					//continue;
				if (IsLineStartingAt(i, j))
				{
					//cout << "==========Line starts at [" << i << "," << j << "]" << endl;
					//cout << winstr << endl;
					*winner = board[i][j];
					return true;
				}
		}
		return false;
	}
};

//============================================================================IF CAN WIN
bool can_win_next_move(State &s, char player, int & col) {
	char temp = s.turn;
	//if (player == 'P')
	//{
	//	s.turn = 'P';
	//}
	s.turn = player;
	auto childen = s.possible_moves(s);
	for (int i = 0; i < childen.size(); i++) {
		char c = 'N';
		if (childen[i].check_win(&c) && (c == player) )
		{
			int plus=0;
			//cout << "Turn: " << temp << " can win at " << i << endl;
			for (int j = 0; j < i; j++)
			{
				if (s.heights[j] < 0)
					plus++;
			}
 			col = i + plus;
			s.turn = temp;
			return true;
		}
	}
	s.turn = temp;
	return false;
}

int max_depth = 4;
bool flag = false;
int no = -1;
int d = -1;
//===============================================================================================HEURISTICS
int heuristics(State & field, char player, int & col) {
	bool f = false;
	string c = "";
	string r = "";
	char w = 'N';

	for (int i = 0; i < Rows; i++)
	{
		r = "";
		for (int j = 0; j < Columns; j++) {
			r += field.board[i][j];
		}
		if ((r.find("_CC") != string::npos) && field.heights[r.find("_CC")] >= 0) {
			//if (!can_win_next_move(field.make_move(r.find("_CC")), 'P', col) && d == max_depth - 1)
			{
				col = r.find("_CC");

				//for (int j = 0; j < col; j++)
				//{
				//	if (field.heights[j] < 0)
				//		col++;
				//}

				return 100000;
			}
		}

		if ((r.find("CC_") != string::npos) && field.heights[r.find("CC_")] >= 0)
			//if (!can_win_next_move(field.make_move(r.find("CC_")), 'P', col))
		{
			col = r.find("CC_") + 2;

			/*		for (int j = 0; j < col; j++)
					{
						if (field.heights[j] < 0)
							col++;
					}*/

			return 100000;
		}

		if ((r.find("PP_") != string::npos) && field.heights[r.find("PP_")] >= 0)
			//if (!can_win_next_move(field.make_move(r.find("PP_")), 'P', col))
		{
			col = r.find("PP_") + 2;

			/*		for (int j = 0; j < col; j++)
					{
						if (field.heights[j] < 0)
							col++;
					}*/

			return 100000;
		}

		if ((r.find("_PP") != string::npos) && field.heights[r.find("_PP")] >= 0)
			//if (!can_win_next_move(field.make_move(r.find("_PP")), 'P', col))
		{
			col = r.find("_PP");

			/*		for (int j = 0; j < col; j++)
					{
						if (field.heights[j] < 0)
							col++;
					}*/

			return 100000;
		}

	}
	for (int i = 0; i < Columns; i++)
	{
		c = "";
		for (int j = 0; j < Rows; j++) {
			c += field.board[j][i];
		}

		if ((c.find("_CC") != string::npos) && (i != no) && field.heights[i] >= 0) {
			col = i;

			/*		for (int j = 0; j < i; j++)
					{
						if (field.heights[j] < 0)
							col++;
					}*/

			return 100000;
		}

		if ((c.find("CC_") != string::npos) && (i != no) && field.heights[i] >= 0) {
			col = i;
			return 100000;
		}


		if ((c.find("_PP") != string::npos) && (i != no) && field.heights[i] >= 0) {
			col = i;
			return 100000;
		}

		if ((c.find("PP_") != string::npos) && (i != no) && field.heights[i] >= 0) {
			col = i;
			return 100000;
		}
	}
	return 500;
}

int depth_of_recursion = 0;



vector<int> good_m;
vector<int> bad_m;


//=================================================================================================MINIMAX
int minmax(int alpha, int beta, State & s, int depth, bool maximazingPlayer, int &column) {
	depth_of_recursion++;
	//Priority - if anyone can win with the next move
	if (maximazingPlayer) {
		//flag = false;
		int col = 0;
		//auto b = s.make_move(i);
		if (can_win_next_move(s, 'C', col) && depth == max_depth)
		{
			column = col;
			flag = true;
			return INT_MAX;
		}
		if (can_win_next_move(s, 'P', col) && depth == max_depth) 
		{
			{
				column = col;
				flag = true;
				return INT_MAX;
			}
		}

		//int col = 0;

		////if (s.is_valid(column) && can_win_next_move(s.make_move(column), 'P', col) && depth == max_depth - 1)
		if (depth == max_depth && !flag)
		{
			bad_m.clear();
			good_m.clear();
			for (int i = 0; i < Columns; i++)
			{
				if (s.is_valid(i) && can_win_next_move(s.make_move(i), 'P', col))
				{
					bad_m.push_back(i);
				}
				else
					if (s.is_valid(i))
					good_m.push_back(i);
			}
		}

	}
	else
	{

	}


	if (flag == true)
		return INT_MAX;

	//If drawn
	if (s.moves_played == Columns*Rows)
		return 9000;	//return a large score, but not as large as winner


	//If anyone wins
	char w;
	if (s.check_win(&w)) 
	{
		if (w == 'C')
			return INT_MAX;
		else
			return INT_MIN;
	}


	//If max_depth - heuristics
	if (depth == 0 && flag == false)
		return heuristics(s, 'C', column);

	// Recursive minimax starts
	if (maximazingPlayer) {

		int bestValue = INT_MIN;
		auto childen = s.possible_moves(s);
		for (int i = 0; i < childen.size(); i++) {

			int plus = 0;
			for (int j = 0; j < i; j++)
			{
				if (s.heights[j] < 0)
					plus++;
			}
		    
			//Checks if move is ok and doesn't lead to person's win
			int new_ind = i + plus;
			if (depth == max_depth && find(bad_m.begin(), bad_m.end(), new_ind) != bad_m.end() && !flag)
			{
				if (find(bad_m.begin(), bad_m.end(), column) != bad_m.end())
				{
					for (i = 0; i < good_m.size(); i++)
					{
						if (s.is_valid(good_m[i]))
						{
							column = good_m[i] + plus;
							return 100000;
						}
					}
				}
				else continue;
			}

			int score = minmax(alpha, beta, childen[i], depth - 1, false, column);		

			if (bestValue < score && s.is_valid(i))
			{
					column = i+plus;
					bestValue = score;
			}

			if (alpha < bestValue) //alpha = max(alpha, f(Vi)); для уровня максимизации.
				alpha = bestValue;
			if (beta < alpha)
				break;
		}
		return bestValue;
	}
	// (*minimizing player *)
	else 
	{
		int bestValue = INT_MAX;
		auto childen = s.possible_moves(s);
		for (int i = 0; i < childen.size(); i++) {
			int score =  minmax(alpha, beta, childen[i], depth - 1, true, column);
			if (bestValue > score)
			{
				bestValue = score;
			}
			if (beta > bestValue)//beta = min(beta, f(Vi)); для уровня минимизации.
				beta = bestValue;
			if (beta < alpha)
				break;
		}
		return bestValue;
	}
}



//===============================================================================================MAIN!!!!!!
int main() {
	State s;
	s.print_board();

	char c;
	cout << "Who goes first? C or P?" << endl;
	cin >> c;
	if (c == 'C')
	{
		int bestmove = 6;
		minmax(INT_MIN, INT_MAX, s, 4, true, bestmove);
		cout << "Computer goes to " << bestmove << endl;
		bad_m.clear();
		s.turn = 'C';
		s = s.make_move(bestmove);
		s.print_board();
	}

	while (true) {
		char w;
		if (s.check_win(&w)) {
			cout << w << " WINS!!!" << endl;
			cout << endl << endl << endl;
			s.print_board();
			cout << endl << endl << endl;
			system("pause");
		}

		s.get_move();
		s.print_board();


		char c = 'C';
		int bestmove = 0;
		minmax(INT_MIN, INT_MAX, s, 4, true,bestmove);
		cout << "Computer goes to " << bestmove << endl;
		s = s.make_move(bestmove);
		cout << "Recurtion depth " << depth_of_recursion << endl;
		depth_of_recursion = 0;
		s.print_board();
		int g = 0;
		char winner = 'P';
		flag = false;
		no = -1;
		good_m.clear();
		bad_m.clear();
		/*cout << "Person can win: " << can_win_norm(s, winner, g) << endl;
		cout << "Position " << g << " WINNER " << winner << endl;
		winner = 'C';
		cout << "Computer can win: " << can_win_norm(s, winner, g) << endl;
		cout << "Position " << g << " WINNER " << winner << endl;*/
	}
}
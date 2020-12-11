// tetris-cls.c9pp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<conio.h>
#include<iostream>
#include<Windows.h>
#include<time.h>
#include<random>
#include<stdlib.h>
using namespace std;

struct ShapeStruct
{
	static const int Height = 3;
	static const int Width = 3;
	int X = 0, Y = 0;
	char S[Height][Width] =
	{
		{ '*','*','*' },
		{ '*','*','*' },
		{ '*','*','*' }
	};
	void SetShpae(char SA[3][3])
	{
		for (int Y = 0; Y < Height; Y++)
		{
			for (int X = 0; X < Width; X++)
			{
				S[Y][X] = SA[Y][X];
			}
		}
	}
};

class tetris{
protected:
	int downspace, savestop;
	int mid;
	ShapeStruct sp[4];
	char box[3][3] = { {'*','*','*'},
	                   {'*','*','*'},
					   {'*','*','*'} };
	char line[3][3] = { {' ','*',' '},
	                    {' ','*',' '},
	                    {' ','*',' '} };
	char angle[3][3] = { {'*','*',' '},
						 {' ','*',' '},
						 {' ','*',' '} };
	char tshap[3][3] = { {' ','*',' '},
						 {' ','*',' '},
						 {'*','*','*'} };
	static const int width = 22;
	static const int height = 20;
	char secScr[height][width];
public:
	tetris() {
		srand(time(NULL));
		downspace = savestop = 0;
		mid = 50;
		sp[0].SetShpae(box);
		sp[1].SetShpae(line);
		sp[2].SetShpae(angle);
		sp[3].SetShpae(tshap);
		for (int i = 0; i < 21; i++) {
			for (int j = 0; j < 22; j++) {
				secScr[i][j] = ' ';
			}
		}
	}
	int scoreChk() {
		int scor = 0;
		bool score = false;
		for (int decY = 20; decY >= 0; decY--)
		{
			for (int x = 0; x < 22; x++)
			{
				if (secScr[decY][x] != ' ') {
					score = true;
				}
				else
				{
					score = false;
					x = 22;
				}
			}
			if (score)
			{
				scor++;
				decY = 21;
				for (int secy = decY; secy >= 0; secy--)
				{
					for (int x = 0; x < 22; x++)
					{
						if (secy != 0)
						{
							secScr[secy][x] = secScr[secy - 1][x];
						}
					}
				}
			}
		}
		return scor;
	}

	void setpos(int x, int y) {
		HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos = { x,y };
		SetConsoleCursorPosition(output, pos);
	}

	void setmid(int pos) {
		mid = pos;
	}

	int* midret() {
		int startpos = mid;
		int* pointpos = &startpos;
		return pointpos;
	}

	bool spaceChk(int step, int sh_no) {

		bool chk = false, space = true;

		for (int posx = mid - 41; posx < (mid - 41) + 3; posx++)
		{
			if (secScr[step + 3][posx] == ' ' ) {
				space = true;
			}
			else
			{
				// add if step is 0 at top game end
				int shapeLast = 0;
				int shapeY = 2;
				for (int decY = (step + 3); decY > step; decY--)
				{
					for (int lastrow = mid - 41; lastrow < (mid - 41) + 3; lastrow++)
					{
						//checking fix space 
						if (sp[sh_no].S[shapeY][shapeLast] != ' ') {
							if (secScr[decY][lastrow] == ' ' && (step + 3) <= 20) {
								space = true;
							}
							else
							{
								decY = step;
								lastrow = (mid - 41) + 3;
								posx = (mid - 41) + 3;
								space = false;
							}
						}
						shapeLast++;
					}
					shapeLast = 0;
					shapeY--;
				}
			}
		}
		return space;
	}

	int boxshow(int step,int sh_no){

		// screen set chk
		
		bool space = spaceChk(step,sh_no);

		// box show start
		int lastpos = 0;
		for (int i = 0; i < 3; i++) {
			lastpos = step + i;
			for (int j = 0; j < 3; j++) {
				if (sp[sh_no].S[i][j] != ' ') {
					setpos((mid+j), step + i);
					cout << sp[sh_no].S[i][j];
				}
			}
		}
		// box show end

		if (!space)
		{
			int shapY = 0;
			bool setshape = true;
			if (step != 0)
			{
				if (lastpos == 20)
				{
					//---- checking last row of shape is empty or not for next cycle
					for (int shapelastrow = 0; shapelastrow < 3; shapelastrow++)
					{
						if (sp[sh_no].S[2][shapelastrow] == ' ') {
							setshape = false;
						}
						else
						{
							setshape = true;
							shapelastrow = 3;
						}
					}
					//----
				}

				if (setshape)
				{
					for (int posy = step; posy < step + 3; posy++)
					{
						int posx = mid - 41;
						for (int shapex = 0; shapex < 3; shapex++)
						{
							if (sp[sh_no].S[shapY][shapex] != ' ') {
								secScr[posy][posx] = sp[sh_no].S[shapY][shapex];
							}
							posx++;
						}
						shapY++;
					}
					lastpos = 20;
				}
				else
				{
					lastpos--;
				}
				
			}
			else
			{
				lastpos = NULL;
			}
		}
		return lastpos; // return box y-axis down position 
	}

	int randshape() {
		int shpno = rand() % 4 + 0;
		return shpno;
	}

	char posRet() {
	}

	void boarder() {
		for (int i = 0; i <= 21; i++) {
			for (int j = 40; j <= 63; j++) {
				if (j == 40 || j == 63) {
					setpos(j, i);
					cout << '|';
				}
				if (i == 21 && j!=63) {
					cout << '_';
				}
			}
		}
	}

	void down(int step,int shNo) {
		bool space = spaceChk(step, shNo);
		if (space) {
		Sleep(800);
		system("cls");
		setpos(mid, step);
		}
	}

	int downward(int step,int shNo) {
		bool space = spaceChk(step, shNo);
		if(space) {
		system("cls");
		setpos(mid, step);
		step++;
		}
		return step;
	}

	void right(int shNo,int step) {
		bool rigTurn = false;
		// check right index to move block
		// moving stars in shape block
		for (int i = 0; i < 3; i++) {
			if (sp[shNo].S[i][2]==' '){
				if (i == 2) {
					rigTurn = true;
				}
			}
			if (sp[shNo].S[i][2] != ' ') {
				i = 3;
				rigTurn = false;
			}
		} 
		//stop moving stars in shape block
		// move block
		if (rigTurn == true) {
			for (int i = 0; i <3; i++) {
				for (int j = 2; j >= 0; j--) {
					int dec = j - 1;
					if (j == 0) {
						sp[shNo].S[i][j] = ' ';
					}
					else
					{
						sp[shNo].S[i][j] = sp[shNo].S[i][dec];
					}
				}
			}
		}
		else {//noraml move
			for (int rightPosy = step; rightPosy < step + 3; rightPosy++)
			{
				if (secScr[rightPosy][(mid - 41) + 3] == ' ')
				{
					rigTurn = true;
				}
				else
				{
					rigTurn = false;
					rightPosy = step + 3;
				}
			}
			if (mid < 60 && rigTurn) {
				mid = mid + 1;
			}
		}
	}

	void left(int shNo, int step) {
		bool leftTurn = false;
		//check left trun
		for (int i = 0; i < 3; i++) {
			if (sp[shNo].S[i][0] == ' ') {
				if (i == 2) {
					leftTurn = true;
				}
			}
			if (sp[shNo].S[i][0] != ' ') {
				i = 3;
				leftTurn = false;
			}
		}
		// move block
		if (leftTurn == true) {
			for (int i = 0; i <3; i++) {
				for (int j = 0; j <3; j++) {
					int inc = j + 1;
					if (j == 2) {
						sp[shNo].S[i][j] = ' ';
					}
					else
					{
						sp[shNo].S[i][j] = sp[shNo].S[i][inc];
					}
				}
			}
		}
		else
		{//normal move
			for (int rightPosy = step; rightPosy < step + 3; rightPosy++)
			{
				if (secScr[rightPosy][(mid - 41) - 1] == ' ')
				{
					leftTurn = true;
				}
				else
				{
					leftTurn = false;
					rightPosy = step + 3;
				}
			}
			if (mid > 41 && leftTurn) {
				mid = mid - 1;
			}
		}
	}
   // set screen in boxshow function 

	void showScreen() {
		for (int i = 0; i <= 20; i++) {
			for (int j = 0; j <= 21; j++) {
				setpos((j + 41), i);
				cout << secScr[i][j];
			}
		}
	}
	void leftRotate(int shNo,int step) {
		bool rotate = false;
		for (int posx = mid - 41; posx < (mid - 41) + 3; posx++)
		{
			if (secScr[step + 3][posx] == ' ' && (step + 3) <= 20)
			{
				rotate = true;
			}
			else
			{
				rotate = false;
				posx = (mid - 41) + 3;
			}
		}

		if (rotate)
		{
			ShapeStruct tempshp;
			for (int x = 0; x < 3; x++)
			{
				int xinc = 0;
				for (int y = 2; y >= 0; y--)
				{
					tempshp.S[y][x] = sp[shNo].S[x][xinc];
					xinc++;
				}
			}
			sp[shNo].SetShpae(tempshp.S);
		}
	}
	void rightRotate(int shNo, int step) {
		bool rotate = false;
		for (int posx = mid - 41; posx < (mid - 41) + 3; posx++)
		{
			if (secScr[step + 3][posx] == ' ' && (step + 3) <= 20)
			{
				rotate = true;
			}
			else
			{
				rotate = false;
				posx = (mid - 41) + 3;
			}
		}

		if (rotate)
		{
			ShapeStruct tempshp;
			for (int y = 0; y < 3; y++)
			{
				int xinc = 0;
				for (int x = 2; x >= 0; x--)
				{
					tempshp.S[y][x] = sp[shNo].S[xinc][y];
					xinc++;
				}
			}
			sp[shNo].SetShpae(tempshp.S);
		}
	}
	

};

int main()
{
	tetris obj;
	int posy=0,i=0;
	int score = 0;
	int sh_no=0, midd=50;
	cout << "\t\t\t\t Welcome to Tetris";
	cout << "\n\n\t\t\t Press a to move shape left, d to move right, s to move down";
	cout << "\n\n\t\t\t Press q to rotate left and r to rotate right shape";
	cout << "\n\n\t\t\t       Press any key to start";
	_getch();
	system("cls");
	while(posy < 20)
	{
		obj.setpos(5, 5);
		cout << "Score : " << score;
		//strSh.showScreen();
		obj.showScreen();
		if (i == 0) {
			sh_no = obj.randshape();
			obj.setmid(midd);
		}
		//cout << no;
		obj.boarder();
		posy = obj.boxshow(i,sh_no);
		int kbp=0;
		char chk;
		bool downal = false;
		if (kbhit()) {
			chk = getch();
			kbp = chk;
			switch (kbp)
			{
			case 115://downward s
				i = obj.downward(i, sh_no);
				downal = true;
				break;
			case 100://right d
				system("cls");
				obj.right(sh_no,i);
				downal = true;
				break;
			case 97://left a
				system("cls");
				obj.left(sh_no,i);
				downal = true;
				break;
			case 113://left rotate q
				system("cls");
				obj.leftRotate(sh_no,i);
				downal = true;
				break;
			case 114://right rotate r
				system("cls");
				obj.rightRotate(sh_no, i);
				downal = true;
				break;
			default:
				break;
			}
		}
		if (downal == false) {
			obj.down(i,sh_no);
			i++;
		}
		if (posy >= 20) {
			score += obj.scoreChk();
			posy = i = 0;
		}
		else
		{
			if (posy == NULL)
			{
				posy = 20;
			}
		}
	}
	cout << "Game Over";
	_getch();
	return 0;
}
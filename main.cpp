#include <iostream>
#include <time.h>
#include <conio.h>
#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
using namespace std;
int i = 0;
enum eDir { STOP = 0, LEFT = 1, UPLEFT = 2, DOWNLEFT = 3, RIGHT = 4, UPRIGHT = 5, DOWNRIGHT = 6};
int paddle_offset = 60;
int paddle_size = 80;
int wall_thicc = 12;
int paddle_thicc = 12;
int ball_rad = 8;
class cBall {
	private:
		int x, y;
		int originalX, originalY;
		eDir direction;
		bool pause;
	public:
		cBall(int posX, int posY, bool pause) {
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
			direction = STOP;
			pause = false;
		}
		void Reset() {
			x = originalX;
			y = originalY;
			direction = STOP;
		}
		void changeDirection(eDir d) {
			direction = d;
		}
		void randomDirection() {
			direction = (eDir)((rand() % 6) + 1);
		}
		inline int getX() {
			return x;
		}
		inline int getY() {
			return y;
		}
		inline eDir getDirection() {
			return direction;
		}
		inline void ball_pause(bool pause_game) {
			pause = pause_game;
		}
		void Move() {
			if(pause == true) {
				switch (direction) {
					case STOP:
						break;
					case LEFT:
						x-=3;
						break;
					case RIGHT:
						x+=3;
						break;
					case UPLEFT:
						x-=3;
						y-=3;
						break;
					case DOWNLEFT:
						x-=3;
						y+=3;
						break;
					case UPRIGHT:
						x+=3;
						y-=3;
						break;
					case DOWNRIGHT:
						x+=3;
						y+=3;
						break;
					default:
						break;
				}
			}
		}
		friend ostream & operator<<(ostream & o, cBall c) {
			o << "Ball [" << c.x << "," << c.y << "][" << c.direction << "]";
			return o;
		}
};
class cPaddle {
	private:
		int x, y;
		int originalX, originalY;
	public:
		cPaddle() {
			x = y = 0;
		}
		cPaddle(int posX, int posY) {
			originalX = posX;
			originalY = posY;
			x = posX;
			y = posY;
		}
		inline void Reset() {
			x = originalX;
			y = originalY;
		}
		inline int getX() {
			return x;
		}
		inline int getY() {
			return y;
		}
		inline void moveUp() {
			y-=8;
		}
		inline void moveDown() {
			y+=8;
		}
		friend ostream & operator<<(ostream & o, cPaddle c) {
			o << "Paddle [" << c.x << "," << c.y << "]";
			return o;
		}
};
class cGameManager {
	private:
		int width, height;
		int score1, score2;
		char up1, down1, up2, down2;
		bool quit;
		bool pause;
		char pause_screen[20];
		cBall *ball;
		cPaddle *player1;
		cPaddle *player2;
	public:
		cGameManager(int w,int h) {
			initwindow(w,h,"Window BGI");
			setvisualpage(0);
			setactivepage(0);
			srand(time(NULL));
			quit = false;
			pause = false;
			up1 = 'w';
			up2 = 'i';
			down1 = 's';
			down2 = 'k';
			score1 = score2 = 0;
			width = w;
			height = h;
			ball = new cBall(w / 2, h / 2,pause);
			player1 = new cPaddle(paddle_offset, h / 2 - paddle_size / 2);
			player2 = new cPaddle(w - paddle_offset, h / 2 - paddle_size / 2);
			setactivepage(1);
		}
		~cGameManager() {
			delete ball, player1, player2;
		}
		void ScoreUp(cPaddle * player) {
			if (player == player1)
				score1++;
			else if (player == player2)
				score2++;

			ball->Reset();
			player1->Reset();
			player2->Reset();

			if(score1 == 10 | score2 == 10) {
				score1 = 0;
				score2 = 0;
			}

			pause = false;
		}
		void Draw() {
			cleardevice();
			int ballx = ball->getX();
			int bally = ball->getY();
			int player1x = player1->getX();
			int player2x = player2->getX();
			int player1y = player1->getY();
			int player2y = player2->getY();
			//setfillstyle(LINE_FILL,3);
			bar(0,0,wall_thicc,height); // right edge of play area
			bar(width - wall_thicc,0,width,height); // right edge of play area
			setfillstyle(SOLID_FILL,2);
			bar(0,0,width,wall_thicc);  // top edge of play area
			bar(0, height - wall_thicc, width,height);  // bottom edge of play area
			setlinestyle(DOTTED_LINE,1,1);
			line(width / 2, wall_thicc, width / 2, height - wall_thicc);

			// Score digits !!!
			// Left Digit
			if(score1 == 0 | score1 == 2 | score1 == 3 | score1 >= 5  & score1 < 10) {
				bar(width / 2 - 80,25, width / 2 - 50, 30);
			}		// top bar (1)
			if(score1 == 0 | score1 == 4 | score1 == 5 | score1 == 6 | score1 == 8 | score1 == 9 & score1 < 10) {
				bar(width / 2 - 80,30, width / 2 - 85,60);
			}		// top left bar (2)
			if(score1 <= 4 | score1 >= 7 & score1 <= 9 & score1 < 10) {
				bar(width / 2 - 50,30, width / 2 - 45,60);
			}		// top right bar (4)
			if(score1 >= 2 & score1 != 7 & score1 < 10) {
				bar(width / 2 - 80,60, width / 2 - 50, 65);
			}		// middle bar (3)
			if(score1 == 0 | score1 == 2 | score1 == 6 | score1 == 8 & score1 < 10) {
				bar(width / 2 - 80,65, width / 2 - 85,95);
			}		// bottom left bar (5)
			if(score1 != 2 & score1 < 10) {
				bar(width / 2 - 50,65, width / 2 - 45,95);
			}		// bottom right bar (7)
			if(score1 != 1 & score1 != 4 & score1 != 7 & score1 < 10) {
				bar(width / 2 - 80,95, width / 2 - 50, 100);
			}		// bottom bar (6)

			// Right Digit
			if(score2 == 0 | score2 == 2 | score2 == 3 | score2 >= 5  & score2 < 10) {
				bar(width / 2 + 80,25, width / 2 + 50, 30);
			}		// top bar (1)
			if(score2 == 0 | score2 == 4 | score2 == 5 | score2 == 6 | score2 == 8 | score2 == 9 & score2 < 10) {
				bar(width / 2 + 50,30, width / 2 + 45,60);
			}		// top left bar (2)
			if(score2 <= 4 | score2 >= 7 & score2 <= 9 & score2 < 10) {
				bar(width / 2 + 80,30, width / 2 + 85,60);
			}		// top right bar (4)
			if(score2 >= 2 & score2 != 7 & score2 < 10) {
				bar(width / 2 + 80,60, width / 2 + 50, 65);
			}		// middle bar (3)
			if(score2 == 0 | score2 == 2 | score2 == 6 | score2 == 8 & score2 < 10) {
				bar(width / 2 + 50,65, width / 2 + 45,95);
			}		// bottom left bar (5)
			if(score2 != 2 & score2 < 10) {
				bar(width / 2 + 80,65, width / 2 + 85,95);
			}		// bottom right bar (7)
			if(score2 != 1 & score2 != 4 & score2 != 7 & score2 < 10) {
				bar(width / 2 + 80,95, width / 2 + 50, 100);
			}		// bottom bar (6)


			if (ballx > wall_thicc && ballx < width - wall_thicc && bally > wall_thicc && bally < height - wall_thicc) {
				setcolor(WHITE);
				setlinestyle(SOLID_LINE,1,6);
				circle(ballx,bally,ball_rad);   // ball
				setcolor(GREEN);
				setlinestyle(SOLID_LINE,1,1);
			}
			if (player1y > wall_thicc && player1y < height - wall_thicc) {
				bar(player1x,player1y,player1x+paddle_thicc,player1y+paddle_size);   // player 1 paddle
			}
			if (player2y > wall_thicc && player2y < height - wall_thicc) {
				bar(player2x,player2y,player2x+paddle_thicc,player2y+paddle_size);   // player 2 paddle
			}


			// Pause screen
			if(pause == false) {
				settextstyle(TRIPLEX_FONT, HORIZ_DIR, 4);
				setusercharsize(6,1,3,1);
				sprintf(pause_screen,"PAUSED");
				outtextxy(width / 2 - 100, height / 2 - 10, pause_screen);
			}
			swapbuffers();
		}
		void Input(int paddle_size) {
			ball->Move();

			int ballx = ball->getX();
			int bally = ball->getY();
			int player1x = player1->getX();
			int player2x = player2->getX();
			int player1y = player1->getY();
			int player2y = player2->getY();


			if(GetAsyncKeyState(0x50)) {
				pause = !pause;
				ball->ball_pause(pause);
				delay(100);
			}
			if(pause == true) {
				if (GetAsyncKeyState(0x57)) {
					if (player1y > 25) {
						player1->moveUp();
					}
				}
				if (GetAsyncKeyState(0x49)) {
					if (player2y > 25) {
						player2->moveUp();
					}
				}
				if (GetAsyncKeyState(0x53)) {
					if (player1y + paddle_size < height - 25) {
						player1->moveDown();
					}
				}
				if (GetAsyncKeyState(0x4B)) {
					if (player2y + paddle_size < height - 25) {
						player2->moveDown();
					}
				}


				if (ball->getDirection() == STOP) {
					ball->randomDirection();
				}
			}

			if (GetAsyncKeyState(0x51)) {
				quit = true;
			}
		}
		void Logic() {
			int ballx = ball->getX();
			int bally = ball->getY();
			int player1x = player1->getX();
			int player2x = player2->getX();
			int player1y = player1->getY();
			int player2y = player2->getY();

			//left paddle
			if(ballx >= player1x && ballx <= player1x + paddle_thicc + ball_rad && bally > player1y && bally < player1y + 80) {
				ball->changeDirection((eDir)((rand() % 3) + 4));
			}

			//right paddle
			if(ballx >= player2x - ball_rad && ballx <= player2x + paddle_thicc && bally > player2y && bally < player2y + 80) {
				ball->changeDirection((eDir)((rand() % 3) + 1));
			}

			//bottom wall
			if (bally > height - wall_thicc - ball_rad)
				ball->changeDirection(ball->getDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
			//top wall
			if (bally < wall_thicc + ball_rad)
				ball->changeDirection(ball->getDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
			//right wall
			if (ballx > width - wall_thicc)
				ScoreUp(player1);
			//left wall
			if (ballx < wall_thicc)
				ScoreUp(player2);
		}
		void Run() {
			while (!quit) {
				Input(paddle_size);
				if(pause == true) {
					Logic();
				}
				Draw();
				delay(10);
			}
		}
};
int main() {
	cGameManager c(640, 480);
	c.Run();
}
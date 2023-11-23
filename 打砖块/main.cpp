#include<iostream>
#include<easyx.h>
#include<ctime>
#include<random>
#define ROW 10
#define COL 5
IMAGE img[5];
// 贴背景图
IMAGE bk;
int map[ROW][COL];

struct Board {
	float x;
	float y;
	float speed;
	float w;
	float h;
}board;

struct Ball {
	float x;
	float vx;
	float vy;
	float y;
	float r;
}ball;


void loadImg() {
	// 加载图片
	loadimage(&bk, _T("./images/bk.png"));
	for (int i = 0; i < 5; i++) {
		WCHAR imgPath[20] = { 0 };
		wsprintf(imgPath, _T("./images/%d.png"), i + 1);
		loadimage(img + i, imgPath);
	}
}
void gameInt() {
	loadImg();
	std::default_random_engine e;
	std::uniform_int_distribution<int> u(0, 4);
	e.seed(time(0));
	for (auto i = 0; i < ROW; i++) {
		for (auto j = 0; j < COL; j++) {
			map[i][j] = u(e);
		}
	}
	board.w = 100;
	board.h = 20;
	board.x = (getwidth()- board.w) / 2;
	board.y = getheight() - 50;
	board.speed = 0.5;

	ball.x = getwidth() / 2;
	ball.y = 460;
	ball.r = 10;
	ball.vx = -0.15;
	ball.vy = -0.15;

}
void gameDraw() {
	
	// 贴图
	putimage(0, 0, &bk);

	// 画砖块
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			//Sleep(10);
			putimage(i * 48, j * 30, img + map[i][j]);
		}
	}

	// 画一块板子
	setfillcolor(RGB(0, 170, 170));
	solidroundrect(board.x, board.y, board.w + board.x, board.y + board.h, 20, 20);

	// 画小球
	setfillcolor(RED);
	solidcircle(ball.x, ball.y, ball.r);
}

void moveBoard() {
	// 获取键盘按键消息
	if (GetAsyncKeyState(VK_LEFT) && board.x>=0) {
		board.x -= board.speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && board.x+board.w<=getwidth()) {
		board.x += board.speed;
	}
}

// 判断球是否碰撞板子
bool ballIsInBoard() {
	if (ball.x >= board.x && ball.x <= board.x + board.w &&
		ball.y + ball.r > board.y - 3 && ball.y + ball.r < board.y + 3) {
		return true;
	}
	return false;
}

void moveBall() {
	ball.x += ball.vx;
	ball.y += ball.vy;

	// 反弹
	if (ball.x - ball.r <= 0 || ball.x + ball.r >= getwidth()) {
		ball.vx = -ball.vx;
	}
	if (ball.y - ball.r <= 0 || ball.y + ball.r >= getheight() || ballIsInBoard()) {
		ball.vy = -ball.vy;
	}
}

// 球和砖块的碰撞
void playBrick() {
	// 判断小球和砖块是否发生碰撞了
	if (ball.y + ball.r <= COL * 30) {
		int c = (ball.y - ball.r) / 30;
		int r = ball.x / 48;
		int dr = (ball.y + ball.r) / 30;
		if (map[r][c] != 0) {
			ball.vy = -ball.vy;
			map[r][c] = -1;
		}
		if (map[dr][c] != 0) {
			ball.vy = -ball.vy;
			map[dr][c] = -1;
		}
	}
}

int main() {
	// 创建图形窗口
	initgraph(480, 720);
	gameInt();
	// 双缓冲绘图
	BeginBatchDraw();
	while (true) {
		gameDraw();
		moveBoard();
		moveBall();
		playBrick();
		FlushBatchDraw();
	}
	EndBatchDraw();

	getchar();
}
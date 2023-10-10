#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>

#define length 640
#define width 480

/*精灵类*/
class Sprite
{
public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y), m_color(RED) {};
	//绘制精灵
	virtual void draw()
	{
		//设置填充颜色
		setfillcolor(m_color);
		//绘制矩形
		fillrectangle(m_x, m_y, m_x + 10, m_y + 10);
	}
	//移动
	void moveBy(int dx, int dy)
	{
		m_x += dx;
		m_y += dy;
	}
	//碰撞检测
	bool collision(const Sprite& other)
	{
		return m_x == other.m_x && m_y == other.m_y;
	}
	//撞墙检测
	bool climbwall()
	{
		if (m_x == length || m_y == width|| m_x == 0 || m_y == 0)
			std::cout << "撞墙了！" << std::endl;
		return m_x == length || m_y == width || m_x == 0 || m_y == 0;
	}
protected:
	int m_x;
	int m_y;
	COLORREF m_color;  //颜色
};

/*蛇类*/
class Snake : public Sprite
{
public:
	Snake():Snake(0,0) {};
	Snake(int x, int y) :Sprite(x, y),dir(VK_RIGHT)
	{
		//初始化三节蛇
		nodes.push_back(Sprite(20, 20));
		nodes.push_back(Sprite(10, 20));
		nodes.push_back(Sprite(0, 20));
	};
	void draw() override
	{
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}
	//蛇的身体移动
	void bodyMove()
	{
		//身体跟着蛇头移动
		for (size_t i = nodes.size()-1; i > 0; i--)
		{
			nodes[i] = nodes[i - 1];
		}
		//移动蛇头
		switch (dir)
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);
			break;
		}
	}
	bool collision(const Sprite& other)
	{
		return nodes[0].collision(other);
	}

	bool climbwall()
	{
		return nodes[0].climbwall();
	}
	//蛇增加一节
	void incrment()
	{
		nodes.push_back(Sprite());
	}
	int dir; //蛇的方向
private:
	std::vector<Sprite> nodes;  //蛇的所有节点
	

};

/*食物*/
class Food :public Sprite
{
public:
	Food():Sprite(0,0)
	{
		changePos();
	}
	void draw()override
	{
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}
	//改变食物的坐标
	void changePos()
	{
		//随机生成坐标
		m_x = rand() % 64 * 10;
		m_y = rand() % 48 * 10;
	}
};

/*墙体*/
class Wall:public Sprite
{
public:
	Wall() :Wall(0, 0) {};
	Wall(int x, int y) :Sprite(x, y) {};

	void draw()override
	{
		//设置填充颜色
		setfillcolor(GREEN);

		//绘制上边框
		for (int i = 0; i < length; i += 10)
		{
			fillrectangle(i, 0, i + 10, 10);
		}
		//绘制下边框
		for (int i = 0; i < length; i += 10)
		{
			fillrectangle(i, width, i + 10, width + 10);
		}
		//绘制左边框
		for (int i = 0; i < width; i += 10)
		{
			fillrectangle(0, i, 10, i + 10);
		}
		//绘制右边框
		for (int i = 0; i < width; i += 10)
		{
			fillrectangle(length, i, length + 10, i + 10);
		}
	}
};

/*游戏场景*/
class GameScene
{
public:
	GameScene() {};
	void run()
	{
		//双缓冲绘图
		BeginBatchDraw();
		snake.draw();
		food.draw();
		wall.draw();
		snakeEatFood();
		snakeHitWall();
		EndBatchDraw();
		
		//移动蛇，改变蛇的坐标
		snake.bodyMove();
		//改变蛇的移动方向 获取键盘输入

		//获取消息
		ExMessage msg = { 0 };
		while (peekmessage(&msg, EX_KEY))
		{
			onMsg(msg);
		}
	}
	//响应消息：鼠标消息，键盘消息
	void onMsg(const ExMessage& msg)
	{
		//如果有键盘消息（有没有键盘按下）
		if (msg.message == WM_KEYDOWN)
		{
			//判断具体是哪个按键按下
			if (msg.vkcode == VK_UP || msg.vkcode == VK_DOWN ||
				msg.vkcode == VK_RIGHT || msg.vkcode == VK_LEFT)
			{
				if((snake.dir==VK_UP && msg.vkcode!=VK_DOWN)||
					(snake.dir == VK_DOWN && msg.vkcode != VK_UP)||
					(snake.dir == VK_LEFT && msg.vkcode != VK_RIGHT) || 
					(snake.dir == VK_RIGHT && msg.vkcode != VK_LEFT))
					snake.dir = msg.vkcode; //虚拟键码
			}
		}
	}
	//判断蛇能否吃到食物
	void snakeEatFood()
	{
		if (snake.collision(food))//如果蛇和食物产生了碰撞
		{
			//蛇的节数增加
			snake.incrment();

			//食物重新产生在别的位置
			food.changePos();
		}
	}

	//判断蛇是否撞墙
	void snakeHitWall()
	{
		if (snake.climbwall())//如果蛇撞了墙
		{
			//输出游戏结束
			std::cout << "游戏结束！" << std::endl;

			//清屏
			return;
		}
	}
private:
	Snake snake;
	Food food;
	Wall wall;
};

int main()
{
	//设置随机数种子
	srand(time(nullptr));
	initgraph(length+10, width+10);
	GameScene scene;
	while (true)
	{
		
		cleardevice();
		scene.run();
		Sleep(100);
	}
	
	
	getchar();
	return 0;
}
// 扫雷.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include<time.h>
#include<graphics.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib")

using namespace std;

#define ROW 10 //定义数组的行
#define COL 10 //定义数组的列
#define MINE_NUM 10 //雷的数量
#define GRID_W 40 //格子的宽度
//定义二维数组来存放数据
int map[ROW][COL];
int cnt = 0;
IMAGE img[12]; //保存所有的图片

void playMusic(const char fileName[])
{
    char str[50] = "";
    sprintf_s(str, "close ./resource/%s", fileName);
    mciSendString(str, 0, 0, 0);
    sprintf_s(str, "open ./resource/%s", fileName);
    mciSendString(str , 0, 0, 0);
    sprintf_s(str, "play ./resource/%s", fileName);
    mciSendString(str, 0, 0, 0);

}

//数据初始化
void gameInit()
{
    playMusic("start.mp3");
    //设置随机数种子
    srand((unsigned)time(NULL));
    //随机在数组中生成10个-1用来表示雷
    for (int i = 0; i < MINE_NUM;)
    {
        //随机生成下标
        int r = rand() % ROW;
        int c = rand() % COL;
        if (map[r][c] == 0)
        {
            map[r][c] = -1;
            i++;
        }     
    }
    //让以-1为中心的九宫格所有的数据+1（-1除外）
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            if (map[i][k] == -1)
            {
                for(int r = i-1;r<=i+1;r++)
                    for (int c = k - 1; c <= k + 1; c++)
                    {
                        if ((r >= 0 && r < ROW && c >= 0 && c < COL) && map[r][c] != -1)
                            map[r][c]++;
                    } 
            }
        }
    }
    char str[50] = "";
    for (int i = 0; i < 12; i++)
    {
        sprintf_s(str, "./resource/%d.jpg", i);
        loadimage(&img[i], str, GRID_W, GRID_W);
    }

    //数据加密   把所有的数据加20
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            map[i][k] += 20;
        }

    }

}


//界面绘制
void gameDraw()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            if (map[i][k] >= 0 && map[i][k] <= 8)
                putimage(i* GRID_W, k* GRID_W, &img[map[i][k]]);
            else if (map[i][k] == -1)
            {
                putimage(i * GRID_W, k * GRID_W, &img[9]);
            }
            else if (map[i][k] >= 19 && map[i][k]<=28)
            {
                putimage(i * GRID_W, k * GRID_W, &img[10]);
            }
            else if(map[i][k] > 28)
                putimage(i * GRID_W, k * GRID_W, &img[11]);
        }

    }
}

void jude(int row, int col)
{
    if (map[row][col] == -1)
    {
        //输
    }
    
    if (cnt == (ROW * ROW - MINE_NUM))
        MessageBox(GetHWnd(), "赢了", "good", MB_OK);
}

//点开附近空白格
void openNull(int row, int col)
{
    if (map[row][col] == 0)
    {
        for (int i = row - 1; i <= row + 1; i++)
        {
            for (int k = col - 1; k < col + 1; k++)
            {
                //如果不是雷就直接打开
                if ((i>=0&&i<ROW&&k>=0&&k<COL)&&map[i][k] > 19)
                {
                    cnt++;
                    map[i][k] -= 20;
                    openNull(i, k);
                }
            }
        }
    }
}

//通过鼠标打开格子
void mouseEvent()
{
    MOUSEMSG msg = GetMouseMsg(); //获取鼠标消息
    //把鼠标当前点击坐标转成二维数组下标
    int row = msg.x / GRID_W;
    int col = msg.y / GRID_W;
    //判断左右键
    if (msg.uMsg == WM_LBUTTONDOWN)
    {
        playMusic("click.wav");
        if (map[row][col] >= 19)
        {
            map[row][col] -= 20;
            cnt++;
            if (map[row][col] == 0)
                playMusic("search.wav");
            openNull(row, col);
            jude(row, col);
        }
            
    }
    else if (msg.uMsg == WM_RBUTTONDOWN)//右键点击，标记格子
    {
        playMusic("rightClick.wav");
        //没有打开才能标记
        if (map[row][col] >= 19 && map[row][col] <= 28)
        { 
            map[row][col] += 20;
        }
        else if(map[row][col] > 28)
            map[row][col] -= 20;
    }
}

void show()
{
    for (int i = 0; i < ROW; i++)
    {
        for (int k = 0; k < COL; k++)
        {
            printf("%2d", map[i][k]);
        }
        cout << endl;
    }
}



int main()
{
    //创建图形窗口
    initgraph(GRID_W* ROW, GRID_W * COL);
    gameInit();
    
    show();
    while (true)
    {
        gameDraw();
        mouseEvent();
    }
    return 0;
}


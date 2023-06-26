#include "framework.h"
#include"Resource.h"
#include<Windows.h>
#include"time.h"
#include"stdlib.h"
#include"conio.h"
#include"string.h"
#define IDI_ICON1                       101

#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        103
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif


#define WINDOW_WIDTH	540		///窗口的宽度
#define WINDOW_HEIGHT	639		///窗口的高度
#define ID_DOWNTIMER	1		///下降定时器


//实例句柄

HINSTANCE g_hInstance = NULL;
class Block {
private:
	int g_arrBackground[20][10] = { 0 };
	int g_arrPreviewBlock[2][4] = { 0 };
	int g_arr[2][4] = { 0 };
	int score = 0;
	int BlockForm = -1;
	int CurrentBlock = -1;
	POINT Pos = { -1,-1 };//坐标
	POINT CurrentPos = { -1,-1 };
	int index = 0;
	int speed = 1000;
public:
	//绘图
	void OnPaint(HDC hDC)
	{
		//创建一个内存兼容DC
		HDC hMemDC = CreateCompatibleDC(hDC);
		//创建一张兼容位图
		HBITMAP hBackBmp = CreateCompatibleBitmap(hDC, WINDOW_WIDTH, WINDOW_HEIGHT);
		SelectObject(hMemDC, hBackBmp);
		//绘制
		DrawBlock(hMemDC);
		//复制
		BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, hMemDC, 0, 0, SRCCOPY);
		//释放DC
		DeleteObject(hMemDC);
	}
	//绘制方块
	void DrawBlock(HDC hDC)
	{
		//填充客户区
		Rectangle(hDC, 0, 0, WINDOW_WIDTH - 16, 600);
		//绘制大背景
		Rectangle(hDC, 0, 0, 300, 600);

		//创建一个画刷
		HBRUSH hBrush = CreateSolidBrush(RGB(153, 0, 255));

		switch (BlockForm)
		{
		case 0:
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 1:
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			break;
		case 2:
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 3:
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			break;
		case 4:
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		default:
			break;
		}
		HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
		//绘制下降中每一个小方块
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1)
				{
					//绘制一个矩形区域
					Rectangle(hDC, j * 30, i * 30, j * 30 + 30 - 1, i * 30 + 30 - 1);
				}
				else if (g_arrBackground[i][j] == 3) {
					HBRUSH hBrush1 = CreateSolidBrush(RGB(192, 192, 192));
					HBRUSH HoldBrush = (HBRUSH)SelectObject(hDC, hBrush1);
					Rectangle(hDC, j * 30, i * 30, j * 30 + 30 - 1, i * 30 + 30 - 1);
				}
			}
		}
		//创建一个画刷
		HBRUSH hRedBrush = CreateSolidBrush(RGB(0, 0, 255));
		SelectObject(hDC, hRedBrush);
		//绘制已经降落的每一个小方块
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 2)
				{
					//绘制一个矩形区域
					Rectangle(hDC, j * 30, i * 30, j * 30 + 30 - 1, i * 30 + 30 - 1);
				}
			}
		}

		SelectObject(hDC, hOldBrush);
		//绘制预览图
		Rectangle(hDC, 360 - 3, 60 - 3, 480 + 3, 180 + 3);
		switch (CurrentBlock)
		{
		case 0:
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 1:
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			break;
		case 2:
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 3:
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			break;
		case 4:
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		default:
			break;
		}
		SelectObject(hDC, hBrush);
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (g_arrPreviewBlock[i][j] == 1)
					Rectangle(hDC, j * 30 + 360, i * 30 + 30 + 60, j * 30 + 30 - 1 + 360, i * 30 + 30 - 1 + 30 + 60);
				//         左上角的坐标    右下角坐标
			}
		}


		HFONT hFONT = CreateFont(
			20,
			0,
			0,
			0,
			FW_NORMAL,
			FALSE,
			FALSE,
			0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_SWISS,
			_T("楷体")
		);
		SelectObject(hDC, hFONT);
		TCHAR szText[200];
		wsprintf(szText, L"SCORE：%d", score);
		TextOut(hDC, 340, 210, szText, wcslen(szText));
		wsprintf(szText, L"难度系数：%d", (1000-speed)/100);
		TextOut(hDC, 340, 430, szText, wcslen(szText));
		TextOut(hDC, 340, 240, L"游戏说明：", wcslen(L"游戏说明："));
		TextOut(hDC, 340, 270, L"回车键：开始游戏", wcslen(L"回车键：开始游戏"));
		TextOut(hDC, 340, 300, L"向上键：变形", wcslen(L"向上键：变形"));
		TextOut(hDC, 340, 330, L"向下键：加速", wcslen(L"向下键：加速"));
		TextOut(hDC, 340, 360, L"向左键：左移", wcslen(L"向左键：左移"));
		TextOut(hDC, 340, 390, L"向右键：右移", wcslen(L"向右键：右移"));
		TextOut(hDC, 360, 30, L"NEXT BLOCK:",wcslen(L"NEXT BLOCK:"));
		SelectObject(hDC, hOldBrush);
	}

	//窗口创建处理函数
	void OnCreate()
	{
		//随机数播种
		srand((UINT)time(NULL));
		//生成随机方块图形
		CreateNewBlock();

	}

	//创建俄罗斯方块
	void CreateNewBlock()
	{
		memset(g_arrPreviewBlock, 0, sizeof(g_arrPreviewBlock));
		memset(g_arr, 0, sizeof(g_arr));
		index = rand() % 5;//0-4
		CurrentBlock = index;
		switch (index)
		{
		case 0:
			g_arrPreviewBlock[0][0] = 1, g_arrPreviewBlock[0][1] = 1, g_arrPreviewBlock[0][2] = 1, g_arrPreviewBlock[0][3] = 1;
			g_arrPreviewBlock[1][0] = 0, g_arrPreviewBlock[1][1] = 0, g_arrPreviewBlock[1][2] = 0, g_arrPreviewBlock[1][3] = 0;
			CurrentPos.x = 0;
			CurrentPos.y = 4;
			break;
		case 1:
			g_arrPreviewBlock[0][0] = 0, g_arrPreviewBlock[0][1] = 0, g_arrPreviewBlock[0][2] = 1, g_arrPreviewBlock[0][3] = 0;
			g_arrPreviewBlock[1][0] = 1, g_arrPreviewBlock[1][1] = 1, g_arrPreviewBlock[1][2] = 1, g_arrPreviewBlock[1][3] = 0;
			CurrentPos.x = 0;
			CurrentPos.y = 3;
			break;
		case 2:
			g_arrPreviewBlock[0][0] = 1, g_arrPreviewBlock[0][1] = 1, g_arrPreviewBlock[0][2] = 0, g_arrPreviewBlock[0][3] = 0;
			g_arrPreviewBlock[1][0] = 0, g_arrPreviewBlock[1][1] = 1, g_arrPreviewBlock[1][2] = 1, g_arrPreviewBlock[1][3] = 0;
			CurrentPos.x = 0;
			CurrentPos.y = 3;
			break;
		case 3:
			g_arrPreviewBlock[0][0] = 0, g_arrPreviewBlock[0][1] = 1, g_arrPreviewBlock[0][2] = 1, g_arrPreviewBlock[0][3] = 0;
			g_arrPreviewBlock[1][0] = 0, g_arrPreviewBlock[1][1] = 1, g_arrPreviewBlock[1][2] = 1, g_arrPreviewBlock[1][3] = 0;
			CurrentPos.x = 0;
			CurrentPos.y = 3;
			break;
		case 4:
			g_arrPreviewBlock[0][0] = 0, g_arrPreviewBlock[0][1] = 1, g_arrPreviewBlock[0][2] = 0, g_arrPreviewBlock[0][3] = 0;
			g_arrPreviewBlock[1][0] = 1, g_arrPreviewBlock[1][1] = 1, g_arrPreviewBlock[1][2] = 1, g_arrPreviewBlock[1][3] = 0;
			CurrentPos.x = 0;
			CurrentPos.y = 3;
			break;
		default:
			break;
		}
		BottomPre();
	}

	//复制随机块到网格地图
	void CopyCurrentBlock()
	{
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				g_arrBackground[i][j + 3] = g_arr[i][j];
			}
		}
	}

	//按下回车键
	void OnReturn(HWND hWnd)
	{
		//排除正在下落时又下降一个
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (g_arrBackground[i][j] == 1) {
					return;
				}
			}
		}
		//预览块复制给下落块
		memcpy(g_arr, g_arrPreviewBlock, sizeof(g_arrPreviewBlock));
		Pos = CurrentPos;
		BlockForm = CurrentBlock;
		//复制到网格
		CopyCurrentBlock();
		//创建新预览方块
		CreateNewBlock();
		SetTimer(hWnd, ID_DOWNTIMER, 1000, NULL);
	}
	//定时器消息处理函数
	void OnTimer(HWND hWnd)
	{
		HDC hDC = GetDC(hWnd);
		//判断方块是否到了最底端
		//没有到达底端
		if (FALSE == IsGridBottom() && FALSE == IsOtherBlockTop())
		{
			//方块下落
			BlockDown();
			Pos.x++;
		}
		else//已到达底端
		{
			//调整原有方块状态
			AdjustBlockState();
			//消除行
			ClearLine();
			//调整下落速度
			KillTimer(hWnd, ID_DOWNTIMER);
			if (score > 8)
				speed = 300;
			else
				speed = 1000 - score * score * 10;
			SetTimer(hWnd, ID_DOWNTIMER, speed, NULL);
			//判断游戏是否结束
			if (IsGameOver() == TRUE)
			{
				//结束关闭定时器
				KillTimer(hWnd, ID_DOWNTIMER);
				return;
			}
			//预览块复制给下落块
			memcpy(g_arr, g_arrPreviewBlock, sizeof(g_arrPreviewBlock));
			//预览块坐标给当前块
			Pos = CurrentPos;
			BlockForm = CurrentBlock;
			//复制到网格
			CopyCurrentBlock();
			//创建新预览方块
			CreateNewBlock();
		}
		//绘制出来
		OnPaint(hDC);
	}
	void BlockDown()
	{
		for (int i = 19; i >= 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1)
				{
					g_arrBackground[i + 1][j] = 1;
					g_arrBackground[i][j] = 0;
				}
			}
		}
	}

	//判断方块是否降落到网格最底端
	BOOL IsGridBottom()
	{
		for (int i = 0; i < 10; i++)
		{
			if (g_arrBackground[19][i] == 1)
				return TRUE;
		}
		return FALSE;
	}

	//调整原有方块状态
	void AdjustBlockState()
	{
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1)
					g_arrBackground[i][j] = 2;
			}
		}
	}

	//判断方块是否降落到其他方块上边
	BOOL IsOtherBlockTop()
	{
		for (int i = 19; i >= 0; i--)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1 && g_arrBackground[i + 1][j] == 2)
					return TRUE;
			}
		}
		return FALSE;
	}

	//方块左移
	void OnLeft()
	{
		//限制左边界
		for (int i = 0; i < 20; i++)
		{
			if (g_arrBackground[i][0] == 1)
				return;
		}
		//碰到左边方块
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1 && g_arrBackground[i][j - 1] == 2)
					return;
			}
		}

		//方块左移
		for (int i = 0; i < 20; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 1)
				{
					g_arrBackground[i][j - 1] = 1;
					g_arrBackground[i][j] = 0;
				}
			}
		}
		Pos.y--;
	}

	//方块右移
	void OnRight()
	{

		//限制右边界
		for (int i = 0; i < 20; i++)
		{
			if (g_arrBackground[i][9] == 1)
				return;
		}
		//碰到右边方块
		for (int i = 0; i < 20; i++)
		{
			for (int j = 9; j >= 0; j--)
			{
				if (g_arrBackground[i][j] == 1 && g_arrBackground[i][j + 1] == 2)
					return;
			}
		}

		//方块右移
		for (int i = 0; i < 20; i++)
		{
			for (int j = 9; j >= 0; j--)
			{
				if (g_arrBackground[i][j] == 1)
				{
					g_arrBackground[i][j + 1] = 1;
					g_arrBackground[i][j] = 0;
				}
			}
		}

		Pos.y++;
	}

	//方块下移
	void OnDown(HWND hWnd)
	{
		OnTimer(hWnd);
	}

	void LongBlock() {
		//判断是否第一行。
		if (Pos.x < 1) {
			return;
		}
		//判断是横向
		if (g_arrBackground[Pos.x][Pos.y - 1] == 1)
		{
			if (Pos.y > 8)//在第十列的情况
			{
				if (g_arrBackground[Pos.x - 1][Pos.y] == 2 || g_arrBackground[Pos.x + 1][Pos.y] == 2 || g_arrBackground[Pos.x + 2][Pos.y] == 2 || g_arrBackground[Pos.x - 1][Pos.y - 1] == 2 || g_arrBackground[Pos.x - 2][Pos.y - 1] == 2)
					return;
				g_arrBackground[Pos.x - 1][Pos.y] = 1;
				g_arrBackground[Pos.x + 1][Pos.y] = 1;
				g_arrBackground[Pos.x + 2][Pos.y] = 1;
				g_arrBackground[Pos.x][Pos.y - 1] = 0;
				g_arrBackground[Pos.x][Pos.y - 2] = 0;
				g_arrBackground[Pos.x][Pos.y - 3] = 0;
			}
			else if (g_arrBackground[Pos.x - 1][Pos.y] == 2 || g_arrBackground[Pos.x + 1][Pos.y] == 2 || g_arrBackground[Pos.x + 2][Pos.y] == 2 || g_arrBackground[Pos.x - 1][Pos.y + 1] == 2 || g_arrBackground[Pos.x - 1][Pos.y + 2] == 2) {
				return;
			}
			else {
				g_arrBackground[Pos.x][Pos.y - 1] = 0;
				g_arrBackground[Pos.x][Pos.y + 1] = 0;
				g_arrBackground[Pos.x][Pos.y + 2] = 0;
				g_arrBackground[Pos.x - 1][Pos.y] = 1;
				g_arrBackground[Pos.x + 1][Pos.y] = 1;
				g_arrBackground[Pos.x + 2][Pos.y] = 1;

			}
		}
		else //当前纵向
		{
			if (Pos.y < 1)//在第一列的情况
			{
				if (g_arrBackground[Pos.x][Pos.y + 1] == 2 || g_arrBackground[Pos.x][Pos.y + 2] == 2 || g_arrBackground[Pos.x][Pos.y + 3] == 2 || g_arrBackground[Pos.x - 1][Pos.y + 1] == 2 || g_arrBackground[Pos.x - 2][Pos.y + 1] == 2)
					return;
				g_arrBackground[Pos.x - 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 2][Pos.y] = 0;
				g_arrBackground[Pos.x][Pos.y + 1] = 1;
				g_arrBackground[Pos.x][Pos.y + 2] = 1;
				g_arrBackground[Pos.x][Pos.y + 3] = 1;
				Pos.y = Pos.y + 1;
			}
			else if (Pos.y == 8) {//在第九列时
				if (g_arrBackground[Pos.x][Pos.y + 1] == 2 || g_arrBackground[Pos.x][Pos.y - 2] == 2 || g_arrBackground[Pos.x][Pos.y - 1] == 2 || g_arrBackground[Pos.x + 1][Pos.y - 1] == 2 || g_arrBackground[Pos.x + 1][Pos.y - 2] == 2)
					return;
				g_arrBackground[Pos.x - 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 2][Pos.y] = 0;
				g_arrBackground[Pos.x][Pos.y - 1] = 1;
				g_arrBackground[Pos.x][Pos.y - 2] = 1;
				g_arrBackground[Pos.x][Pos.y + 1] = 1;
				Pos.y = Pos.y - 1;

			}
			else if (Pos.y > 8)//在第十列的情况
			{
				if (g_arrBackground[Pos.x][Pos.y - 1] == 2 || g_arrBackground[Pos.x][Pos.y - 2] == 2 || g_arrBackground[Pos.x][Pos.y - 3] == 2 || g_arrBackground[Pos.x - 1][Pos.y - 1] == 2 || g_arrBackground[Pos.x - 2][Pos.y - 1] == 2)
					return;
				g_arrBackground[Pos.x - 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 2][Pos.y] = 0;
				g_arrBackground[Pos.x][Pos.y - 1] = 1;
				g_arrBackground[Pos.x][Pos.y - 2] = 1;
				g_arrBackground[Pos.x][Pos.y - 3] = 1;
				Pos.y = Pos.y - 2;
			}
			else {
				//在其他列时，判断是否符合旋转条件
				if (g_arrBackground[Pos.x][Pos.y - 1] == 2 || g_arrBackground[Pos.x][Pos.y + 1] == 2 || g_arrBackground[Pos.x][Pos.y + 2] == 2)
					return;
				if (g_arrBackground[Pos.x - 1][Pos.y - 1] == 2 || g_arrBackground[Pos.x - 2][Pos.y - 1] == 2 || g_arrBackground[Pos.x - 1][Pos.y + 1] == 2 || g_arrBackground[Pos.x - 2][Pos.y + 1] == 2)
					return;
				g_arrBackground[Pos.x - 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 1][Pos.y] = 0;
				g_arrBackground[Pos.x + 2][Pos.y] = 0;
				g_arrBackground[Pos.x][Pos.y - 1] = 1;
				g_arrBackground[Pos.x][Pos.y + 1] = 1;
				g_arrBackground[Pos.x][Pos.y + 2] = 1;
			}
		}
	}
	void CommonBlock()
	{
		int arrChange[3][3] = { 0 };
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				arrChange[i][j] = g_arrBackground[Pos.x + i][Pos.y + j];
			}
		}
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (arrChange[i][j] == 2)//排除不能转的情况
					return;
			}
		}
		if (Pos.y < 0)
			Pos.y = 0;
		if (Pos.y + 2 > 9)
			Pos.y = 7;
		for (int i = 0; i < 3; i++)
		{
			int Temp = 2;
			for (int j = 0; j < 3; j++)
			{
				g_arrBackground[Pos.x + i][Pos.y + j] = arrChange[Temp][i];
				Temp--;
			}
		}

	}
	void OnUp()
	{
		switch (BlockForm)
		{
		case 0:
			LongBlock();
			break;
		case 1:
			CommonBlock();
			break;
		case 2:
			CommonBlock();
			break;
		case 3:
			break;
		case 4:
			CommonBlock();
			break;
		default:
			break;
		}
	}

	//消除行
	void ClearLine()
	{
		int line = 0;
		int total;
		for (int i = 19; i >= 0; i--) {
			total = 0;
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 2)
				{
					total++;
				}
			}
			if (total == 10)
				line++;
		}
		if (line > 1)
			score = score + line * line;
		if (line == 1)
			score++;
		for (int i = 19; i >= 0; i--) {

			total = 0;
			for (int j = 0; j < 10; j++)
			{
				if (g_arrBackground[i][j] == 2)
				{
					total++;
				}
			}
			if (total == 10) {
				for (int k = 0; k < 10; k++) {
					Sleep(20);
					g_arrBackground[i][k] = 0;
				}
				for (int t = i; t >= 0; t--) {
					for (int m = 0; m < 10; m++) {
						g_arrBackground[t][m] = g_arrBackground[t - 1][m];
					}
				}
				i++;
			}
		}
	}

	//判断游戏结束
	BOOL IsGameOver()
	{
		for (int i = 0; i < 10; i++)
		{
			if (g_arrBackground[0][i] == 2)
			{
				TCHAR szText[20];
				wsprintf(szText, L"  游戏结束!\n得分：%d", score);
				MessageBox(NULL, szText, L"俄罗斯方块", MB_OK);
				return TRUE;
			}
		}
		return FALSE;
	}

	void BottomPre() {
		for (int i = 0; i < 20; i++) {
			for (int j = 0; j < 10; j++) {
				if (g_arrBackground[i][j] == 3)
					g_arrBackground[i][j] = 0;
			}
		}
		switch (BlockForm)
		{
		case 0:
			for (int i = 19; i >= Pos.x; i--) {
				int flag = 0;
				if (g_arrBackground[Pos.x][Pos.y + 1] == 1) {//是横向的
					if (g_arrBackground[i][Pos.y] == 0 && g_arrBackground[i][Pos.y - 1] == 0 && g_arrBackground[i][Pos.y + 1] == 0 && g_arrBackground[i][Pos.y + 2] == 0) {
						flag = 1;
						/*for (int a = Pos.x+1; a < i; a++) {
								if (g_arrBackground[i][a] == 0 || g_arrBackground[i][a] == 1)
									continue;
								else
									flag = 0;
						}*/
						if (flag == 1)
						{
							g_arrBackground[i][Pos.y - 1] = 3;
							g_arrBackground[i][Pos.y] = 3;
							g_arrBackground[i][Pos.y + 1] = 3;
							g_arrBackground[i][Pos.y + 2] = 3;
						}

					}
				}
				else if (g_arrBackground[Pos.x + 1][Pos.y] == 1) {//是纵向的
					if (g_arrBackground[i][Pos.y] == 0 && g_arrBackground[i - 1][Pos.y] == 0 && g_arrBackground[i + 1][Pos.y] == 0 && g_arrBackground[i + 2][Pos.y] == 0) {
						flag = 1;
						/* for (int a = Pos.x + 3; a <= i-2; a++) {
								 if (g_arrBackground[a][Pos.y] == 0 || g_arrBackground[a][Pos.y] == 1)
									 continue;
								 else
									 flag = 0;
						 }*/
						if (flag == 1)
						{
							if (i == 19)
								i -= 2;
							if (g_arrBackground[i - 1][Pos.y] != 1)
								g_arrBackground[i - 1][Pos.y] = 3;
							if (g_arrBackground[i][Pos.y] != 1)
								g_arrBackground[i][Pos.y] = 3;
							if (g_arrBackground[i + 1][Pos.y] != 1)
								g_arrBackground[i + 1][Pos.y] = 3;
							if (g_arrBackground[i + 2][Pos.y] != 1)
								g_arrBackground[i + 2][Pos.y] = 3;
						}
					}
				}
				if (flag == 1)
					break;
			}
			break;
		case 1:case 2:case 3:case 4:
		{
			int n = 0;
			int x = 19;
			int temp = Pos.x + 2;
			for (; x > Pos.x; x--)
			{
				int flag = 0;
				n = x;
				for (int k = Pos.x + 2; k >= Pos.x; k--)//下落中的方块横向坐标  
				{
					for (int j = Pos.y; j <= Pos.y + 2; j++)
						if (g_arrBackground[k][j] == 1 && g_arrBackground[n][j] == 0) {
							flag = flag + 1;                              //预览方块横坐标
						}
					n = n - 1;//减到了x-2;
				}
				if (flag == 4)
				{
					flag = 1;
					for (int a = Pos.x; a <= n; a++) {
						for (int j = Pos.y; j <= Pos.y + 2; j++)
						{
							if (g_arrBackground[a][j] == 0 || g_arrBackground[a][j] == 1)
								continue;
							else {
								flag = 0;

							}
						}
					}
					if (flag == 1)
					{
						temp = x;
						break;
					}
					else
						continue;
				}
			}
			int flag1 = 0;
			int k = 0;
		//for (int j = Pos.y; j <= Pos.y + 2; j++)//判断第三行是否都为0
		//	{
		//		if (g_arrBackground[Pos.x + 2][j] == 0)
		//			flag1 = flag1 + 1;
		//	}
		//	if (flag1 == 3) //说明最后一行都是0
		//		k = Pos.x + 1;
		//	else
				k = Pos.x + 2;
			if (temp > Pos.x + 2)
			{
				for (; k >= Pos.x; k--) {
					for (int j = Pos.y; j <= Pos.y + 2; j++) {
						if (g_arrBackground[k][j] == 1 && g_arrBackground[temp][j] == 0)
							g_arrBackground[temp][j] = 3;                              //预览方块横坐标   
					}
					temp--;
				}
			}
		}
		break;
		default:
			break;
		}
	}
};



Block block;
//窗口处理函数
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	static HDC hDC = NULL;

	switch (uMsg)
	{
	case WM_CREATE:		//窗口创建消息
		block.OnCreate();
		break;
	case WM_KEYDOWN:	//键盘按下
		switch (wParam)
		{
		case VK_RETURN:	//按回车键
			block.OnReturn(hWnd);
			break;
		case VK_LEFT:	//按左键
			block.OnLeft();
			block.BottomPre();
			break;
		case VK_RIGHT:	//按右键
			block.OnRight();
			block.BottomPre();
			break;
		case VK_UP:		//按上键
			block.OnUp();
			block.BottomPre();
			break;
		case VK_DOWN:	//按下键
			block.OnDown(hWnd);
			break;
		}
		break;
	case WM_TIMER:		//定时器消息
		switch (wParam)
		{
		case ID_DOWNTIMER:
			block.OnTimer(hWnd);
			break;
		}
		break;
	case WM_PAINT:		//绘图消息
		hDC = BeginPaint(hWnd, &ps);
		block.OnPaint(hDC);
		EndPaint(hWnd, &ps);
		break;
	case WM_CLOSE:		//窗口关闭消息
		KillTimer(hWnd, ID_DOWNTIMER);			//关闭定时器
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:	//窗口销毁消息
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpCmdLine, int nCmdShow)
{
	g_hInstance = hInstance;

	//设计窗口类
	TCHAR szAppClassName[] = TEXT("Tetrics");

	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY));
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WindowProc;
	wndClass.lpszClassName = szAppClassName;
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

	//注册窗口类
	RegisterClass(&wndClass);

	//获取屏幕的宽和高
	int nWidth = GetSystemMetrics(SM_CXSCREEN);
	int nHeight = GetSystemMetrics(SM_CYSCREEN);

	//创建窗口
	HWND hWnd = CreateWindow(szAppClassName, 
		L"经典俄罗斯方块",
		WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX|WS_MAXIMIZEBOX, 
		(nWidth - WINDOW_WIDTH) / 2, (nHeight - WINDOW_HEIGHT) / 2, 
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		NULL, 
		NULL,
		hInstance, 
		NULL);

	//显示窗口
	ShowWindow(hWnd, SW_SHOW);

	//更新窗口
	UpdateWindow(hWnd);

	//消息循环
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

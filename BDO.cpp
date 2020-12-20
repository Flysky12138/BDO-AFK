#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>
#include <atlimage.h>
#include <gdiplus.h>
#include <shlobj.h>
#include <windows.h>
#pragma comment(lib, "Gdiplus.lib")
#pragma warning(disable : 4996)

using namespace std;
using namespace Gdiplus;

string Code[200][5]; //待运行文本
string KeyCode[10][2] = {
	{"0", "48"},
	{"1", "49"},
	{"2", "50"},
	{"3", "51"},
	{"4", "52"},
	{"5", "53"},
	{"6", "54"},
	{"7", "55"},
	{"8", "56"},
	{"9", "57"},
}; //虚拟键码
string GameKeyCode[80][2] = {
	{"ESC", "1"},
	{"1", "2"},
	{"2", "3"},
	{"3", "4"},
	{"4", "5"},
	{"5", "6"},
	{"6", "7"},
	{"7", "8"},
	{"8", "9"},
	{"9", "10"},
	{"0", "11"},
	{"", "12"},
	{"", "13"},
	{"", "14"},
	{"TAB", "15"},
	{"Q", "16"},
	{"W", "17"},
	{"E", "18"},
	{"R", "19"},
	{"T", "20"},
	{"Y", "21"},
	{"U", "22"},
	{"I", "23"},
	{"O", "24"},
	{"P", "25"},
	{"", "26"},
	{"", "27"},
	{"ENTER", "28"},
	{"CTRL", "29"},
	{"A", "30"},
	{"S", "31"},
	{"D", "32"},
	{"F", "33"},
	{"G", "34"},
	{"H", "35"},
	{"J", "36"},
	{"K", "37"},
	{"L", "38"},
	{"", "39"},
	{"", "40"},
	{"~", "41"},
	{"SHIFT", "42"},
	{"", "43"},
	{"Z", "44"},
	{"X", "45"},
	{"C", "46"},
	{"V", "47"},
	{"B", "48"},
	{"N", "49"},
	{"M", "50"},
	{",", "51"},
	{"", "52"},
	{"", "53"},
	{"", "54"},
	{"", "55"},
	{"ALT", "56"},
	{"SPACE", "57"},
	{"", "58"},
	{"F1", "59"},
	{"F2", "60"},
	{"F3", "61"},
	{"F4", "62"},
	{"F5", "63"},
	{"F6", "64"},
	{"F7", "65"},
	{"F8", "66"},
	{"F9", "67"},
	{"F10", "68"},
	{"F11", "69"},
	{"SCRLK", "70"},
}; //硬件扫描代码
HWND exeWhnd = GetConsoleWindow();
HWND gameWhnd, headWhnd;
int QuickKey = VK_OEM_3; //	`
string Title = "BDO - Flysky";

//string to int
int Getint(string str)
{
	return atoi(str.c_str());
}
//string to wstring
wstring Getwstring(string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t *wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	return wszUtf8;
}
//返回str在数组中对应的键码值
int GetCodeNum(string str, string keycode[][2])
{
	int i = 0;
	while (keycode[i])
	{
		if (str == keycode[i][0])
		{
			return Getint(keycode[i][1]);
		}
		i++;
	}
}
//返回num在数组中对应的字符串
string GetCodeStr(int num, string keycode[][2])
{
	int i = 0;
	while (keycode[i])
	{
		if (to_string(num) == keycode[i][1])
		{
			return keycode[i][0];
		}
		i++;
	}
}
//模拟键盘
void K(int key, int delay, int end)
{
	cout << "\r\u6309\u952e  [ " << GetCodeStr(key, GameKeyCode) << " ]" << endl;
	keybd_event(0, key, 0, 0);
	Sleep(delay);
	keybd_event(0, key, 2, 0);
	Sleep(end);
}
//模拟键盘
void K1(int key, int delay, int end)
{
	cout << "\r\u6309\u952e  [ " << GetCodeStr(key, KeyCode) << " ]" << endl;
	keybd_event(key, 0, 0, 0);
	Sleep(delay);
	keybd_event(key, 0, 2, 0);
	Sleep(end);
}
//模拟键盘(组合按键)
void K2(int key1, int key2, int delay, int end)
{
	cout << "\r\u6309\u952e  [ " << GetCodeStr(key1, GameKeyCode) << " + " << GetCodeStr(key2, GameKeyCode) << " ]" << endl;
	keybd_event(0, key1, 0, 0);
	Sleep(50);
	keybd_event(0, key2, 0, 0);
	Sleep(delay);
	keybd_event(0, key2, 2, 0);
	Sleep(50);
	keybd_event(0, key1, 2, 0);
	Sleep(end);
}
//移动鼠标指针
void MoveMouse(int x, int y)
{
	RECT rect = {x, y, x + 1, y + 1};
	ClipCursor(&rect);
	ClipCursor(NULL);
	Sleep(50);
}
//模拟鼠标左击
void L(int x, int y, int end)
{
	cout << "\r\u5de6\u51fb  [ " << x << "," << y << " ]" << endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
void L(int delay, int end)
{
	cout << "\r\u5de6\u51fb" << endl;
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标右击
void R(int x, int y, int end)
{
	cout << "\r\u53f3\u51fb  [ " << x << "," << y << " ]" << endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end);
}
void R(int delay, int end)
{
	cout << "\r\u53f3\u51fb" << endl;
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标拖动
void M(int x1, int y1, int x2, int y2, int end)
{
	cout << "\r\u79fb\u52a8  [ " << x1 << "," << y1 << " " << x2 << "," << y2 << " ]" << endl;
	MoveMouse(x1, y1);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	MoveMouse(x2, y2);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标滚轮
void W(int x, int y, int z, int end)
{
	cout << "\r\u6eda\u8f6e  [ " << x << "," << y << " " << z << " ]" << endl;
	MoveMouse(x, y);
	while (z)
	{
		mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -WHEEL_DELTA, 0);
		z--;
		if (z)
		{
			Sleep(50 + rand() % 101); //[50,150]
		}
	}
	Sleep(end);
}
//等待时间
void S(int end)
{
	Sleep(end);
}
//打开游戏
void OPEN(int end)
{
	while (IsWindowVisible(gameWhnd))
	{
		Sleep(5000);
	}
	headWhnd = GetForegroundWindow();
	cout << "\r\u6253\u5f00" << endl;
	ShowWindow(gameWhnd, 1);
	SetWindowPos(gameWhnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	MoveMouse(960, 540);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	if (end < 0)
	{
		end = -end;
		Sleep(100);
		SetWindowPos(gameWhnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	}
	if (!IsWindowVisible(gameWhnd))
	{
		exit(0);
	}
	Sleep(end);
}
//关闭游戏
void CLOSE()
{
	if (IsWindowVisible(gameWhnd))
	{
		cout << "\r\u5173\u95ed" << endl;
		ShowWindow(gameWhnd, 0);
		SetForegroundWindow(headWhnd);
	}
}
//读取文件并储存于Code
void ReadFiletoCode(string file)
{
	string line, out;
	ifstream in(file);
	if (in)
	{
		for (int i = 0; i < sizeof(Code) / sizeof(Code[0]); i++)
		{
			for (int j = 0; j < sizeof(Code[0]) / sizeof(Code[0][0]); j++)
			{
				Code[i][j] = "\0";
			}
		}
		int length = 0;
		while (getline(in, line))
		{
			int y = 0;
			istringstream str(line);
			while (str >> out)
			{
				Code[length][y] = out;
				y++;
			}
			length++;
		}
	}
}
//输出字的颜色
void CoutColor(int tag)
{
	switch (tag)
	{
	case 0:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		break;
	case 111:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 100:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
}
//倒计时
void ShowTime(int delay)
{
	while (delay != 0)
	{
		cout << "\r\u5012\u8ba1\u65f6: ";
		CoutColor(100);
		cout << setw(2) << setfill('0') << delay / 60 << "\u5206" << setw(2) << setfill('0') << delay % 60 << "\u79d2 ";
		CoutColor(111);
		if (delay <= 3 && !IsWindowVisible(gameWhnd))
		{
			HWND sqa = GetForegroundWindow();
			if (exeWhnd != sqa)
			{
				ShowWindow(exeWhnd, 1);
				SetWindowPos(exeWhnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				SetForegroundWindow(sqa);
			}
			if (delay == 1)
				ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
		}
		Sleep(1000);
		delay--;
	}
}
//设置热键
void SetHotKey(int key, string tag)
{
	if (tag == "start")
	{
		while (true)
		{
			if (key > 0)
			{
				if (GetAsyncKeyState(key) & 0x8000)
				{
					break;
				}
			}
			else
			{
				if (GetAsyncKeyState(-key) & 0x8000)
				{
					while (GetAsyncKeyState(-key) & 0x8000)
					{
						Sleep(1);
					}
					break;
				}
			}
			Sleep(1);
		}
	}
	else if (tag == "stop")
	{
		while (true)
		{
			if (GetAsyncKeyState(VK_END) & 0x8000)
			{
				exit(0);
			}
			Sleep(1);
		}
	}
}
//设置窗口标题
void SetTitle(string path)
{
	string str1 = path.substr(path.find_last_of('\\') + 1);
	string str2 = str1.substr(0, str1.find_last_of('.'));
	LPCSTR title = (LPCSTR)str2.c_str();
	SetConsoleTitleA(title);
}
//设置窗口样式
void SetStyle()
{
	SMALL_RECT rc = {0, 0, 30, 20};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &rc);
	SetWindowLongPtrA(
		exeWhnd,
		GWL_STYLE,
		GetWindowLongPtrA(exeWhnd, GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX);
	ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
}
//str复制到剪切板
void ClipBoard(char *str)
{
	HWND hWnd = NULL;
	OpenClipboard(hWnd);
	EmptyClipboard();
	HANDLE hHandle = GlobalAlloc(GMEM_FIXED, 1000);
	char *pData = (char *)GlobalLock(hHandle);
	strcpy(pData, str);
	SetClipboardData(CF_TEXT, hHandle);
	GlobalUnlock(hHandle);
	CloseClipboard();
}
//快捷键获取坐标，输出显示，复制到剪切板
void GetPosition()
{
	int hotkey = 1000;
	RegisterHotKey(NULL, hotkey, MOD_ALT, QuickKey);
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.wParam == hotkey)
		{
			POINT p;
			GetCursorPos(&p);
			string str = to_string(p.x) + "," + to_string(p.y);
			char *position = (char *)str.c_str();
			cout << position << endl;
			ClipBoard(position);
		}
	}
}
//快捷键获取鼠标坐标
void GetPosition(int &x, int &y)
{
	int hotkey = 1000;
	RegisterHotKey(NULL, hotkey, MOD_ALT, QuickKey);
	MSG msg = {0};
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.wParam == hotkey)
		{
			POINT p;
			GetCursorPos(&p);
			x = p.x;
			y = p.y;
			break;
		}
	}
}
//获取文档路径
string GetDocumentsPath()
{
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	char szDir[200];
	if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_PERSONAL, &pidl)))
		{
			SHGetPathFromIDListA(pidl, szDir);
			pShellMalloc->Free(pidl);
		}
		pShellMalloc->Release();
	}
	return string(szDir);
}
//游戏全屏截取保存为BDO.bmp
void ScreenShot()
{
	HDC hDCScreen = GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hDCScreen, BITSPIXEL);
	int nWidthScreen = GetDeviceCaps(hDCScreen, HORZRES);
	int nHeightScreen = GetDeviceCaps(hDCScreen, VERTRES);
	CImage m_MyImage;
	m_MyImage.Create(nWidthScreen, nHeightScreen, nBitPerPixel);
	HDC hDCImg = m_MyImage.GetDC();
	BitBlt(hDCImg, 0, 0, nWidthScreen, nHeightScreen, hDCScreen, 0, 0, SRCCOPY);
	CString strFileName = GetDocumentsPath().c_str();
	strFileName += _T("\\Black Desert\\ScreenShot\\");
	CreateDirectory((LPCTSTR)strFileName, NULL);
	strFileName += _T("BDO.bmp");
	m_MyImage.Save(strFileName, ImageFormatPNG);
	m_MyImage.ReleaseDC();
}
//快捷键获取坐标RGB颜色，输出显示，复制到剪切板
void GetXyColor()
{
	int x = 0, y = 0;
	GetPosition(x, y);
	ScreenShot();
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	string imgPath = GetDocumentsPath() + "\\Black Desert\\ScreenShot\\BDO.bmp";
	Bitmap *image = new Bitmap(Getwstring(imgPath).c_str());
	Color color1, color2;
	image->GetPixel(x, y, &color1);
	image->GetPixel(x + 10, y + 10, &color2);
	string colorStr;
	colorStr = "color(" + to_string(color1.GetRed()) + "," + to_string(color1.GetGreen()) + "," + to_string(color1.GetBlue()) + ")(";
	colorStr = colorStr + to_string(color2.GetRed()) + "," + to_string(color2.GetGreen()) + "," + to_string(color2.GetBlue()) + ")";
	delete image;
	GdiplusShutdown(gdiplusToken);
	char *str = (char *)colorStr.c_str();
	cout << str << endl;
	ClipBoard(str);
}
//根据颜色找到匹配的第一个坐标
void getColorXY(int red1, int green1, int blue1, int red2, int green2, int blue2, int &x, int &y)
{
	ULONG_PTR gdiplusToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	string imgPath = GetDocumentsPath() + "\\Black Desert\\ScreenShot\\BDO.bmp";
	Bitmap *image = new Bitmap(Getwstring(imgPath).c_str());
	int width = image->GetWidth();
	int height = image->GetHeight();
	Color color1, color2;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			image->GetPixel(i, j, &color1);
			if (red1 == color1.GetRed() && green1 == color1.GetGreen() && blue1 == color1.GetBlue())
			{
				image->GetPixel(i + 10, j + 10, &color2);
				if (red2 == color2.GetRed() && green2 == color2.GetGreen() && blue2 == color2.GetBlue())
				{
					x = i;
					y = j;
					return;
				}
			}
		}
	}
	delete image;
	GdiplusShutdown(gdiplusToken);
}
//得到坐标
void GetXY(string &str, int &x, int &y)
{
	//x,y
	if (str == "x,y")
	{
		GetPosition(x, y);
		str = to_string(x) + ',' + to_string(y);
		Sleep(1000);
	}
	//color(255,255,255)
	else if (str.substr(0, 5) == "color")
	{
		ScreenShot();
		int R1 = 0, G1 = 0, B1 = 0;
		int R2 = 0, G2 = 0, B2 = 0;
		{
			string rgb1;
			rgb1 = str.substr(str.find("(") + 1, str.find(")") - 6); //r,g,b
			R1 = Getint(rgb1.substr(0, rgb1.find(",")));
			rgb1 = rgb1.substr(rgb1.find(",") + 1); //g,b
			G1 = Getint(rgb1.substr(0, rgb1.find(",")));
			rgb1 = rgb1.substr(rgb1.find(",") + 1); //b
			B1 = Getint(rgb1);
		}
		{
			string rgb2;
			rgb2 = str.substr(str.find(")(") + 2, str.find_last_of(")") - 16); //r,g,b
			R2 = Getint(rgb2.substr(0, rgb2.find(",")));
			rgb2 = rgb2.substr(rgb2.find(",") + 1); //g,b
			G2 = Getint(rgb2.substr(0, rgb2.find(",")));
			rgb2 = rgb2.substr(rgb2.find(",") + 1); //b
			B2 = Getint(rgb2);
		}
		getColorXY(R1, G1, B1, R2, G2, B2, x, y);
	}
	//0,0
	else
	{
		size_t division = str.find_last_of(',');
		x = Getint(str.substr(0, division));
		y = Getint(str.substr(1 + division));
	}
}
//单行脚本处理
void RunKey(string str[])
{
	if (IsWindowVisible(gameWhnd))
	{
		if (str[0] == "K")
		{
			K(GetCodeNum(str[1], GameKeyCode), Getint(str[2]), Getint(str[3]));
		}
		if (str[0] == "K1")
		{
			K1(GetCodeNum(str[1], KeyCode), Getint(str[2]), Getint(str[3]));
		}
		if (str[0] == "K2")
		{
			K2(GetCodeNum(str[1], GameKeyCode), GetCodeNum(str[2], GameKeyCode), Getint(str[3]), Getint(str[4]));
		}
		if (str[0] == "L" || str[0] == "R" || str[0] == "W")
		{
			int x = 0, y = 0;
			bool Sign = false;
			if (str[1].find(',') != string::npos || str[1].find("color") != string::npos)
			{
				GetXY(str[1], x, y);
				Sign = true;
			}
			if (str[0] == "L")
			{
				Sign ? L(x, y, Getint(str[2])) : L(Getint(str[1]), Getint(str[2]));
			}
			if (str[0] == "R")
			{
				Sign ? R(x, y, Getint(str[2])) : R(Getint(str[1]), Getint(str[2]));
			}
			if (str[0] == "W")
			{
				W(x, y, Getint(str[2]), Getint(str[3]));
			}
		}
		if (str[0] == "M")
		{
			int x1 = 0, y1 = 0;
			int x2 = 0, y2 = 0;
			GetXY(str[1], x1, y1);
			GetXY(str[2], x2, y2);
			M(x1, y1, x2, y2, Getint(str[3]));
		}
		if (str[0] == "S")
		{
			S(Getint(str[1]));
		}
	}
	if (str[0] == "OPEN")
	{
		OPEN(Getint(str[1]));
	}
	if (str[0] == "CLOSE")
	{
		CLOSE();
	}
}
//FOR循环
int RunFor(int i, int times)
{
	int j;
	while (times--)
	{
		for (j = i + 1; Code[j][0] != "END" && Code[j][0] != "\0"; j++)
		{
			if (Code[j][0] == "FOR")
			{
				j = RunFor(j, Getint(Code[j][1]));
			}
			RunKey(Code[j]);
		}
	}
	return j;
}
//脚本整体处理
void RunCode()
{
	if (Code[0][0] == "BDO")
	{
		int num = 1, total = Getint(Code[0][1]), HotKey = Getint(Code[0][3]);
		while (total--)
		{
			if (HotKey)
			{
				CoutColor(100);
				cout << "\u542f\u52a8\u70ed\u952e\u952e\u7801  " << HotKey << endl;
				cout << "\u505c\u6b62\u70ed\u952e\u6309\u952e  END ";
				SetHotKey(HotKey, "start");
			}
			system("cls");
			CoutColor(111);
			cout << "\u8fd0\u884c\u6b21\u6570: ";
			CoutColor(100);
			cout << num++ << " / " << Getint(Code[0][1]) << endl
				 << endl;
			CoutColor(0);
			gameWhnd = FindWindow("BlackDesertWindowClass", NULL);
			for (int i = 0; Code[i][0] != "\0"; i++)
			{
				if (Code[i][0] == "FOR")
				{
					i = RunFor(i, Getint(Code[i][1]));
				}
				RunKey(Code[i]);
			}
			CoutColor(111);
			cout << endl;
			if (total != 0)
			{
				ShowTime(Getint(Code[0][2]));
			}
		}
	}
}
//主函数
int main(int argc, char *argv[])
{
	switch (argc)
	{
	case 1:
		SetStyle();
		SetTitle(Title);
		CoutColor(111);
		cout << "\u9009\u62e9\u6267\u884c\u9879\u76ee\uff1a" << endl;
		CoutColor(100);
		cout << "\u0061\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807" << endl;
		cout << "\u0062\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807\u5904\u989c\u8272" << endl;
		CoutColor(0);
		cout << "\u8f93\u5165\uff1a";
		CoutColor(111);
		char ch;
		ch = getchar();
		system("cls");
		cout << "\u5feb\u6377\u952e\u0020\u005b\u0020\u0041\u004c\u0054\u0020\u002b\u0020\u0060\u0020\u005d" << endl
			 << endl;
		CoutColor(0);
		switch (ch)
		{
		case 'A':
			GetPosition();
			break;
		default:
			while (true)
			{
				GetXyColor();
			}
		}
		break;
	case 2:
		SetStyle();
		SetTitle(argv[1]);
		ReadFiletoCode(argv[1]);
		Sleep(1000);
		if (Getint(Code[0][3]))
		{
			thread t(SetHotKey, 0, "stop");
			t.detach();
		}
		RunCode();
		break;
	default:
		SetStyle();
		SetTitle(Title);
		ShowWindow(exeWhnd, 1);
		cout << "QQ: 2797299614" << endl
			 << endl;
	}
	if (argc != 2)
		system("pause");
	return 0;
}

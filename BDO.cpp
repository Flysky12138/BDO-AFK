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
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4996)

CImage m_MyImage;
HWND exeWhnd = GetConsoleWindow();
HWND gameWhnd, headWhnd;
int QuickKey = VK_OEM_3;
std::string Title = "BDO - Flysky";
std::string KeyCode[86][3] = {
	{"Lb", "1", "0"},
	{"Rb", "2", "0"},
	{"Mb", "4", "0"},
	{"Esc", "27", "1"},
	{"1", "49", "2"},
	{"2", "50", "3"},
	{"3", "51", "4"},
	{"4", "52", "5"},
	{"5", "53", "6"},
	{"6", "54", "7"},
	{"7", "55", "8"},
	{"8", "56", "9"},
	{"9", "57", "10"},
	{"0", "48", "11"},
	{"-", "189", "12"},
	{"=", "187", "13"},
	{"Bs", "8", "14"},
	{"Tab", "9", "15"},
	{"Q", "81", "16"},
	{"W", "87", "17"},
	{"E", "68", "18"},
	{"R", "82", "19"},
	{"T", "84", "20"},
	{"Y", "89", "21"},
	{"U", "85", "22"},
	{"I", "73", "23"},
	{"O", "79", "24"},
	{"P", "80", "25"},
	{"[", "219", "26"},
	{"]", "221", "27"},
	{"Enter", "13", "28"},
	{"Ctrl", "17", "29"},
	{"A", "65", "30"},
	{"S", "83", "31"},
	{"D", "68", "32"},
	{"F", "70", "33"},
	{"G", "71", "34"},
	{"H", "72", "35"},
	{"J", "74", "36"},
	{"K", "75", "37"},
	{"L", "76", "38"},
	{";", "186", "39"},
	{"'", "222", "40"},
	{"`", "192", "41"},
	{"LShift", "160", "42"},
	{"\\", "220", "43"},
	{"Z", "90", "44"},
	{"X", "88", "45"},
	{"C", "67", "46"},
	{"V", "86", "47"},
	{"B", "66", "48"},
	{"N", "78", "49"},
	{"M", "77", "50"},
	{",", "188", "51"},
	{".", "190", "52"},
	{"/", "191", "53"},
	{"RShift", "161", "54"},
	{"PrtSc", "42", "55"},
	{"Alt", "18", "56"},
	{"Space", "32", "57"},
	{"Caps", "20", "58"},
	{"F1", "112", "59"},
	{"F2", "113", "60"},
	{"F3", "114", "61"},
	{"F4", "115", "62"},
	{"F5", "116", "63"},
	{"F6", "117", "64"},
	{"F7", "118", "65"},
	{"F8", "119", "66"},
	{"F9", "120", "67"},
	{"F10", "121", "68"},
	{"Num", "144", "69"},
	{"Scroll", "145", "70"},
	{"Home", "36", "71"},
	{"Up", "38", "72"},
	{"PgUp", "33", "73"},
	{"/-", "109", "74"},
	{"Left", "37", "75"},
	{"Center", "1", "76"},
	{"Right", "39", "77"},
	{"/+", "107", "78"},
	{"End", "35", "79"},
	{"Down", "40", "80"},
	{"PgDn", "34", "81"},
	{"Insert", "45", "82"},
	{"Delete", "46", "83"},
}; //按键名、虚拟键码、OEM扫描码
std::string Code[200][5];
int Length = 0;

//string to int
int Getint(std::string str)
{
	return atoi(str.c_str());
}
//string to wstring
std::wstring Getwstring(std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t *wszUtf8 = new wchar_t[len + 1.0];
	memset(wszUtf8, 0, len * 2.0 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	return wszUtf8;
}
//返回str在数组中对应的值
int GetCodeNum(std::string str, int tag)
{
	int i = 0;
	while (KeyCode[i])
	{
		if (str == KeyCode[i][0])
		{
			if (tag == 0)
			{
				return Getint(KeyCode[i][1]);
			}
			else if (tag == 1)
			{
				return Getint(KeyCode[i][2]);
			}
		}
		i++;
	}
	return 0;
}
//模拟键盘
void K(std::string key, int end)
{
	std::cout << "\r\u6309\u952e  [ " << key << " ]" << std::endl;
	int e1 = GetCodeNum(key, 0);
	int e2 = GetCodeNum(key, 1);
	keybd_event(e1, e2, 0, 0);
	Sleep(50);
	keybd_event(e1, e2, 2, 0);
	Sleep(end);
}
//模拟键盘(组合按键)
void K2(std::string key1, std::string key2, int delay, int end)
{
	std::cout << "\r\u6309\u952e  [ " << key1 << " + " << key2 << " ]" << std::endl;
	int e11 = GetCodeNum(key1, 0);
	int e12 = GetCodeNum(key1, 1);
	int e21 = GetCodeNum(key2, 0);
	int e22 = GetCodeNum(key2, 1);
	keybd_event(e11, e12, 0, 0);
	Sleep(50);
	keybd_event(e21, e22, 0, 0);
	Sleep(delay);
	keybd_event(e21, e22, 2, 0);
	Sleep(50);
	keybd_event(e11, e12, 2, 0);
	Sleep(end);
}
//移动鼠标指针
void MoveMouse(int x, int y)
{
	// RECT rect = {x, y, x + 1, y + 1};
	// ClipCursor(&rect);
	// ClipCursor(NULL);
	SetCursorPos(x, y);
	Sleep(50);
}
//模拟鼠标左击
void L(int x, int y, int end)
{
	std::cout << "\r\u5de6\u51fb  [ " << x << "," << y << " ]" << std::endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
void L(int delay, int end)
{
	std::cout << "\r\u5de6\u51fb" << std::endl;
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标右击
void R(int x, int y, int end)
{
	std::cout << "\r\u53f3\u51fb  [ " << x << "," << y << " ]" << std::endl;
	MoveMouse(x, y);
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end);
}
void R(int delay, int end)
{
	std::cout << "\r\u53f3\u51fb" << std::endl;
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	Sleep(delay);
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标拖动
void M(int x1, int y1, int x2, int y2, int end)
{
	std::cout << "\r\u79fb\u52a8  [ " << x1 << "," << y1 << " " << x2 << "," << y2 << " ]" << std::endl;
	MoveMouse(x1, y1);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	MoveMouse(x2, y2);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	Sleep(end);
}
//模拟鼠标滚轮
void W(int x, int y, int z, int end)
{
	std::cout << "\r\u6eda\u8f6e  [ " << x << "," << y << " " << z << " ]" << std::endl;
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
//热键运行
void KU(std::string str)
{
	std::cout << "\r\u70ed\u952e  [ " << str << " ]" << std::endl;
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
		{
			while (GetAsyncKeyState(hotkey) & 0x8000)
			{
				Sleep(1);
			}
			break;
		}
		Sleep(1);
	}
}
void KD(std::string str)
{
	std::cout << "\r\u70ed\u952e  [ " << str << " ]" << std::endl;
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
		{
			break;
		}
		Sleep(1);
	}
}
//热键停止
void Kill(std::string str)
{
	int hotkey = GetCodeNum(str, 0);
	while (true)
	{
		if (GetAsyncKeyState(hotkey) & 0x8000)
		{
			exit(0);
		}
		Sleep(1);
	}
}
//发送字符串
void ClipBoard(char *str);
void CV(std::string str, int end)
{
	str = str.substr(str.find('"') + 1, str.find_last_of('"') - 1);
	std::cout << "\r\u7c98\u8d34  [ " << str << " ]" << std::endl;
	char *p = (char *)str.c_str();
	ClipBoard(p);
	int e11 = GetCodeNum("Ctrl", 0);
	int e12 = GetCodeNum("Ctrl", 1);
	int e21 = GetCodeNum("V", 0);
	int e22 = GetCodeNum("V", 1);
	keybd_event(e11, e12, 0, 0);
	Sleep(50);
	keybd_event(e21, e22, 0, 0);
	Sleep(50);
	keybd_event(e21, e22, 2, 0);
	Sleep(50);
	keybd_event(e11, e12, 2, 0);
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
	std::cout << "\r\u6253\u5f00" << std::endl;
	ShowWindow(gameWhnd, 1);
	SetWindowPos(gameWhnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	Sleep(50);
	MoveMouse(960, 540);
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	Sleep(50);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	if (end < 0)
	{
		end = -end;
		Sleep(50);
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
		std::cout << "\r\u5173\u95ed" << std::endl;
		SetWindowPos(gameWhnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		ShowWindow(gameWhnd, 0);
		SetForegroundWindow(headWhnd);
		SetActiveWindow(headWhnd);
		SetFocus(headWhnd);
	}
}
//转码读取UTF-8文件
char *changeTxtEncoding(char *szU8)
{
	int wcsLen = MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
	wchar_t *wszString = new wchar_t[wcsLen + 1.0];
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
	wszString[wcsLen] = '\0';
	int ansiLen = WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	char *szAnsi = new char[ansiLen + 1.0];
	WideCharToMultiByte(CP_ACP, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = '\0';
	return szAnsi;
}
//读取文件并储存于Code
void ReadFiletoCode(std::string file)
{
	std::string line, out;
	std::ifstream in(file);
	if (in)
	{
		// for (int i = 0; i < sizeof(Code) / sizeof(Code[0]); i++)
		// {
		// 	for (int j = 0; j < sizeof(Code[0]) / sizeof(Code[0][0]); j++)
		// 	{
		// 		Code[i][j] = "\0";
		// 	}
		// }
		while (getline(in, line))
		{
			int y = 0;
			std::istringstream str(line);
			while (str >> out)
			{
				char *strc = new char[strlen(out.c_str()) + 1];
				strcpy(strc, out.c_str());
				Code[Length][y] = changeTxtEncoding(strc);
				y++;
			}
			Length++;
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
		std::cout << "\r\u5012\u8ba1\u65f6: ";
		CoutColor(100);
		std::cout << std::setw(2) << std::setfill('0') << delay / 60 << "\u5206" << std::setw(2) << std::setfill('0') << delay % 60 << "\u79d2 ";
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
//设置窗口标题
void SetTitle(std::string path)
{
	std::string str1 = path.substr(path.find_last_of('\\') + 1);
	std::string str2 = str1.substr(0, str1.find_last_of('.'));
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
	Sleep(50);
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
			std::string str = std::to_string(p.x) + "," + std::to_string(p.y);
			char *position = (char *)str.c_str();
			std::cout << position << std::endl;
			ClipBoard(position);
		}
	}
}
//快捷键获取坐标
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
//CImage转Gdiplus::Bitmap
Gdiplus::Bitmap *CImage2Image(CImage *pImage)
{
	Gdiplus::Bitmap *image = new Gdiplus::Bitmap(pImage->GetWidth(), pImage->GetHeight());
	Gdiplus::Rect bound(0, 0, pImage->GetWidth(), pImage->GetHeight());
	Gdiplus::BitmapData lockedBitmapData;
	int bpp = pImage->GetBPP();
	int imageRowSize = pImage->GetWidth() * (bpp / 8);
	if (bpp == 24)
	{
		image->LockBits(&bound, Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &lockedBitmapData);
	}
	else if (bpp == 32)
	{
		image->LockBits(&bound, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &lockedBitmapData);
	}
	BYTE *pSrcPointer = (BYTE *)pImage->GetBits();
	BYTE *pDstPointer = (BYTE *)lockedBitmapData.Scan0;
	for (int i = 0; i < pImage->GetHeight(); i++)
	{
		memcpy(pDstPointer, pSrcPointer, imageRowSize);
		pSrcPointer += pImage->GetPitch();
		pDstPointer += lockedBitmapData.Stride;
	}
	image->UnlockBits(&lockedBitmapData);
	return image;
}
//全屏截取
Gdiplus::Bitmap *ScreenShot()
{
	HDC hDCScreen = GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hDCScreen, BITSPIXEL);
	int nWidthScreen = GetDeviceCaps(hDCScreen, HORZRES);
	int nHeightScreen = GetDeviceCaps(hDCScreen, VERTRES);
	m_MyImage.Create(nWidthScreen, nHeightScreen, nBitPerPixel);
	HDC hDCImg = m_MyImage.GetDC();
	BitBlt(hDCImg, 0, 0, nWidthScreen, nHeightScreen, hDCScreen, 0, 0, SRCCOPY);
	// m_MyImage.Save("C:/BDO.PNG", Gdiplus::ImageFormatPNG);
	Gdiplus::Bitmap *image = CImage2Image(&m_MyImage);
	m_MyImage.ReleaseDC();
	return image;
}
//快捷键获取坐标RGB颜色，输出显示，复制到剪切板
void GetXyColor()
{
	int x = 0, y = 0;
	GetPosition(x, y);
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::Bitmap *image = ScreenShot();
	Gdiplus::Color color1, color2;
	image->GetPixel(x, y, &color1);
	image->GetPixel(x + 3, y + 3, &color2);
	std::string colorStr;
	colorStr = "color(" + std::to_string(color1.GetRed()) + "," + std::to_string(color1.GetGreen()) + "," + std::to_string(color1.GetBlue()) + ")(";
	colorStr = colorStr + std::to_string(color2.GetRed()) + "," + std::to_string(color2.GetGreen()) + "," + std::to_string(color2.GetBlue()) + ")";
	Gdiplus::GdiplusShutdown(gdiplusToken);
	char *str = (char *)colorStr.c_str();
	std::cout << str << std::endl;
	ClipBoard(str);
}
//判断某点颜色是否匹配
bool checkColor(Gdiplus::Bitmap *image, int array[], int i, int j)
{
	Gdiplus::Color color;
	image->GetPixel(i, j, &color);
	if (array[0] == color.GetRed() &&
		array[1] == color.GetGreen() &&
		array[2] == color.GetBlue())
	{
		return true;
	}
	else
	{
		return false;
	}
}
//根据颜色找到匹配的第一个坐标
void getColorXY(int array[][3], int &x, int &y)
{
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	Gdiplus::Bitmap *image = ScreenShot();
	int width = image->GetWidth();
	int height = image->GetHeight();
	Gdiplus::Color color1, color2;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (checkColor(image, array[0], i, j) && checkColor(image, array[1], i + 3, j + 3))
			{
				x = i;
				y = j;
				Gdiplus::GdiplusShutdown(gdiplusToken);
				return;
			}
		}
	}
	Gdiplus::GdiplusShutdown(gdiplusToken);
}
//得到坐标
void GetXY(std::string &str, int &x, int &y)
{
	//x,y
	if (str == "x,y")
	{
		GetPosition(x, y);
		str = std::to_string(x) + ',' + std::to_string(y);
		Sleep(1000);
	}
	//color(255,255,255)(0,0,0)
	else if (str.substr(0, 5) == "color")
	{
		int colorArray[2][3];
		std::string strColor = str;
		for (int i = 0; i < 2; i++)
		{
			strColor = strColor.substr(strColor.find("(") + 1); //r,g,b)(r,g,b)
			colorArray[i][0] = Getint(strColor.substr(0, strColor.find(",")));
			strColor = strColor.substr(strColor.find(",") + 1); //g,b)(r,g,b)
			colorArray[i][1] = Getint(strColor.substr(0, strColor.find(",")));
			strColor = strColor.substr(strColor.find(",") + 1); //b)(r,g,b)
			colorArray[i][2] = Getint(strColor.substr(0, strColor.find(")")));
		}
		getColorXY(colorArray, x, y);
	}
	//0,0
	else
	{
		size_t division = str.find_last_of(',');
		x = Getint(str.substr(0, division));
		y = Getint(str.substr(1 + division));
	}
}
//FOR循环
int RunKey(std::string[], int);
int RunFor(int line)
{
	int j, times = Getint(Code[line][1]);
	while (times--)
	{
		j = line + 1;
		while (Code[j][0] != "END")
		{
			j = RunKey(Code[j], j);
		}
	}
	return j;
}
//IF判断
int RunIf(int line)
{
	int j = line + 1;
	int x = 0, y = 0;
	GetXY(Code[line][1], x, y);
	if (x != 0 && y != 0)
	{
		while (Code[j][0] != "FI")
		{
			j = RunKey(Code[j], j);
		}
	}
	else
	{
		int k = 1; //记录FI数,含嵌套情况
		while (k != 0)
		{
			if (Code[j][0] == "IF")
			{
				k++;
			}
			else if (Code[j][0] == "FI")
			{
				k--;
			}
			j++;
		}
		j--; //定位到FI那一行
	}
	return j;
}
//单行脚本处理
int RunKey(std::string str[], int line)
{
	if (gameWhnd == GetForegroundWindow())
	{
		if (str[0] == "K")
		{
			K(str[1], Getint(str[2]));
		}
		if (str[0] == "K2")
		{
			K2(str[1], str[2], Getint(str[3]), Getint(str[4]));
		}
		if (str[0] == "L" || str[0] == "R")
		{
			if (str[1].find(',') != std::string::npos)
			{
				int x = 0, y = 0;
				GetXY(str[1], x, y);
				if (x != 0 && y != 0)
				{
					if (str[0] == "L")
					{
						L(x, y, Getint(str[2]));
					}
					if (str[0] == "R")
					{
						R(x, y, Getint(str[2]));
					}
				}
			}
			else
			{
				if (str[0] == "L")
				{
					L(Getint(str[1]), Getint(str[2]));
				}
				if (str[0] == "R")
				{
					R(Getint(str[1]), Getint(str[2]));
				}
			}
		}
		if (str[0] == "W")
		{
			int x = 0, y = 0;
			GetXY(str[1], x, y);
			if (x != 0 && y != 0)
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
			if (x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0)
			{
				M(x1, y1, x2, y2, Getint(str[3]));
			}
		}
		if (str[0] == "S")
		{
			S(Getint(str[1]));
		}
		if (str[0] == "CV")
		{
			CV(str[1], Getint(str[2]));
		}
		if (str[0] == "WHILE")
		{
			int x = 0, y = 0;
			do
			{
				GetXY(str[1], x, y);
				Sleep(200);
			} while (x == 0 && y == 0);
		}
		if (str[0] == "CLOSE")
		{
			CLOSE();
		}
	}
	if (str[0] == "FOR")
	{
		line = RunFor(line);
	}
	if (str[0] == "IF")
	{
		line = RunIf(line);
	}
	if (str[0] == "KU")
	{
		KU(str[1]);
	}
	if (str[0] == "KD")
	{
		KD(str[1]);
	}
	if (str[0] == "OPEN")
	{
		OPEN(Getint(str[1]));
	}
	return line + 1;
}
//脚本整体处理
void RunCode()
{
	int startLine = 0, num = 1;
	while (Code[startLine][0] != "BDO")
	{
		startLine++;
	}
	if (Code[startLine][3] != "\0")
	{
		std::thread t(Kill, Code[startLine][3]);
		t.detach();
	}
	int total = Getint(Code[startLine][1]), time = Getint(Code[startLine][2]);
	total = total == 0 ? 1 : total;
	while (total--)
	{
		gameWhnd = FindWindow("BlackDesertWindowClass", NULL);
		system("cls");
		CoutColor(111);
		std::cout << "\u8fd0\u884c\u6b21\u6570: ";
		CoutColor(100);
		if (Getint(Code[startLine][1]) == 0)
		{
			std::cout << "∞";
			total++;
		}
		else
		{
			std::cout << num++ << "/" << Getint(Code[startLine][1]);
		}
		std::cout << "  " << Code[startLine][3] << std::endl;
		std::cout << std::endl;
		CoutColor(0);
		int i = startLine + 1;
		while (i < Length)
		{
			i = RunKey(Code[i], i);
		}
		CoutColor(111);
		std::cout << std::endl;
		if (total != 0)
		{
			ShowTime(time);
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
		ShowWindow(exeWhnd, 1);
		CoutColor(111);
		std::cout << "\u9009\u62e9\u6267\u884c\u9879\u76ee\uff1a" << std::endl;
		CoutColor(100);
		std::cout << "1\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807" << std::endl;
		std::cout << "2\u002e\u83b7\u53d6\u9f20\u6807\u5750\u6807\u5904\u989c\u8272" << std::endl;
		CoutColor(0);
		std::cout << "\u8f93\u5165\uff1a";
		CoutColor(111);
		char ch;
		ch = getchar();
		ShowWindow(exeWhnd, SW_SHOWMINIMIZED);
		system("cls");
		std::cout << "\u5feb\u6377\u952e\u0020\u005b\u0020\u0041\u004c\u0054\u0020\u002b\u0020\u0060\u0020\u005d" << std::endl
				  << std::endl;
		CoutColor(0);
		switch (ch)
		{
		case 49:
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
		RunCode();
		break;
	}
	if (argc != 2)
		system("pause");
	return 0;
}

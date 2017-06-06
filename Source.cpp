bool load = 0;
#include<windows.h>
#include<fstream>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include"Header.h"
#include"Header1.h"
#define MENU_SLOTS 12
HINSTANCE Descriptor_program;
class button
{
public:
	RECT rect;
	HBRUSH Brush;
	char inf[10];
	button(int x1, int y1, int x2, int y2, HBRUSH Brush_new, char*inf_new)
	{
		rect = { x1,y1,x2,y2 };
		Brush = Brush_new;
		strcpy_s(inf, strlen(inf_new) + 1, inf_new);
	}
	void draw(HDC Context)
	{
		FillRect(Context, &rect, Brush);
		SetBkMode(Context, TRANSPARENT);
		DrawText(Context, inf, 10, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}
	bool CheckClick(int x, int y)
	{
		if (x > rect.left && x<rect.right && y>rect.top && y < rect.bottom)
			return 1;
		return 0;
	}
}m[MENU_SLOTS]{
	{ 292,198,736,846,CreateSolidBrush(RGB(238,164,189)),"" },//0
	{ 306,220,708,820,CreateSolidBrush(RGB(1,162,217)),"" },//1
	{ 348,159,675,222,CreateSolidBrush(RGB(3,164,50)),"" },//2
	{ 359,165,663,216,CreateSolidBrush(RGB(255,255,255)),"MENU" },//3
	{ 383,330,638,415,CreateSolidBrush(RGB(158,214,239)),"" },//4
	{ 389,337,634,398,CreateSolidBrush(RGB(255,255,255)),"PLAY" },//5
	{ 383,443,638,525,CreateSolidBrush(RGB(158,214,239)),"" },//6
	{ 389,449,634,512,CreateSolidBrush(RGB(255,255,255)),"SETTINGS" },//7
	{ 383,553,638,636,CreateSolidBrush(RGB(158,214,239)),"" },//8
	{ 389,556,634,620,CreateSolidBrush(RGB(255,255,255)),"LOAD" },//9
	{ 383,664,638,748,CreateSolidBrush(RGB(158,214,239)),"" },//10
	{ 389,671,634,732,CreateSolidBrush(RGB(255,255,255)),"EXIT" },//11
};
LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam);
void draw(HWND Descriptor);
void ButtonFunction(int i, HWND Descriptor);
int WINAPI	WinMain(HINSTANCE Descriptor_program, HINSTANCE, LPSTR, int)
{
	WNDCLASS wnd_class = { NULL , Menu_Function , NULL , NULL ,Descriptor_program,NULL,NULL,CreateSolidBrush(RGB(110, 63, 131)),NULL,"Window_WNDCLASS" };
	RegisterClass(&wnd_class);
	HWND Descriptor = CreateWindow("Window_WNDCLASS", "Menu", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, 1000, 1000, NULL, NULL, Descriptor_program, NULL);
	ShowWindow(Descriptor, SW_SHOW);
	UpdateWindow(Descriptor);
	MSG massage_code;
	while (GetMessage(&massage_code, NULL, 0, 0))
	{
		TranslateMessage(&massage_code);
		DispatchMessage(&massage_code);
	}
	return 0;
}
void ButtonFunction(int i, HWND Descriptor)
{
	switch (i)
	{
	case 5:
		DestroyWindow(Descriptor);
		Main_game::WinMain2(Descriptor_program);
		break;
	case 7:
		Settings::WinMain3(Descriptor_program);
		break;
	case 9:
		load = 1;
		DestroyWindow(Descriptor);
		Main_game::WinMain2(Descriptor_program);
		break;
	case 11:PostQuitMessage(0); return;
	default: return;
	}
	InvalidateRect(NULL, NULL, TRUE);
}
LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam)
{
	switch (massage)
	{
	case WM_PAINT:
		draw(Descriptor); return 0;
	case WM_LBUTTONUP:
		for (int i = 0; i < MENU_SLOTS; i++)
			if (m[i].CheckClick(LOWORD(lparam), HIWORD(lparam)))
				ButtonFunction(i, Descriptor);
		return 0;
	case WM_DESTROY:
		return 0;
	default:break;
	}
	return DefWindowProc(Descriptor, massage, wparam, lparam);
}
void draw(HWND Descriptor)
{
	PAINTSTRUCT Paintstruct;
	HDC Context = BeginPaint(Descriptor, &Paintstruct);
	for (int i = 0; i < MENU_SLOTS; i++)m[i].draw(Context);
	EndPaint(Descriptor, &Paintstruct);
	ReleaseDC(Descriptor, Context);
}

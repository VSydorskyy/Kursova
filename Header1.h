#pragma once
namespace Settings
{
#define SIZE 50
#define MAX_SIZE 10
#define MIN_SIZE 5
	class Setting_Box
	{
		RECT rect;
		HBRUSH Brush;
		int pole_size;
		char inf[12];
	public:
		Setting_Box(int x1, int y1, int x2, int y2, HBRUSH nBrush)
		{
			rect.top = y1;
			rect.left = x1;
			rect.bottom = y2;
			rect.right = x2;
			for (int i = 0; i < 10; i++)
				inf[i] = NULL;
			pole_size = 5;
			Brush = nBrush;
		}
		RECT getrect() { return rect; }
		int getsize() { return pole_size; }
		void draw(HDC Context)
		{
			FillRect(Context, &rect, Brush);
			SetBkMode(Context, TRANSPARENT);
			DrawText(Context, inf, strlen(inf) + 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		void setsize(int size)
		{
			pole_size = size;
			sprintf_s(inf, "%d", size);
		}
		void setinf(char* text) { strcpy_s(inf, text); }
		bool CheckClick(int x, int y)
		{
			if (x < rect.right && x>rect.left && y>rect.top && y < rect.bottom)
				return 1;
			return 0;
		}
	}high_board(0, 0, 3 * SIZE, SIZE, CreateSolidBrush(RGB(204, 0, 0))),
		left_board(0, SIZE, SIZE, SIZE * 2, CreateSolidBrush(RGB(0, 0, 204))),
		medium_board(SIZE, SIZE, 2 * SIZE, 2 * SIZE, CreateSolidBrush(RGB(150, 150, 150))),
		right_board(SIZE * 2, SIZE, 3 * SIZE, 2 * SIZE, CreateSolidBrush(RGB(0, 0, 204))),
		low_board(0, SIZE * 2, SIZE * 3, SIZE * 3, CreateSolidBrush(RGB(204, 0, 0)));
	LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam);
	void draw1(HWND Descriptor);
	void create();
	void ButtonFunction(int x, int y, HWND Descriptor);
	int WinMain3(HINSTANCE Descriptor_program)
	{
		WNDCLASS wnd_class = { NULL , Menu_Function , NULL , NULL ,Descriptor_program,NULL,NULL,CreateSolidBrush(RGB(255, 255, 255)),NULL,"Window_WNDCLASS3" };//create class
		RegisterClass(&wnd_class);//register window
		HWND Descriptor = CreateWindow("Window_WNDCLASS3", "Menu", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, SIZE * 3 + 16, SIZE * 3 + 38, NULL, NULL, Descriptor_program, NULL);
		ShowWindow(Descriptor, SW_SHOW);
		create();
		UpdateWindow(Descriptor);
		MSG massage_code;
		while (GetMessage(&massage_code, NULL, 0, 0))
		{
			TranslateMessage(&massage_code);
			DispatchMessage(&massage_code);
		}
		return 0;
	}
	LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam)
	{
		switch (massage)
		{
		case WM_PAINT:
			draw1(Descriptor); return 0;
		case WM_LBUTTONUP:
			ButtonFunction(LOWORD(lparam), HIWORD(lparam), Descriptor);
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:break;
		}
		return DefWindowProc(Descriptor, massage, wparam, lparam);
	}
	void create()
	{
		high_board.setinf("FIELD SIZE");
		left_board.setinf("<");
		right_board.setinf(">");
		low_board.setinf("BACK");
		medium_board.setsize(5);
	}
	void ButtonFunction(int x, int y, HWND Descriptor)
	{
		if (left_board.CheckClick(x, y) && medium_board.getsize() > MIN_SIZE)
			medium_board.setsize(medium_board.getsize() - 1);
		if (right_board.CheckClick(x, y) && medium_board.getsize() < MAX_SIZE)
			medium_board.setsize(medium_board.getsize() + 1);
		if (low_board.CheckClick(x, y))
			DestroyWindow(Descriptor);
		InvalidateRect(Descriptor, &medium_board.getrect(), FALSE);
		Main_game::MASS_SIZE = medium_board.getsize();
	}
	void draw1(HWND Descriptor)
	{
		PAINTSTRUCT Paintstruct;
		HDC Context = BeginPaint(Descriptor, &Paintstruct);
		high_board.draw(Context);
		left_board.draw(Context);
		right_board.draw(Context);
		low_board.draw(Context);
		medium_board.draw(Context);
		EndPaint(Descriptor, &Paintstruct);
		ReleaseDC(Descriptor, Context);
	}
}
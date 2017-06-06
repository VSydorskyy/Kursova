#pragma once
using namespace std;
namespace Main_game
{
#define SHIFT 5
#define CELL_SIZE 50
#define SPEED 20
#define BOARD_SIZE 50
	class cell;
	int MASS_SIZE;
	int fallTimer = -1;
	int switchTimer = -1;
	int scorecount;
	HBITMAP *red, *green, *blue;
	HWND Descriptor;
	HINSTANCE Descriptor_program;
	int tempi = -1, tempj = -1;
	class FillBlack
	{
	protected:
		RECT rect;
		FillBlack *next;
	public:
		static FillBlack *firstElement;
		void draw(HDC context)
		{
			FillRect(context, &rect, CreateSolidBrush(RGB(0, 0, 0)));
			delete this;
		}
		FillBlack(RECT _rect)
		{
			rect = _rect;
			InvalidateRect(Descriptor, &rect, FALSE);
			next = firstElement;
		}
		~FillBlack() { if (this == firstElement)firstElement = next; }
	};
	FillBlack* FillBlack::firstElement = NULL;
	class Scoreboard
	{
		RECT rect;
		int score;
		char cscore[100];
		HBRUSH Brush;
	public:
		void draw(HDC Context)
		{
			FillRect(Context, &rect, Brush);
			SetBkMode(Context, TRANSPARENT);
			DrawText(Context, cscore, strlen(cscore) + 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
		Scoreboard()
		{
			rect.top = 0;
			rect.left = 0;
			rect.right = 0;
			rect.bottom = 0;
			score = 0;
			sprintf_s(cscore, "%d", score);
			Brush = NULL;
		}
		void Setboard(int x1, int y1, int x2, int y2, HBRUSH Brushn)
		{
			rect.top = y1;
			rect.left = x1;
			rect.right = x2;
			rect.bottom = y2;
			Brush = Brushn;
		}
		void Set_score(int nscore)
		{
			score = nscore;
			sprintf_s(cscore, "%d", score);
		}
		RECT Get_rect() { return rect; }
		int getScore() { return score; }
	}board;
	class cell
	{
		RECT rect;
		int shift_x, shift_y;
		HBITMAP *bitmap;
	public:
		cell()
		{
			rect = { 0,0,0,0 };
			bitmap = 0;
			shift_x = 0;
			shift_y = 0;
		}
		void SetRect(int x1n, int y1n, int x2n, int y2n)
		{
			rect = { x1n,y1n,x2n,y2n };
		}
		void SetRect(RECT rectn) { rect = rectn; }
		void SetBitmap(HBITMAP *bitmap_new) { bitmap = bitmap_new; }
		int Get_x1() { return rect.left; }
		int Get_y1() { return rect.top; }
		int Get_x2() { return rect.right; }
		int Get_y2() { return rect.bottom; }
		int Get_shiftx() { return shift_x; }
		int Get_shifty() { return shift_y; }
		void Set_shifty(int nschift) { shift_y = nschift; }
		void Set_shiftx(int nschift) { shift_x = nschift; }
		RECT Get_rect() { return rect; }
		HBITMAP* Get_bitmap() { return bitmap; }
		void draw(HDC Context, HWND hWnd)
		{
			HDC tempContext = CreateCompatibleDC(Context);
			SelectObject(tempContext, *bitmap);
			BitBlt(Context, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, tempContext, 0, 0, SRCCOPY);
			ReleaseDC(hWnd, tempContext);
			DeleteDC(tempContext);
		}
		bool CheckClick(int x, int y)
		{
			if (x < rect.right && x>rect.left && y>rect.top && y < rect.bottom)
				return 1;
			return 0;
		}
	}**cell_mass;
	class Save
	{
		RECT rect;
		HBRUSH Brush;
		char inf[20];
	public:
		Save()
		{
			rect.left = 0;
			rect.top = 0;
			rect.right = 0;
			rect.bottom = 0;
			Brush = NULL;
			for(int i=0;i<5;i++)
				inf[i] = NULL;
		}
		void setSave(int x1, int y1, int x2, int y2, HBRUSH nBrush, char* ninf)
		{
			rect.left = x1;
			rect.top = y1;
			rect.right = x2;
			rect.bottom = y2;
			Brush = nBrush;
			strcpy_s(inf, strlen(ninf) + 1, ninf);
		}
		bool CheckClick(int x, int y)
		{
			if (x < rect.right && x>rect.left && y>rect.top && y < rect.bottom)
				return 1;
			return 0;
		}
		void draw(HDC Context)
		{
			FillRect(Context, &rect, Brush);
			SetBkMode(Context, TRANSPARENT);
			DrawText(Context, inf, strlen(inf) + 1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}
	}save;
	void UpdateScore(HWND Descriptor);
	void CheckMass(HWND Descriptor);
	bool CheckCell(int i, int j);
	void Click(int x, int y, HWND Descriptor);
	void timerFunction(HWND Descriptor);
	void CreateMass();
	void loadMass();
	LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam);
	void draw(HWND Descriptor);
	int WinMain2(HINSTANCE descriptor_program)
	{
		if (!MASS_SIZE)
			MASS_SIZE = 5;
		Descriptor_program = descriptor_program;
		srand(time(NULL));
		red = new HBITMAP;
		blue = new HBITMAP;
		green = new HBITMAP;
		*red = static_cast<HBITMAP>(LoadImage(Descriptor_program, "Red.bmp", IMAGE_BITMAP, CELL_SIZE, CELL_SIZE, LR_LOADFROMFILE));
		*green = static_cast<HBITMAP>(LoadImage(Descriptor_program, "Green.bmp", IMAGE_BITMAP, CELL_SIZE, CELL_SIZE, LR_LOADFROMFILE));
		*blue = static_cast<HBITMAP>(LoadImage(Descriptor_program, "Blue.bmp", IMAGE_BITMAP, CELL_SIZE, CELL_SIZE, LR_LOADFROMFILE));
		WNDCLASS wnd_class = { NULL , Menu_Function , NULL , NULL ,Descriptor_program,NULL,NULL,CreateSolidBrush(RGB(250, 250, 250)),NULL,"Window_WNDCLASS1" };
		RegisterClass(&wnd_class);
		Descriptor = CreateWindow("Window_WNDCLASS1", "Menu", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 0, 0, MASS_SIZE*CELL_SIZE + 16 + BOARD_SIZE, MASS_SIZE * CELL_SIZE + 38, NULL, NULL, Descriptor_program, NULL);
		board.Setboard(MASS_SIZE*CELL_SIZE, 0, MASS_SIZE*CELL_SIZE + BOARD_SIZE, BOARD_SIZE, CreateSolidBrush(RGB(100, 100, 100)));
		if (!load)
			CreateMass();
		else
			loadMass();
		save.setSave(MASS_SIZE*CELL_SIZE, MASS_SIZE*CELL_SIZE - BOARD_SIZE, MASS_SIZE*CELL_SIZE + BOARD_SIZE, MASS_SIZE*CELL_SIZE, CreateSolidBrush(RGB(100, 100, 100)), "SAVE");
		ShowWindow(Descriptor, SW_SHOW);
		UpdateWindow(Descriptor);
		SetTimer(Descriptor, 0, SPEED, 0);
		MSG massage_code;
		while (GetMessage(&massage_code, NULL, 0, 0))
		{
			TranslateMessage(&massage_code);
			DispatchMessage(&massage_code);
		}
		PostQuitMessage(0);
		return 0;
	}
	LRESULT CALLBACK Menu_Function(HWND Descriptor, UINT massage, WPARAM wparam, LPARAM lparam)
	{
		switch (massage)
		{
		case WM_TIMER:
			CheckMass(Descriptor);
			timerFunction(Descriptor);
		case WM_PAINT:
			draw(Descriptor); return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_LBUTTONDOWN:
			Click(LOWORD(lparam), HIWORD(lparam),Descriptor);
		default:break;
		}
		return DefWindowProc(Descriptor, massage, wparam, lparam);
	}
	void CreateBitmap(int i, int j)
	{
		switch (rand() % 3 + 1)
		{
		case 1:
			cell_mass[i][j].SetBitmap(red); break;
		case 2:
			cell_mass[i][j].SetBitmap(green); break;
		default:
			cell_mass[i][j].SetBitmap(blue); break;
		}
	}
	void CreateMass()
	{
		cell_mass = new cell*[MASS_SIZE];
		for (int i = 0; i < MASS_SIZE; i++)
			cell_mass[i] = new cell[MASS_SIZE];
		for (int i = 0; i < MASS_SIZE; i++)
			for (int j = 0; j < MASS_SIZE; j++)
			{
				cell_mass[i][j].SetRect(i*CELL_SIZE, j*CELL_SIZE, (i + 1)*CELL_SIZE, (j + 1)*CELL_SIZE);
				CreateBitmap(i, j);
			}
	}
	void draw(HWND Descriptor)
	{
		PAINTSTRUCT Paintstruct;
		HDC Context = BeginPaint(Descriptor, &Paintstruct);
		while (FillBlack::firstElement != NULL)
			FillBlack::firstElement->draw(Context);
		for (int i = 0; i < MASS_SIZE; i++)
		{
			for (int j = 0; j < MASS_SIZE; j++)
			{
				if (cell_mass[i][j].Get_bitmap() != NULL)
					cell_mass[i][j].draw(Context, Descriptor);
			}
		}
		if(tempi!=-1 && tempj!=-1)
			FrameRect(Context, &cell_mass[tempi][tempj].Get_rect(), CreateSolidBrush(RGB(255, 255, 255)));
		board.draw(Context);
		save.draw(Context);
		EndPaint(Descriptor, &Paintstruct);
		ReleaseDC(Descriptor, Context);
		DeleteDC(Context);
	}
	void timerFunction(HWND Descriptor)
	{
		if (fallTimer >= 0)
		{
			if (fallTimer == 0)
			{
				bool temp = 0;
				for (int j = MASS_SIZE - 1; j > 0; j--)
					for (int i = 0; i < MASS_SIZE; i++)
						if (cell_mass[i][j].Get_bitmap() == NULL && cell_mass[i][j - 1].Get_bitmap() != NULL)
						{
							cell_mass[i][j].SetBitmap(cell_mass[i][j - 1].Get_bitmap());
							cell_mass[i][j - 1].SetBitmap(NULL);
							cell_mass[i][j].SetRect(cell_mass[i][j - 1].Get_x1(), cell_mass[i][j - 1].Get_y1(), cell_mass[i][j - 1].Get_x2(), cell_mass[i][j - 1].Get_y2());
							temp = 1;
							cell_mass[i][j].Set_shifty(CELL_SIZE);
						}
				for (int j = 0; j < MASS_SIZE; j++)
					if (cell_mass[j][0].Get_bitmap() == NULL)
					{
						cell_mass[j][0].SetRect(cell_mass[j][0].Get_x1(), cell_mass[j][0].Get_y1() - CELL_SIZE, cell_mass[j][0].Get_x2(), cell_mass[j][0].Get_y2() - CELL_SIZE);
						CreateBitmap(j, 0);
						cell_mass[j][0].Set_shifty(CELL_SIZE);
						temp = 1;
					}
				if (temp)
					fallTimer = CELL_SIZE / SHIFT;
				else
				{
					fallTimer = -1;
					return;
				}
			}
			for (int j = MASS_SIZE - 1; j >= 0; j--)
				for (int i = 0; i < MASS_SIZE; i++)
					if (cell_mass[j][i].Get_shifty() > 0)
					{
						RECT temp = { cell_mass[j][i].Get_x1(), cell_mass[j][i].Get_y1(), cell_mass[j][i].Get_x2(), cell_mass[j][i].Get_y1() + SHIFT };
						FillBlack::firstElement = new FillBlack{ temp };
						cell_mass[j][i].SetRect(cell_mass[j][i].Get_x1(), cell_mass[j][i].Get_y1() + SHIFT, cell_mass[j][i].Get_x2(), cell_mass[j][i].Get_y2() + SHIFT);
						cell_mass[j][i].Set_shifty(cell_mass[j][i].Get_shifty() - SHIFT);
						InvalidateRect(Descriptor, &cell_mass[j][i].Get_rect(), FALSE);
					}
			fallTimer--;
		}
		if (switchTimer > 0)
		{
			for (int j = MASS_SIZE - 1; j >= 0; j--)
				for (int i = 0; i < MASS_SIZE; i++)
				{
					if (cell_mass[j][i].Get_shifty() > 0)
					{
						FillBlack::firstElement = new FillBlack{ cell_mass[j][i].Get_rect() };
						cell_mass[j][i].SetRect(cell_mass[j][i].Get_x1(), cell_mass[j][i].Get_y1() + SHIFT, cell_mass[j][i].Get_x2(), cell_mass[j][i].Get_y2() + SHIFT);
						cell_mass[j][i].Set_shifty(cell_mass[j][i].Get_shifty() - SHIFT);
						InvalidateRect(Descriptor, &cell_mass[j][i].Get_rect(), FALSE);
					}
					if (cell_mass[j][i].Get_shifty() < 0)
					{
						FillBlack::firstElement = new FillBlack{ cell_mass[j][i].Get_rect() };
						cell_mass[j][i].SetRect(cell_mass[j][i].Get_x1(), cell_mass[j][i].Get_y1() - SHIFT, cell_mass[j][i].Get_x2(), cell_mass[j][i].Get_y2() - SHIFT);
						cell_mass[j][i].Set_shifty(cell_mass[j][i].Get_shifty() + SHIFT);
						InvalidateRect(Descriptor, &cell_mass[j][i].Get_rect(), FALSE);
					}
					if (cell_mass[j][i].Get_shiftx() > 0)
					{
						FillBlack::firstElement = new FillBlack{ cell_mass[j][i].Get_rect() };
						cell_mass[j][i].SetRect(cell_mass[j][i].Get_x1() + SHIFT, cell_mass[j][i].Get_y1(), cell_mass[j][i].Get_x2() + SHIFT, cell_mass[j][i].Get_y2());
						cell_mass[j][i].Set_shiftx(cell_mass[j][i].Get_shiftx() - SHIFT);
						InvalidateRect(Descriptor, &cell_mass[j][i].Get_rect(), FALSE);
					}
					if (cell_mass[j][i].Get_shiftx() < 0)
					{
						FillBlack::firstElement = new FillBlack{ cell_mass[j][i].Get_rect() };
						cell_mass[j][i].SetRect(cell_mass[j][i].Get_x1() - SHIFT, cell_mass[j][i].Get_y1(), cell_mass[j][i].Get_x2() - SHIFT, cell_mass[j][i].Get_y2());
						cell_mass[j][i].Set_shiftx(cell_mass[j][i].Get_shiftx() + SHIFT);
						InvalidateRect(Descriptor, &cell_mass[j][i].Get_rect(), FALSE);
					}
				}
			switchTimer--;
		}
		if (switchTimer == 0)
		{
			switchTimer = -1;
		}
	}
	void Click(int x, int y, HWND Descriptor)
	{
		if (save.CheckClick(x, y))
		{
			int bit;
			ofstream fout("load.txt");
			for(int i=0;i<MASS_SIZE;i++)
				for (int j = 0; j < MASS_SIZE; j++)
				{
					if (cell_mass[i][j].Get_bitmap() == red)
						bit = 1;
					if (cell_mass[i][j].Get_bitmap() == green)
						bit = 2;
					if (cell_mass[i][j].Get_bitmap() == blue)
						bit = 3;
					fout << bit<<endl;
				}
			fout << board.getScore() << endl;
			fout.close();
		}
		if (fallTimer < 0)
			for (int i = 0; i < MASS_SIZE; i++)
				for (int j = 0; j<MASS_SIZE; j++)
					if (cell_mass[i][j].CheckClick(x, y))
					{
						if (tempi == -1)
						{
							tempi = i;
							tempj = j;
							InvalidateRect(Descriptor, &cell_mass[tempi][tempj].Get_rect(), FALSE);
							return;
						}
						if ((abs(tempi - i) == 1 && tempj == j) || (abs(tempj - j) == 1 && tempi == i))//Поменять указатель на номер массива
						{
							HBITMAP* temp_bitmap = cell_mass[tempi][tempj].Get_bitmap();
							cell_mass[tempi][tempj].SetBitmap(cell_mass[i][j].Get_bitmap());
							cell_mass[i][j].SetBitmap(temp_bitmap);
							if (CheckCell(i, j) || CheckCell(tempi, tempj))
							{
								RECT temp_rect = cell_mass[tempi][tempj].Get_rect();
								cell_mass[tempi][tempj].SetRect(cell_mass[i][j].Get_rect());
								cell_mass[i][j].SetRect(temp_rect);
								if (tempi - i == -1)
								{
									cell_mass[i][j].Set_shiftx(CELL_SIZE);
									cell_mass[tempi][tempj].Set_shiftx(-CELL_SIZE);
								}
								if (tempi - i == 1)
								{
									cell_mass[i][j].Set_shiftx(-CELL_SIZE);
									cell_mass[tempi][tempj].Set_shiftx(CELL_SIZE);
								}
								if (tempj - j == -1)
								{
									cell_mass[i][j].Set_shifty(CELL_SIZE);
									cell_mass[tempi][tempj].Set_shifty(-CELL_SIZE);
								}
								if (tempj - j == 1)
								{
									cell_mass[i][j].Set_shifty(-CELL_SIZE);
									cell_mass[tempi][tempj].Set_shifty(CELL_SIZE);
								}
								switchTimer = CELL_SIZE / SHIFT;
							}
							else
							{
								HBITMAP* temp_bitmap = cell_mass[tempi][tempj].Get_bitmap();
								cell_mass[tempi][tempj].SetBitmap(cell_mass[i][j].Get_bitmap());
								cell_mass[i][j].SetBitmap(temp_bitmap);
							}
						}
						if(tempi<MASS_SIZE && tempi >= 0 && tempj<MASS_SIZE && tempj >= 0)
							InvalidateRect(Descriptor, &cell_mass[tempi][tempj].Get_rect(), FALSE);
						tempi = -1;
						tempj = -1;
						return;
					}
	}
	bool CheckCell(int i, int j)
	{
		int xUp = 0, xDown = 0, yUp = 0, yDown = 0;
		if (i < MASS_SIZE - 1 && (cell_mass[i + 1][j].Get_bitmap() == cell_mass[i][j].Get_bitmap()) )
		{
			xUp = 1;
			if (i < MASS_SIZE - 2 && cell_mass[i + 2][j].Get_bitmap() == cell_mass[i][j].Get_bitmap() )
				xUp = 2;
		}
		if (i > 0 && (cell_mass[i - 1][j].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
		{
			xDown = 1;
			if (i > 1 && (cell_mass[i - 2][j].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
				xDown = 2;
		}
		if (j < MASS_SIZE - 1 && (cell_mass[i][j + 1].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
		{
			yUp = 1;
			if (j < MASS_SIZE - 2 && (cell_mass[i][j + 2].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
				yUp = 2;
		}
		if (j > 0 && (cell_mass[i][j - 1].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
		{
			yDown = 1;
			if (j > 1 &&(cell_mass[i][j - 2].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
				yDown = 2;
		}
		if (xUp + xDown > 1 || yUp + yDown > 1)
			return TRUE;
		else
			return FALSE;
	}
	void CheckMass(HWND Descriptor)
	{
		if (fallTimer < 0 && switchTimer < 0)
		{
			for (int j = 0; j < MASS_SIZE; j++)
				for (int i = 0; i < MASS_SIZE - 2; i++)
					if (cell_mass[i][j].Get_bitmap() != NULL)
						if ((cell_mass[i + 1][j].Get_bitmap() == cell_mass[i][j].Get_bitmap()) && (cell_mass[i + 2][j].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
						{
							for (int k = i + 1; (k < MASS_SIZE) && (cell_mass[i][j].Get_bitmap() == cell_mass[k][j].Get_bitmap()); k++)
							{
								scorecount++;
								cell_mass[k][j].SetBitmap(NULL);
								FillBlack::firstElement = new FillBlack{ cell_mass[k][j].Get_rect() };
							}
							scorecount++;
							cell_mass[i][j].SetBitmap(NULL);
							FillBlack::firstElement = new FillBlack{ cell_mass[i][j].Get_rect() };
							fallTimer = 0;
						}
			for (int j = 0; j < MASS_SIZE - 2; j++)
				for (int i = 0; i < MASS_SIZE; i++)
					if (cell_mass[i][j].Get_bitmap() != NULL)
						if ((cell_mass[i][j + 1].Get_bitmap() == cell_mass[i][j].Get_bitmap()) && (cell_mass[i][j + 2].Get_bitmap() == cell_mass[i][j].Get_bitmap()))
						{
							for (int k = j + 1; k < MASS_SIZE && (cell_mass[i][j].Get_bitmap() == cell_mass[i][k].Get_bitmap()); k++)
							{
								scorecount++;
								cell_mass[i][k].SetBitmap(NULL);
								FillBlack::firstElement = new FillBlack{ cell_mass[i][k].Get_rect() };
							}
							scorecount++;
							cell_mass[i][j].SetBitmap(NULL);
							FillBlack::firstElement = new FillBlack{ cell_mass[i][j].Get_rect() };
							fallTimer = 0;
						}
		}
		UpdateScore(Descriptor);
	}
	void UpdateScore(HWND Descriptor)
	{
		board.Set_score(scorecount);
		InvalidateRect(Descriptor, &board.Get_rect(), FALSE);
	}
	void loadMass()
	{
		int bit=0;
		ifstream fin("load.txt");
		cell_mass = new cell*[MASS_SIZE];
		for (int i = 0; i < MASS_SIZE; i++)
			cell_mass[i] = new cell[MASS_SIZE];
		for (int i = 0; i < MASS_SIZE; i++)
			for (int j = 0; j < MASS_SIZE; j++)
			{
				cell_mass[i][j].SetRect(i*CELL_SIZE, j*CELL_SIZE, (i + 1)*CELL_SIZE, (j + 1)*CELL_SIZE);
				fin >> bit;
				if (bit==1)
					cell_mass[i][j].SetBitmap(red);
				if (bit==2)
					cell_mass[i][j].SetBitmap(green);
				if (bit==3)
					cell_mass[i][j].SetBitmap(blue);
			}
		fin >> bit;
		scorecount = bit;
		fin.close();
	}
}
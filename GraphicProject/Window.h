#pragma once
#include<windows.h>
#include<string>
using namespace std;
class Window {
public:
	int width;
	int height;
	string name;
	DWORD style;
	HWND hwnd;
	HINSTANCE hinstance;
	bool window_fullscreen = false;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];

	void create(int window_width, int window_height, string window_name);
	void processMessages();
	void updateMouse(int x,int y) {
		mousex = x;
		mousey = y;
	}
};



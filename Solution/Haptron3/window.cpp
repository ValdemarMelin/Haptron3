
#include "Haptron.h"
#include <GL\glew.h>
#include <Windows.h>

using namespace std;

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);

static  PIXELFORMATDESCRIPTOR pfd =                  // pfd Tells Windows How We Want Things To Be
{
	sizeof(PIXELFORMATDESCRIPTOR),                  // Size Of This Pixel Format Descriptor
	1,                              // Version Number
	PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
	PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
	PFD_DOUBLEBUFFER,                       // Must Support Double Buffering
	PFD_TYPE_RGBA,                          // Request An RGBA Format
	32,                               // Select Our Color Depth
	0, 0, 0, 0, 0, 0,                       // Color Bits Ignored
	0,                              // No Alpha Buffer
	0,                              // Shift Bit Ignored
	0,                              // No Accumulation Buffer
	0, 0, 0, 0,                         // Accumulation Bits Ignored
	16,                             // 16Bit Z-Buffer (Depth Buffer)
	0,                              // No Stencil Buffer
	0,                              // No Auxiliary Buffer
	PFD_MAIN_PLANE,                         // Main Drawing Layer
	0,                              // Reserved
	0, 0, 0                             // Layer Masks Ignored
};

HDC dc;
HGLRC rc;

INT WINAPI wWinMain(HINSTANCE inst, HINSTANCE inst2, LPWSTR cmdLine, INT cmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH) 8;
	wc.lpszClassName = "HAPTRON WINDOW CLASS";
	wc.lpfnWndProc = WndProc;
	wc.hIcon = LoadIcon(inst, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(inst, IDI_APPLICATION);
	wc.hInstance = inst;
	wc.style = CS_OWNDC;
	RegisterClassEx(&wc);

	HWND wnd = CreateWindow("HAPTRON WINDOW CLASS", "HELLO", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, inst, NULL);

	ShowWindow(wnd, cmdShow);
	UpdateWindow(wnd);

	dc = GetDC(wnd);
	rc = NULL;
	int pf = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pf, &pfd);
	rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);

	int err = glewInit();
	if (err != GLEW_OK)
	{
		string s = (const char*)glewGetErrorString(err);
		s.insert(0, "glewInit() failed. Error message: ");
		s.append(". Program will now terminate.");
		MessageBoxA(wnd, s.c_str(), "ERROR", MB_OK);
		return 1;
	}


	Haptron::game.start();
	Haptron::Graphics::render_unit.init();

	MSG msg;
	while (Haptron::game.get_state() == Haptron::GameState::PLAY) {

		if (PeekMessage(&msg, wnd, 0, 0, true)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			Haptron::game.tick();
			Haptron::Graphics::render_unit.render();
			SwapBuffers(dc);
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND wnd, UINT msg, WPARAM wp, LPARAM lp) {

	switch (msg) {
	case WM_PAINT:
	{
		LRESULT v = DefWindowProc(wnd, msg, wp, lp);
		Haptron::Graphics::render_unit.render();
		SwapBuffers(dc);
		return v;
	}
		break;
	case WM_SIZE:
		Haptron::Graphics::render_unit.render_surface.width = LOWORD(lp);
		Haptron::Graphics::render_unit.render_surface.height = HIWORD(lp);
		break;
	case WM_CLOSE:
		Haptron::game.quit();
		return DefWindowProc(wnd, msg, wp, lp);
	case WM_DESTROY:
		Haptron::Graphics::render_unit.destroy();
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		Haptron::game.key_down(wp);
		break;
	case WM_KEYUP:
		Haptron::game.key_up(wp);
		break;
	default:
		return DefWindowProc(wnd, msg, wp, lp);
	}
	return 0;
}
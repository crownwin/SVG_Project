// gdiplustest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "gdiplustest.h"
#include "CImg.h"
#include <fstream>
#include <string>
#include "Circle.h"
#include "Ellipse.h"
#include "Figure.h"
#include "Line.h"
#include "ListFigure.h"
#include "HamXuLy.h"
#include "Polygon.h"
#include "Polyline.h"
#include "Rectangle.h"
#include "Transform.h"
#include "Text.h"
#include "rapidxml.hpp"
using namespace cimg_library;
using namespace std;
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;									// current instance
TCHAR szTitle[MAX_LOADSTRING];						// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];				// the main window class name

												// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
ListFigure list;
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	CImg<>canvas(1000, 1000, 1, 3, 255);
	string input_xml;
	string line;
	ifstream in("sample.svg");

	if (!in)
		return 1;				// không mở được file sample.svg

								// đọc toàn bộ nội dung file vào string input_xml
	while (getline(in, line))
		input_xml += line;

	in.close();

	// tạo một mảng (vector) char từ input_xml
	vector<char> xml_copy(input_xml.begin(), input_xml.end());
	xml_copy.push_back('\0');	// thêm kí tự kết thúc chuỗi

								// dùng doc để dọc trên xml_copy
	xml_document<> doc;
	doc.parse<parse_declaration_node | parse_no_data_nodes>(&xml_copy[0]);

	// lấy thẻ svg đầu tiên, gán vào node
	xml_node<> *node = doc.first_node("svg");

	// node->first_node() lấy thẻ (tag) con đầu tiên của svg

	node = node->first_node();
	do
	{
		// cứ mỗi một tag (hay node), duyệt qua các thuộc tính
		// node->first_attribute() lấy thuộc tính đầu tiên
		// attr->next_attribute() lấy thuộc tính tiếp theo thuộc tính hiện tại
		if (strcmp(node->name(), "line") == 0) {
			Figure* a = new Line(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "ellipse") == 0) {
			Figure* a = new CEllipse(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "rect") == 0) {
			Figure* a = new CRectangle(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "polygon") == 0) {
			Figure* a = new CPolygon(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "polyline") == 0) {
			Figure* a = new CPolyline(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "circle") == 0) {
			Figure* a = new CCircle(node);
			list.addFigure(a);
		}
		if (strcmp(node->name(), "text") == 0) {
			Figure* a = new CText(node);
			list.addFigure(a);
		}
		node = node->next_sibling();
	} while (node);		// cho đến khi không lấy được tag tiếp theo (giống như linked list)

	list.drawCImg(canvas);
	canvas.display("Draw with CImg");

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GDIPLUSTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPLUSTEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);
	return (int)msg.wParam;
	return 0;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLUSTEST));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_GDIPLUSTEST);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		list.drawGDI(hdc);
		//MyPaint(hdc);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

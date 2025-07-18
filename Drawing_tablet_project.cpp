#include <iostream>
#include <windows.h>
#include <stdio.h>

#include<string>
#include<cmath>
#include<cstdlib>

#include <windows.graphics.h>
#include <cstring>
#include<fstream>

using namespace std;



void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

}

void myLine(int x1, int y1, int x2, int y2, int R, int G, int B) //use three 3 integers if you want colored lines.
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255) to get shades of gray. For other colors use 3 integers for colors.
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(R, G, B)); //2 is the width of the pen
	SelectObject(device_context, pen);
	MoveToEx(device_context, x1, y1, NULL);
	LineTo(device_context, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);

}


// This function checks if any of the 4 cursor keys are pressed. 
// If any cursor key is pressed, then the function returns true, and whichKey identifies which of the 4 cursor keys is pressed.
// whichkey is assigned following values if any cursor key is pressed. 1 for left, 2 for up, 3 for right and 4 for left.
bool isCursorKeyPressed(int& whichKey)   //whichKey passed as reference.... 
{
	char key;
	key = GetAsyncKeyState(37);
	if (key == 1)
	{
		whichKey = 1;		// 1 if left key is pressed 
		return true;
	}
	key = GetAsyncKeyState(38);
	if (key == 1)
	{

		whichKey = 2;		// 2 if up key is pressed
		return true;
	}

	key = GetAsyncKeyState(39);
	if (key == 1)
	{

		whichKey = 3; // 3 if right key is pressed
		return true;
	}
	key = GetAsyncKeyState(40);
	if (key == 1)
	{

		whichKey = 4;   // 4 if down key is pressed
		return true;
	}
	return false;
}

void myRectangle(int x1, int y1, int x2, int y2, int R, int G, int B)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	//change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
	SelectObject(device_context, pen);
	HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0)); //Fill color can also be passed as parameter to the function!!!

	SelectObject(device_context, brush);

	Rectangle(device_context, x1, y1, x2, y2);
	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
	DeleteObject(brush);
}
void myEllipse(int x1, int y1, int x2, int y2, int R, int G, int B)
{
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);

	// Change the color by changing the values in RGB (from 0-255)
	HPEN pen = CreatePen(PS_SOLID, 3, RGB(R, G, B));
	SelectObject(device_context, pen);

	// Do not create a brush or fill the ellipse
	SelectObject(device_context, GetStockObject(NULL_BRUSH)); // Select a null brush

	Ellipse(device_context, x1, y1, x2, y2);

	ReleaseDC(console_handle, device_context);
	DeleteObject(pen);
}



void calculateEndPointFD(int x1, int y1, int angle, int length, int& x2, int& y2) {
	if (angle % 30 != 0 && angle % 45 != 0) {

		x2 = x1;
		y2 = y1 - length; //Move forward by the given length
		return;
	}

	float angle_rad = angle * (3.14159 / 180);
	x2 = x1 + static_cast<int>(round(length * cos(angle_rad)));
	y2 = y1 + static_cast<int>(round(length * sin(angle_rad)));
}

void calculateEndPointBK(int x1, int y1, int angle, int length, int& x2, int& y2) {
	if (angle % 30 != 0 && angle % 45 != 0) {

		x2 = x1;
		y2 = y1 + length; // Move backward by the given length
		return;
	}

	float angle_rad = angle * (3.14159 / 180); // Convert angle to radians
	x2 = x1 - static_cast<int>(round(length * cos(angle_rad)));
	y2 = y1 - static_cast<int>(round(length * sin(angle_rad)));
}

bool fdValidation(char entry[], int& value, int x1, int y1, int& x2, int& y2, int angle, int pen, int r, int g, int b) {
	char commands[4][20] = { "fd", "FD", "forward", "FORWARD" };
	int temp, sizeA, index = 0;
	bool found = false, valid = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 4; i++) {
		if (commands[i][sizeA] == 0) {
			valid = true;
			index = i;
			break;
		}
	}
	for (int i = index; commands[i][sizeA] == 0; i++) {
		found = true;
		for (int j = 0; j < sizeA; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true && valid == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res == true) {
		calculateEndPointFD(x1, y1, angle, value, x2, y2);
		if (pen == 1) {
			myLine(x1, y1, x2, y2, r, g, b);
			return res;
		}
	}
	else return res;
}
bool bkValidation(char entry[], int& value, int x1, int y1, int& x2, int& y2, int angle, int& pen, int r, int g, int b) {
	char commands[4][20] = { "bk", "BK", "backward", "BACKWARD" };
	int temp, sizeA, index = 0;
	bool found = false, valid = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 4; i++) {
		if (commands[i][sizeA] == 0) {
			valid = true;
			index = i;
			break;
		}
	}
	for (int i = index; commands[i][sizeA] == 0; i++) {
		found = true;
		for (int j = 0; j < sizeA; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true && valid == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res == true) {
		calculateEndPointBK(x1, y1, angle, value, x2, y2);
		if (pen == 1) {
			myLine(x1, y1, x2, y2, r, g, b);
			return res;
		}
	}
	else return res;
}

bool rtValidation(char entry[], int& angle) {
	char commands[2][20] = { "rt", "RT" };
	int temp, sizeA, index = 0, value = 0;
	bool found = false, valid = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 2; i++) {
		if (commands[i][sizeA] == 0) {
			valid = true;
			index = i;
			break;
		}
	}
	for (int i = index; commands[i][sizeA] == 0; i++) {
		found = true;
		for (int j = 0; j < sizeA; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true && valid == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res = true) {
		angle = angle + value;
	}
	else return res;
}
bool ltValidation(char entry[], int& angle) {
	char commands[2][20] = { "lt", "LT" };
	int temp, sizeA, index = 0, value = 0;
	bool found = false, valid = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 2; i++) {
		if (commands[i][sizeA] == 0) {
			valid = true;
			index = i;
			break;
		}
	}
	for (int i = index; commands[i][sizeA] == 0; i++) {
		found = true;
		for (int j = 0; j < sizeA; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true && valid == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res = true) {
		angle = angle - value;
	}
	else return res;
}
bool puValidation(char entry[], int& pen) {
	char commands[2][20] = { "pu", "PU" };
	int sizeA; // Represents size considering alphabets only.
	bool found = false, res = false;
	sizeA = strlen(entry);
	for (int i = 0; i < 2; i++) {
		if (strcmp(entry, commands[i]) == 0) {
			found = true;
			break;
		}
	}
	if (found) {
		res = true;
	}
	if (res == true) {
		pen = 0;
		return res;
	}
	else return res;
}
bool pdValidation(char entry[], int& pen) {
	char commands[2][20] = { "pd", "PD" };
	int sizeA; // Represents size considering alphabets only.
	bool found = false, res = false;
	sizeA = strlen(entry);
	for (int i = 0; i < 2; i++) {
		if (strcmp(entry, commands[i]) == 0) {
			found = true;
			break;
		}
	}
	if (found) {
		res = true;
	}
	if (res == true) {
		pen = 1;
		return res;
	}
	else return res;
}
bool colorValidation(char entry[], char color[], char colors[][30], int hexa[][3], int& index, int& R, int& G, int& B) {
	char commands[2][20] = { "color", "COLOR" };
	int temp = 0;
	bool found = false, res = false;
	for (int i = 0; i < 2; i++) {
		found = true;
		for (int j = 0; j < 5; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true) {
			int i = 0;
			temp = 5;
			while ((entry[temp] == 32 || (entry[temp] >= 65 && entry[temp] <= 90) || (entry[temp] >= 97 && entry[temp] <= 122)) && color[0] == '\0') {
				if ((entry[temp] >= 65 && entry[temp] <= 90) || (entry[temp] >= 97 && entry[temp] <= 122)) {
					color[i] = entry[temp];
					i++;
				}
				temp++;
			}
			while ((entry[temp] >= 65 && entry[temp] <= 90) || (entry[temp] >= 97 && entry[temp] <= 122)) {
				color[i] = entry[temp];
				i++;
				temp++;
			}
			break;
		}
		for (int i = 0; i < temp; i++) {
			if ((entry[i] < 32 || entry[i] > 32) && (entry[i] < 65 || entry[i] > 90)) {
				color[0] = '\0';
			}
		}
	}
	for (int i = 0; i < 20; ++i) {
		if (strcmp(color, colors[i]) == 0) {
			index = i;
			break;
		}
	}
	if (found && color[0] != '\0' && entry[temp] == '\0' && index != -1) {
		res = true;
	}
	if (res == true) {
		R = hexa[index][0];
		G = hexa[index][1];
		B = hexa[index][2];
		return res;
	}
	else return res;
}


void clearScreen() {
	// ANSI escape codes to clear the screen
	std::cout << "\x1B[2J\x1B[H";
}

bool csValidation(char entry[]) {
	char commands[2][20] = { "cs", "CS" };
	int sizeA; // Represents size considering alphabets only.
	bool found = false, res = false;
	sizeA = strlen(entry);
	for (int i = 0; i < 2; i++) {
		if (strcmp(entry, commands[i]) == 0) {
			found = true;
			break;
		}
	}
	if (found) {
		res = true;
	}
	if (res == true) {
		clearScreen();
		return res;
	}
	else return res;
}
void drawCircle(float x, float y, float rad, int r, int g, int b)
{
	float cornerX1, cornerY1, cornerX2, cornerY2;
	cornerX1 = x - rad;
	cornerY1 = y + rad;
	cornerX2 = x + rad;
	cornerY2 = y - rad;
	myEllipse(cornerX1, cornerY1, cornerX2, cornerY2, r, g, b);
}
bool circleValidation(char entry[], int& value, float x1, float y1, int r, int g, int b, int pen) {
	char commands[2][20] = { "circle", "CIRCLE" };
	int temp, sizeA;
	bool found = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 2; i++) {
		found = true;
		for (int j = 0; j < 6; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res == true && pen == 1) {
		drawCircle(x1, y1, value, r, g, b);
		return res;
	}
	else return res;
}
bool widthValidation(char entry[], int& width) {
	char commands[2][20] = { "width", "WIDTH" };
	int temp, sizeA, index = 0, value = 0;
	bool found = false, valid = false, res = false;
	for (sizeA = 0; (entry[sizeA] >= 65 && entry[sizeA] <= 90) || (entry[sizeA] >= 97 && entry[sizeA] <= 122); sizeA++);
	for (int i = 0; i < 2; i++) {
		if (commands[i][sizeA] == 0) {
			valid = true;
			index = i;
			break;
		}
	}
	for (int i = index; commands[i][sizeA] == 0; i++) {
		found = true;
		for (int j = 0; j < sizeA; j++) {
			if (entry[j] != commands[i][j]) {
				found = false;
				break;
			}
		}
		if (found == true && valid == true) {
			temp = sizeA;
			while ((entry[temp] == 32 || (entry[temp] >= 48 && entry[temp] <= 57)) && value == 0) {
				if (entry[temp] >= 48 && entry[temp] <= 57) {
					value = entry[temp] - '0';
				}
				temp++;
			}
			while (entry[temp] >= 48 && entry[temp] <= 57) {
				value = value * 10 + (entry[temp] - '0');
				temp++;
			}
			break;
		}
	}
	if (found && value != 0 && entry[temp] == 0) {
		res = true;
	}
	if (res = true) {
		width = value;
		return res;
	}
	else return res;
}
void drawCursor(int x1, int y1, int angle, int x2, int y2) {
	calculateEndPointFD(x1, y1, angle, 12, x2, y2);
	myLine(x1, y1, x2, y2, 192, 192, 192);
	drawCircle(x1, y1, 5, 192, 192, 192);
}

void setinput(int x, int y)
{
	COORD cursorPosition;
	cursorPosition.X = x;
	cursorPosition.Y = y; // Adjust this value to leave space after the rectangle

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

void setTextAtCoordinates(int x, int y)
{
	// Escape sequence for moving the cursor to the specified position
	cout << "\033[" << y << ";" << x << "H";
}



ofstream store("commandfile.txt");
int main()
{
	char exit[5] = "exit";
	myRectangle(175, 10, 1070, 530, 0, 255, 0);  //tablet display
	drawCursor(620, 270, 270, 620, 270);
	setTextAtCoordinates(3, 15);
	cout << "COMMAND HISTORY";

	const int maxCommands = 100;
	char commands[maxCommands][20];
	int r = 0;
	for (int i = 0; i < maxCommands && (commands[i] != exit); i++)
	{
		setTextAtCoordinates(3, 29);
		cout << "Enter command: ";
		cin.getline(commands[i], 20);       //Command Input

		store << commands[i] << endl;        //Storing commands in file

		system("cls");                        //erase cin statement
		myRectangle(175, 10, 1070, 530, 0, 255, 0);   //again tablet display
		setTextAtCoordinates(3, 15);
		cout << "COMMAND HISTORY";

		char colors[20][30] = {
		"black", "BLACK", "white", "WHITE", "red", "RED",
		"orange", "ORANGE", "yellow", "YELLOW", "green", "GREEN",
		"blue", "BLUE", "purple", "PURPLE", "pink", "PINK",
		"silver", "SILVER" };
		int hexa[20][3] = { {0, 0, 0}, {0, 0, 0}, {255, 255, 255}, {255, 255, 255}, {255, 0, 0}, {255, 0, 0},
			{255, 165, 0}, {255, 165, 0}, {255, 255, 0}, {255, 255, 0}, {0, 128, 0}, {0, 128, 0},
			{0, 0, 255}, {0, 0, 255}, {128, 0, 128}, {128, 0, 128}, {255, 182, 193}, {255, 182, 193},
			{192, 192, 192}, {192, 192, 192} };
		int value = 0, index = -1, angle = 270, width = 3, x1 = 620, y1 = 270, x2, y2, R = 255, G = 0, B = 0, pen = 1, stop = 0;
		char color[30] = { '\0' };

		for (int k = 0; k <= i && (commands[k] != exit); k++)
		{
			rtValidation(commands[k], angle);
			ltValidation(commands[k], angle);
			puValidation(commands[k], pen);
			pdValidation(commands[k], pen);
			fdValidation(commands[k], value, x1, y1, x2, y2, angle, pen, R, G, B);
			colorValidation(commands[k], color, colors, hexa, index, R, G, B);
			bkValidation(commands[k], value, x1, y1, x2, y2, angle, pen, R, G, B);
			widthValidation(commands[k], width);
			circleValidation(commands[k], value, x1, y1, R, G, B, pen);
			csValidation(commands[k]);
			if (puValidation(commands[k], pen) == 1 ||
				pdValidation(commands[k], pen) == 1 ||
				csValidation(commands[k]) == 1 || widthValidation(commands[k], width) == 1) {
				value = 1;
			}
			if (value == 0)
				cout << "Unknown command!" << endl;

			x1 = x2;
			y1 = y2;
			value = 0;
			for (int i = 0; i < 30; i++)
			{
				color[i] = 0;
			}
			if (commands[k] == commands[i])
				drawCursor(x1, y1, angle, x2, y2);
		}




		//Code of printing last five commands
		int start;
		if (i < (maxCommands - 96))
		{
			r = 0;
			start = 0;
			for (int j = start; j <= i; j++)
			{
				setTextAtCoordinates(3, 16 + r);
				cout << commands[j];
				r++;
			}

		}
		else
		{
			start = i - (maxCommands - 96);
			r = 0;
			for (int j = start; j <= i; j++)
			{
				setTextAtCoordinates(3, 16 + r);
				cout << commands[j];
				r++;
			}
		}


	}

	char ch;
	cin >> ch;
}
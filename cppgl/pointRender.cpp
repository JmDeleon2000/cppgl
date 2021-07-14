#pragma warning(disable : 4996)

#include <iostream>
#include <cmath>


#define pixel_size  3

#define tau 6.28318


float clamp01(float x)
{
	return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x;
}

class vec3
{
public:
	unsigned char col[3];

	vec3()
	{
		this->col[0] = 0;
		this->col[1] = 0;
		this->col[2] = 0;
	}

	vec3(float r, float g, float b)
	{
		this->col[0] = (int)(clamp01(b) * 255);
		this->col[1] = (int)(clamp01(g) * 255);
		this->col[2] = (int)(clamp01(r) * 255);
	}

};

vec3* clear_col = new vec3();
vec3* draw_col = new vec3();

const vec3* WHITE = new vec3(1, 1, 1);
const vec3* BLACK = new vec3();

vec3** frameBuffer;

int width = 0;
int height = 0;
int vp_width = 0;
int vp_height = 0;
int vp_x = 0;
int vp_y = 0;
int vp_boundx = 0;
int vp_boundy = 0;
const int filesize = width * height * pixel_size;

void glCreateViewPort(int x, int y, int width, int height)
{
	vp_x = x;
	vp_y = y;
	vp_width = width;
	vp_height = height;
	vp_boundx = (vp_x + vp_width) > ::width ? ::width : (vp_x + vp_width);
	vp_boundy = (vp_y + vp_height) > ::height ? ::height : (vp_y + vp_height);
}

void glCreateWindow(int new_width = 1920, int new_height = 1080)
{
	for (int i = 0; i < width; i++)
		delete[] frameBuffer[i];
	delete[] frameBuffer;

	width = new_width;
	height = new_height;
	glCreateViewPort(0, 0, new_width, new_height);
	frameBuffer = new vec3 * [width];
	for (int i = 0; i < width; i++)
		frameBuffer[i] = new vec3[height];
}

bool outOfBounds(int x, int y, int xOff = 0, int yOff = 0)
{
	return x < xOff + vp_x || y <  yOff + vp_y || x > vp_boundx - 1 - xOff || y > vp_boundy - 1 - yOff;
}


void glFinish(const char* filename)
{
	unsigned char header[14] = { 'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	unsigned char headerinfo[40] = { 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0 };

	*(int*)(header + 2) = filesize;

	*(int*)(headerinfo + 4) = width;

	*(int*)(headerinfo + 8) = height;

	FILE* dump = fopen(filename, "wb");

	fwrite(header, 1, 14, dump);
	fwrite(headerinfo, 1, 40, dump);

	int i, j = 0;
	while (j < height)
	{
		i = 0;
		while (i < width)
		{
			fwrite(&frameBuffer[i][j], 1, 3, dump);
			i++;
		}
		j++;
	}

	fclose(dump);
	for (int i = 0; i < width; i++)
		delete[] frameBuffer[i];
	delete[] frameBuffer;


	exit(EXIT_SUCCESS);
}

void clear()
{
	int i = 0, j;

	while (i < width)
	{
		j = 0;
		while (j < height)
		{
			if (outOfBounds(i, j)) { j++; continue; }
			frameBuffer[i][j] = *clear_col;
			j++;
		}
		i++;
	}
}

void gldraw_vertex(int x, int y)
{
	if (outOfBounds(x, y)) return;
	frameBuffer[x][y] = *draw_col;
}

void set_draw_col(vec3* new_col)
{
	draw_col = new_col;
}

void set_draw_col(const vec3* new_col)
{
	delete draw_col;
	draw_col = (vec3*)new_col;
}

void set_clear_col(vec3* new_col)
{
	delete clear_col;
	clear_col = new_col;
}

void set_clear_col(const vec3* new_col)
{
	clear_col = (vec3*)new_col;
}

void draw_cross(int x, int y)
{
	if (outOfBounds(x, y)) return;
	frameBuffer[x][y] = *draw_col;
	if (x < 1 || y < 1 || x > width - 2 || y > height - 2) return;
	frameBuffer[x - 1][y] = *draw_col;
	frameBuffer[x + 1][y] = *draw_col;
	frameBuffer[x][y + 1] = *draw_col;
	frameBuffer[x][y - 1] = *draw_col;
}


void glInit() 
{
	glCreateWindow();
	clear();
}

int main()
{
	glCreateWindow();
	set_clear_col(new vec3(1, 0.5, 0.25));
	clear();

	glCreateViewPort(300, 200, 1320, 680);

	set_clear_col(new vec3());
	clear();

	set_draw_col(WHITE);
	int i = vp_x;
	double h;
	while (i < vp_boundx)
	{
		h = 0.51;
		while (h > 0.49)
		{
			draw_cross(i, (double)vp_height / 100.0 * (cos((i - vp_x) / (double)vp_width * tau * 20 + tau / 3) * 50.0 * abs(cos((i - vp_x) / (double)vp_width * tau - tau / 4))) + vp_y + (double)vp_height * h - (double)vp_height / 100.0);
			h -= 0.0001;
		}

		i++;
	}


	glFinish("articMonkeys.bmp");


	return 0;
}


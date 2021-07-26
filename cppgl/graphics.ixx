#pragma warning(disable : 4996)
#include <iostream>
import ModelImporter;
export module graphics;


using namespace modelImp;


#define pixel_size  3

export namespace gl {

	 float clamp01(float x)
	{
		return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x;
	}

	 class col3
	{
	public:
		unsigned char col[3];

		col3()
		{
			this->col[0] = 0;
			this->col[1] = 0;
			this->col[2] = 0;
		}

		col3(float r, float g, float b)
		{
			this->col[0] = (int)(clamp01(b) * 255);
			this->col[1] = (int)(clamp01(g) * 255);
			this->col[2] = (int)(clamp01(r) * 255);
		}

	};

	 struct vert2
	{
		float x = 0;
		float y = 0;
	};

	col3* clear_col = new col3();
	col3* draw_col = new col3();

	 const col3* WHITE = new col3(1, 1, 1);
	 const col3* BLACK = new col3();

	col3** frameBuffer;

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
		vp_boundx = (vp_x + vp_width) > gl::width ? gl::width : (vp_x + vp_width);
		vp_boundy = (vp_y + vp_height) > gl::height ? gl::height : (vp_y + vp_height);
	}

	 void glCreateWindow(int new_width = 1920, int new_height = 1080)
	{
		for (int i = 0; i < width; i++)
			delete[] frameBuffer[i];
		delete[] frameBuffer;

		width = new_width;
		height = new_height;
		glCreateViewPort(0, 0, new_width, new_height);
		frameBuffer = new col3 * [width];
		for (int i = 0; i < width; i++)
			frameBuffer[i] = new col3[height];
	}

	bool outOfBounds(int x, int y, int x0ff = 0, int y0ff = 0)
	{
		return x < x0ff + vp_x || y <  y0ff + vp_y || x > vp_boundx - 1 - x0ff || y > vp_boundy - 1 - y0ff;
	}


	 void glFinish(const char* filename, bool end_exec = true)
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


		if (end_exec) exit(EXIT_SUCCESS);
	}

	 void clear()
	{
		int i = vp_x, j;

		while (i < vp_boundx)
		{
			j = vp_y;
			while (j < vp_boundy)
			{
				if (outOfBounds(i, j)) { j++; continue; }
				frameBuffer[i][j] = *clear_col;
				j++;
			}
			i++;
		}
	}

	 void gldraw_vertex(int x, int y, col3* color = nullptr)
	{
		if (outOfBounds(x, y)) return;
		if (color)
		{
			frameBuffer[x][y] = *color;
			delete color;
			return;
		}
		frameBuffer[x][y] = *draw_col;
	}

	 void set_draw_col(col3* new_col)
	{
		delete draw_col;
		draw_col = new_col;
	}

	 void set_draw_col(const col3* new_col)
	{
		delete draw_col;
		draw_col = (col3*)new_col;
	}

	 void set_clear_col(col3* new_col)
	{
		delete clear_col;
		clear_col = new_col;
	}

	 void set_clear_col(const col3* new_col)
	{
		delete clear_col;
		clear_col = (col3*)new_col;
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


	 void glLine(vert2* v1, vert2* v2)
	{
		float x0 = v1->x;
		float x1 = v2->x;
		float y0 = v1->y;
		float y1 = v2->y;
		float m; // slope also used as temp
		float dx = abs(v2->x - v1->x);
		float dy = abs(v2->y - v1->y);

		if (x0 == x1 && y0 == y1) 
		{
			gldraw_vertex(x0, x1);
			return;
		}

		bool steep = dy > dx;
		if (steep)
		{
			m = x0;
			x0 = y0;
			y0 = m;
			m = x1;
			x1 = y1;
			y1 = m;
		}


		if (x0 > x1)
		{
			m = x0;
			x0 = x1;
			x1 = m;
			m = y0;
			y0 = y1;
			y1 = m;
		}

		dx = abs(x1 - x0);
		dy = abs(y1 - y0);

		m = dy / dx;

		float offset = 0;
		float limit = 0.5;

		int i = (int)x0, y = y0;

		while (i < (int)x1 + 1)
		{
			if (steep)
				gldraw_vertex(y, i);
			else
				gldraw_vertex(i, y);

			offset += m;

			if (offset >= limit)
			{
				y = y0 < y1 ? y + 1 : y - 1;
				limit += 1;
			}

			i++;
		}
	}

	obj* glLoadModel(const char* filename, vert2 translate, vert2 scale, bool discard = false)
	{
		obj* model = new obj(filename);
		int i = 0, j;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];
				vect3 v2 = model->v[model->f[i].data[(j+3)% model->f[i].size] - 1];


				vert2* begin = new vert2(), * end = new vert2();
				begin->x = v1.x * scale.x + translate.x;
				begin->y = v1.y * scale.y + translate.y;

				end->x = v2.x * scale.x + translate.x;
				end->y = v2.y * scale.y + translate.y;
				glLine(begin, end);
				delete begin;
				delete end;
				j+=3;
			}

			i++;
		}
		if (discard)
		{
			delete model;
			return nullptr;
		}
		return model;
	}

	obj* glLoadModel(obj* model, vert2 translate, vert2 scale, bool discard = false)
	{
		int i = 0, j;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];
				vect3 v2 = model->v[model->f[i].data[(j + 3) % model->f[i].size] - 1];


				vert2* begin = new vert2(), * end = new vert2();
				begin->x = v1.x * scale.x + translate.x;
				begin->y = v1.y * scale.y + translate.y;

				end->x = v2.x * scale.x + translate.x;
				end->y = v2.y * scale.y + translate.y;
				glLine(begin, end);
				delete begin;
				delete end;
				j += 3;
			}

			i++;
		}
		if (discard)
		{
			delete model;
			return nullptr;
		}
		return model;
	}
}

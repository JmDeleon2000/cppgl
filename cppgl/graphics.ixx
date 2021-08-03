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
		if (color != nullptr)
		{
			frameBuffer[x][y] = *color;
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


	 void glLine(vert2* v1, vert2* v2, col3 *color = nullptr)
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
			gldraw_vertex(x0, y1, color);
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

		int i = x0, y = y0;

		while (i < x1 + 1)
		{
			if (steep)
				gldraw_vertex(y, i, color);
			else
				gldraw_vertex(i, y, color);

			offset += m;

			if (offset >= limit)
			{
				y = y0 < y1 ? y + 1 : y - 1;
				limit += 1;
			}

			i++;
		}
	}

	 bool light(vect3 n0, vect3 n1, vect3  dir) 
	 {
		 // cos(angulo) = a*b;
		 if ((n0.x * dir.x + n0.y * dir.y + n0.z * dir.z) < 0) {  return false; }
		 if ((n1.x * dir.x + n1.y * dir.y + n1.z * dir.z) < 0) {  return false; }
		 return true;
	 }

	 bool light(vect3 n0,  vect3  dir)
	 {
		 // cos(angulo) = a*b;
		 if ((n0.x * dir.x + n0.y * dir.y + n0.z * dir.z) < 0) { return false; }
		 return true;
	 }

	 bool light(vect3 * n, int size, vect3  dir)
	 {
		 // cos(angulo) = a*b;
		 int i = 0;
		 float x = 0, y = 0, z = 0;
		 while (i < size)
		 {
			 x += n[i].x;
			 y += n[i].y;
			 z += n[i].z;
			 i++;
		 }
		 x /= size;
		 y /= size;
		 z /= size;
		 if ((x * dir.x + y * dir.y + z * dir.z) < 0)  return false;
		 return true;
	 }

	 /*void fillTriangle(vert2* triangle)
	 {
		 col3* color = new col3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
		 vert2 middle;
		 float highest, leftmost, rightmost;
		 int lmi = 0, rmi = 0;

		 highest = triangle[0].y;
		 leftmost = triangle[0].x;
		 rightmost = triangle[0].x;

		 int i = 0;
		 while (i < 3)
		 {
			 if (highest < triangle[i].y) highest = triangle[i].y;
			 if (leftmost > triangle[i].x) { leftmost = triangle[i].x; lmi = i; }
			 if (rightmost < triangle[i].x) { rightmost = triangle[i].x; rmi = i; }
			 i++;
		 }
		 middle = triangle[((lmi == 0 && rmi == 2) ? 1 : ((lmi == 1 && rmi == 2) ? 0 : 2))];
		 
		 if ((triangle[rmi].x - triangle[lmi].x) == 0 || (middle.x - triangle[lmi].x) == 0 || (triangle[rmi].x - middle.x) == 0 
			 || (middle.x - triangle[lmi].x) == 0 || (triangle[rmi].x - middle.x) == 0) return;
		 float ms0 = 0, ms1 = 0, mi0 = 0, mi1 = 0;
		 if (highest == middle.y)
		 {
			 mi0 = mi1 = (triangle[rmi].y - triangle[lmi].y) / (triangle[rmi].x - triangle[lmi].x);
			 ms0 = (middle.y - triangle[lmi].y) / (middle.x - triangle[lmi].x);
			 ms1 = (triangle[rmi].y - middle.y) / (triangle[rmi].x - middle.x);
		 }
		 else
		 {
			 ms0 = ms1 = (triangle[rmi].y - triangle[lmi].y) / (triangle[rmi].x - triangle[lmi].x);
			 mi0 = (middle.y - triangle[lmi].y) / (middle.x - triangle[lmi].x);
			 mi1 = (triangle[rmi].y - middle.y) / (triangle[rmi].x - middle.x);

		 }

		 
		 vert2 v1, v2;
		 v1.y = triangle[lmi].y;
		 v2.y = triangle[lmi].y;


		 i = leftmost;
		 while (i < middle.x)
		 {
			 
			 v1.x = v2.x = i;
			 glLine(&v1, &v2, color);
			 v1.y += mi0;
			 v2.y += ms0;
			 i++;
		 }

		 float mid_start_v2 = v2.y;
		 float mid_start_v1 = v1.y;

		 while (i < rightmost + 1)
		 {
			 
			 v1.x = v2.x = i;
			 glLine(&v1, &v2, color);
			 v1.y += mi1;
			 v2.y += ms1;
			 i++;
		 }
		 delete color;
	 }*/
	 // naive triangle

#define test < 1.0f
	 void flatBotTriangle(vert2 v0, vert2 v1, vert2 v2, col3* color = nullptr)
	 {
		 float d_10 = (v1.x - v0.x) / (v1.y - v0.y);
		 float d_20 = (v2.x - v0.x) / (v2.y - v0.y);

		 if ((v1.y - v0.y) == 0) return;
		 if ((v2.y - v0.y) == 0) return;
		 if (abs((v1.y - v0.y)) test || abs((v2.y - v0.y))test) { gldraw_vertex(v0.x, v0.y, color); return; }

		 vert2 p0, p1;
		 p0.x = v1.x;
		 p1.x = v2.x;
		 int i = v1.y;
		 while (i < v0.y + 1)
		 {
			 p0.y = p1.y = i;
			 glLine(&p0, &p1, color);
			 i++;
			 p0.x += d_10;
			 p1.x += d_20;
		 }
	 }
	 void flatTopTriangle(vert2 v0, vert2 v1, vert2 v2, col3* color = nullptr)
	 {
		 float d_20 = (v2.x - v0.x) / (v2.y - v0.y);
		 float d_21 = (v2.x - v1.x) / (v2.y - v1.y);

		 if ((v2.y - v0.y) == 0) return;
		 if ((v2.y - v1.y) == 0) return;
		 if (abs((v2.y - v0.y)) test || abs((v2.y - v1.y))test) { gldraw_vertex(v0.x, v0.y, color); return; }

		 vert2 p0, p1;
		 p0.x = v2.x;
		 p1.x = v2.x;
		 int i = v2.y;
		 while (i < v1.y + 1)
		 {
			 p0.y = p1.y = i;
			 glLine(&p0, &p1, color);
			 i++;
			 p0.x += d_20;
			 p1.x += d_21;
		 }
	 }

	 void fillTriangle(vert2* triangle, col3* color = nullptr, bool randCol = false)
	 {
		 vert2 v0 = triangle[0], v1 = triangle[1], v2 = triangle[2];
		 if (randCol) color = new col3(rand() / (float)RAND_MAX, rand() / (float)RAND_MAX, rand() / (float)RAND_MAX);
		 vert2 temp;

		 if (v0.y < v1.y)
		 {
			 temp = v0;
			 v0 = v1;
			 v1 = temp;
		 }
		 if (v0.y < v2.y)
		 {
			 temp = v0;
			 v0 = v2;
			 v2 = temp;
		 }
		 if (v1.y < v2.y)
		 {
			 temp = v1;
			 v1 = v2;
			 v2 = temp;
		 }
		 
		 if (v1.y == v2.y) 
		 { 
			 flatBotTriangle(v0, v1, v2, color); 
			 if (randCol) delete color;
			 return; 
		 }
		 if (v0.y == v1.y) 
		 { 
			 flatTopTriangle(v0, v1, v2, color); 
			 if (randCol)delete color; 
			 return; 
		 }
		 
		 temp.y = v1.y;
		 temp.x = v0.x + ((v1.y - v0.y) / (v2.y - v0.y)) * (v2.x - v0.x);
		 //if ((v2.y - v0.y) == 0) return;
		 flatBotTriangle(v0, v1, temp, color);
		 flatTopTriangle(v1, temp, v2, color);
		 if (randCol) delete color;
	 }

	 obj* glLoadModel(const char* filename, vert2 translate, vert2 scale, vect3 lightDir, bool discard = false)
	{
		obj* model = new obj(filename);
		vert2 poly[10];
		vect3 normals[10];
		int i = 0, j;
		bool  success = true;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];
				
				poly[j / 3].x = v1.x * scale.x + translate.x;
				poly[j / 3].y = v1.y * scale.y + translate.y;
				vect3 n0 = model->n[model->f[i].data[(j + 2) % model->f[i].size] - 1];
				

				normals[j / 3] = n0;
				

				j += 3;
			}
			//std::cout << j / 3 << "    " << success << "    " << i << std::endl;

			if (light(normals, (j-3)/3, lightDir))
				fillTriangle(poly, nullptr, true);
			i++;
		}
		if (discard)
		{
			delete model;
			return nullptr;
		}
		return model;
	}

	obj* glLoadModel(obj* model, vert2 translate, vert2 scale, vect3 lightDir, bool discard = false)
	{
		vert2* poly = new vert2[10];
		vect3* normals = new vect3[10];
		int i = 0, j;
		bool  success = true;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];

				poly[j / 3].x = v1.x * scale.x + translate.x;
				poly[j / 3].y = v1.y * scale.y + translate.y;
				vect3 n0 = model->n[model->f[i].data[(j + 2) % model->f[i].size] - 1];


				normals[j / 3] = n0;


				j += 3;
			}
			//std::cout << j / 3 << "    " << success << "    " << i << std::endl;

			if (light(normals, (j - 3) / 3, lightDir))
				fillTriangle(poly, nullptr, true);
			i++;
		}
		delete[] poly;
		delete[] normals;
		if (discard)
		{
			delete model;
			return nullptr;
		}
		return model;
	}


	
}

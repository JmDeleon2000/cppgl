#pragma warning(disable : 4996)
#include <iostream>
import ModelImporter;
import hb_math;

export module graphics;


using namespace modelImp;
using namespace hb_math;


#define pixel_size  3
#define baryCentric true








export namespace gl {



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

		unsigned char& operator[](int index)
		{
			switch (index)
			{
			case 0:
				return col[2];
			case 1:
				return col[1];
			case 2:
				return col[0];
			default:
				return col[2];
			}
		}
	};

	

	col3* clear_col = new col3();
	col3* draw_col = new col3();

	 col3* WHITE = new col3(1, 1, 1);
	 col3* BLACK = new col3();
	 col3* maxZ;

	col3** frameBuffer;
	float** zBuffer;

	int width = 0;
	int height = 0;
	int vp_width = 0;
	int vp_height = 0;
	int vp_x = 0;
	int vp_y = 0;
	int vp_boundx = 0;
	int vp_boundy = 0;
	const int filesize = width * height * pixel_size;

	M4x4 Cam;
	M4x4 VM;
	vect3 worldUp;

	 void glCreateViewPort(int x, int y, int width, int height)
	{
		vp_x = x;
		vp_y = y;
		vp_width = width;
		vp_height = height;
		vp_boundx = (vp_x + vp_width) > gl::width ? gl::width : (vp_x + vp_width);
		vp_boundy = (vp_y + vp_height) > gl::height ? gl::height : (vp_y + vp_height);
	}

	 void LookAt(vect3 eye, vect3 campos) 
	 {

		 vect3 forward, right, up;
		 forward = !(campos - eye);
		 right = !(worldUp * forward);
		 up = !(forward * right);

		 Cam = 'I';
		 Cam[0][0] = right.x;
		 Cam[1][0] = right.y;
		 Cam[2][0] = right.z;

		 Cam[0][1] = up.x;
		 Cam[1][1] = up.y;
		 Cam[2][1] = up.z;

		 Cam[0][2] = forward.x;
		 Cam[1][2] = forward.y;
		 Cam[2][2] = forward.z;

		 Cam[0][3] = campos.x;
		 Cam[1][3] = campos.y;
		 Cam[2][3] = campos.z;
		 VM = !Cam;
	 }

	 void glCreateWindow(int new_width = 1920, int new_height = 1080)
	 {
		 worldUp.y = 1;
		 maxZ = new col3();
		 maxZ->col[0] = maxZ->col[0] = maxZ->col[0] = -FLT_MAX;
		 for (int i = 0; i < width; i++)
			 delete[] frameBuffer[i];
		 delete[] frameBuffer;

		 width = new_width;
		 height = new_height;
		 glCreateViewPort(0, 0, new_width, new_height);
		 frameBuffer = new col3 * [width];
		 zBuffer = new float * [width];
		 for (int i = 0; i < width; i++) 
		 {
			 frameBuffer[i] = new col3[height];
			 zBuffer[i] = new float[height];
		 }

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

	 void glPrintZBuffer(const char* filename)
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
			 {//TODO
				 fwrite(&zBuffer[i][j], 1, 3, dump);
				 i++;
			 }
			 j++;
		 }

		 fclose(dump);
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
				zBuffer[i][j] = -FLT_MAX;
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


	 void glLine(vect2* v1, vect2* v2, col3 *color = nullptr)
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


	 /*void fillTriangle(vect2* triangle)
	 {
		 col3* color = new col3(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, rand() / (double)RAND_MAX);
		 vect2 middle;
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

		 
		 vect2 v1, v2;
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
	 void flatBotTriangle(vect2 v0, vect2 v1, vect2 v2, col3* color = nullptr)
	 {
		 float d_10 = (v1.x - v0.x) / (v1.y - v0.y);
		 float d_20 = (v2.x - v0.x) / (v2.y - v0.y);

		 if ((v1.y - v0.y) == 0) return;
		 if ((v2.y - v0.y) == 0) return;
		 if (abs((v1.y - v0.y)) test || abs((v2.y - v0.y))test) { gldraw_vertex(v0.x, v0.y, color); return; }

		 vect2 p0, p1;
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
	 void flatTopTriangle(vect2 v0, vect2 v1, vect2 v2, col3* color = nullptr)
	 {
		 float d_20 = (v2.x - v0.x) / (v2.y - v0.y);
		 float d_21 = (v2.x - v1.x) / (v2.y - v1.y);

		 if ((v2.y - v0.y) == 0) return;
		 if ((v2.y - v1.y) == 0) return;
		 if (abs((v2.y - v0.y)) test || abs((v2.y - v1.y))test) { gldraw_vertex(v0.x, v0.y, color); return; }


		 vect2 p0, p1;
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

	 inline void barycentricCords(vect2 A, vect2 B, vect2 C, vect2 P, vect3* out)
	 {
		 float u, v, w, ABC;

		 ABC = ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y)); //-+

		 if (ABC == 0)
		 {
			 out->x = -1;
			 out->y = -1;
			 out->z = -1;
			 return;
		 }

		 u = ((B.y - C.y) * (P.x - C.x) + (C.x - B.x) * (P.y - C.y)) / ABC;

		 v = ((C.y - A.y) * (P.x - C.x) + (A.x - C.x) * (P.y - C.y)) / ABC;

		 w = 1 - u - v;
		 out->x = u;
		 out->y = v;
		 out->z = w;
	 }

	 void fillTriangle(vect3* triangle, col3* color = nullptr, bool randCol = false, float intensity = 0, texture * t = nullptr, vect3* texcords = nullptr)
	 {
#if baryCentric
		 vect3 A = triangle[0], B = triangle[1], C = triangle[2], temp;
		 vect2 Ap, Bp, Cp;

		 Ap.x = A.x;
		 Ap.y = A.y;
		 Bp.x = B.x;
		 Bp.y = B.y;
		 Cp.x = C.x;
		 Cp.y = C.y;

		 int minX,  maxX, minY, maxY;
		 float z;
		 float tcol[3];
		 tcol[0] = tcol[1] = tcol[2] = 255;
		 maxX = minX = A.x;
		 maxY = minY = A.y;
		 vect2 P;
		 vect3* uvw = new vect3();
		 col3* col = color;
		 if (color == nullptr)
			 col = new col3();
		 
		 if (minX > B.x)
			 minX = B.x;
		 if (minX > C.x)
			 minX = C.x;

		 if (maxX < B.x)
			 maxX = B.x;
		 if (maxX < C.x)
			 maxX = C.x;

		 if (minY > B.y)
			 minY = B.y;
		 if (minY > C.y)
			 minY = C.y;

		 if (maxY < A.y)
			 maxY = A.y;
		 if (maxY < B.y)
			 maxY = B.y;
		 if (maxY < C.y)
			 maxY = C.y;
		 // sacar min y max

		 if (minX < vp_x)
			 minX = vp_x;
		 if (minY < vp_y)
			 minY = vp_y;

		 if (maxX > vp_boundx)
			 maxX = vp_boundx;
		 if (maxY > vp_boundy)
			 maxY = vp_boundy;

		 int i = minX - 1, j;
		 while (i++ < maxX)
		 {
			 j = minY - 1;
			 while (j++ < maxY)
			 {
				 P.x = i;
				 P.y = j;
				 barycentricCords(Ap, Bp, Cp, P, uvw);
				 if (uvw->z >= 0 && uvw->y >= 0 && uvw->x >= 0)
				 {
					 z = A.z * uvw->x + B.z * uvw->y + C.z * uvw->z;
					 if (z > zBuffer[i % vp_boundx][j%vp_boundy] && z >= -1 && z <= 1)
					 {
						 if (t && texcords) // calculates the uvs							 
							 t->getColor( (texcords[0].y * uvw->x + texcords[1].y * uvw->y + texcords[2].y * uvw->z), 
								  (texcords[0].x * uvw->x + texcords[1].x * uvw->y + texcords[2].x * uvw->z)
								 , tcol);
						 
						 
						 col->col[0] = (int)(intensity * tcol[0]);
						 col->col[1] = (int)(intensity * tcol[1]);
						 col->col[2] = (int)(intensity * tcol[2]);
						 gldraw_vertex(P.x, P.y, col);
						 zBuffer[(int)P.x % vp_boundx][(int)P.y % vp_boundy] = z;
					 }
					 
				 }
			 }
		 }
		 if (color == nullptr)
			 delete col;
		 delete uvw;

#else

		 vect2 v0 = triangle[0], v1 = triangle[1], v2 = triangle[2];
		 if (randCol) color = new col3(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, rand() / (double)RAND_MAX);
		 vect2 temp;

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
#endif
	 }
#define pi 3.1415926535897932

	 M4x4 createObjectMatrix(vect3 scale, vect3 translate, vect3 rotation) 
	 {
		 M4x4 Mt, Ms, Mr, Rx, Ry, Rz;
		 Rz = Ry = Rx = Ms = Mt = 'I';
		 Mt[0][3] = translate.x;
		 Mt[1][3] = translate.y;
		 Mt[2][3] = translate.z;

		 Ms[0][0] = scale.x;
		 Ms[1][1] = scale.y;
		 Ms[2][2] = scale.z;

		 rotation = rotation * (pi / 180);
		 Rx[1][1] = Rx[2][2] = cos(rotation.x);
		 Rx[1][2] = sin(rotation.x);
		 Rx[2][1] = -sin(rotation.x);
		 Ry[0][0] = Ry[2][2] = cos(rotation.y);
		 Ry[2][0] = sin(rotation.y);
		 Ry[0][2] = -sin(rotation.y);
		 Rz[0][0] = Rz[1][1] = cos(rotation.z);
		 Rz[0][1] = sin(rotation.z);
		 Rz[1][0] = -sin(rotation.z);

		 Mr = Rx * Ry * Rz;
		 
		 return  Mt * Mr * Ms;
	 }

	 M4x4 getVpxPxVM(float n = 0.5, float f = 10000, float fov = 60) 
	 {
		 float ar = ((float)vp_width / (float)vp_height);
		 float t = tan(fov * (pi / 360)) * n;
		 float r = t * ar;
		 
		 M4x4 p, Vp;
		 p = Vp = 'I';

		 Vp[0][0] = vp_width / 2;
		 Vp[1][1] = vp_height / 2;
		 Vp[2][2] = 0.5;
		 Vp[0][3] = vp_x + vp_width / 2;
		 Vp[1][3] = vp_y + vp_height / 2;
		 Vp[2][3] = 0.5;

		 p[0][0] = n / r;
		 p[1][1] = n / t;
		 p[2][2] = -(f + n) / (f - n);
		 p[3][3] = 0;
		 p[3][2] = -1;
		 p[2][3] = -(2 * f * n) / (f - n);

		 return Vp * p * VM;
	 }

	 void viewMatrix(vect3 translate, vect3 rotation)
	 {
		 vect3 scale;
		 scale.x = scale.y = scale.z = 1;
		 Cam = createObjectMatrix(scale, translate, rotation);
		 VM = !Cam;
	 }

	 vect3 transform(vect3 V, M4x4 Matrix) 
	 {
		 vect4 result;
		 
		 result = V;
		 result = Matrix * result;
		 result = result * (1 / result.w);

		 vect3 vf;
		 vf.x = result.x;
		 vf.y = result.y;
		 vf.z = result.z;

		 return vf;
	 }



	 //old glLoad that loaded the model
	/* obj* glLoadModel(const char* filename, vect3 translate, vect3 scale, vect3 rotation, vect3 lightDir, texture* text = nullptr, bool discard = false)
	{
		obj* model = new obj(filename);
		vect3 poly[10];
		vect3 normal,  texcords[10];


		int i = 0, j;
		float intensity;
		bool  success = true;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];
				vect3 uv = model->uvs[model->f[i].data[(j+1) % model->f[i].size] - 1];

				poly[j / 3] = objectToClip(v1, scale, translate, rotation);

				texcords[j / 3] = uv;

				

				j += 3;
			}

			

			normal = !((poly[1] - poly[0]) * (poly[2] - poly[0]));

			intensity = normal ^ lightDir;
			intensity = clamp01(intensity);

			fillTriangle(poly, nullptr, true, intensity, text, texcords);
			
			if (model->f[i].size >= 12)
			{
				poly[1] = poly[3];
				texcords[1] = texcords[3];
				fillTriangle(poly, nullptr, true, intensity, text, texcords);
			}
			
			i++;
		}
		
		
		if (discard)
		{
			delete model;
			return nullptr;
		}
		return model;
	}*/

	obj* glRenderModel(obj* model, vect3 translate, vect3 scale, vect3 rotation, vect3 lightDir, texture* text = nullptr)
	{
		vect3 poly[10];
		vect3 normal, texcords[10];
		M4x4 M = createObjectMatrix(scale, translate, rotation); 
		M4x4 VpxPxVM = getVpxPxVM();


		int i = 0, j;
		float intensity;
		bool  success = true;
		while (i < model->f_size)
		{
			j = 0;
			while (j < model->f[i].size)
			{
				vect3 v1 = model->v[model->f[i].data[j] - 1];
				vect3 uv = model->uvs[model->f[i].data[(j + 1) % model->f[i].size] - 1];


				poly[j / 3] = transform(v1, M);

				texcords[j / 3].x = uv.x;
				texcords[j / 3].y = uv.y;
				texcords[j / 3].z = uv.z;

				j += 3;
			}

			normal = !((poly[1] - poly[0]) * (poly[2] - poly[0]));

			intensity = normal ^ lightDir * -1.0f;
			intensity = clamp01(intensity);

			poly[0] = transform(poly[0], VpxPxVM);
			poly[1] = transform(poly[1], VpxPxVM);
			poly[2] = transform(poly[2], VpxPxVM);

			fillTriangle(poly, nullptr, true, intensity, text, texcords);

			if (model->f[i].size >= 12)
			{
				poly[3] = transform(poly[3], VpxPxVM);
				poly[1] = poly[3];
				texcords[1] = texcords[3];
				fillTriangle(poly, nullptr, true, intensity, text, texcords);
			}

			i++;
		}
		return model;
	}

}

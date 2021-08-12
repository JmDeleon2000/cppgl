#include <chrono>
#include <iostream>
import graphics;
import ModelImporter;
import hb_math;
using namespace hb_math;

using namespace std::chrono;
using namespace gl;


int main()
{
	auto start = high_resolution_clock::now();
	const int width = 1920;
	const int height = 1080;
	glCreateWindow(1920, 1080);
	set_draw_col(WHITE);
	vect3* scale = new vect3();
	vect3* trans = new vect3();
	scale->x = 150;
	scale->y = 150;
	scale->z = 150;
	trans->x = width / 2;
	trans->y = height / 2;
	trans->z = height;
	vect3 light = *new vect3();
	modelImp::texture* gradient = new modelImp::texture("models/model.bmp");
	light.x = 0;
	light.y = 1;
	light.z = 1;
	modelImp::obj* model = glLoadModel("models/model.obj", *trans, *scale, light, gradient, false);

	trans->x = width - 300;


	M4x4 a, b;
	a[0][0] = 1;
	a[0][1] = 3;
	a[0][2] = 8;
	a[0][3] = 9;

	a[1][0] = -3;
	a[1][1] = 6;
	a[1][2] = 5;
	a[1][3] = 9;

	a[2][0] = 6;
	a[2][1] = -2;
	a[2][2] = -6;
	a[2][3] = 5;

	a[3][0] = -3;
	a[3][1] = 0;
	a[3][2] = -4;
	a[3][3] = 2;
	M4x4 c = a + b;
	int i = 0, j;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			b[i][j] = 0;
			j++;
		}
		b[i][i] = 1;
		i++;
	}
	c = b;
	c = a * b;

	if (c == a)
		std::cout << "mul correcta" << std::endl;
	i = 0, j;
	while (i < 4)
	{
		j = 0;
		while (j < 4)
		{
			b[i][j] = 0;
			j++;
		}
		b[i][i] = -1;
		i++;
	}
	vect4 v;
	v[0] = 1;
	v[1] = 4;
	v[2] = 5;
	v[3] = -9;
	v = b * v;

	//https://ximera.osu.edu/la/LinearAlgebra/LTR-M-0030/main

	std::cout << v[0] << " " << v[1] << " " << v[2] << " " << v[3] << std::endl;

	if (model)
		glLoadModel(model, *trans, *scale, light, gradient, false);

	trans->x = 300;

	if (model)
		glLoadModel(model, *trans, *scale, light, gradient, false);
	glFinish("transformations.bmp", false);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	return 0;
}
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
	scale->x = 0.01;
	scale->y = 0.01;
	scale->z = 0.01;
	trans->x = width / 2;
	trans->y = height / 2;
	trans->z = height;
	vect3 light = *new vect3();
	modelImp::texture* gradient = new modelImp::texture("texture.bmp");
	light.x = 0;
	light.y = 1;
	light.z = 1;
	modelImp::obj* model = glLoadModel("models/Skeleton.obj", *trans, *scale, light, gradient, false);
	std::cout << "sk1" << std::endl;
	trans->x = width - 300;

	if (model)
		glLoadModel(model, *trans, *scale, light, gradient, false);

	trans->x = 300;

	if (model)
		glLoadModel(model, *trans, *scale, light, gradient, false);
	glFinish("flatShading.bmp", false);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	return 0;
}
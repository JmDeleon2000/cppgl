#include <chrono>
#include <iostream>
import graphics;
import ModelImporter;


using namespace std::chrono;
using namespace gl;

/*int main()
{
	auto start = high_resolution_clock::now();
	const int width = 1920;
	const int height = 1080;
	glCreateWindow(1920, 1080);
	set_draw_col(WHITE);
	vert2* scale = new vert2();
	vert2* trans = new vert2();
	scale->x = 1500;
	scale->y = 1500;
	trans->x = width / 2;
	trans->y = height / 2-500;
	modelImp::vect3 light = *new modelImp::vect3(-1, -1, 0);
	modelImp::obj* model = glLoadModel("models/gekota.obj", *trans, *scale, light, false);
	scale->x = 1500;
	scale->y = 1500;
	trans->y = height / 2 - 500;
	trans->x = 300;
	light.x *= -1;
	if (model)
		glLoadModel(model, *trans, *scale, light, false);
	trans->x = width-300;
	light.x = 0.5;
	light.y *= -1;
	if (model)
		glLoadModel(model, *trans, *scale, light, false);
	glFinish("combo.bmp", false);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	return 0;
}*/
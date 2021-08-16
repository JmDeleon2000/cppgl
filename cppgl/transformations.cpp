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
	vect3 scale;
	vect3 trans;
	vect3 rotation;
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	scale.x = 500;
	scale.y = 500;
	scale.z = 500;
	trans.y = 250;
	vect3 light = *new vect3();
	modelImp::texture* gradient = new modelImp::texture("models/gekotaTexture.bmp");
	light.x = 1;
	light.y = 1;
	light.z = -1;
	modelImp::obj* model = nullptr;
	vect3 eye;
	eye.y = 500;
	vect3 camPos;
	camPos.y = 500;
	camPos.z = -400;
	LookAt(eye, camPos);
	
	
	model = glRenderModel(new modelImp::obj("models/gekota.obj"), trans, scale, rotation, light, gradient);

	glFinish("lowAngle.bmp", false);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	return 0;
}
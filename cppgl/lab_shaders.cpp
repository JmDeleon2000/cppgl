#include <chrono>
#include <iostream>
import graphics;
import ModelImporter;
import hb_math;
import shaders;

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
	scale.x = 4;
	scale.y = 4;
	scale.z = 4;

	vect3 light = *new vect3();
	vect3 lightColor;
	modelImp::texture* gradient = new modelImp::texture("models/model.bmp");

	modelImp::obj* model = nullptr;
	vect3 eye;
	eye.y = 0;
	vect3 camPos;
	camPos.y = 0;
	camPos.z = -50;
	LookAt(eye, camPos);
	gl::active_textures[0] = gradient;
	light.z = -1;
	lightColor.x = 0;
	lightColor.y = 0;
	lightColor.z = 0;
	setDirLight(0, light, lightColor);
	light.z = 0;
	light.x = 1;
	lightColor.x = 0.5;
	lightColor.y = 0;
	lightColor.z = 0;
	setDirLight(1, light, lightColor);
	lightColor.x = 0;
	lightColor.y = 0;
	lightColor.z = 0.5;
	light.y = 1;
	light.x = 0;
	setDirLight(2, light, lightColor, 1);
	light.x = 1;
	light.y = 4;
	light.z = 2;
	lightColor.z = 0;
	lightColor.y = 0.2;
	setSpotLight(0, light, lightColor, 3);



	gl::active_shader = shd::toon;
	model = glRenderModel(new modelImp::obj("models/model.obj"), trans, scale, rotation, light, gradient);

	trans.x = -10;
	gl::active_shader = shd::flat;
	//glRenderModel(model, trans, scale, rotation, light, gradient);

	trans.x = 10;
	gl::active_shader = shd::gourad;
	//glRenderModel(model, trans, scale, rotation, light, gradient);


	glFinish("lowAngle.bmp", false);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;
	return 0;
}
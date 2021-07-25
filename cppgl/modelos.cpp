import graphics;

using namespace gl;

int main() 
{
	const int width = 1920;
	const int height = 1080;
	glCreateWindow(1920, 1080);
	set_draw_col(WHITE);
	vert2* scale = new vert2();
	vert2* trans = new vert2();
	scale->x = 300;
	scale->y = 300;
	trans->x = width / 2;
	trans->y = height / 2;
	glLoadModel("models/model.obj", *trans, *scale);
	glFinish("model.bmp");
	return 0;
}
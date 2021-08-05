import graphics;

using namespace gl;

int main()
{
	glCreateWindow(1920, 1080);
	set_draw_col(WHITE);

	vert2 poly[3];
	poly[0].x = 0;
	poly[0].y = 0;
	poly[1].x = 1920 / 2;
	poly[1].y = 1080;
	poly[2].x = 1920;
	poly[2].y = 0;

	fillTriangle(poly);

	glFinish("barycentricTriangle.bmp");
	return 0;
}
#include <cmath>
import graphics;
import hb_math;
using namespace hb_math;



using namespace gl;




#define tau 6.28318

void figure(int lines, vect2* init, int length) 
{
	const float angle = tau / (double)lines;
	vect2* v1 = new vect2(), * v2 = new vect2();
	*v1 = *init;
	float currAngle = angle;

	int i = 0;
	while (i < lines)
	{
		v2->x = v1->x + cos(currAngle) * length;
		v2->y = v1->y + sin(currAngle) * length;
		glLine(v1, v2);
		*v1 = *v2;
		currAngle += angle;
		i++;
	}
	

	delete v1;
	delete v2;
}

/*
	int main()
	{
		glCreateWindow(500, 500);
		clear();

		set_draw_col(WHITE);
		
		vect2* v1 = new vect2(), * v2 = new vect2();

		v1->x = 10;
		v1->y = 10;
		v2->x = 50;
		v2->y = 10;

		glLine(v1, v2);
		
		v2->x  = 10;
		v2->y = 90;

		glLine(v1, v2);

		*v1 = *v2;

		v2->x = 50;
		v2->y = 10;
		glLine(v1, v2);


		int i = 0;
		while (i+3 < 10)
		{
			v1->x = i * 70;
			v1->y = i * 50;
			figure(i+3, v1, 30);
			i++;
		}
		
	

		glFinish("lines.bmp");
		
		return 0;
	}

	*/
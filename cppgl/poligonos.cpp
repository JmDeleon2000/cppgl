import graphics;
import hb_math;
using namespace hb_math;
using namespace gl;


void trivialTriangle(vect2 v0, vect2 v1, vect2 v2)
{
	float m0, m1;
	if (v0.x < v2.x)
	{
		m0 = (v1.y - v0.y) / (v1.x - v0.x);
		m1 = (v0.y - v2.y) / (v0.x - v2.x);
	}
	else 
	{
		m0 = (v1.y - v0.y) / (v1.x - v0.x);
		m1 = (v0.y - v2.y) / (v0.x - v2.x);
	}
}




float sqrDist(vect2 v1, vect2 v2) 
{
	return (v2.y - v1.y) * (v2.y - v1.y) + (v2.x - v1.x) * (v2.x - v1.x);
}




void drawPoly(vect2* poly, int size) 
{
	int i = 0;

	while (i < size)
	{
		glLine(&poly[i], &poly[(i + 1) % size], nullptr);
		i++;
	}
}

/*int main()
{
	vect2* p1 = new vect2[10];
	vect2* p2 = new vect2[4];
	vect2* p3 = new vect2[3];
	vect2* p4 = new vect2[18];
	vect2* p5 = new vect2[4];
	vect2 v = *new vect2();
	{
		{
			v.x = 165;
			v.y = 380;
			p1[0] = v;
			v.x = 185;
			v.y = 360;
			p1[1] = v;
			v.x = 180;
			v.y = 330;
			p1[2] = v;
			v.x = 207;
			v.y = 345;
			p1[3] = v;
			v.x = 233;
			v.y = 330;
			p1[4] = v;
			v.x = 230;
			v.y = 360;
			p1[5] = v;
			v.x = 250;
			v.y = 380;
			p1[6] = v;
			v.x = 220;
			v.y = 385;
			p1[7] = v;
			v.x = 205;
			v.y = 410;
			p1[8] = v;
			v.x = 193;
			v.y = 383;
			p1[9] = v;
		}//brackets para poder minizar con editor
		{
			v.x = 321;
			v.y = 355;
			p2[0] = v;
			v.x = 288;
			v.y = 286;
			p2[1] = v;
			v.x = 399;
			v.y = 251;
			p2[2] = v;
			v.x = 374;
			v.y = 302;
			p2[3] = v;
		}
		{
			v.x = 377;
			v.y = 249;
			p3[0] = v;
			v.x = 411;
			v.y = 197;
			p3[1] = v;
			v.x = 436;
			v.y = 249;
			p3[2] = v;
		}
		{
			v.x = 413;
			v.y = 177;
			p4[0] = v;
			v.x = 448;
			v.y = 159;
			p4[1] = v;
			v.x = 502;
			v.y = 88;
			p4[2] = v;
			v.x = 533;
			v.y = 53;
			p4[3] = v;
			v.x = 535;
			v.y = 36;
			p4[4] = v;
			v.x = 617;
			v.y = 37;
			p4[5] = v;
			v.x = 660;
			v.y = 52;
			p4[6] = v;
			v.x = 750;
			v.y = 145;
			p4[7] = v;
			v.x = 761;
			v.y = 179;
			p4[8] = v;
			v.x = 672;
			v.y = 192;
			p4[9] = v;
			v.x = 659;
			v.y = 214;
			p4[10] = v;
			v.x = 615;
			v.y = 214;
			p4[11] = v;
			v.x = 632;
			v.y = 230;
			p4[12] = v;
			v.x = 580;
			v.y = 230;
			p4[13] = v;
			v.x = 597;
			v.y = 215;
			p4[14] = v;
			v.x = 552;
			v.y = 214;
			p4[15] = v;
			v.x = 517;
			v.y = 144;
			p4[16] = v;
			v.x = 466;
			v.y = 180;
			p4[17] = v;

		}
		{
			v.x = 682;
			v.y = 175;
			p5[0] = v;
			v.x = 708;
			v.y = 120;
			p5[1] = v;
			v.x = 735;
			v.y = 148;
			p5[2] = v;
			v.x = 739;
			v.y = 170;
			p5[3] = v;
		}
	}
	//brackets para minimizar con editor
	glCreateWindow(800, 500);
	set_draw_col(WHITE);

	fillTriangle(p3);

	fillTriangle(p2);
	vect2* temp = new vect2[3];
	temp[0] = p2[1];
	p2[1] = p2[3];
	fillTriangle(p2);
	p2[1] = temp[0];

	
	fillTriangle(&p1[1]);
	fillTriangle(&p1[3]);
	fillTriangle(&p1[5]);
	fillTriangle(&p1[7]);
	temp[0] = p1[0];
	temp[1] = p1[9];
	temp[2] = p1[1];
	fillTriangle(temp);
	temp[0] = p1[3];
	temp[1] = p1[9];
	temp[2] = p1[1];
	fillTriangle(temp);
	temp[0] = p1[3];
	temp[1] = p1[5];
	temp[2] = p1[7];
	fillTriangle(temp);
	temp[0] = p1[7];
	temp[1] = p1[9];
	temp[2] = p1[3];
	fillTriangle(temp);

	fillTriangle(&p4[3]);
	fillTriangle(&p4[5]);
	temp[0] = p4[17];
	temp[1] = p4[0];
	temp[2] = p4[1];
	fillTriangle(temp);
	temp[1] = p4[16];
	temp[2] = p4[1];
	fillTriangle(temp);
	temp[0] = p4[2];
	fillTriangle(temp);
	temp[2] = p4[3];
	fillTriangle(temp);
	temp[0] = p4[5];
	fillTriangle(temp);
	temp[2] = p4[15];
	fillTriangle(temp);
	temp[1] = p4[14];
	fillTriangle(temp);
	temp[1] = p4[11];
	fillTriangle(temp);
	temp[1] = p4[10];
	fillTriangle(temp);
	temp[2] = p4[9];
	fillTriangle(temp);
	temp[1] = p4[8];
	fillTriangle(temp);
	temp[2] = p4[7];
	fillTriangle(temp);
	temp[0] = p4[14];
	temp[1] = p4[13];
	temp[2] = p4[12];
	fillTriangle(temp);
	temp[1] = p4[11];
	fillTriangle(temp);

	set_draw_col(new col3(0, 0, 0));
	fillTriangle(p5);
	temp[0] = p5[0];
	temp[1] = p5[3];
	temp[2] = p5[2];
	fillTriangle(temp);

	set_draw_col(new col3(1, 0, 0));
	drawPoly(p1, 10);
	drawPoly(p2, 4);
	drawPoly(p3, 3);
	drawPoly(p4, 18);
	drawPoly(p5, 4);

	glFinish("polys.bmp", false);
	return 0;
}*/
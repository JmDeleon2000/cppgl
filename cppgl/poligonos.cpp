import graphics;

using namespace gl;


//polys
class poly
{
public:
	vert2* vertices;
	int lowest, highest, leftmost, rightmost, quantity;
	poly()
	{
		vertices = nullptr;
	}
	poly(vert2* vertices, int quantity = 3)
	{
		poly::vertices = vertices;
		poly::quantity = quantity;

		lowest = vertices[0].y;
		highest = vertices[0].y;
		leftmost = vertices[0].x;
		rightmost = vertices[0].y;

		int i = 1;
		while (i < quantity)
		{
			if (lowest > vertices[i].y) lowest = vertices[i].y;
			if (highest < vertices[i].y) highest = vertices[i].y;
			if (leftmost > vertices[i].x) leftmost = vertices[i].x;
			if (rightmost < vertices[i].x) rightmost = vertices[i].x;
			i++;
		}
	}
};


void fillTriangle(vert2* triangle) 
{
	vert2 middle;
	int lowest, highest, leftmost, rightmost, lmi = 0, rmi = 0;
	lowest = triangle[0].y;
	highest = triangle[0].y;
	leftmost = triangle[0].x;
	rightmost = triangle[0].y;

	int i = 0;
	while (i<3)
	{
		if (lowest > triangle[i].y) lowest = triangle[i].y;
		if (highest < triangle[i].y) highest = triangle[i].y;
		if (leftmost > triangle[i].x) { leftmost = triangle[i].x; lmi = i; }
		if (rightmost < triangle[i].x) { rightmost = triangle[i].x; rmi = i; }
		i++;
	}
	middle = triangle[((lmi == 0 && rmi == 2) ? 1 : (lmi == 1 && rmi == 2) ? 0 : 2)];

	if (highest == middle.y) 
	{
		
	}
	else 
	{

	}

	int j;
	i = leftmost;
	while (i < rightmost)
	{
		j = lowest;
		while (j < highest)
		{

			j++;
		}
		i++;
	}

}


int main() 
{
	poly* poly1, poly2, poly3, poly4, poly5;
	vert2* p1 = new vert2[10];
	vert2* p2 = new vert2[4];
	vert2* p3 = new vert2[3];
	vert2* p4 = new vert2[18];
	vert2* p5 = new vert2[4];
	vert2 v = *new vert2();
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
		v.x = 0;
		v.y = 0;
		p4[0] = v;
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
	//brackets para minimizar con editor


	return 0;
}
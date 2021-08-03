import graphics;

using namespace gl;


void trivialTriangle(vert2 v0, vert2 v1, vert2 v2)
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




float sqrDist(vert2 v1, vert2 v2) 
{
	return (v2.y - v1.y) * (v2.y - v1.y) + (v2.x - v1.x) * (v2.x - v1.x);
}

//https://www.geeksforgeeks.org/cpp-program-for-quicksort/
//me dio hueva buscar documentación de std::sort
void swap(vert2* a, vert2* b)
{
	vert2 t = *a;
	*a = *b;
	*b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
	array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition(vert2 arr[], int low, int high)
{
	float pivot = arr[high].x;    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[j].x <= pivot)
		{
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(vert2 arr[], int low, int high)
{
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		   at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before
		// partition and after partition
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}

void fillPoly(vert2* poly, int size) 
{
	vert2* triangle = new vert2[3];
	float d1, d2, dist;
	int index0 = 0, index1 = 1;
	int i = 0, j;
	quickSort(poly, 0, size-1);
	while (i < size)
	{
		j = i+1;
		d1 = 10000000.0f;
		d2 = 10000000.0f;
		while (j < size)
		{
			dist = sqrDist(poly[i], poly[j]);
			if (dist < d1)
			{
				index1 = index0;
				index0 = j;
				d2 = d1;
				d1 = dist;
			}
			if (dist < d2 && dist > d1)
			{
				index1 = j;
				d2 = dist;
			}
			j++;
			
		}
		//index0 = i + 1;
		//index1 = i + 2;
		triangle[0] = poly[i];
		triangle[1] = poly[index0];
		triangle[2] = poly[index1];
		gl::fillTriangle(triangle);
		
		i++;
	}
	delete[] triangle;
}


void drawPoly(vert2* poly, int size) 
{
	int i = 0;

	while (i < size)
	{
		glLine(&poly[i], &poly[(i + 1) % size], nullptr);
		i++;
	}
}

int main()
{
	vert2* p1 = new vert2[10];
	vert2* p2 = new vert2[4];
	vert2* p3 = new vert2[3];
	vert2* p4 = new vert2[18];
	vert2* p5 = new vert2[4];
	vert2 v = *new vert2();
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
	vert2* temp = new vert2[3];
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
}
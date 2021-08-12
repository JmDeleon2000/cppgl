export module hb_math;

export namespace hb_math
{
	struct vect2
	{
		float x = 0;
		float y = 0;
	};

	struct vect3 {
	
		double  x = 0, y = 0, z = 0;
		
	};

	float clamp01(float x)
	{
		return x < 0.0f ? 0.0f : x > 1.0f ? 1.0f : x;
	}

	



	float R2_cross(vect2 a, vect2 b) 
	{
		return a.x * b.y - a.y * b.x;
	}

	void R3_cross(vect3 a, vect3 b, vect3* out)
	{
		out->x = a.y * b.z - b.y * a.z;
		out->y = a.z * b.x - b.z * a.x;
		out->z = a.x * b.y - b.x * a.y;
	}

	//Quake's fast inverse sqrt
	float q_sqrt(float x) 
	{
		long i;
		float x2, y = x;

		const float threehalfs = 1.5F;

		x2 = x * 0.5F;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));
	
		return y;
	}

	void normalize(vect2* a) 
	{
		float inv_length;

		inv_length = q_sqrt(a->x * a->x + a->y * a->y);
		a->x *= inv_length;
		a->y *= inv_length;

	}

	void normalize(vect3* a)
	{
		float inv_length;

		inv_length = q_sqrt(a->x * a->x + a->y * a->y + a->z * a->z);
		a->x *= inv_length;
		a->y *= inv_length;
		a->z *= inv_length;
	}
	
	float dot(vect3 a, vect3 b) 
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
}
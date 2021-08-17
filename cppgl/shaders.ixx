export module shaders;
import ModelImporter;
import hb_math;

using namespace modelImp;
using namespace hb_math;

export namespace shd
{
	struct shdArgs
	{
		vect3 A;
		vect3 B;
		vect3 C;
		vect3 interpolators;
		vect3* texCords = nullptr;
		vect3* normals = nullptr;
		vect3* lightDir = nullptr;
		texture** textures;
	};

	vect3 flat(void* args)
	{
		vect3 col;
		col.x = col.y = col.z = 1;
		vect3 A = ((shdArgs*)args)->A;
		vect3 B = ((shdArgs*)args)->B;
		vect3 C = ((shdArgs*)args)->C;
		vect3* texCords = ((shdArgs*)args)->texCords;
		vect3 uvw = ((shdArgs*)args)->interpolators;
		vect3 lightDir = ((shdArgs*)args)->lightDir[0];
		texture text = *((shdArgs*)args)->textures[0];

		float intensity;
		text.getColor((texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z),
			(texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z),  &col);
		
		vect3 normal;
		normal = !((B - A) * (C - A));
		intensity = clamp01(normal ^ (lightDir * -1.0f));

		col *=  intensity;

		return col;
	}
}
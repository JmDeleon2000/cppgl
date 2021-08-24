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
		vect3 triangleNormal;
		vect3* texCords = nullptr;
		vect3* normals = nullptr;
		vect3* lightDir = nullptr;
		vect3* lightCol = nullptr;
		float* lightInt = nullptr;
		vect3* spotLightPos = nullptr;
		vect3* spotLightCol = nullptr;
		float* spotLightInt = nullptr;
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
		vect3* normals = ((shdArgs*)args)->normals;
		vect3 normal = ((shdArgs*)args)->triangleNormal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx,  &col);
		
		float intensity;

		intensity = clamp01(normal ^ lightDir * -1.0f);

		col = col * intensity;

		return col;
	}


	vect3 gourad(void* args)
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
		vect3* normals = ((shdArgs*)args)->normals;


		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx, &col);

		float intensity, intensityA, intensityB, intensityC;
		intensityA = clamp01(normals[0] ^ lightDir * -1.0f);
		intensityB = clamp01(normals[1] ^ lightDir * -1.0f);
		intensityC = clamp01(normals[2] ^ lightDir * -1.0f);

		intensity = intensityA * uvw.x + intensityB * uvw.y + intensityC * uvw.z;

		col = col * intensity;

		return col;
	}


	vect3 multiLight(void* args)
	{
		vect3 col;
		col.x = col.y = col.z = 1;
		vect3 A = ((shdArgs*)args)->A;
		vect3 B = ((shdArgs*)args)->B;
		vect3 C = ((shdArgs*)args)->C;
		vect3* texCords = ((shdArgs*)args)->texCords;
		vect3 uvw = ((shdArgs*)args)->interpolators;
		vect3* lightDir = ((shdArgs*)args)->lightDir;
		vect3* lightCol = ((shdArgs*)args)->lightCol;
		float* lightInt = ((shdArgs*)args)->lightInt;
		vect3* spotLightPos = ((shdArgs*)args)->spotLightPos;
		vect3* spotLightCol = ((shdArgs*)args)->spotLightCol;
		float* spotLightInt = ((shdArgs*)args)->spotLightInt;
		texture text = *((shdArgs*)args)->textures[0];
		vect3* normals = ((shdArgs*)args)->normals;
		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;
		
		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i]) * lightInt[i];
			intensity += colorIntensity;
			col = col + lightCol[i] * colorIntensity;
			i++;
		}
		vect3 temp;
		float dist;
		i = 0;
		while (i < 10)
		{
			temp = A * uvw.x + B * uvw.y + C * uvw.z;
			dist = Dist(temp, spotLightPos[i]);
			temp =  spotLightPos[i]-temp;
			colorIntensity = clamp01(normal ^ temp * (spotLightInt[i] - dist)) * spotLightInt[i];
			intensity += colorIntensity;
			col = col + spotLightCol[i] * clamp01(colorIntensity - dist);
			i++;
		}

		col = col * clamp01(intensity);
		col.x = clamp01(col.x);
		col.y = clamp01(col.y);
		col.z = clamp01(col.z);

		return col;
	}

	vect3 phong(void* args)
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
		vect3* normals = ((shdArgs*)args)->normals;
		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity;

		intensity = clamp01(normal ^ lightDir * 1.0f);

		col = col * intensity;

		return col;
	}

	vect3 normalMap(void* args)
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
		texture normalMap = *((shdArgs*)args)->textures[1];
		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx, &col);

		normalMap.getColor(ty,
			tx, &normal);

		float intensity;

		intensity = clamp01(!normal ^ lightDir * 1.0f);

		col = col * intensity;

		return col;
	}

	vect3 toon(void* args)
	{
		vect3 col;
		col.x = col.y = col.z = 1;
		vect3 A = ((shdArgs*)args)->A;
		vect3 B = ((shdArgs*)args)->B;
		vect3 C = ((shdArgs*)args)->C;
		vect3* texCords = ((shdArgs*)args)->texCords;
		vect3 uvw = ((shdArgs*)args)->interpolators;
		vect3* lightDir = ((shdArgs*)args)->lightDir;
		vect3* lightCol = ((shdArgs*)args)->lightCol;
		float* lightInt = ((shdArgs*)args)->lightInt;
		vect3* spotLightPos = ((shdArgs*)args)->spotLightPos;
		vect3* spotLightCol = ((shdArgs*)args)->spotLightCol;
		float* spotLightInt = ((shdArgs*)args)->spotLightInt;
		texture text = *((shdArgs*)args)->textures[0];
		vect3* normals = ((shdArgs*)args)->normals;
		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(ty,
			tx, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;

		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i]) * lightInt[i];
			intensity += colorIntensity;
			col = col + lightCol[i] * colorIntensity;
			i++;
		}
		vect3 temp;
		float dist;
		i = 0;
		while (i < 10)
		{
			temp = A * uvw.x + B * uvw.y + C * uvw.z;
			dist = Dist(temp, spotLightPos[i]);
			temp = spotLightPos[i] - temp;
			colorIntensity = clamp01(normal ^ temp * (spotLightInt[i] - dist)) * spotLightInt[i];
			intensity += colorIntensity;
			col = col + spotLightCol[i] * clamp01(colorIntensity - dist);
			i++;
		}

		
		if (intensity < 0.25) intensity = 0;
		else if (intensity < 0.5) intensity = 0.25;
		else if (intensity < 0.75) intensity = 0.5;
		else  intensity = 1;

		col = col * clamp01(intensity);
		col.x = clamp01(col.x);
		col.y = clamp01(col.y);
		col.z = clamp01(col.z);

		return col;
	}
}
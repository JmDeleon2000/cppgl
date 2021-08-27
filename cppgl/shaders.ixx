export module shaders;
import ModelImporter;
import hb_math;

using namespace modelImp;
using namespace hb_math;

export namespace shd
{
	struct shaderOut
	{
		int blending = 0;
		bool zWrite = true;
		float alpha = 1;
		vect3 CamPos;
	};

	struct shdArgs
	{
		vect3 A;
		vect3 B;
		vect3 C;
		vect3 interpolators;
		vect3 triangleNormal;
		vect3 cameraPositon;
		vect3* texCords = nullptr;
		vect3* normals = nullptr;
		vect3* lightDir = nullptr;
		vect3* lightCol = nullptr;
		float* lightInt = nullptr;
		vect3* spotLightPos = nullptr;
		vect3* spotLightCol = nullptr;
		float* spotLightInt = nullptr;
		texture** textures;
		shaderOut out;
	};


	float fresnelITP(vect3 normal, vect3 camDir) 
	{
		return normal ^ !camDir;
	}

	float frac(float x) 
	{
		return x - (long)x;
	}


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
		text.getColor(tx, ty, &col);
		
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
		text.getColor(tx, ty, &col);

		float intensity, intensityA, intensityB, intensityC;
		intensityA = normals[0] ^ lightDir * -1.0f;
		intensityB = normals[1] ^ lightDir * -1.0f;
		intensityC = normals[2] ^ lightDir * -1.0f;

		intensity = clamp01(intensityA * uvw.x + intensityB * uvw.y + intensityC * uvw.z);

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
		text.getColor(tx, ty, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;
		
		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i] * -1.0f) * lightInt[i];
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
		text.getColor(tx, ty, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity;

		intensity = clamp01(normal ^ lightDir * -1.0f);

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
		text.getColor(tx, ty, &col);

		normalMap.getColor(ty,
			tx, &normal);

		float intensity;

		intensity = clamp01(!normal ^ lightDir * 1.0f);

		col = col * intensity;

		return col;
	}//unfinished

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
		text.getColor(tx, ty, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;

		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i] * -1.0f) * lightInt[i];
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


	vect3 alpha(void* args)
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

		// cambiar blending a add y dejar de escribir en el zBuffer
		((shdArgs*)args)->out.blending = 1;
		((shdArgs*)args)->out.zWrite = false;

		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(tx, ty, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;

		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i] * -1.0f) * lightInt[i];
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

		((shdArgs*)args)->out.alpha = frac(ty*100);

		col = col * clamp01(intensity);
		col.x = clamp01(col.x);
		col.y = clamp01(col.y);
		col.z = clamp01(col.z);

		return col;
	}


	vect3 outline(void* args)
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
		vect3 camPos = ((shdArgs*)args)->cameraPositon;

		// cambiar blending a add y dejar de escribir en el zBuffer
		((shdArgs*)args)->out.blending = 1;
		((shdArgs*)args)->out.zWrite = false;

		vect3 normal;

		float tx = (texCords[0].x * uvw.x + texCords[1].x * uvw.y + texCords[2].x * uvw.z);
		float	ty = (texCords[0].y * uvw.x + texCords[1].y * uvw.y + texCords[2].y * uvw.z);
		text.getColor(tx, ty, &col);

		normal = (normals[0] * uvw.x) + (normals[1] * uvw.y) + (normals[2] * uvw.z);

		float intensity = 0, colorIntensity;

		vect3 position = A * uvw.x + B * uvw.y + C * uvw.z;

		int i = 0;
		while (i < 10)
		{
			colorIntensity = clamp01(normal ^ lightDir[i] * -1.0f) * lightInt[i];
			intensity += colorIntensity;
			col = col + lightCol[i] * colorIntensity;
			i++;
		}
		vect3 temp;
		float dist;
		i = 0;
		while (i < 10)
		{
			temp = position;
			dist = Dist(temp, spotLightPos[i]);
			temp = spotLightPos[i] - temp;
			colorIntensity = clamp01(normal ^ temp * (spotLightInt[i] - dist)) * spotLightInt[i];
			intensity += colorIntensity;
			col = col + spotLightCol[i] * clamp01(colorIntensity - dist);
			i++;
		}


		

		col = col * clamp01(intensity);

		col.x = clamp01(col.x);
		col.y = clamp01(col.y);
		col.z = clamp01(col.z);


		float res = fresnelITP(normal, position - camPos);
		if (res < 0) res *= -1;
		((shdArgs*)args)->out.alpha = clamp01((1-clamp01(res))*0.5);
		

		return col;
	}
}
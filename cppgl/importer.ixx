#include <fstream>
#include <string>
#include <iostream>
#include <vector>

export module ModelImporter;
using namespace std;


#define importer_debug false

export namespace modelImp
{

	class vect3 {
	public:
		double  x, y, z;
		vect3(double x, double y, double z)
		{
			vect3::x = x;
			vect3::y = y;
			vect3::z = z;
		}
		vect3() 
		{
			vect3::x = 0;
			vect3::y = 0;
			vect3::z = 0;
		}
	};

	struct face
	{
		int size;
		int *data;
	};
	
	class obj
	{
	public:
		vect3* v;
		vect3* uvs;
		vect3* n;
		face* f;
		int v_size, uv_size, n_size, f_size;

	private:
		int v_b_size, uv_b_size, n_b_size, f_b_size;

	public:
		obj(const char* file)
		{
			vector<vect3> v_temp;
			vector<vect3> uvs_temp;
			vector<vect3> n_temp;
			vector<face> f_temp;

			ifstream stream;
			stream.open(file);
			string line;

			while (getline(stream, line))
			{
				if (line[0] == 'v' && line[1] != 't' && line[1] != 'n') //vert
				{ 
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 2);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ")+1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ")+1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());
#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug

					v_temp.push_back(*new vect3(x, y, z));
					continue;
				}
				if (line[0] == 'v' && line[1] == 't') //uv
				{
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 3);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());

#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug

					uvs_temp.push_back(*new vect3(x, y, z));
					continue;
				}
				if (line[0] == 'v' && line[1] == 'n') //normals
				{					
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 3);
					double x = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double y = (double)atof(line.substr(0, line.find(" ")).c_str());
					line.erase(0, line.find(" ") + 1);
					double z = (double)atof(line.substr(0, line.find(" ")).c_str());

#if importer_debug
					cout << x << " " << y << " " << z << endl;
#endif // importer_debug
					
					n_temp.push_back(*new vect3(x, y, z));
					continue;
				}
				if (line[0] == 'f') //faces
				{
					
					face new_face;
#if importer_debug
					cout << line << endl;
#endif
					line.erase(0, 2);
					
					vector<int> k;
					string sub;
					while (line.find("/") != string::npos)
					{
						sub = line.substr(0, line.find(" "));
						while (!sub.empty())
						{
							k.push_back((int)atoi(sub.substr(0, sub.find("/")).c_str()));
							sub.erase(0, line.find("/") + 1);
						}
						if (line.find(" ") == string::npos)
							break;
						line.erase(0, line.find(" ") + 1);
					}
					k.shrink_to_fit();



					new_face.data = new int[k.size()];
					std::memcpy(new_face.data, k.data(), k.size() * sizeof(int));
					new_face.size = k.size();
#if importer_debug
					cout << new_face.data[0] << " " << new_face.data[1] << " " << new_face.data[2] << endl;
					cout << new_face.data[3] << " " << new_face.data[4] << " " << new_face.data[5] << endl;
					cout << new_face.data[6] << " " << new_face.data[7] << " " << new_face.data[8] << endl;
#endif
					f_temp.push_back(new_face);
				}
			}

			v_temp.shrink_to_fit();
			uvs_temp.shrink_to_fit();
			n_temp.shrink_to_fit();
			f_temp.shrink_to_fit();
			v_size = v_temp.size();
			uv_size = uvs_temp.size();
			n_size = n_temp.size();
			f_size = f_temp.size();

			v = new vect3[v_temp.size()];
			std::memcpy(v, v_temp.data(), v_temp.size() * sizeof(vect3));
			v_temp.clear();
			v_temp.shrink_to_fit();

			uvs = new vect3[uvs_temp.size()];
			std::memcpy(uvs, uvs_temp.data(), uvs_temp.size() * sizeof(vect3));
			uvs_temp.clear();
			uvs_temp.shrink_to_fit();

			n = new vect3[n_temp.size()];
			std::memcpy(n, n_temp.data(), n_temp.size() * sizeof(vect3));
			n_temp.clear();
			n_temp.shrink_to_fit();

			f = new face[f_temp.size()];
			std::memcpy(f, f_temp.data(), f_temp.size() * sizeof(face));
			f_temp.clear();
			f_temp.shrink_to_fit();			

#if importer_debug
			cout << v[v_size - 1].x << " " << v[v_size - 1].y << " " << v[v_size - 1].z << endl;
			cout << uvs[uv_size - 1].x << " " << uvs[uv_size - 1].y << " " << uvs[uv_size - 1].z << endl;
			cout << n[n_size - 1].x << " " << n[n_size - 1].y << " " << n[n_size - 1].z << endl;
#endif

			stream.close();
		}



		~obj()
		{
			delete[] v;
			delete[] uvs;
			delete[] n;
			delete[] f;
		}
	};
}
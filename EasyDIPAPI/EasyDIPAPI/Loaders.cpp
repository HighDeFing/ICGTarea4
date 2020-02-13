#define GLM_ENABLE_EXPERIMENTAL

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define ABS(N) ((N<0)?(-N):(N))

#include "Loaders.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "Windows.h"
#include "commdlg.h"
#include "shlwapi.h"
#include <filesystem> //to get the extension
#include <vector>
#include <string>
#include <ctype.h>
#include <fstream>
#include <string_view>
#include <sstream>
#include "glm/gtx/string_cast.hpp"

using std::vector;

class triangle {
public:
	glm::vec3 v0;
	glm::vec3 v1;
	glm::vec3 v2;
};

FILE* doc;

namespace CG
{ 
	void Load(const std::string path)
	{
		//if path termina en .obj
		//else if path termina en .off
		char s[1024];
		strncpy_s(s, path.c_str(), sizeof(s));
		s[sizeof(s) - 1] = 0;
		fopen_s(&doc, s, "r");
		if (doc == NULL) return;
			if (std::filesystem::path(s).extension() == ".obj")
			{
				LoadObj(s);
			}
			else
			{
				LoadOff(s);
			}
			return;
	}

	void LoadObj(const std::string path) {
		std::cout << "ich bin in obj." << std::endl;

	}

	void LoadOff(const std::string path) {
		std::cout << "ich bin in off." << std::endl;
		vector <glm::vec3> all_vertex;
		vector <glm::vec3> face_triangles;
		vector <triangle> all_triangles;

		triangle aux_triangle;
		int found;
		int i_3 = 0;
		int v_3 = 0;
		int f_count = 0;
		glm::vec3 aux;
		Vertex aux_vertex;
		Vertex aux_vertex_trian;
		std::string temp;

		vector <Vertex> vertices_trian;
		vector <Vertex> vertices;
		unsigned int indices_aux;
		vector <unsigned int> indices;

		float number_of_vertices;
		float number_of_faces;
		float number_of_edges;
		float number_of_vertices_faces;

		float vertex[3];
		float aux_faces;
		vector <float> faces_index;

		std::string line;
		std::string uncommented_file = "";
		std::ifstream myfile(path);
		std::stringstream ss;

		glm::vec3 Centro;
		glm::vec3 minVertex;
		glm::vec3 maxVertex;
		float median_point;


		if (myfile.is_open())
		{
			while (std::getline(myfile, line))
			{
					// This is a comment
				std::size_t found = line.find('#');
				if (found != std::string::npos)
				{
					//std::cout << "This is a comment:" << line.substr(found + 1) << '\n';
					uncommented_file += line.substr(0, found) + '\n';
					//std::cout << line.substr(0, found) << '\n';
					//ignore rest
				}
				else
				{
					uncommented_file += line + '\n';
					//std::cout << line << '\n';
				}
				
			}
			myfile.close();
		}
		else std::cout << "Unable to open file";
		//std::cout << uncommented_file;
		ss << uncommented_file;
		//find 3 numbers
		while (!ss.eof()) 
		{
			if (i_3 < 3) {
				ss >> temp;
				if (std::stringstream(temp) >> found)
				{
					switch (i_3)
					{
					case 0: number_of_vertices = found;
					case 1: number_of_faces = found;
					case 2: number_of_edges = found;
					}
					i_3++;
				}
			}
			else
			{
				if (v_3 < number_of_vertices)
				{
					//read vertices
					ss >> vertex[0] >> vertex[1] >> vertex[2];
					aux = glm::vec3(vertex[0], vertex[1], vertex[2]);
					//aux = glm::normalize(aux);
					//aux_vertex = { glm::normalize(glm::vec3(vertex[0], vertex[1], vertex[2])) };
					aux_vertex = { glm::vec3(vertex[0], vertex[1], vertex[2]) };
					vertices.insert(vertices.begin() + v_3, aux_vertex);
					all_vertex.insert(all_vertex.begin() + v_3, aux);
					//std::cout << vertex[0] << " " << vertex[1] << " " << vertex[2] << '\n';
					v_3++;
				}
				else
				{
					if (f_count < number_of_faces)
					{
						//read number of faces
						ss >> number_of_vertices_faces;
						for (int i = 0; i < number_of_vertices_faces; i++) {
							ss >> aux_faces;
							faces_index.insert(faces_index.begin() + i, aux_faces);
						}

						if (number_of_vertices_faces > 3) //if its not a triangle
						{
							//make triangles
							int k = 0;
							while ((k + 2) < number_of_vertices_faces)
							{
								//indices
								indices_aux = faces_index[0];
								indices.push_back(indices_aux);
								indices_aux = faces_index[k + 1];
								indices.push_back(indices_aux);
								indices_aux = faces_index[k + 2];
								indices.push_back(indices_aux);

								// indices en triangulos
								aux_vertex_trian.Position = all_vertex[faces_index[0]];
								vertices_trian.push_back(aux_vertex_trian);
								aux_vertex_trian.Position = all_vertex[faces_index[k+1]];
								vertices_trian.push_back(aux_vertex_trian);
								aux_vertex_trian.Position = all_vertex[faces_index[k+2]];
								vertices_trian.push_back(aux_vertex_trian);

								//vector de triangulos
								aux_triangle.v0 = all_vertex[faces_index[0]];
								aux_triangle.v1 = all_vertex[faces_index[k+1]];
								aux_triangle.v2 = all_vertex[faces_index[k+2]];

								//std::cout << glm::to_string(aux_triangle.v0) <<" "<< glm::to_string(aux_triangle.v1)<< " "<< glm::to_string(aux_triangle.v2)<< std::endl;
								all_triangles.push_back(aux_triangle);
								k++;
							}
						}
						else
						{
							indices_aux = faces_index[0];
							indices.push_back(indices_aux);
							indices_aux = faces_index[1];
							indices.push_back(indices_aux);
							indices_aux = faces_index[2];
							indices.push_back(indices_aux);

							// indices en triangulos
							aux_vertex_trian.Position = all_vertex[faces_index[0]];
							vertices_trian.push_back(aux_vertex_trian);
							aux_vertex_trian.Position = all_vertex[faces_index[1]];
							vertices_trian.push_back(aux_vertex_trian);
							aux_vertex_trian.Position = all_vertex[faces_index[2]];
							vertices_trian.push_back(aux_vertex_trian);


							aux_triangle.v0 = all_vertex[faces_index[0]];
							aux_triangle.v1 = all_vertex[faces_index[1]];
							aux_triangle.v2 = all_vertex[faces_index[2]];
							//std::cout << glm::to_string(aux_triangle.v0) << " " << glm::to_string(aux_triangle.v1) << " " << glm::to_string(aux_triangle.v2) << std::endl;
							all_triangles.push_back(aux_triangle);
						}
						//std::cout <<'\n';
						std::getline(ss, line);
						f_count++;
					}
					else break;
						
				}
			}
			
		}
		std::cout << number_of_vertices << " " <<number_of_faces << " " << number_of_edges <<'\n';
		std::cout << indices.size();
		maxVertex = glm::vec3(vertices[0].Position.x,vertices[0].Position.y,vertices[0].Position.z);
		minVertex = glm::vec3(vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
		for (int i = 0; i < vertices.size();i++) 
		{
			maxVertex = glm::vec3(MAX(maxVertex.x,vertices[i].Position.x),MAX(maxVertex.y,vertices[i].Position.y),MAX(maxVertex.z,vertices[i].Position.z));
			minVertex = glm::vec3(MIN(minVertex.x, vertices[i].Position.x), MIN(minVertex.y, vertices[i].Position.y), MIN(minVertex.z, vertices[i].Position.z));
				
		}
		Centro = (maxVertex + minVertex)/glm::vec3(2);
		float median_pointMAX_MAX = MAX(maxVertex.z, MAX(maxVertex.x, maxVertex.y));
		float median_pointMAX_MIN = MAX(minVertex.z, MAX(minVertex.x, minVertex.y));
		median_point = ABS(MAX(median_pointMAX_MAX,median_pointMAX_MIN));
		/*for (int k = 0; k < vertices_trian.size(); k++)
		{
			aux_vertex_trian.Position = (vertices_trian[k].Position - Centro) / glm::vec3(median_point);
			vertices_trian[k] = aux_vertex_trian;
			std::cout << "vertices" << glm::to_string(vertices_trian[k].Position) << std::endl;
		}*/
		for (int k = 0; k < vertices.size(); k++)
		{
			aux_vertex.Position = (vertices[k].Position - Centro) / glm::vec3(median_point);
			vertices[k] = aux_vertex;
			std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}
		std::cout << "Max vertex: " << glm::to_string(maxVertex) << '\n';
		std::cout << "Min vertex: " << glm::to_string(minVertex) << '\n';
		std::cout << "Centro: " << glm::to_string(Centro) << '\n';
		std::cout << "median_point: " << median_point << '\n';
		std::cout << "triangle: "<< glm::to_string(aux_vertex_trian.Position) << '\n';
		/*for (int k = 0;k < vertices.size();k++)
		{
			std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}*/
		std::cout << vertices.size() << " ";
		std::cout << vertices_trian.size();
		Mesh* mesh = Mesh::Instance();
		mesh->MeshCreate(vertices_trian, indices);
	}

}

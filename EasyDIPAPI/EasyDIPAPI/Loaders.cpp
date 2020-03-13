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
#include "obj_loader.h"

using std::vector;
FILE* doc;

namespace CG
{ 

	Mesh* Load(const std::string path)
	{
		//if path termina en .obj
		//else if path termina en .off
		Mesh* mesh = new Mesh();
		char s[1024];
		strncpy_s(s, path.c_str(), sizeof(s));
		s[sizeof(s) - 1] = 0;
		fopen_s(&doc, s, "r");
		if (doc == NULL) return NULL;
			if (std::filesystem::path(s).extension() == ".obj")
			{
				mesh = LoadObj(s);
				return mesh;
			}
			else
			{
				mesh = LoadOff(s);
				return mesh;
			}
			return NULL;
	}


	Mesh* LoadObj(const std::string path) 
	{
		// model read structures
		OBJModel *model = new OBJModel(path);
		std::vector<OBJIndex> indices_model;
		indices_model.clear();
		std::vector<glm::vec3> vertices_model;
		vertices_model.clear();
		std::vector<glm::vec3> normals_model;
		normals_model.clear();
		std::vector<glm::vec2> textures_model;
		textures_model.clear();

		Vertex aux_vertex;
		std::vector <Vertex> vertices;
		vertices.clear();
		vector <unsigned int> indices;
		indices.clear();
		//faster mode
		//model->ToIndexedModel();
		//model assing to reades
		indices_model = model->OBJIndices;
		vertices_model = model->vertices;
		normals_model = model->normals;
		textures_model = model->uvs;

		//center
		glm::vec3 Centro;
		glm::vec3 minVertex;
		glm::vec3 maxVertex;
		float median_point;

		// sizes
		float indices_size = indices_model.size();
		float vertices_size = vertices_model.size();
		float normals_size = normals_model.size();
		float textures_size = textures_model.size();

		/*std::cout << "vertices_size: "<< vertices_size << '\n';
		std::cout << "normals_size:" << normals_size << '\n';
		std::cout << "indices_size: " << indices_size << '\n';
		std::cout << "textures_model: " << textures_model.size() << '\n';*/
		for (int i = 0; i < vertices_size; i++)
		{
			//std::cout << glm::to_string(vertices[i]) << '\n';
			//aux_vertex.Position = vertices_model[indices_model[i].vertexIndex];
			//vertices.push_back(aux_vertex);
			aux_vertex.Position = vertices_model[i];
			vertices.insert(vertices.begin() + i, aux_vertex);
			//std::cout << "Position:" << glm::to_string(vertices[i].Position) << '\n';
		}
		//std::cout << glm::to_string(vertices_model[indices_model[0].vertexIndex]) << '\n';
		maxVertex = glm::vec3(vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
		minVertex = glm::vec3(vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
		for (int i = 0; i < vertices.size();i++)
		{
			maxVertex = glm::vec3(MAX(maxVertex.x, vertices[i].Position.x), MAX(maxVertex.y, vertices[i].Position.y), MAX(maxVertex.z, vertices[i].Position.z));
			minVertex = glm::vec3(MIN(minVertex.x, vertices[i].Position.x), MIN(minVertex.y, vertices[i].Position.y), MIN(minVertex.z, vertices[i].Position.z));

		}
		Centro = (maxVertex + minVertex) / glm::vec3(2);
		float median_pointMAX_MAX = MAX(maxVertex.z, MAX(maxVertex.x, maxVertex.y));
		float median_pointMAX_MIN = MAX(minVertex.z, MAX(minVertex.x, minVertex.y));
		median_point = ABS(MAX(median_pointMAX_MAX, median_pointMAX_MIN));
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
			//std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}
		for (int i = 0; i < indices_size; i++)
		{

			if (normals_size > 0)
			{
				/*std::cout << "indices_model[i].normalIndex: " << indices_model[i].normalIndex << '\n';
				std::cout << "normals_model[indices_model[i].normalIndex] " << glm::to_string(normals_model[indices_model[i].normalIndex]) << '\n';
				std::cout << "indices_model[i].vertexIndex" << indices_model[i].vertexIndex << '\n';*/
				//normals by face
				if (vertices.size() > indices_model[i].vertexIndex && normals_model.size()>indices_model[i].normalIndex)
				{
					aux_vertex.Normal = normals_model[indices_model[i].normalIndex];
					vertices[indices_model[i].vertexIndex].Normal = aux_vertex.Normal;
				}
				//vertices.insert(vertices.begin() + indices_model[i].vertexIndex, aux_vertex);
			}
			if (textures_size > 0)
			{
				//std::cout << indices_model[i].vertexIndex + 1 << "/";
				//std::cout << indices_model[i].uvIndex + 1 << "/normal" << '\n';
				if (vertices.size() > indices_model[i].vertexIndex && textures_model.size()> indices_model[i].uvIndex)
				{
					aux_vertex.TexCoords = textures_model[indices_model[i].uvIndex];
					vertices[indices_model[i].vertexIndex].TexCoords = aux_vertex.TexCoords;
				}
				//std::cout << "Textures:" << glm::to_string(vertices[indices_model[i].vertexIndex].TexCoords) << '\n';
				//vertices.insert(vertices.begin() + indices_model[i].vertexIndex, aux_vertex);
			}
			//indices_aux[i] = indices[0].vertexIndex;
			indices.push_back(indices_model[i].vertexIndex);
			//std::cout << indices[i] << '\n';
			//std::cout << indices[i] << " " << '\n';
			//std::cout << indices_model[i].uvIndex << " " << '\n';
		}
		//std::cout << "indices_size_model: " << indices.size() << '\n';
		/*for (int i = 0; i < vertices_size; i++)
		{
			std::cout << i << '\n';
			std::cout << "Position:" << glm::to_string(vertices[i].Position) << '\n';
			std::cout << "Normals:" << glm::to_string(vertices[i].Normal) << '\n';
			std::cout << "Textures:" << glm::to_string(vertices[i].TexCoords) << '\n';
		}*/

		Mesh *mesh = new Mesh();
		mesh->MeshCreate(vertices, indices);
		return mesh;

	}
	//void LoadObj(const std::string path) {

	//	//path variables
	//	std::string line;
	//	std::string uncommented_file = "";
	//	std::ifstream myfile(path);
	//	std::stringstream ss;
	//	std::string temp;
	//	float found;
	//	//vertex variables
	//	float vertex[3];
	//	unsigned int index[3];
	//	unsigned int found_unsigned;
	//	int v_i = 0;
	//	int i_index = 0;
	//	vector <glm::vec3> all_vertex;
	//	glm::vec3 aux_vertex;
	//	//end vertices variables
	//	vector <Vertex> vertices;
	//	Vertex aux_vertices;
	//	vector <unsigned int> indices;
	//	unsigned int aux_indices;

	//	//center
	//	glm::vec3 Centro;
	//	glm::vec3 minVertex;
	//	glm::vec3 maxVertex;
	//	float median_point;


	//	if (myfile.is_open())
	//	{
	//		while (std::getline(myfile, line))
	//		{
	//			// This is a comment
	//			std::size_t found = line.find('#');
	//			if (found != std::string::npos)
	//			{
	//				//std::cout << "This is a comment:" << line.substr(found + 1) << '\n';
	//				uncommented_file += line.substr(0, found) + '\n';
	//				//std::cout << line.substr(0, found) << '\n';
	//				//ignore rest
	//			}
	//			else
	//			{
	//				found = line.find('s');
	//				if (found != std::string::npos)
	//				{
	//					uncommented_file += line.substr(0, found) + '\n';
	//				}
	//				else
	//				{
	//					found = line.find('o');
	//					if (found != std::string::npos)
	//					{
	//						uncommented_file += line.substr(0, found) + '\n';
	//					}
	//					else
	//					{
	//						found = line.find('g');
	//						if (found != std::string::npos)
	//						{
	//							uncommented_file += line.substr(0, found) + '\n';
	//						}
	//						else
	//						{
	//							found = line.find('u');
	//							if (found != std::string::npos)
	//							{
	//								uncommented_file += line.substr(0, found) + '\n';
	//							}
	//							else 
	//							{
	//								found = line.find('vt');
	//								if (found != std::string::npos)
	//								{
	//									uncommented_file += line.substr(0, found) + '\n';
	//								}
	//								else 
	//								{
	//									found = line.find('m');
	//									if (found != std::string::npos)
	//									{
	//										uncommented_file += line.substr(0, found) + '\n';
	//									}
	//									else 
	//									{
	//											uncommented_file += line + '\n';
	//											//std::cout << line << '\n';
	//									}
	//								}
	//							}
	//						}
	//					}
	//				}
	//			}

	//		}
	//		myfile.close();
	//	}
	//	else std::cout << "Unable to open file";
	//	//std::cout << uncommented_file;
	//	ss << uncommented_file;
	//	std::size_t found_first = line.find('v');
	//	while (!ss.eof())
	//	{
	//		ss >> line;
	//		std::size_t found_first = line.find('v');
	//		if (found_first != std::string::npos)
	//		{

	//			while (v_i < 3) {
	//				ss >> line;
	//				if (std::stringstream(line) >> found) {
	//					//std::cout << " Else:" << line << '\n';
	//					//std::cout << v_i << " Else: found:" << found << " ";
	//					vertex[v_i] = found;
	//				}
	//				v_i++;
	//			}
	//			if (v_i == 3) {
	//				//GET VERTEX and put then in vector all_vertex
	//				aux_vertex = glm::vec3(vertex[0], vertex[1], vertex[2]);
	//				aux_vertices.Position = glm::vec3(vertex[0], vertex[1], vertex[2]);
	//				all_vertex.push_back(aux_vertex);
	//				vertices.push_back(aux_vertices);
	//				v_i = 0;
	//				//std::cout << '\n';
	//			}
	//		}	
	//		found_first = line.find('f');
	//		if (found_first != std::string::npos)
	//		{
	//			while (i_index < 3) {
	//				ss >> line;
	//				if (std::stringstream(line) >> found_unsigned) {
	//					//std::cout << found_unsigned;
	//					index[i_index] = found_unsigned - 1;
	//					//std::cout << index[i_index];
	//				}
	//				i_index++;
	//			}
	//			if (i_index == 3)
	//			{
	//				aux_indices = index[0];
	//				indices.push_back(aux_indices);
	//				aux_indices = index[1];
	//				indices.push_back(aux_indices);
	//				aux_indices = index[2];
	//				indices.push_back(aux_indices);
	//				i_index = 0;
	//			}
	//		}
	//		
	//	}

	//	maxVertex = glm::vec3(vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
	//	minVertex = glm::vec3(vertices[0].Position.x, vertices[0].Position.y, vertices[0].Position.z);
	//	for (int i = 0; i < vertices.size();i++)
	//	{
	//		maxVertex = glm::vec3(MAX(maxVertex.x, vertices[i].Position.x), MAX(maxVertex.y, vertices[i].Position.y), MAX(maxVertex.z, vertices[i].Position.z));
	//		minVertex = glm::vec3(MIN(minVertex.x, vertices[i].Position.x), MIN(minVertex.y, vertices[i].Position.y), MIN(minVertex.z, vertices[i].Position.z));

	//	}
	//	Centro = (maxVertex + minVertex) / glm::vec3(2);
	//	float median_pointMAX_MAX = MAX(maxVertex.z, MAX(maxVertex.x, maxVertex.y));
	//	float median_pointMAX_MIN = MAX(minVertex.z, MAX(minVertex.x, minVertex.y));
	//	median_point = ABS(MAX(median_pointMAX_MAX, median_pointMAX_MIN));
	//	/*for (int k = 0; k < vertices_trian.size(); k++)
	//	{
	//		aux_vertex_trian.Position = (vertices_trian[k].Position - Centro) / glm::vec3(median_point);
	//		vertices_trian[k] = aux_vertex_trian;
	//		std::cout << "vertices" << glm::to_string(vertices_trian[k].Position) << std::endl;
	//	}*/
	//	for (int k = 0; k < vertices.size(); k++)
	//	{
	//		aux_vertices.Position = (vertices[k].Position - Centro) / glm::vec3(median_point);
	//		vertices[k] = aux_vertices;
	//		//std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
	//	}
	//	Mesh* mesh = Mesh::Instance();
	//	mesh->MeshCreate(vertices, indices);

	//}

	Mesh* LoadOff(const std::string path) {
		//std::cout << "ich bin in off." << std::endl;
		vector <glm::vec3> all_vertex;
		vector <glm::vec3> face_triangles;
		vector <CG::triangle> all_triangles;

		CG::triangle aux_triangle;
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

		vector <glm::vec3> Face_normals;


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
								/*aux_triangle.v0 = all_vertex[faces_index[0]];
								aux_triangle.v1 = all_vertex[faces_index[k+1]];
								aux_triangle.v2 = all_vertex[faces_index[k+2]];*/

								//std::cout << glm::to_string(aux_triangle.v0) <<" "<< glm::to_string(aux_triangle.v1)<< " "<< glm::to_string(aux_triangle.v2)<< std::endl;
								//all_triangles.push_back(aux_triangle);
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


							/*aux_triangle.v0 = all_vertex[faces_index[0]];
							aux_triangle.v1 = all_vertex[faces_index[1]];
							aux_triangle.v2 = all_vertex[faces_index[2]];*/
							//std::cout << glm::to_string(aux_triangle.v0) << " " << glm::to_string(aux_triangle.v1) << " " << glm::to_string(aux_triangle.v2) << std::endl;
							//all_triangles.push_back(aux_triangle);
						}
						//std::cout <<'\n';
						std::getline(ss, line);
						f_count++;
					}
					else break;
						
				}
			}
			
		}
		//std::cout << number_of_vertices << " " <<number_of_faces << " " << number_of_edges <<'\n';
		//std::cout << indices.size();
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
			//std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}

		//create triangles:
		triangle auxtriang;
		for (int k = 0; k < indices.size(); k+=3)
		{
			auxtriang.v0 = vertices[indices[k]].Position;
			auxtriang.v1 = vertices[indices[k+1]].Position;
			auxtriang.v2 = vertices[indices[k+2]].Position;
			all_triangles.push_back(auxtriang);
			//std::cout << glm::to_string(auxtriang.v0) <<","<< glm::to_string(auxtriang.v1) << ","<< glm::to_string(auxtriang.v2) << '\n';
		}

		/*std::cout << "Max vertex: " << glm::to_string(maxVertex) << '\n';
		std::cout << "Min vertex: " << glm::to_string(minVertex) << '\n';
		std::cout << "Centro: " << glm::to_string(Centro) << '\n';
		std::cout << "median_point: " << median_point << '\n';
		std::cout << "triangle: "<< glm::to_string(aux_vertex_trian.Position) << '\n';
		for (int k = 0;k < vertices.size();k++)
		{
			std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}
		std::cout << vertices.size() << " ";
		std::cout << vertices_trian.size();*/

		//do normals
		glm::vec3 edge1, edge2;
		for (int k = 0; k < all_triangles.size(); k++)
		{
			//std::cout << "vertices_trian: " << glm::to_string(all_triangles[k].v0) << glm::to_string(all_triangles[k].v1) << " " << glm::to_string(all_triangles[k].v2) << std::endl;
			edge1 = all_triangles[k].v1 - all_triangles[k].v0;
			edge2 = all_triangles[k].v2 - all_triangles[k].v1;
			all_triangles[k].normal = glm::normalize(glm::cross(edge1,edge2));
			//std::cout << glm::to_string(all_triangles[k].normal) << '\n';
		}

		for (int k = 0; k < vertices.size(); k++)
		{
			vertices[k].Normal = all_triangles[k].normal;
			//std::cout << "vertices" << glm::to_string(vertices[k].Position) << std::endl;
		}


		Mesh* mesh = new Mesh();
		mesh->MeshCreate(vertices, indices);
		return mesh;
		/*Quad* quad = Quad::Instance();
		quad->min = glm::normalize(minVertex);
		quad->max = glm::normalize(maxVertex);*/
	}

}

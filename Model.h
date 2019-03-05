#pragma once


#include <iostream>
#include<vector>
#include<string>
#include<glm\glm\glm.hpp>
#include <fstream>
#include<sstream>


void load_obj(const char* filepath,std::vector<glm::vec4> &vertices,std::vector<glm::vec3> &normals,std::vector<int> &elements)
{
	std::ifstream read_model(filepath,std::ios::in);
	if (!read_model) {
		std::cout << "Cannot load model"<<std::endl;
		exit(1);
	}

	std::string line;
	while (getline(read_model,line)) {
		if (line.substr(0, 2) == "v ") {
			std::istringstream stream(line.substr(2));
			glm::vec4 v;
			stream >> v.x;
			stream >> v.y;
			stream >> v.z;
			v.w = 1.0f;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ") {
			std::istringstream stream(line.substr(2));
			int a, b, c;
			stream >> a;
			stream >> b;
			stream >> c;
			a--;
			b--;
			c--;
			elements.push_back(a);
			elements.push_back(b);
			elements.push_back(c);
		}
		else if (line[0] == '#') {
		}
		else
		{
		}
	}

	normals.resize(vertices.size(),glm::vec3(0.0,0.0,0.0));
	std::cout << "Normals";
	for (unsigned int i = 0;i < elements.size(); i+=3) {
		GLushort ia = elements[i];
		GLushort ib = elements[i+1];
		GLushort ic = elements[i+2];
		glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]) , glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;
	}
}




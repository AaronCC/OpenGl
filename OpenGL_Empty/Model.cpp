#include "Model.h"

Model::Model()
{
	vertArr = 0;
	locInds =
	{ 0, 1, 2,
		0, 2, 3 };
	vertCount = locInds.size();
	locs = {
		{ { 2, 0, 0 },{ 1,1 } },
		{ { 0, 0, 0 },{ 0,1 } },
		{ { 0, -2, 0 },{ 0,0 } },
		{ { 2, -2, 0 },{ 1,0 } }
	};
}


std::pair<std::vector<GameObject::Vertex>,GLuint> Model::genVertBufData(std::string objFileName)
{
	std::vector<GameObject::Vertex> vertBufData;
	std::vector<glm::vec3> locations;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<VertInd> vertInds;
	std::ifstream objFile(objFileName);
	std::string line, label;
	if (objFile.is_open())
	{
		while (getline(objFile, line))
		{
			std::stringstream objStream(line);
			objStream >> label;
			if (label == "v")
			{
				float x, y, z;
				objStream >> x >> y >> z;
				locations.push_back(glm::vec3(x, y, z));
			}
			else if (label == "vt")
			{
				float uv0, uv1;
				objStream >> uv0 >> uv1;
				uvs.push_back(glm::vec2(uv0, uv1));
			}
			else if (label == "vn")
			{
				float n0, n1, n2;
				objStream >> n0 >> n1 >> n2;
				normals.push_back(glm::vec3(n0, n1, n2));
			}
			else if (label == "f")
			{
				for (int i = 0; i < 3; i++)
				{
					GLuint u1, u2, u3;
					char c;
					objStream >> u1 >> c >> u2 >> c >> u3;
					u1--; u2--; u3--;
					vertInds.push_back(VertInd{ u1, u2, u3 });
				}
			}
		}
		objFile.close();
	}
	vertCount = vertInds.size();
	vertBufData.resize(vertCount);

	for (unsigned int i = 0; i < vertCount; i++)
	{
		vertBufData[i] = { locations[vertInds[i].locInd],
			uvs[vertInds[i].uvInd],
			normals[vertInds[i].normInd] };
	}
	return{ vertBufData, vertCount };
}

Model::~Model()
{
}

bool Model::buffer(std::vector<GameObject*> objects, std::string objFileName)
{
	for (int i = 0; i < objects.size(); i++)
	{
		glGenVertexArrays(1, &objects[i]->vertArr);
		glGenBuffers(1, &vertBuf);
		glBindVertexArray(objects[i]->vertArr);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
		glBufferData(GL_ARRAY_BUFFER,
			sizeof(GameObject::Vertex) * objects[i]->vertCount,
			&objects[i]->vertBufData[0],
			GL_STATIC_DRAW);

		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GameObject::Vertex),
			0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(
			1,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GameObject::Vertex),
			(void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(
			2,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(GameObject::Vertex),
			(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)));
		glEnableVertexAttribArray(2);
	}
	return true;
}

void Model::render(GameObject* obj)
{
	glBindVertexArray(obj->vertArr);
	glDrawArrays(GL_TRIANGLES, 0, obj->vertCount);
}

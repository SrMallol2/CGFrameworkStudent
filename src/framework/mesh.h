/*
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#pragma once

#include <vector>
#include "framework.h"
#include "camera.h"

class Mesh
{
public:
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> uvs;

	Mesh();
	void Clear();
	void Render(int primitive);

	void CreatePlane(float size);
	bool LoadOBJ(const char* filename);

	const std::vector<Vector3>& GetVertices() { return vertices; }
};

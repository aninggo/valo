// Copyright © 2015 Mikko Ronkainen <firstname@mikkoronkainen.com>
// License: MIT, see the LICENSE file.

#include <algorithm>
#include <chrono>
#include <fstream>
#include <stdexcept>

#include "tinyformat/tinyformat.h"

#include "Utils/ObjReader.h"
#include "App.h"
#include "Utils/Log.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Raytracing/Primitives/Triangle.h"

using namespace Raycer;

std::vector<Triangle> ObjReader::getTriangles(const std::string& objFileName)
{
	Log& log = App::getLog();

	log.logInfo("Parsing OBJ file (%s)", objFileName);

	auto startTime = std::chrono::high_resolution_clock::now();

	std::ifstream file(objFileName);

	if (!file.good())
		throw std::runtime_error("Could not open the OBJ file");

	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;
	std::vector<Triangle> triangles;

	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string part;
		ss >> part;

		// vertex
		if (part == "v")
		{
			Vector3 vertex;

			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;

			vertices.push_back(vertex);
			continue;
		}

		// texcoord
		if (part == "vt")
		{
			Vector2 texcoord;

			ss >> texcoord.x;
			ss >> texcoord.y;

			texcoords.push_back(texcoord);
			continue;
		}

		// normal
		if (part == "vn")
		{
			Vector3 normal;

			ss >> normal.x;
			ss >> normal.y;
			ss >> normal.z;

			normals.push_back(normal);
			continue;
		}

		// face (convex polygon)
		if (part == "f")
			processFace(ss, vertices, texcoords, normals, triangles);
	}

	file.close();

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	int milliseconds = (int)std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();

	log.logInfo("OBJ file parsing finished (time: %d ms, triangles: %d)", milliseconds, triangles.size());

	return triangles;
}

ObjReaderResult ObjReader::getMeshes(const std::string& objFileName, int idStartOffset)
{
	Log& log = App::getLog();

	log.logInfo("Parsing OBJ file (%s)", objFileName);

	auto startTime = std::chrono::high_resolution_clock::now();

	std::ifstream file(objFileName);

	if (!file.good())
		throw std::runtime_error("Could not open the OBJ file");

	Mesh mesh;
	ObjReaderResult result;
	std::map<std::string, int> materialMap;
	int currentId = idStartOffset;
	bool hasMesh = false;

	std::vector<Vector3> vertices;
	std::vector<Vector2> texcoords;
	std::vector<Vector3> normals;

	std::string line;

	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string part;
		ss >> part;

		// material file
		if (part == "mtllib")
		{
			ss >> part;
			processMaterialFile(part, result, materialMap, currentId);
			continue;
		}

		// new mesh group
		if (part == "g")
		{
			if (hasMesh)
				result.meshes.push_back(mesh);

			hasMesh = true;
			mesh = Mesh();
			continue;
		}

		// material name
		if (part == "usemtl")
		{
			ss >> part;

			if (!materialMap.count(part))
				throw std::runtime_error(tfm::format("Could not find material \"%s\"", part));

			mesh.materialId = materialMap[part];
			continue;
		}

		// vertex
		if (part == "v")
		{
			Vector3 vertex;

			ss >> vertex.x;
			ss >> vertex.y;
			ss >> vertex.z;

			vertices.push_back(vertex);
			continue;
		}

		// texcoord
		if (part == "vt")
		{
			Vector2 texcoord;

			ss >> texcoord.x;
			ss >> texcoord.y;

			texcoords.push_back(texcoord);
			continue;
		}

		// normal
		if (part == "vn")
		{
			Vector3 normal;

			ss >> normal.x;
			ss >> normal.y;
			ss >> normal.z;

			normals.push_back(normal);
			continue;
		}

		// face (convex polygon)
		if (part == "f")
			processFace(ss, vertices, texcoords, normals, mesh.triangles);
	}

	file.close();

	if (hasMesh)
		result.meshes.push_back(mesh);

	auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
	int milliseconds = (int)std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();

	log.logInfo("OBJ file parsing finished (time: %d ms)", milliseconds);

	return result;
}

void ObjReader::processMaterialFile(const std::string& mtlFileName, ObjReaderResult& result, std::map<std::string, int>& materialMap, int& currentId)
{
	(void)result;

	App::getLog().logInfo("Parsing MTL file (%s)", mtlFileName);

	std::ifstream file(mtlFileName);

	if (!file.good())
		throw std::runtime_error("Could not open the MTL file");

	Material material;
	bool hasMaterial = false;
	bool hasTexture = false;
	bool hasBumpMapTexture = false;

	std::string line;
	
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string part;
		ss >> part;

		// new material
		if (part == "newmtl")
		{
			if (hasMaterial)
			{
				result.materials.push_back(material);
				hasTexture = false;
				hasBumpMapTexture = false;
			}

			hasMaterial = true;

			material = Material();
			material.id = ++currentId;

			ss >> part;
			materialMap[part] = currentId;
			continue;
		}

		// ambient reflectance
		if (part == "Ka")
		{
			ss >> material.ambientReflectance.r;
			ss >> material.ambientReflectance.g;
			ss >> material.ambientReflectance.b;
			continue;
		}

		// diffuse reflectance
		if (part == "Kd")
		{
			ss >> material.diffuseReflectance.r;
			ss >> material.diffuseReflectance.g;
			ss >> material.diffuseReflectance.b;
			continue;
		}

		// specular reflectance
		if (part == "Ks")
		{
			ss >> material.specularReflectance.r;
			ss >> material.specularReflectance.g;
			ss >> material.specularReflectance.b;
			continue;
		}

		// shininess
		if (part == "Ns")
		{
			ss >> material.shininess;
			continue;
		}

		// refractive index
		if (part == "Ni")
		{
			ss >> material.refractiveIndex;
			continue;
		}

		if ((part == "map_Ka" || part == "map_Kd" || part == "map_Ks") && !hasTexture)
		{
			ImageTexture imageTexture;
			imageTexture.id = ++currentId;
			imageTexture.applyGamma = true;
			material.textureId = imageTexture.id;
			ss >> imageTexture.imageFilePath;
			hasTexture = true;
			result.imageTextures.push_back(imageTexture);

			continue;
		}

		if ((part == "map_bump" || part == "bump") && !hasBumpMapTexture)
		{
			ImageTexture imageTexture;
			imageTexture.id = ++currentId;
			imageTexture.applyGamma = true;
			material.bumpMapTextureId = imageTexture.id;
			ss >> imageTexture.imageFilePath;
			hasBumpMapTexture = true;
			result.imageTextures.push_back(imageTexture);

			continue;
		}
	}

	file.close();

	if (hasMaterial)
		result.materials.push_back(material);
}

void ObjReader::processFace(std::istringstream& ss, std::vector<Vector3>& vertices, std::vector<Vector2>& texcoords, std::vector<Vector3>& normals, std::vector<Triangle>& triangles)
{
	std::vector<int> vertexIndices;
	std::vector<int> texcoordIndices;
	std::vector<int> normalIndices;

	std::string part;
	ss >> part;

	// determine what indices are available from the slash count
	int slashCount = (int)std::count(part.begin(), part.end(), '/');
	bool doubleSlash = (part.find("//") != std::string::npos);
	bool hasTexcoord = (slashCount > 0 && !doubleSlash);
	bool hasNormal = (slashCount > 1);

	do
	{
		// stringstream likes spaces
		std::replace(part.begin(), part.end(), '/', ' ');
		std::istringstream ssp(part);

		int vertexIndex;
		ssp >> vertexIndex;
		
		if (vertexIndex < 0)
			vertexIndex = (int)vertices.size() + vertexIndex;
		else
			vertexIndex--;

		if (vertexIndex < 0 || vertexIndex >= (int)vertices.size())
			throw std::runtime_error("Vertex index was out of bounds");

		vertexIndices.push_back(vertexIndex);

		if (hasTexcoord)
		{
			int texcoordIndex;
			ssp >> texcoordIndex;
			
			if (texcoordIndex < 0)
				texcoordIndex = (int)texcoords.size() + texcoordIndex;
			else
				texcoordIndex--;

			if (texcoordIndex < 0 || texcoordIndex >= (int)texcoords.size())
				throw std::runtime_error("Texcoord index was out of bounds");

			texcoordIndices.push_back(texcoordIndex);
		}

		if (hasNormal)
		{
			int normalIndex;
			ssp >> normalIndex;

			if (normalIndex < 0)
				normalIndex = (int)normals.size() + normalIndex;
			else
				normalIndex--;

			if (normalIndex < 0 || normalIndex >= (int)normals.size())
				throw std::runtime_error("Normal index was out of bounds");

			normalIndices.push_back(normalIndex);
		}

	} while (ss >> part);

	if (vertexIndices.size() < 3)
		throw std::runtime_error("Too few vertices in a face");

	// triangulate
	for (int i = 2; i < (int)vertexIndices.size(); ++i)
	{
		Triangle triangle;

		triangle.vertices[0] = vertices[vertexIndices[0]];
		triangle.vertices[1] = vertices[vertexIndices[i - 1]];
		triangle.vertices[2] = vertices[vertexIndices[i]];

		if (hasTexcoord)
		{
			triangle.texcoords[0] = texcoords[texcoordIndices[0]];
			triangle.texcoords[1] = texcoords[texcoordIndices[i - 1]];
			triangle.texcoords[2] = texcoords[texcoordIndices[i]];
		}

		// calculate triangle normal CCW
		Vector3 v0tov1 = triangle.vertices[1] - triangle.vertices[0];
		Vector3 v0tov2 = triangle.vertices[2] - triangle.vertices[0];
		Vector3 normal = v0tov1.cross(v0tov2).normalized();

		triangle.normal = normal;

		if (hasNormal)
		{
			triangle.normals[0] = normals[normalIndices[0]];
			triangle.normals[1] = normals[normalIndices[i - 1]];
			triangle.normals[2] = normals[normalIndices[i]];
		}
		else
			triangle.normals[0] = triangle.normals[1] = triangle.normals[2] = normal;

		triangles.push_back(triangle);
	}
}

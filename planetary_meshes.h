#pragma once

#include <vector>
using std::vector;

#include <array>
using std::array;

#include <algorithm> 
using std::for_each;

#include <glm/vec3.hpp>
using glm::vec3;

// Possible errors:
// Barycenter calculation will fail if connectivity (3, 6(5)) is not fulfilled correctly.
// Vectors might not average as neatly as I thought.

// Possible changes:
// May want to calculate barycenters on the sphere are opposed to a euclidean norm.

struct TriangularVertex
{
	vec3 position;
	vector<unsigned> cellular_indices;

	TriangularVertex (vec3 position): position(position) {}
};

struct TriangularCell
{
	vector<unsigned> vertical_indices;

	TriangularCell (vector<unsigned> vertical_indices): vertical_indices(vertical_indices) {}
};

struct Triangulation
{
	vector<TriangularVertex> vertices;
	vector<TriangularCell> cells;

	vec3 find_cellular_barycenter (TriangularCell cell)
	{
		vec3 sum;

		for (auto vertical_index : cell.vertical_indices) sum += vertices[vertical_index].position;

		return sum / vec3(cell.vertical_indices.size());
	}
};

struct TessellarVertex
{
	vec3 position;
	vector<unsigned> cellular_indices;

	TessellarVertex (vec3 position, vector<unsigned> cellular_indices): position(position), cellular_indices(cellular_indices) {}
};

struct TessellarCell
{
	vector<unsigned> vertical_indices;

	TessellarCell (vector<unsigned> vertical_indices): vertical_indices(vertical_indices) {}
};

struct Tessellation
{
	vector<TessellarVertex> vertices;
	vector<TessellarCell> cells;

	vec3 find_cellular_barycenter (TessellarCell cell)
	{
		vec3 sum;

		for (auto vertical_index : cell.vertical_indices) sum += vertices[vertical_index].position;

		return sum / vec3(cell.vertical_indices.size());
	}
};

// TODO:
// align connectivity with cardinal planar direction - is this even possible?
Tessellation create_dual (Triangulation t)
{
	Tessellation s;

	// add new vertices, establish vertical connectivity from triangular barycenters
	for (auto triangle : t.cells)
	{
		s.vertices.push_back(TessellarVertex(t.find_cellular_barycenter(triangle), triangle.vertical_indices));
	}

	// add new cells from triangular vertices
	for (auto triangular_vertex : t.vertices)
	{
		s.cells.push_back(TessellarCell(triangular_vertex.cellular_indices));
	}

	return s;
}

// TODO: 
// align connectivity with cardinal planar direction - is this even possible?
Triangulation barycentrically_subdivide (Tessellation s)
{
	Triangulation t;

	// retain old vertices without connectivity
	for (auto tessellar_vertex : s.vertices)
	{
		t.vertices.push_back(TriangularVertex(tessellar_vertex.position));
	}

	// add new vertices, cells, establish vertical connectivity from tessellar barycenters
	for (auto tessella : s.cells)
	{
		t.vertices.push_back(TriangularVertex(s.find_cellular_barycenter(tessella)));

		const unsigned new_vertical_index = t.vertices.size() - 1;
		const unsigned num_tessellar_vertical_indices = tessella.vertical_indices.size();

		for (int i = 0; i < num_tessellar_vertical_indices; i++)
		{
			const unsigned neighboring_index_0 = tessella.vertical_indices[i];
			const unsigned neighboring_index_1 = tessella.vertical_indices[(i + 1) % num_tessellar_vertical_indices];

			t.cells.push_back(TriangularCell({new_vertical_index, neighboring_index_0, neighboring_index_1}));

			const unsigned new_cellular_index = t.cells.size() - 1;

			t.vertices[new_vertical_index].cellular_indices.push_back(new_cellular_index);
			t.vertices[neighboring_index_0].cellular_indices.push_back(new_cellular_index);
			t.vertices[neighboring_index_1].cellular_indices.push_back(new_cellular_index);
		}
	}

	return t;
}


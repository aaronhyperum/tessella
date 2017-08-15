#pragma once

#include "planetary_meshes.h"


#include "vertex.h"

// TODO:
// align connectivity with cardinal planar direction - is this even possible?
// also, will initial connectivital alignment automatically translate to connectivital alignment in subdivisions? may want to try it out on paper.
Tessellation create_truncated_icosahedron ()
{
	Triangulation icosahedron;

	// edges of a golden rectangle with radius 1
	const double X = .525731112119133606;
	const double Z = .850650808352039932;

	icosahedron.vertices = 
	{
		TriangularVertex(vec3(-X, 0.0, Z)),
		TriangularVertex(vec3(X, 0.0, Z)),
		TriangularVertex(vec3(-X, 0.0, -Z)),
		TriangularVertex(vec3(X, 0.0, -Z)),

		TriangularVertex(vec3(0.0, Z, X)),
		TriangularVertex(vec3(0.0, Z, -X)),
		TriangularVertex(vec3(0.0, -Z, X)),
		TriangularVertex(vec3(0.0, -Z, -X)),

		TriangularVertex(vec3(Z, X, 0.0)),
		TriangularVertex(vec3(-Z, X, 0.0)),
		TriangularVertex(vec3(Z, -X, 0.0)),
		TriangularVertex(vec3(-Z, -X, 0.0))
	};

	// alignment is counterclockwise, row-aligned with starting positions dependent on planar row parity

	// top row: 1, 0, 9, 6, 8 (pole = 4)
	// bot row: 6, 11, 2, 3, 10 (pole = 7)

	icosahedron.cells =
	{
		TriangularCell({1, 0, 4}),
		TriangularCell({0, 9, 4}),
		TriangularCell({9, 5, 4}),
		TriangularCell({5, 8, 4}),
		TriangularCell({8, 1, 4}),

		TriangularCell({0, 1, 6}),
		TriangularCell({9, 0, 11}),
		TriangularCell({5, 9, 2}),
		TriangularCell({8, 5, 3}),
		TriangularCell({1, 8, 10}),

		TriangularCell({6, 11, 0}),
		TriangularCell({11, 2, 9}),
		TriangularCell({2, 3, 5}),
		TriangularCell({3, 10, 8}),
		TriangularCell({10, 6, 1}),

		TriangularCell({11, 6, 7}),
		TriangularCell({2, 11, 7}),
		TriangularCell({3, 2, 7}),
		TriangularCell({10, 3, 7}),
		TriangularCell({6, 10, 7}),
	};

	// might give connectivity meaningful alignment, requiring manual placement. shouldn't be an issue, so we'll set up manual first.
	// 1 = vertical.
	// 0, 2 = respective counterclockwise 

	icosahedron.vertices[0].cellular_indices = {1, 0, 5, 10, 6};
	icosahedron.vertices[1].cellular_indices = {0, 4, 9, 14, 5};
	icosahedron.vertices[2].cellular_indices = {12, 7, 11, 16, 17};
	icosahedron.vertices[3].cellular_indices = {13, 8, 12, 17, 18};
	icosahedron.vertices[4].cellular_indices = {2, 3, 4, 0, 1};
	icosahedron.vertices[5].cellular_indices = {3, 2, 7, 12, 8};

	icosahedron.vertices[6].cellular_indices = {10, 5, 14, 19, 15};
	icosahedron.vertices[7].cellular_indices = {15, 19, 18, 17, 16};
	icosahedron.vertices[8].cellular_indices = {4, 3, 8, 13, 9};
	icosahedron.vertices[9].cellular_indices = {2, 1, 6, 11, 7};
	icosahedron.vertices[10].cellular_indices = {14, 9, 13, 18, 19};
	icosahedron.vertices[11].cellular_indices = {11, 6, 10, 15, 16};

	return create_dual(icosahedron);
}

Tessellation a3h_subdivide (Tessellation h)
{
	return create_dual(barycentrically_subdivide(h));
}


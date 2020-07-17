#ifndef MESH_UTILS_H
#define MESH_UTILS_H

#define CGAL_MESH_2_OPTIMIZER_VERBOSE

#include <vector>
#include <iostream>
#include <gmp.h>
#include <gmpxx.h>

#include <glm/glm.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesher_2.h>
#include <CGAL/Delaunay_mesh_vertex_base_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/lloyd_optimize_mesh_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
//typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Exact_predicates_tag EPT;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds, EPT> CDT;
typedef CGAL::Delaunay_mesh_size_criteria_2<CDT> Criteria;
typedef CGAL::Delaunay_mesher_2<CDT, Criteria> Mesher;

typedef CDT::Point Point;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CDT::Face_handle Face_handle;


std::vector<glm::vec3> meshCreator(std::vector<glm::vec3> input);
glm::vec3 performZLifting(glm::vec3 p, std::vector<glm::vec3> boundary);
#endif // MESH_UTILS_H

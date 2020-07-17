#include "mesh_utils.h"
#include <limits>
#include <cmath>
#define EPSILON 1e-6


std::vector<glm::vec3> meshCreator(std::vector<glm::vec3> input)
{
      std::vector<glm::vec3> vectorarr;

      // Create constraint polygon
      Polygon_2 cpoly;
      for (unsigned long i=0; i < input.size(); i++)
      {
          cpoly.push_back(Point(input[i].x, input[i].y));
      }

      // Create mesh inside the polygon area
      CDT cdt;
      cdt.insert_constraint(cpoly.vertices_begin(), cpoly.vertices_end(), true);
      assert(cdt.is_valid());
      Mesher mesher(cdt);
      mesher.refine_mesh();
      CGAL::lloyd_optimize_mesh_2(cdt,
                                  CGAL::parameters::max_iteration_number = 30);

      for(CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end();it++)
      {
          if (it->is_in_domain())
              for(int i=0;i<3;i++)
              {
                  Point v1 = it->vertex(i)->point();
                  glm::vec3 v1_3d = performZLifting(glm::vec3(v1.x(), v1.y(), 0.0f), input);
                  Point v2 = it->vertex((i+1)%3)->point();
                  glm::vec3 v2_3d = performZLifting(glm::vec3(v2.x(), v2.y(), 0.0f), input);

                  vectorarr.push_back(v1_3d);
                  vectorarr.push_back(v2_3d);
                  // For symmetry
                  vectorarr.push_back(v1_3d*glm::vec3(1.0f,1.0f,-1.0f));
                  vectorarr.push_back(v2_3d*glm::vec3(1.0f,1.0f,-1.0f));
              }
      }

      return vectorarr;
}

glm::vec3 performZLifting(glm::vec3 p, std::vector<glm::vec3> boundary)
{
    // Find the closest distance of each point to boundary
    float minDist = std::numeric_limits<float>::max();
    for (unsigned int i=0;i<boundary.size();i++)
    {
        float dist = minDist;
        glm::vec3 b1 = boundary[i];
        glm::vec3 b2 = boundary[(i+1)%boundary.size()]; //For handling loop
        if ( glm::all(glm::lessThan(glm::abs(b1-p), glm::vec3(EPSILON))) || glm::all(glm::lessThan(glm::abs(b1-p), glm::vec3(EPSILON))) )
            dist = 0.0f;
        else if ( glm::acos(glm::dot((p-b1)/glm::length(p-b1), (b2-b1)/glm::length(b2-b1))) > M_PI/2.0f )
            dist = glm::length(p-b1);
        else if ( glm::acos(glm::dot((p-b2)/glm::length(p-b2), (b1-b2)/glm::length(b1-b2))) > M_PI/2.0f )
            dist = glm::length(p-b2);
        else
            dist = glm::length( glm::cross(b1-b2, b1-p) ) / glm::length(b2-b1);

        if (dist < minDist)
            minDist = dist;
    }

    // Compute z-coordinate as per the nearest distance
    float z = powf(minDist,0.9f);
    return glm::vec3(p.x, p.y, z);
}

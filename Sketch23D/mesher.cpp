#include "mesher.h"
#define BUF 10000

std::vector<float> meshCreator(std::vector<float> input, int w2, int h2)
{
      int count=0,len;
      std::vector<float> vectorarr;
      CDT cdt;
      Vertex_handle vhand[BUF];

      for (int i = 0; i < (input.size())/2 ; ++i)
      {
          vhand[i] = cdt.insert(Point(input[i*2],input[i*2+1]));
          len = i;
      }

      for (int i = 0; i < len-1; ++i)
      {
          cdt.insert_constraint(vhand[i],vhand[i+1]);
      }
      cdt.insert_constraint(vhand[len-1], vhand[0]);

      //cdt.insert(Point(2, 0.6));

      std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
      std::cout << "Meshing the triangulation with default criterias..."
                << std::endl;
      Mesher mesher(cdt);
      //mesher.refine_mesh();
      //std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
      //std::cout << "Meshing with new criterias..." << std::endl;
      // 0.125 is the default shape bound. It corresponds to abound 20.6 degree.
      // 0.5 is the upper bound on the length of the longuest edge.
      // See reference manual for Delaunay_mesh_size_traits_2<K>.
      mesher.set_criteria(Criteria(0.125, 10));
      mesher.refine_mesh();
      for(CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end();it++)
          {
            for(int i=0;i<len-1;i++){
                float coordx = ((cdt.triangle(it)[i].x()/w2)-1)*0.5f;
                vectorarr.push_back(coordx);
                float coordy = (1-(cdt.triangle(it)[i].x()/h2))*0.5f;
                vectorarr.push_back(coordy);}

                std::cout << vectorarr[count] << " " << vectorarr[count] << std::endl;
              count++;
          }

      std::cout << "Number of vertices: " << cdt.number_of_vertices() << " " << count <<std::endl;

      return vectorarr;
}

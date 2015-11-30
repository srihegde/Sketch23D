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
      std::cout << "Meshing the triangulation with default criterias..."<< std::endl;
      Mesher mesher(cdt);
      //mesher.refine_mesh();
      //std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
      //std::cout << "Meshing with new criterias..." << std::endl;
      // 0.125 is the default shape bound. It corresponds to abound 20.6 degree.
      // 0.5 is the upper bound on the length of the longuest edge.
      // See reference manual for Delaunay_mesh_size_traits_2<K>.
      mesher.set_criteria(Criteria(0.125, 10));
      mesher.refine_mesh();
      std::vector<float> vectorcir;
      for(CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end();it++)
      {
          //3d
          float coordcX = (cdt.circumcenter(it).x())/4.0;
          float coordcY = (cdt.circumcenter(it).y())/4.0;
          float z=0.0;
          vectorcir.push_back((cdt.circumcenter(it).x())/4.0);
          vectorcir.push_back((cdt.circumcenter(it).y())/4.0);
          //-------
        for(int i=0;i<len-1;i++)
        {
            float coordx = (cdt.triangle(it)[i].x()/*/w2*/);
            vectorarr.push_back(coordx);
            float coordy = (cdt.triangle(it)[i].y()/*/h2*/);
            vectorarr.push_back(coordy);
            z = sqrt(pow((coordcX-coordx),2.0) + pow((coordcY - coordy),2.0));
            vectorarr.push_back(z);
        }
        for (int i = len-1; i >= 0 ; i--)
        {
            float coordx = (cdt.triangle(it)[i].x()/*/w2*/);
            vectorarr.push_back(coordx);
            float coordy = (cdt.triangle(it)[i].y()/*/h2*/);
            vectorarr.push_back(coordy);
            z = sqrt(pow((coordcX-coordx),2.0) + pow((coordcY - coordy),2.0));
            vectorarr.push_back(-z);
        }

//        vectorarr = meshAverager(vectorarr);
//           std::cout << vectorarr[count] << " " << vectorarr[count] << std::endl;
          count++;
      }

//      std::cout << "Number of vertices: " << cdt.number_of_vertices() << " " << count <<std::endl;

//      for (int i = 0; i < vectorarr.size(); i+=3)
//          std::cout << vectorarr[i]<<" "<<vectorarr[i+1]<<" "<<vectorarr[i+2]<<"\n";
      return vectorarr;
}


//std::vector<float> meshCreator(std::vector<float> input, int w2, int h2)
//{
//    std::vector<float> vectorarr;
//    int count=0;
//    CDT cdt;
//    Vertex_handle va = cdt.insert(Point(-5,0));
//    Vertex_handle vb = cdt.insert(Point(0,-5));
//    Vertex_handle vc = cdt.insert(Point(5,0));
//    Vertex_handle vd = cdt.insert(Point(0,5));
//    //cdt.insert(Point(2, 0.6));
//    cdt.insert_constraint(va, vb);
//    cdt.insert_constraint(vb, vc);
//    cdt.insert_constraint(vc, vd);
//    cdt.insert_constraint(vd, va);
//    std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
//    std::cout << "Meshing the triangulation with default criterias..."
//              << std::endl;
//    Mesher mesher(cdt);
//    //mesher.refine_mesh();
//    //std::cout << "Number of vertices: " << cdt.number_of_vertices() << std::endl;
//    //std::cout << "Meshing with new criterias..." << std::endl;
//    // 0.125 is the default shape bound. It corresponds to abound 20.6 degree.
//    // 1.0 is the upper bound on the length of the longest edge.
//    // See reference manual for Delaunay_mesh_size_traits_2<K>.
//    mesher.set_criteria(Criteria(0.125, 5));
//    mesher.refine_mesh();
//    std::vector<float> vectorcir;
//    for(CDT::Finite_faces_iterator it = cdt.finite_faces_begin(); it != cdt.finite_faces_end();it++)
//        {
//        float coordcX = (cdt.circumcenter(it).x())/4.0;
//                  float coordcY = (cdt.circumcenter(it).y())/4.0;
//                  float z=0.0;
//                  vectorcir.push_back((cdt.circumcenter(it).x())/4.0);
//                  vectorcir.push_back((cdt.circumcenter(it).y())/4.0);
//            for(int i=0;i<3;i++){
//              float coordx = (cdt.triangle(it)[i].x()/*/w2*/);
//                          vectorarr.push_back(coordx);
//                          float coordy = (cdt.triangle(it)[i].y()/*/h2*/);
//                          vectorarr.push_back(coordy);
//                          z = sqrt(pow((coordcX-coordx),2.0) + pow((coordcY - coordy),2.0));
//                          vectorarr.push_back(z);
//                          vectorarr.push_back(coordx);
//                          vectorarr.push_back(coordy);
//                          vectorarr.push_back(-z);}

////              std::cout << vectorarr[count] << " " << vectorarr[count] << std::endl;
//            count++;
//  //            std::cout << cdt.triangle(it)[0].x() << " " << cdt.triangle(it)[0].y() << std::endl;
//  //          std::cout << cdt.triangle(it)[1].x() << " " << cdt.triangle(it)[1].y() << std::endl;
//  //          std::cout << cdt.triangle(it)[2].x() << " " << cdt.triangle(it)[2].y() << std::endl;
//        }

////    std::cout << "Number of vertices: " << cdt.number_of_vertices() << " " << count <<std::endl;
//    return vectorarr;
//}


std::vector<float> meshAverager(std::vector<float> input)
{
    float threshold = 30.0f;
    float avgpos, avgneg, sumpos=0, sumneg=0;
    std::vector<float> newvect;

    for (int i = 2; i < input.size(); i+=3) {
        if(input[i] >= 0)
            sumpos += input[i];
        else
            sumneg += input[i];
    }

    avgpos = 2*sumpos/input.size();
    avgneg = 2*sumneg/input.size();

    for (int i = 0; i < input.size(); i+=3) {
        newvect.push_back(input[i]);
        newvect.push_back(input[i+1]);
        if(input[i+2] > 0)
        {
            if(fabs(input[i+2] - avgpos) > threshold)
                newvect.push_back(avgpos);
            else
                newvect.push_back(input[i+2]);
        }
        else
        {
            if(fabs(input[i+2] - avgneg) > threshold)
                newvect.push_back(avgneg);
            else
                newvect.push_back(input[i+2]);
        }
    }

    return newvect;

}

// model_format_conversion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vcg/complex/complex.h>

#include "wrap/io_trimesh/import.h">"
#include "wrap/io_trimesh/export_ply.h"
#include "wrap/io_trimesh/export_obj.h"
#include "wrap/io_trimesh/export_stl.h"

#include <vcg/complex/algorithms/refine.h>
#include <vcg/complex/algorithms/refine_loop.h>


//class CFace;
//class CVertex;
//struct UsedTypes :public vcg::UsedTypes< vcg::Use<CFace>::AsFaceType, vcg::Use<CVertex>::AsVertexType> {};
//class CVertex : public vcg::Vertex<UsedTypes, vcg::vertex::Coord3d, vcg::vertex::Qualityf, vcg::vertex::Normal3d, vcg::vertex::Color4b, vcg::vertex::BitFlags> {};
//class CFace : public vcg::Face< UsedTypes, vcg::face::VertexRef, vcg::face::Normal3d, vcg::face::Color4b, vcg::face::Mark, vcg::face::BitFlags> {};
//class CMesh : public vcg::tri::TriMesh< std::vector<CVertex>, std::vector<CFace> > {};
//

class MyEdge;    // dummy prototype never used
class MyFace;
class MyVertex;

struct MyUsedTypes : public vcg::UsedTypes<	vcg::Use<MyVertex>::AsVertexType,
    vcg::Use<MyFace>::AsFaceType> {};

class MyVertex : public vcg::Vertex< MyUsedTypes, vcg::vertex::Coord3f, vcg::vertex::Normal3f, vcg::vertex::BitFlags  > {};
class MyFace : public vcg::Face  < MyUsedTypes, vcg::face::InfoOcf, vcg::face::FFAdjOcf, vcg::face::VertexRef, vcg::face::BitFlags > {};
class MyMesh : public vcg::tri::TriMesh< std::vector<MyVertex>, vcg::face::vector_ocf<MyFace> > {};

int main(int argc, char* argv[])
{

    std::string fullfilename = argv[1];

    MyMesh mesh;

    std::cout << "load mesh " << fullfilename << std::endl;
    int result = vcg::tri::io::ImporterPLY<MyMesh>::Open(mesh, fullfilename.c_str(), 0);

    mesh.face.EnableFFAdjacency();
    vcg::tri::UpdateTopology<MyMesh>::FaceFace(mesh);
    vcg::tri::UpdateFlags<MyMesh>::FaceBorderFromFF(mesh);
    vcg::tri::UpdateNormal<MyMesh>::PerVertexNormalized(mesh);
    printf("Input mesh  vn:%i fn:%i\n", mesh.VN(), mesh.FN());

    int iter = 2;

    for (int i = 0; i < iter; ++i)
    {
        vcg::tri::RefineOddEven<MyMesh, vcg::tri::OddPointLoop<MyMesh>, vcg::tri::EvenPointLoop<MyMesh> >(mesh, vcg::tri::OddPointLoop<MyMesh>(mesh), vcg::tri::EvenPointLoop<MyMesh>(), 0);

        printf("Input mesh  vn:%i fn:%i\n", mesh.VN(), mesh.FN());
    }


    std::string plyfilename = argv[2];
    std::cout << "write file: " << plyfilename << std::endl;
    result = vcg::tri::io::ExporterOBJ<MyMesh>::Save(mesh, plyfilename.c_str(), 0);

    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

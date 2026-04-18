#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/writeOBJ.h>
#include <directional/cut_mesh_with_singularities.h>
#include <directional/read_singularities.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <singularities.file> <output.mesh>\n";
    std::cout << "  input.mesh      - Input mesh file (OBJ format)\n";
    std::cout << "  singularities - Singularity file (N numElements idx ...)\n";
    std::cout << "  output.mesh    - Output cut mesh file (OBJ format)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string sing_file = argv[2];
    std::string output_file = argv[3];

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    int N;
    Eigen::VectorXi singElements, singIndices;
    directional::read_singularities(sing_file, N, singElements, singIndices);

    Eigen::MatrixXi face2cut;
    directional::cut_mesh_with_singularities(mesh, singElements, face2cut);

    directional::writeOBJ(output_file, mesh);

    std::cout << "Mesh cut with " << singElements.size() << " singularities\n";
    std::cout << "Done!\n";
    return 0;
}
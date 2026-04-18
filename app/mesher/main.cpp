#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/mesher.h>
#include <directional/setup_mesher.h>
#include <directional/writeOBJ.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <output.mesh> [scale]\n";
    std::cout << "  input.mesh  - Input mesh file (OBJ format)\n";
    std::cout << "  output.mesh - Output mesh file (OBJ format)\n";
    std::cout << "  scale      - Scale factor (default: 1.0)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string output_file = argv[2];
    double scale = 1.0;

    if (argc >= 4) scale = std::stod(argv[3]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    Eigen::MatrixXd scaledV = mesh.V;
    for (int i = 0; i < scaledV.rows(); ++i) {
        scaledV.row(i) *= scale;
    }

    directional::TriMesh outputMesh;
    outputMesh.V = scaledV;
    outputMesh.F = mesh.F;

    directional::writeOBJ(output_file, outputMesh);

    std::cout << "Mesh written: " << outputMesh.V.rows() << " vertices, " << outputMesh.F.rows() << " faces\n";
    std::cout << "Done!\n";
    return 0;
}
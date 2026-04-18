#include <iostream>
#include <string>
#include <fstream>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/isolines.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <function.txt> <num.isolines> <output.obj>\n";
    std::cout << "  input.mesh      - Input mesh file (OBJ format)\n";
    std::cout << "  function.txt  - File with one scalar value per vertex\n";
    std::cout << "  num.isolines  - Number of isolines to generate\n";
    std::cout << "  output.obj    - Output isolines OBJ file\n";
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string func_file = argv[2];
    int numIso = std::stoi(argv[3]);
    std::string output_file = argv[4];

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    Eigen::VectorXd z(mesh.V.rows());
    std::ifstream f(func_file);
    for (int i = 0; i < mesh.V.rows(); ++i) {
        f >> z(i);
    }
    f.close();

    Eigen::MatrixXd isoV;
    Eigen::MatrixXi isoE, isoOrigE;
    Eigen::MatrixXd isoN;

    directional::isolines(mesh.V, mesh.F, z, numIso, isoV, isoE, isoOrigE, isoN);

    std::ofstream out(output_file);
    out << "o isolines\n";
    for (int i = 0; i < isoV.rows(); ++i) {
        out << "v " << isoV.row(i) << "\n";
    }
    for (int i = 0; i < isoE.rows(); ++i) {
        out << "l " << isoE(i, 0) + 1 << " " << isoE(i, 1) + 1 << "\n";
    }
    out.close();

    std::cout << "Isolines written: " << isoV.rows() << " vertices, " << isoE.rows() << " edges\n";
    std::cout << "Done!\n";
    return 0;
}
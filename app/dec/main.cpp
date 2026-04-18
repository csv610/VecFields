#include <iostream>
#include <string>
#include <fstream>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/discrete_exterior_calculus.h>
#include <directional/mass_matrices.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <operation> <output.mtx>\n";
    std::cout << "  input.mesh   - Input mesh file (OBJ format)\n";
    std::cout << "  operation   - d0, d1, or mass\n";
    std::cout << "  output.mtx  - Output matrix in MatrixMarket format\n";
}

template<typename NumberType>
void write_sparse_matrix(const std::string& filename, const Eigen::SparseMatrix<NumberType>& mat) {
    std::ofstream f(filename);
    f << "%%MatrixMarket matrix coordinate real general\n";
    f << mat.rows() << " " << mat.cols() << " " << mat.nonZeros() << "\n";
    for (int k = 0; k < mat.outerSize(); ++k) {
        for (typename Eigen::SparseMatrix<NumberType>::InnerIterator it(mat, k); it; ++it) {
            f << it.row() + 1 << " " << it.col() + 1 << " " << it.value() << "\n";
        }
    }
    f.close();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string op = argv[2];
    std::string output_file = argv[3];

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    if (op == "d0") {
        auto d0 = directional::d0_matrix<double>(mesh);
        write_sparse_matrix(output_file, d0);
        std::cout << "d0 matrix written: " << d0.rows() << "x" << d0.cols() << "\n";
    } else if (op == "d1") {
        auto d1 = directional::d1_matrix<double>(mesh);
        write_sparse_matrix(output_file, d1);
        std::cout << "d1 matrix written: " << d1.rows() << "x" << d1.cols() << "\n";
    } else if (op == "mass") {
        auto M0 = directional::vertex_mass_matrix<double>(mesh);
        write_sparse_matrix(output_file, M0);
        std::cout << "Mass matrix written: " << M0.rows() << "x" << M0.cols() << "\n";
    } else {
        std::cerr << "Unknown operation: " << op << "\n";
        return 1;
    }

    std::cout << "Done!\n";
    return 0;
}
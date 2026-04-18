#include <iostream>
#include <string>
#include <fstream>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/mass_matrices.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <matrix_type> <output.mtx> [dim]\n";
    std::cout << "  input.mesh   - Input mesh file (OBJ format)\n";
    std::cout << "  matrix_type  - conf, lumped, vertex, or face\n";
    std::cout << "  output.mtx  - Output mass matrix\n";
    std::cout << "  dim        - dimension (default: 1)\n";
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
    std::string mat_type = argv[2];
    std::string output_file = argv[3];
    int dim = 1;

    if (argc >= 5) dim = std::stoi(argv[4]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    if (mat_type == "conf") {
        auto M = directional::conf_mass_matrix_2D<double>(mesh, dim);
        write_sparse_matrix(output_file, M);
        std::cout << "Conforming mass matrix written: " << M.rows() << "x" << M.cols() << "\n";
    } else if (mat_type == "lumped") {
        auto M = directional::lumped_voronoi_mass_matrix_2D<double>(mesh, dim);
        write_sparse_matrix(output_file, M);
        std::cout << "Lumped mass matrix written: " << M.rows() << "x" << M.cols() << "\n";
    } else if (mat_type == "vertex") {
        auto M = directional::vertex_mass_matrix<double>(mesh);
        write_sparse_matrix(output_file, M);
        std::cout << "Vertex mass matrix written: " << M.rows() << "x" << M.cols() << "\n";
    } else if (mat_type == "face") {
        auto M = directional::face_mass_matrix<double>(mesh);
        write_sparse_matrix(output_file, M);
        std::cout << "Face mass matrix written: " << M.rows() << "x" << M.cols() << "\n";
    } else {
        std::cerr << "Unknown matrix type: " << mat_type << "\n";
        return 1;
    }

    std::cout << "Done!\n";
    return 0;
}
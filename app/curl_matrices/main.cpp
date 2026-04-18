#include <iostream>
#include <string>
#include <fstream>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/curl_matrices.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <output.mtx> [intrinsic] [N]\n";
    std::cout << "  input.mesh   - Input mesh file (OBJ format)\n";
    std::cout << "  output.mtx  - Output curl matrix\n";
    std::cout << "  intrinsic   - true/false (default: false)\n";
    std::cout << "  N           - order (default: 1)\n";
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
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string output_file = argv[2];
    bool intrinsic = false;
    int N = 1;

    if (argc >= 4) intrinsic = (std::string(argv[3]) == "true");
    if (argc >= 5) N = std::stoi(argv[4]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    auto C = directional::curl_matrix_2D<double>(mesh, intrinsic, N);
    write_sparse_matrix(output_file, C);

    std::cout << "Curl matrix written: " << C.rows() << "x" << C.cols() << "\n";
    std::cout << "Done!\n";
    return 0;
}
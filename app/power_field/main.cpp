#include <iostream>
#include <string>
#include <cassert>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/power_field.h>
#include <directional/power_to_raw.h>
#include <directional/write_raw_field.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.obj> <output.field> <N> [normalize]\n";
    std::cout << "  input.obj    - Input mesh file (OBJ format)\n";
    std::cout << "  output.field - Output raw field file\n";
    std::cout << "  N            - Order of the power field (e.g., 4 for 4-directional)\n";
    std::cout << "  normalize    - Optional: normalize field (true/false, default: false)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string input_file = argv[1];
    std::string output_file = argv[2];
    int N = std::stoi(argv[3]);
    bool normalize = false;

    if (argc >= 5) {
        normalize = (std::string(argv[4]) == "true");
    }

    std::cout << "Loading mesh from: " << input_file << "\n";

    directional::TriMesh mesh;
    directional::readOBJ(input_file, mesh);

    std::cout << "Mesh loaded: " << mesh.V.rows() << " vertices, " << mesh.F.rows() << " faces\n";

    directional::IntrinsicVertexTangentBundle vtb;
    vtb.init(mesh);

    directional::CartesianField powerField;
    powerField.init(vtb, directional::fieldTypeEnum::POWER_FIELD, N);

    directional::CartesianField rawField;

    Eigen::VectorXi constVertices;
    Eigen::MatrixXd constVectors;
    Eigen::VectorXd alignWeights;

    directional::power_field(vtb, constVertices, constVectors, alignWeights, N, powerField, normalize);

    directional::power_to_raw(powerField, N, rawField);

    std::cout << "Writing raw field to: " << output_file << "\n";
    directional::write_raw_field(output_file, rawField);

    std::cout << "Done!\n";

    return 0;
}
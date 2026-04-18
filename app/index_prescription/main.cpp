#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/read_singularities.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/index_prescription.h>
#include <directional/write_raw_field.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <singularities.file> <output.field> <N> <globalRotation>\n";
    std::cout << "  input.mesh       - Input mesh file (OBJ format)\n";
    std::cout << "  input.field    - Input raw field file\n";
    std::cout << "  singularities  - Singularity file (N numElements idx ...)\n";
    std::cout << "  output.field   - Output field file\n";
    std::cout << "  N              - Field order\n";
    std::cout << "  globalRotation - Global rotation (radians)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 7) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string input_file = argv[2];
    std::string sing_file = argv[3];
    std::string output_file = argv[4];
    int N = std::stoi(argv[5]);
    double globalRotation = std::stod(argv[6]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    directional::IntrinsicVertexTangentBundle vtb;
    vtb.init(mesh);

    directional::CartesianField field;
    field.init(vtb, directional::fieldTypeEnum::RAW_FIELD, N);
    directional::read_raw_field(input_file, field);

    int singN;
    Eigen::VectorXi singElements, singIndices;
    directional::read_singularities(sing_file, singN, singElements, singIndices);

    Eigen::VectorXi cycleIndices = Eigen::VectorXi::Zero(vtb.cycles.cols());
    for (int i = 0; i < singElements.size(); ++i) {
        int elem = singElements(i);
        int idx = singIndices(i);
        if (elem >= 0 && elem < cycleIndices.size()) {
            cycleIndices(elem) = idx;
        }
    }

    Eigen::SimplicialLDLT<Eigen::SparseMatrix<double>> ldltSolver;
    Eigen::VectorXd rotationAngles;
    double linfError;

    directional::index_prescription(cycleIndices, N, globalRotation, ldltSolver, field, rotationAngles, linfError);

    std::cout << "Index prescription error: " << linfError << "\n";

    directional::write_raw_field(output_file, field);

    std::cout << "Done!\n";
    return 0;
}
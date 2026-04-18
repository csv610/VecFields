#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/raw_to_polyvector.h>
#include <directional/write_raw_field.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <output.field> <N>\n";
    std::cout << "  input.mesh    - Input mesh file (OBJ format)\n";
    std::cout << "  input.field - Input raw field file\n";
    std::cout << "  output.field - Output polyvector field file\n";
    std::cout << "  N           - Target order N\n";
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];
    int N = std::stoi(argv[4]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    directional::IntrinsicVertexTangentBundle vtb;
    vtb.init(mesh);

    directional::CartesianField rawField;
    rawField.init(vtb, directional::fieldTypeEnum::RAW_FIELD, 1);
    directional::read_raw_field(input_file, rawField);

    directional::CartesianField pvField;
    pvField.init(vtb, directional::fieldTypeEnum::POLYVECTOR_FIELD, N);

    directional::raw_to_polyvector(rawField, N, pvField);

    directional::CartesianField outputField;
    outputField.init(vtb, directional::fieldTypeEnum::POLYVECTOR_FIELD, N);
    outputField.copy_from(pvField);

    directional::write_raw_field(output_file, outputField);

    std::cout << "Done!\n";
    return 0;
}
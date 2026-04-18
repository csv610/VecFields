#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/power_to_raw.h>
#include <directional/write_raw_field.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <output.field> <N>\n";
    std::cout << "  input.mesh    - Input mesh file (OBJ format)\n";
    std::cout << "  input.field - Input power field file\n";
    std::cout << "  output.field - Output raw field file\n";
    std::cout << "  N           - Order N\n";
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

    directional::CartesianField powerField;
    powerField.init(vtb, directional::fieldTypeEnum::POWER_FIELD, N);
    directional::read_raw_field(input_file, powerField);

    directional::CartesianField rawField;
    directional::power_to_raw(powerField, N, rawField);

    directional::write_raw_field(output_file, rawField);

    std::cout << "Done!\n";
    return 0;
}
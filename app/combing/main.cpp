#include <iostream>
#include <string>
#include <cassert>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/combing.h>
#include <directional/principal_matching.h>
#include <directional/write_raw_field.h>
#include <directional/write_matching.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <output.field> [output.matching]\n";
    std::cout << "  input.mesh       - Input mesh file (OBJ format)\n";
    std::cout << "  input.field     - Input raw field file\n";
    std::cout << "  output.field    - Output combed raw field file\n";
    std::cout << "  output.matching - Optional: output matching file\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string input_field_file = argv[2];
    std::string output_field_file = argv[3];
    std::string output_matching_file = (argc >= 5) ? argv[4] : "";

    std::cout << "Loading mesh from: " << mesh_file << "\n";

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    std::cout << "Mesh loaded: " << mesh.V.rows() << " vertices, " << mesh.F.rows() << " faces\n";

    directional::IntrinsicVertexTangentBundle vtb;
    vtb.init(mesh);

    directional::CartesianField rawField;
    rawField.init(vtb, directional::fieldTypeEnum::RAW_FIELD, 1);

    directional::read_raw_field(input_field_file, rawField);

    std::cout << "Field loaded\n";

    directional::principal_matching(rawField);

    directional::CartesianField combedField;
    directional::combing(rawField, combedField);

    std::cout << "Writing combed field to: " << output_field_file << "\n";
    directional::write_raw_field(output_field_file, combedField);

    if (!output_matching_file.empty()) {
        std::cout << "Writing matching to: " << output_matching_file << "\n";
        directional::write_matching(output_matching_file, rawField);
    }

    std::cout << "Done!\n";

    return 0;
}
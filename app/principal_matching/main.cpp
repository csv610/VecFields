#include <iostream>
#include <string>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/IntrinsicVertexTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/principal_matching.h>
#include <directional/write_matching.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <output.matching>\n";
    std::cout << "  input.mesh     - Input mesh file (OBJ format)\n";
    std::cout << "  input.field    - Input raw field file\n";
    std::cout << "  output.matching - Output matching file\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string input_file = argv[2];
    std::string output_file = argv[3];

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    directional::IntrinsicVertexTangentBundle vtb;
    vtb.init(mesh);

    directional::CartesianField field;
    field.init(vtb, directional::fieldTypeEnum::RAW_FIELD, 1);
    directional::read_raw_field(input_file, field);

    directional::principal_matching(field);

    directional::write_matching(output_file, field);

    std::cout << "Done!\n";
    return 0;
}
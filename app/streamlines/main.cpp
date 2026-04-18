#include <iostream>
#include <string>
#include <vector>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/read_raw_field.h>
#include <directional/CartesianField.h>
#include <directional/PCFaceTangentBundle.h>
#include <directional/streamlines.h>

void print_usage(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " <input.mesh> <input.field> <output.seeds> [numSeeds]\n";
    std::cout << "  input.mesh   - Input mesh file (OBJ format)\n";
    std::cout << "  input.field - Input raw field file\n";
    std::cout << "  output.seeds - Output seed points file\n";
    std::cout << "  numSeeds   - Number of seeds (default: 100)\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        print_usage(argv[0]);
        return 1;
    }

    std::string mesh_file = argv[1];
    std::string field_file = argv[2];
    std::string output_file = argv[3];
    int numSeeds = 100;

    if (argc >= 5) numSeeds = std::stoi(argv[4]);

    directional::TriMesh mesh;
    directional::readOBJ(mesh_file, mesh);

    directional::PCFaceTangentBundle ftb;
    ftb.init(mesh);

    directional::CartesianField field;
    field.init(ftb, directional::fieldTypeEnum::RAW_FIELD, 1);
    directional::read_raw_field(field_file, field);

    directional::StreamlineData data;
    directional::StreamlineState state;

    Eigen::VectorXi seedLocations;
    directional::streamlines_init(field, seedLocations, 0.1, data, state);

    std::ofstream out(output_file);
    out << state.currElements.size() << "\n";
    for (int i = 0; i < state.currElements.size(); ++i) {
        out << state.currElements(i) << " " << state.currStartPoints.row(i) << "\n";
    }
    out.close();

    std::cout << "Done!\n";
    return 0;
}
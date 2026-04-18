#include <gtest/gtest.h>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/readOFF.h>

TEST(MeshIOTest, ReadOBJ) {
    directional::TriMesh mesh;
    // Using absolute path for consistency across environments
    std::string test_mesh = "/Users/csv610/Projects/Mesh/Directional/tutorial/data/loop.obj";
    bool success = directional::readOBJ(test_mesh, mesh);
    
    EXPECT_TRUE(success);
    EXPECT_GT(mesh.V.rows(), 0);
    EXPECT_GT(mesh.F.rows(), 0);
}

TEST(MeshIOTest, ReadOFF) {
    directional::TriMesh mesh;
    std::string test_mesh = "/Users/csv610/Projects/Mesh/Directional/tutorial/data/rocker-arm1250.off";
    bool success = directional::readOFF(test_mesh, mesh);
    
    EXPECT_TRUE(success);
    EXPECT_GT(mesh.V.rows(), 0);
    EXPECT_GT(mesh.F.rows(), 0);
}

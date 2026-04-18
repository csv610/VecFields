#include <gtest/gtest.h>
#include <directional/TriMesh.h>

TEST(BasicTest, TriMeshInitialization) {
    directional::TriMesh mesh;
    // Basic property check (should start empty)
    EXPECT_EQ(mesh.V.rows(), 0);
    EXPECT_EQ(mesh.F.rows(), 0);
}

TEST(BasicTest, TriMeshSimpleFace) {
    directional::TriMesh mesh;
    mesh.V.resize(3, 3);
    mesh.V << 0, 0, 0,
              1, 0, 0,
              0, 1, 0;
    mesh.F.resize(1, 3);
    mesh.F << 0, 1, 2;
    
    EXPECT_EQ(mesh.V.rows(), 3);
    EXPECT_EQ(mesh.F.rows(), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

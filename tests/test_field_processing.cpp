#include <gtest/gtest.h>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/PCFaceTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/power_field.h>
#include <directional/power_to_raw.h>
#include <directional/principal_matching.h>

TEST(FieldProcessingTest, PowerFieldAndMatching) {
    directional::TriMesh mesh;
    std::string test_mesh = "/Users/csv610/Projects/Mesh/Directional/tutorial/data/loop.obj";
    ASSERT_TRUE(directional::readOBJ(test_mesh, mesh));

    directional::PCFaceTangentBundle ftb;
    ftb.init(mesh);

    int N = 4;
    directional::CartesianField powerField;
    
    // Set a constraint on the first face
    Eigen::VectorXi constFaces(1); constFaces << 0;
    Eigen::MatrixXd constVectors(1, 3); constVectors << 1.0, 0.0, 0.0; // Pointing along X
    
    // Compute power field
    directional::power_field(ftb, constFaces, constVectors, Eigen::VectorXd::Constant(1, -1.0), N, powerField);
    
    // Verify power field
    EXPECT_EQ(powerField.intField.rows(), mesh.F.rows());
    EXPECT_GT(powerField.intField.norm(), 1e-6); // Should not be all zero

    // Convert to raw field
    directional::CartesianField rawField;
    directional::power_to_raw(powerField, N, rawField);
    EXPECT_EQ(rawField.extField.rows(), mesh.F.rows());
    EXPECT_EQ(rawField.extField.cols(), 3 * N);

    // Run principal matching
    directional::principal_matching(rawField);
    EXPECT_EQ(rawField.matching.rows(), mesh.EF.rows());
}

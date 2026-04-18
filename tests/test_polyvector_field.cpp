#include <gtest/gtest.h>
#include <directional/TriMesh.h>
#include <directional/readOBJ.h>
#include <directional/PCFaceTangentBundle.h>
#include <directional/CartesianField.h>
#include <directional/polyvector_field.h>
#include <directional/polyvector_to_raw.h>
#include <directional/principal_matching.h>

TEST(PolyVectorFieldTest, ComputationAndMatching) {
    directional::TriMesh mesh;
    std::string test_mesh = "/Users/csv610/Projects/Mesh/Directional/tutorial/data/loop.obj";
    ASSERT_TRUE(directional::readOBJ(test_mesh, mesh));

    directional::PCFaceTangentBundle ftb;
    ftb.init(mesh);

    int N = 4;
    
    directional::PolyVectorData pvData;
    pvData.N = N;
    pvData.tb = &ftb;
    pvData.verbose = false;
    
    // Set a constraint on the first face
    pvData.constSpaces.resize(1); pvData.constSpaces << 0;
    pvData.constVectors.resize(1, 3); pvData.constVectors << 1.0, 0.0, 0.0; // Pointing along X
    
    // Parameters
    pvData.wSmooth = 1.0;
    pvData.wRoSy = 1.0;
    pvData.wAlignment = Eigen::VectorXd::Constant(1, -1.0); // Hard alignment
    
    // Compute PolyVector field
    directional::CartesianField pvField;
    directional::polyvector_field(pvData, pvField);
    
    // Verify PolyVector field
    EXPECT_EQ(pvField.intField.rows(), mesh.F.rows());
    // PolyVector coefficients are complex, stored as 2 real columns each
    EXPECT_EQ(pvField.intField.cols(), 2 * N); 

    // Convert to raw field
    directional::CartesianField rawField;
    directional::polyvector_to_raw(pvField, rawField, N % 2 == 0);
    EXPECT_EQ(rawField.extField.rows(), mesh.F.rows());
    EXPECT_EQ(rawField.extField.cols(), 3 * N);

    // Run principal matching
    directional::principal_matching(rawField);
    EXPECT_EQ(rawField.matching.rows(), mesh.EF.rows());
}

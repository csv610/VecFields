# Vector Field Processing Terminology

This document defines common terms and concepts used in the VecFields (Directional) library and the broader field of discrete geometry processing.

## Fundamental Structures

### Tangent Bundle
A collection of tangent spaces attached to every point (or discrete element like a face or vertex) of a manifold (mesh). In the discrete setting, it is represented as a graph where nodes are tangent spaces and edges represent adjacency.

### Tangent Space / Tangent Plane
A local 2D linear space tangent to the surface at a specific location. In face-based fields, this is the plane of the triangle; in vertex-based fields, it is the flattened 1-ring neighborhood.

### Connection
A geometric quantity that defines how to "parallel transport" a vector from one tangent space to an adjacent one. It is typically represented as an orthogonal rotation matrix that aligns the local coordinate bases of two neighboring spaces.

### Parallel Transport
The operation of moving a vector along a curve (or across mesh elements) such that it remains "parallel" according to the connection, accounting for the surface's curvature.

### Parallel Transport Problem
A fundamental challenge in geometry processing arising from surface curvature. Unlike in flat space, parallel transporting a vector from point A to point B is path-dependent. Moving a vector around a closed loop results in a rotation called **holonomy**, which is directly proportional to the Gaussian curvature enclosed by the loop. In discrete meshes, this holonomy is concentrated at vertices (angle defect), forcing the presence of singularities and making it impossible to define a globally "parallel" field on any surface that is not topologically a cylinder or a torus.

---

## Field Representations

### Directional Field (Degree N)
A field that assigns $N$ vectors to every point on the surface. These can be independent or constrained by symmetry.

### N-RoSy (N-Rotational Symmetry)
A field where the $N$ vectors in each tangent space are related by rotations of $2\pi/N$. A common example is a 4-RoSy field (cross-field), often used for quad-meshing.

### Power Field
A representation of $N$-RoSy fields using a single complex number $z = A \cdot e^{iN\theta}$ per tangent space. The $N$ actual vectors are the $N$-th roots of $z$.

### PolyVector Field
A generalization of power fields where $N$ vectors are represented as the roots of a monic complex polynomial. This allows for independent vectors that don't necessarily share the same magnitude or perfect rotational symmetry.

### Raw Field
The most explicit representation, storing $N$ individual vectors per tangent space as a list of coordinates.

---

## Processing & Analysis

### Principal Matching
The process of finding the unique "best" alignment between sets of vectors in adjacent tangent spaces by minimizing the rotation (effort) between them.

### Combing
The process of re-indexing or "straightening" the vectors in a field so that they align consistently across as many edges as possible. Combing usually results in "seams" due to the surface topology.

### Seams
Paths on the mesh where the field cannot be combed continuously. These paths often connect singularities or loop around handles of the mesh.

### Singularity
A point where the field direction is undefined. In a discrete field, this occurs when the total rotation (holonomy) of the field around a local cycle (like a vertex) is a non-zero multiple of $2\pi/N$.

### Singularity Index
A number (often fractional, like $1/4$ or $-1/2$) describing the amount of field rotation around a singularity. The sum of all indices is constrained by the mesh topology (Gauss-Bonnet/Poincaré-Hopf theorem).

### Effort
A measure of the total rotation or "energy" required to align the field across the mesh edges according to a given matching.

---

## Discrete Exterior Calculus (DEC)

### Differential Forms ($k$-forms)
- **0-form**: Scalar values defined on vertices (e.g., temperature).
- **1-form**: Values defined on oriented edges, representing the integrated projection of a vector field along those edges.
- **2-form**: Values defined on faces, representing integrated densities or areas.

### Hodge Decomposition
The theorem stating that any vector field can be uniquely decomposed into three components:
1. **Exact**: The gradient of a scalar function (curl-free).
2. **Coexact**: The rotated gradient of a scalar function (divergence-free).
3. **Harmonic**: Both curl-free and divergence-free, related to the global topology (handles) of the surface.

### Seamless Integration
The process of finding scalar functions whose gradients best match a given directional field. "Seamless" implies that the functions match across mesh cuts up to a symmetry transformation (rotation and translation), enabling the generation of consistent textures or quad meshes.

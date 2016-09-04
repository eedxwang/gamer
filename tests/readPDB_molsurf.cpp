/*
 * ***************************************************************************
 * GAMER = < Geometry-preserving Adaptive MeshER >
 * Copyright (C) 1994-- Michael Holst and Zeyun Yu
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * ***************************************************************************
 */

/*
 * ***************************************************************************
 * File:     MolecularMesh.C    < ... >
 *
 * Author:   Zeyun Yu (zeyun.yu@gmail.com)
 *
 * Purpose:  The main function of MolecularMesh (the old GAMer application)
 * ***************************************************************************
 */

#include "gamer.h"
#include <iostream>

// parameters for mesh smoothing, refinement, and coarsening

/** @brief Maximal number of nodes allowed */
#define MeshSizeUpperLimit   80001

/** @brief Minimal number of nodes allowed */
#define MeshSizeLowerLimit   10000

/** @brief Number of iterations in mesh quality improvement */
#define ITER_NUM             15

/*
 * ***************************************************************************
 * Routine:  SmoothMolecularSurface    < ... >
 *
 * Author:   Zeyun Yu (zeyun.yu@gmail.com)
 *
 * Purpose:  Main function for surface smoothing, refining, and coarsening
 *           a molecular surface mesh
 * ***************************************************************************
 */
void SmoothMolecularSurface(SurfaceMesh  *surfmesh,
                            int           off_flag,
                            ATOM         *sphere_list,
                            int           num_spheres,
                            unsigned int *sphere_markers)
{
    int m;
    char   stop;

    bool smoothed;

    // GenerateHistogram(surfmesh);

    /* Check the neighborhood information */
    surfmesh->createNeighborlist();

    /* Feature-preserving quality improvement */

    // initial surface mesh smoothing if the input mesh is not in OFF format ...
    if (!off_flag) {
        surfmesh->smooth(10, 160, ITER_NUM, false);
    }

    while (1) {
        // mesh coarsening ....
        if ((surfmesh->numVertices() > MeshSizeUpperLimit) || !off_flag) {
            off_flag = 1;

            /* Mesh-coarsening based on surface curvature */
            printf("\nbegin assigning active sites ....\n");
            surfmesh->assignActiveSites(sphere_list, num_spheres, sphere_markers);
            printf("\nbegin mesh coarsening ....\n");
            stop = surfmesh->coarse(CoarsenRate, 1, 1, 0.5);

            printf("After coarsening: Nodes = %d,  Faces = %d\n\n",
                   surfmesh->numVertices(),
                   surfmesh->numFaces());

            /* Feature-preserving quality improvement */
            m = 1;

            while (1) {
                smoothed = surfmesh->smooth(20, 140, 1, true);

                if (smoothed) {
                    break;
                }

                m++;

                if (((surfmesh->numVertices() > MeshSizeUpperLimit) &&
                     (m > ITER_NUM)) ||
                    ((surfmesh->numVertices() <= MeshSizeUpperLimit) &&
                     (m > 2 * ITER_NUM))) {
                    break;
                }
            }
            m = 1;
            printf("\nbegin surface diffusion ....\n");
            surfmesh->normalSmooth();
        }

        else if (surfmesh->numVertices() < MeshSizeLowerLimit) {
            surfmesh->refine();

            printf("After Refinement: Nodes = %d, Faces = %d\n",
                   surfmesh->numVertices(),
                   surfmesh->numFaces());
            surfmesh->smooth(10, 150, ITER_NUM, true);

            if (surfmesh->numVertices() >= MeshSizeLowerLimit) {
                break;
            }
        }

        // nothing to be done...
        else {
            break;
        }
    }

    // Assign active sites again
    surfmesh->assignActiveSites(sphere_list, num_spheres,
                                  sphere_markers);

    // GenerateHistogram(surfmesh);

    surfmesh->destroyNeighborlist();
}

/*
 * ***************************************************************************
 * Rutine:   MolecularMeshCALL.C    < ... >
 *
 * Author:   Zeyun Yu (zeyun.yu@gmail.com)
 *
 * Purpose:  The main entrance of GAMER
 * ***************************************************************************
 */
int MolecularMesh_CALL(char active_flag, char molsurf, char *input_name,
                       char *input_site, int output_flag, GemMesh *Gem_mesh)
{
    bool   write_to_file = Gem_mesh == NULL;
    time_t t1, t2;
    SurfaceMesh *surfmesh, *surfmesh_inner, *surfmesh_outer;
    int xdim, ydim, zdim;
    float *dataset;
    float  radius;
    SPNT  *holelist;
    float  min[3], span[3];
    char   filename[256];
    int    atom_num = 0;
    unsigned int num_spheres = 0;
    ATOM *atom_list = NULL;
    ATOM *sphere_list = NULL;
    unsigned int *sphere_markers = NULL;
    ATOM  center_radius;

    tetgenio in, out, addin;

    tetgenio::facet   *f;
    tetgenio::polygon *p;
    int  i, j, k;
    char buf[1024];
    unsigned int c;
    char IsOFF;
    char IsRawiv;
    char IsXYZR;

    // Read sphere list from file if givven
    if (active_flag) {
        ReadActiveSiteFile(input_site, num_spheres, sphere_list, sphere_markers);
    }

    strcpy(filename, input_name);
    IsOFF = 0;

    for (i = 0; i < 256; i++) {
        if (filename[i + 3] == '\0') {
            break;
        }
        else if ((filename[i] == '.') &&
                 ((filename[i + 1] == 'O') || (filename[i + 1] == 'o')) &&
                 ((filename[i + 2] == 'F') || (filename[i + 2] == 'f')) &&
                 ((filename[i + 3] == 'F') || (filename[i + 3] == 'f')) &&
                 (filename[i + 4] == '\0')) {
            IsOFF = 1;
            break;
        }
    }
    strcpy(filename, input_name);
    IsRawiv = 0;

    for (i = 0; i < 256; i++) {
        if (filename[i + 5] == '\0') {
            break;
        }
        else if ((filename[i] == '.') &&
                 ((filename[i + 1] == 'R') || (filename[i + 1] == 'r')) &&
                 ((filename[i + 2] == 'A') || (filename[i + 2] == 'a')) &&
                 ((filename[i + 3] == 'W') || (filename[i + 3] == 'w')) &&
                 ((filename[i + 4] == 'I') || (filename[i + 4] == 'i')) &&
                 ((filename[i + 5] == 'V') || (filename[i + 5] == 'v')) &&
                 (filename[i + 6] == '\0')) {
            IsRawiv = 1;
            break;
        }
    }
    strcpy(filename, input_name);
    IsXYZR = 0;

    for (i = 0; i < 256; i++) {
        if (filename[i + 4] == '\0') {
            break;
        }
        else if ((filename[i] == '.') &&
                 ((filename[i + 1] == 'X') || (filename[i + 1] == 'x')) &&
                 ((filename[i + 2] == 'Y') || (filename[i + 2] == 'y')) &&
                 ((filename[i + 3] == 'Z') || (filename[i + 3] == 'z')) &&
                 ((filename[i + 4] == 'R') || (filename[i + 4] == 'r')) &&
                 (filename[i + 5] == '\0')) {
            IsXYZR = 1;
            break;
        }
    }

    if (IsOFF) {
        // load user-defined molecualr surface meshes in OFF format
        printf("loading the user-specified surface/volumetric mesh....\n");
        surfmesh_inner = SurfaceMesh::readOFF(input_name);
        printf("begin surface smoothing ... \n");

        (void)time(&t1);
        atom_num = 0;
        SmoothMolecularSurface(surfmesh_inner, 1, sphere_list, num_spheres,
                               sphere_markers);
        (void)time(&t2);
        printf("time to smooth surface mesh: %d seconds. \n\n", (int)(t2 - t1));
    }
    else if (IsRawiv) {
        ReadRawiv(&xdim, &ydim, &zdim, &dataset, input_name, span, min);
        printf("begin extracting isosurfaces ... \n");
        (void)time(&t1);
        float iso_val = 255;

        if (iso_val > IsoValue) {
            iso_val = IsoValue;
        }
        printf("isovalue: %f \n", iso_val);
        surfmesh_inner = SurfaceMesh::marchingCube(xdim, ydim, zdim, dataset,
                                                  iso_val, &holelist);
        (void)time(&t2);
        printf("vertices: %d, faces: %d\n", surfmesh_inner->numVertices(),surfmesh_inner->numFaces());
        printf("time to extract isosurface: %d seconds. \n\n", (int)(t2 - t1));
        free(dataset);

        // convert from pixel to angstrom
        for (j = 0; j < surfmesh_inner->numVertices(); j++) {
            auto vertex = surfmesh_inner->vertex_attr(j);
            vertex.x = vertex.x * span[0] + min[0];
            vertex.y = vertex.y * span[1] + min[1];
            vertex.z = vertex.z * span[2] + min[2];
            surfmesh_inner->vertex_attr(j,vertex);
        }

        printf("begin surface smoothing ... \n");
        (void)time(&t1);
        atom_num = 0;
        SmoothMolecularSurface(surfmesh_inner, 0, sphere_list, num_spheres, sphere_markers);
        (void)time(&t2);
        printf("time to smooth surface mesh: %d seconds. \n\n", (int)(t2 - t1));
    }
    else {
        // Read PDB or PQR files, or XYZR format
        if (molsurf == 0) {
            surfmesh_inner = SurfaceMesh::readPDB_gauss(input_name, -0.2, 2.5);
        }
        else if (molsurf == 1) {
            surfmesh_inner = SurfaceMesh::readPDB_molsurf(input_name);
        }
        else {
            printf("molsurf can only be 0 or 1 \n");
            exit(0);
        }

        printf("Save initial Molecular mesh.\n");

        // SurfaceMesh_writeOFF(surfmesh_inner, "Initial_surfmesh.off");

        printf("begin surface smoothing ... \n");
        (void)time(&t1);
        SmoothMolecularSurface(surfmesh_inner, 0, sphere_list, num_spheres,
                               sphere_markers);
        (void)time(&t2);
        printf("time to smooth surface mesh: %d seconds. \n\n", (int)(t2 - t1));
    }

    // Get the center and radius of Surface mesh
    center_radius = surfmesh_inner->getCenterRadius();
    printf("center/radius: %f %f %f  %f\n", center_radius.x, center_radius.y,
           center_radius.z, center_radius.radius);


    // Generate exterior sphere mesh
    printf("Generating the mesh for the bounding sphere ....\n");
    surfmesh_outer = SurfaceMesh::sphere(5);

    for (auto& vertex : surfmesh_outer->vertices())
    {
        vertex.x = vertex.x * center_radius.radius * SphereRatio + center_radius.x;
        vertex.y = vertex.y * center_radius.radius * SphereRatio + center_radius.y;
        vertex.z = vertex.z * center_radius.radius * SphereRatio + center_radius.z;
    }
    /*
    for (j = 0; j < surfmesh_outer->numVertices(); j++) {
        auto vertex = surfmesh_outer->vertex_attr(j);

        vertex.x = vertex.x *
                                      center_radius.radius *
                                      SphereRatio + center_radius.x;
        vertex.y = vertex.y *
                                      center_radius.radius *
                                      SphereRatio + center_radius.y;
        vertex.z = vertex.z *
                                      center_radius.radius *
                                      SphereRatio + center_radius.z;

        surfmesh_outer->vertex_attr(j, vertex);
    }
    */
    printf("\n");

    printf("Mesh generated ....\n");

    // Merge molecular mesh and sphere mesh
    surfmesh = SurfaceMesh::merge(surfmesh_inner, surfmesh_outer);

    // Output surface meshes
    sprintf(filename, "%s.output.surf.off", input_name);

    // SurfaceMesh_writeOFF(surfmesh_inner, filename);
    surfmesh_inner->writeOFF(filename);

    // Compute and Output all tetrahedra meshes into .m format
    printf("begin all tetrahedra generating ... \n");
    (void)time(&t1);
    in.firstnumber    = 1;
    in.numberofpoints = surfmesh->numVertices();
    in.pointlist      = new REAL[in.numberofpoints * 3];

    for (j = 0; j < in.numberofpoints; j++) {
        in.pointlist[j * 3 + 0] = surfmesh->vertex_attr(j).x;
        in.pointlist[j * 3 + 1] = surfmesh->vertex_attr(j).y;
        in.pointlist[j * 3 + 2] = surfmesh->vertex_attr(j).z;
    }
    in.numberoffacets = surfmesh->numFaces();
    in.facetlist      = new tetgenio::facet[in.numberoffacets];

    for (j = 0; j < in.numberoffacets; j++) {
        f                   = &in.facetlist[j];
        f->holelist         = (REAL *)NULL;
        f->numberofholes    = 0;
        f->numberofpolygons = 1;
        f->polygonlist      = new tetgenio::polygon[f->numberofpolygons];
        p                   = &f->polygonlist[0];
        p->numberofvertices = 3;
        p->vertexlist       = new int[p->numberofvertices];
        p->vertexlist[0]    = surfmesh->face_attr(j).a + in.firstnumber;
        p->vertexlist[1]    = surfmesh->face_attr(j).b + in.firstnumber;
        p->vertexlist[2]    = surfmesh->face_attr(j).c + in.firstnumber;
    }

    // insert each atom as a node
    if (atom_num > 0) {
        addin.pointlist = new REAL[atom_num * 3];

        for (i = 0; i < atom_num; i++) {
            addin.pointlist[i * 3 + 0] = atom_list[i].x;
            addin.pointlist[i * 3 + 1] = atom_list[i].y;
            addin.pointlist[i * 3 + 2] = atom_list[i].z;
        }
        addin.numberofpoints = atom_num;
    }

    // add boundary marker on each node
    in.pointmarkerlist = new int[in.numberofpoints];

    for (j = 0; j < in.numberofpoints; j++) {
        if (surfmesh_inner->vertex_attr(j).m > 0) {
            in.pointmarkerlist[j] = surfmesh_inner->vertex_attr(j).m;
        }
        else {
            in.pointmarkerlist[j] = 1;
        }
    }

    std::cout << "tetrahedralize" << std::endl;

    strcpy(buf, "npq1.333VAAYY");
    tetrahedralize(buf, &in, &out, &addin, NULL);
    // tetrahedralize("npq1", &in, &out, &addin, NULL);

    (void)time(&t2);
    printf("time to generate all tetrahedra: %d seconds. \n\n", (int)(t2 - t1));
    printf("begin writing all meshes ... \n");
    (void)time(&t1);

    // Assign active site information
    // Morphology-based hole-filling by dilation
    char *ActiveSite;
    ActiveSite = new char[out.numberofpoints];

    for (i = 0; i < out.numberoftetrahedra; i++) {
        c = 0;

        for (j = 0; j < 4; j++) {
            k = out.tetrahedronlist[i * 4 + j] - 1;

            if (k < surfmesh_inner->numVertices()) {
                if (surfmesh_inner->vertex_attr(k).m > 0) {
                    c = surfmesh_inner->vertex_attr(k).m;
                }
            }
        }

        for (j = 0; j < 4; j++) {
            k = out.tetrahedronlist[i * 4 + j] - 1;

            if (out.pointmarkerlist[k]) {
                // printf("ActiveSite: %d, %c\n", out.pointmarkerlist[k], c);
                ActiveSite[k] = c;
            }
        }
    }


    // Release surface meshs
    std::cout << "Deleting surfmesh" << std::endl;
    delete surfmesh;
    std::cout << "Deleting surfmesh_inner" << std::endl;
    delete surfmesh_inner;
    std::cout << "Deleting surfmesh_outer" << std::endl;
    delete surfmesh_outer;

    // Construct a GemMesh structur

    std::cout << "Constructing GemMesh  " << std::endl;

    radius = 1.0; // JBM - radius was uninitialized. Unsure what to set it to. Setting it to 1.0 for now.
    // TODO: figure out what a reasonable setting is here.
    Gem_mesh = GemMesh_fromPdb(&out,
                               radius * SphereRatio,
                               center_radius.x,
                               center_radius.y,
                               center_radius.z,
                               ActiveSite,
                               output_flag);

    // Check if we are writing the mesh to file
    if (write_to_file) {
        if (output_flag == 1) {
            sprintf(filename, "%s.output.all.m", input_name);
        }

        if (output_flag == 2) {
            sprintf(filename, "%s.output.in.m", input_name);
        }

        if (output_flag == 3) {
            sprintf(filename, "%s.output.out.m", input_name);
        }
        std::cout << "Writing GemMesh  " << std::endl;
        GemMesh_writeMcsf(Gem_mesh, filename);
        std::cout << "Deleting GemMesh  " << std::endl;
        GemMesh_dtor(Gem_mesh);
    }

    std::cout << "Cleaning up" << std::endl;
    delete[] ActiveSite;

    if (atom_list != NULL) {
        free(atom_list);
    }

    if (sphere_list != NULL) {
        free(sphere_list);
    }

    if (sphere_markers != NULL) {
        free(sphere_markers);
    }

    return 0;
}

int main(int argc, char *argv[])
{
    int region_flag;


    if ((argc != 4) && (argc != 3)) {
        printf("\nUsage: MolecularMesh <Input> <Domain> [ActiveSite] \n");
        printf("      <Input>: PDB, PQR, OFF, XYZR, or Rawiv \n");
        printf("      <Domain>: 1 --> generate both inner and outer meshes \n");
        printf("                2 --> generate only inner mesh \n");
        printf("                3 --> generate only outer mesh \n");
        printf("      <ActiveSite>: See README for specification \n\n");
        exit(0);
    }

    region_flag = atoi(argv[2]);

    if ((region_flag != 1) && (region_flag != 2) && (region_flag != 3)) {
        printf("\n<Domain> must be 1, 2 or 3....\n");
        printf("Type <MolecularMesh -help> for more information....\n\n");
        exit(0);
    }


    /*
     * **************************************************************************
     * You may call GAMer in two ways:
     *     (1) Output the meshes to disks...
     *         In this case, you do nothing but just checking you current
     *         direcotry for the outputs when GAMer is finished.
     *
     *     (2) Output the meshes to a data structure "GemMesh"...
     *         For the second case, you convert the GemMesh into your own
     *         data structure such as "Gem" in FETK.
     * ***************************************************************************
     */


    // CASE 1:
    if (argc == 4) {// active site is specified
        // change the second parameter to 1 if the new approach is to be used
        MolecularMesh_CALL(1, 1, argv[1], argv[3], region_flag, NULL);
    }
    else { // no active site is specified
        // change the second parameter to 1 if the new approach is to be used
        MolecularMesh_CALL(0, 1, argv[1], NULL, region_flag, NULL);
    }


    /*
       // CASE 2:
       GemMesh *gamer_mesh;
       if (argc == 4) // active site is specified
        MolecularMesh_CALL(1, 0, argv[1], argv[3], region_flag, gamer_mesh);
       else // no active site is specified
        MolecularMesh_CALL(0, 0, argv[1], NULL, region_flag, gamer_mesh);

       std::cout << gamer_mesh->dim << std::endl;
       // You can now convert "gamer_mesh" into your own data structure...
       // The GemMesh structure is defined in src/tetgen/gamer/tetgen.h
     */

    return 0;
}

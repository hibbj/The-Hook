/*
 *  CPE 471 lab 4 - draws a box without sides
 *  glut/OpenGL application 
 *  uses glm for matrices
 *  Has support for simple transforms - check the keyboard function
 *
 *  Created by zwood on 1/6/12 
 *  Copyright 2010 Cal Poly. All rights reserved.
 *
 *****************************************************************************/
#include "Lab4.h"
#include "InitHelpers.h"
#include "Helper.h"
#include "IO.h"
#include "Shapes.h"

void HideMesh();

//position and color data handles
GLuint triBuffObj, colBuffObj;

int ShadeMode = 0;

//flag and ID to toggle on and off the shader
int shade = 1;
int ShadeProg;

//Handles to the shader data
GLint h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
int g_CiboLen, g_GiboLen, g_RiboLen, g_SCiboLen;

float g_width, g_height;
float g_Camtrans = -3.0;
float g_Mtrans = 0;
GLint h_uLightVec;
GLint h_uLightColor;
GLint h_uCamPos, h_uShadeMode;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;

//declare Matrix stack
RenderingHelper ModelTrans;

/**ALL JUNK FOR MESH LOGIC**/
static glm::vec3 *faceNormals;
static float *MeshPos;
static unsigned short *idx;
int idxSize;
int MeshPosSize;
float *VertexNormal;
float g_sf = 1.7;
glm::vec3 g_axis(0.000000001, 0.000000001, 0.000000001);
glm::mat4 allRot(1.0);
glm::vec3 g_trans(0.0, 0.8, 0.0);
int g_track;
float g_angle = 180;
int g_MiboLen;
//CMesh *TheMesh;
GLint h_uModelMatrix;
GLuint NormalBuffObj;
GLuint MeshBuffObj, MeshIndxBuffObj;

/*float CMesh::WStoPS(float input) {
   if(input > 1) {
      return input;
   }
   return (input * 200) + 200;
}*/

/*int loadASCIIMesh(std::string const & fileName, CMesh *Mesh) {
        std::ifstream File;
        File.open(fileName.c_str());

        if (! File.is_open())
        {
                std::cerr << "Unable to open mesh file: " << fileName << std::endl;
                return 0;
        }

        while (File)
        {
                std::string ReadString;
                std::getline(File, ReadString);

                std::stringstream Stream(ReadString);

                std::string Label;
                Stream >> Label;

                if (Label.find("#") != std::string::npos)
                {
                        continue;
                }

                if ("Vertex" == Label)
                {
                        int Index;
                        Stream >> Index;

                        SVector3 Position;
                        Stream >> Position.X;
                        Stream >> Position.Y;
                        Stream >> Position.Z;

                        SVertex Vertex;
                        Vertex.Position = Position;

                        Mesh->Vertices.push_back(Vertex);
                }
else if ("Face" == Label)
                {
                        int Index;
                        Stream >> Index;

                        int Vertex1, Vertex2, Vertex3;
                        Stream >> Vertex1;
                        Stream >> Vertex2;
                        Stream >> Vertex3;

                        CMesh::STriangle Triangle;
                        Triangle.VertexIndex1 = Vertex1 - 1;
                        Triangle.VertexIndex2 = Vertex2 - 1;
                        Triangle.VertexIndex3 = Vertex3 - 1;

                        size_t Location;
                        if ((Location = ReadString.find("{")) != std::string::npos)
                        {
                                Location = ReadString.find("rgb=(");
                                Location += 5;

                                ReadString = ReadString.substr(Location);
                                std::stringstream Stream(ReadString);
                                float Color;
                                Stream >> Color;
                                Triangle.Color.Red = Color;
                                Stream >> Color;
                                Triangle.Color.Green = Color;
                                Stream >> Color;
                                Triangle.Color.Blue = Color;
                        }

                        Mesh->Triangles.push_back(Triangle);
                }
                else if ("" == Label)
                {
                }
                else if ("Corner" == Label)
                {
                }
                else
                {
                        std::cerr << "While parsing ASCII mesh: Expected 'Vertex' or 'Face' label, found '" << Label << "'." << std::endl;
                }
        }
        if (! Mesh->Triangles.size() || ! Mesh->Vertices.size())
        {
                delete Mesh;
                return 0;
        }

        return 1;
}*/

/*
CMesh::CMesh()
{}

CMesh::~CMesh()
{}*/

/* Just an example of how to iterate over the mesh triangles */
/*void CMesh::PrintMesh() {
    for(unsigned int j = 0; j < Triangles.size(); j++)
    {
        SVertex Vertex;
        cout << "New Vertices from triangle " << j << endl;
        Vertex = Vertices[Triangles[j].VertexIndex1];
        cout << "V1 " << Vertex.Position.X << " " << Vertex.Position.Y << " " << Vertex.Position.Z << endl;
        Vertex = Vertices[Triangles[j].VertexIndex2];
        cout << "V2 " << Vertex.Position.X << " " << Vertex.Position.Y << " " << Vertex.Position.Z << endl;
        Vertex = Vertices[Triangles[j].VertexIndex3];
        cout << "V3 " << Vertex.Position.X << " " << Vertex.Position.Y << " " << Vertex.Position.Z << endl;
    }
}*/

/* center the mesh */
/*void CMesh::centerMeshByExtents(SVector3 const & CenterLocation) {

    if (Vertices.size() < 2)
        return;

    SVector3 Min, Max;
    {
        std::vector<SVertex>::const_iterator it = Vertices.begin();
        Min = it->Position;
        Max = it->Position;
        for (; it != Vertices.end(); ++ it)
        {
            if (Min.X > it->Position.X)
                Min.X = it->Position.X;
            if (Min.Y > it->Position.Y)
                Min.Y = it->Position.Y;
            if (Min.Z > it->Position.Z)
                Min.Z = it->Position.Z;

            if (Max.X < it->Position.X)
                Max.X = it->Position.X;
            if (Max.Y < it->Position.Y)
                Max.Y = it->Position.Y;
            if (Max.Z < it->Position.Z)
                Max.Z = it->Position.Z;
        }
    }
   SVector3 Center = (Max + Min) / 2;

    SVector3 VertexOffset = CenterLocation - Center;
    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Position += VertexOffset;

    /**My own code to get triangles** /
    MeshPos = (float *)calloc(sizeof(float), Vertices.size() * 3);
    for(int p = 0; p < Vertices.size(); p++) {
       SVertex temp = Vertices[p];

       MeshPos[3 * p] = temp.Position.X;
       MeshPos[3 * p + 1] = temp.Position.Y;
       MeshPos[3 * p + 2] = temp.Position.Z;
    }
    idx = (unsigned short *)calloc(sizeof(unsigned short), Triangles.size() * 3);
    for(int k = 0; k < Triangles.size(); k++)
    {
       STriangle temp = Triangles[k];

       idx[3 * k] = temp.VertexIndex1;
       idx[3 * k + 1] = temp.VertexIndex2;
       idx[3 * k + 2] = temp.VertexIndex3;
    }
    MeshPosSize = 3 * Vertices.size();
    idxSize = 3 * Triangles.size();
    VertexNormal = (float *)calloc(sizeof(float), Triangles.size() * 3);
}*/
/* resize the mesh */
/*void CMesh::resizeMesh(SVector3 const & Scale)
{
    if (Vertices.size() < 2)
        return;

    SVector3 Min, Max;
    {
        std::vector<SVertex>::const_iterator it = Vertices.begin();
        Min = it->Position;
        Max = it->Position;
        for (; it != Vertices.end(); ++ it)
        {
            if (Min.X > it->Position.X)
                Min.X = it->Position.X;
            if (Min.Y > it->Position.Y)
                Min.Y = it->Position.Y;
            if (Min.Z > it->Position.Z)
                Min.Z = it->Position.Z;

            if (Max.X < it->Position.X)
                Max.X = it->Position.X;
            if (Max.Y < it->Position.Y)
                Max.Y = it->Position.Y;
            if (Max.Z < it->Position.Z)
                Max.Z = it->Position.Z;
        }
    }
 SVector3 Extent = (Max - Min);
    SVector3 Resize = Scale / std::max(Extent.X, std::max(Extent.Y, Extent.Z));
    for (std::vector<SVertex>::iterator it = Vertices.begin(); it != Vertices.end(); ++ it)
        it->Position *= Resize;

}*/
/*glm::vec3 computeWeightedNormal(int vertex) {
   std::list<glm::vec3> neighbors;
   int vertexIndex1, vertexIndex2, vertexIndex3;
   glm::vec3 sumVector = glm::vec3(0.0f);

   for(int i = 0; i < TheMesh->Triangles.size(); i++) {
      vertexIndex1 = TheMesh->Triangles[i].VertexIndex1;
      vertexIndex2 = TheMesh->Triangles[i].VertexIndex2;
      vertexIndex3 = TheMesh->Triangles[i].VertexIndex3;

      if(vertexIndex1 == vertex) {
         neighbors.push_front(faceNormals[i]);
      }
      else if(vertexIndex2 == vertex) {
         neighbors.push_front(faceNormals[i]);
      }
      else if(vertexIndex3 == vertex) {
         neighbors.push_front(faceNormals[i]);
      }
   }

   for(int i = 0; i < neighbors.size(); i++) {
      sumVector += neighbors.front();
      neighbors.pop_front();
   }

   return sumVector/Magnitude(sumVector);
}*/

/*void SetModelMesh(float x, float y, float z, float angle, float sX, float sY, float sZ) {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, z));
  glm::mat4 Scale = glm::scale( glm::mat4(1.0f), glm::vec3(sX, sY, sZ));
  /*if(g_track) {
    glm::mat4 newRot = glm::rotate( glm::mat4(1.0f), g_angle, g_axis);
    allRot = newRot*allRot;
  }* /

  glm::mat4 ctm = Trans * Scale;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));
}*/
/*static void initMesh() {

  g_MiboLen = idxSize;
  glGenBuffers(1, &MeshBuffObj);
  glBindBuffer(GL_ARRAY_BUFFER, MeshBuffObj);
  glBufferData(GL_ARRAY_BUFFER, MeshPosSize * sizeof(float), MeshPos, GL_STATIC_DRAW);

  glGenBuffers(1, &MeshIndxBuffObj);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshIndxBuffObj);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize * sizeof(unsigned short), idx, GL_STATIC_DRAW);

  unsigned int vertexIndex1, vertexIndex2, vertexIndex3;
  glm::vec3 vertex1, vertex2, vertex3;
  glm::vec3 normal;
  faceNormals = (glm::vec3 *)malloc(sizeof(glm::vec3) * TheMesh->Triangles.size());

  int i;
  for(i = 0; i < TheMesh->Triangles.size(); i++) {
     vertexIndex1 = TheMesh->Triangles[i].VertexIndex1;
     vertexIndex2 = TheMesh->Triangles[i].VertexIndex2;
     vertexIndex3 = TheMesh->Triangles[i].VertexIndex3;

     vertex1 = glm::vec3(TheMesh->Vertices[vertexIndex1].Position.X,
                         TheMesh->Vertices[vertexIndex1].Position.Y,
                         TheMesh->Vertices[vertexIndex1].Position.Z);
     vertex2 = glm::vec3(TheMesh->Vertices[vertexIndex2].Position.X,
                         TheMesh->Vertices[vertexIndex2].Position.Y,
                         TheMesh->Vertices[vertexIndex2].Position.Z);
     vertex3 = glm::vec3(TheMesh->Vertices[vertexIndex3].Position.X,
                         TheMesh->Vertices[vertexIndex3].Position.Y,
                         TheMesh->Vertices[vertexIndex3].Position.Z);

     glm::vec3 normal = glm::cross(vertex3 - vertex1, vertex2 - vertex1);
     faceNormals[i] = normal;
  }
  for(i = 0; i < TheMesh->Vertices.size(); i++) {
     glm::vec3 temp = computeWeightedNormal(i);
     VertexNormal[3 * i] = temp.x;
     VertexNormal[3 * i + 1] = temp.y;
     VertexNormal[3 * i + 2] = temp.z;
  }

    glGenBuffers(1, &NormalBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * idxSize, VertexNormal, GL_STATIC_DRAW);

    free(faceNormals);
}*/

/* projection matrix  - do not change */
void SetProjectionMatrix() {
  glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 100.f);	
  safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

float alpha = 0.0;
float beta = -M_PI/2.0;
float lookAtx = 0.0f;
float lookAty = 0.0f;
float lookAtz = 0.0f;
glm::vec3 lookAtPoint;
glm::vec3 eye = glm::vec3(0.0, 0.0, 0.0);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

/* camera controls - do not change */
void SetView() {
//  printf("lookAtPoint: %f %f %f\n", lookAtPoint.x, lookAtPoint.y, lookAtPoint.z);
//  printf("eye: %f %f %f\n", eye.x, eye.y, eye.z);
  glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
  safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
}

void SetModelStat() {
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
}

/* model transforms */
void SetModel(float x, float y, float z, float Sx, float Sy, float Sz, float angle) {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, z));
  glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(Sx, Sy, Sz));
   printf("%f %f wut\n",Sy,Sz);
  glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
//  glm::mat4 ctm = Trans * Scale * Rotate;
  glm::mat4 ctm = Trans * Rotate * Scale;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));
}

/*void SetModelCube(float x, float y, float z, float angle, float Sx, float Sy, float Sz) {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, z));
  glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(Sx, Sy, Sz));
  glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 ctm = Trans * Scale * Rotate;
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));
}*/


void DrawShadow(float x, float z, float Sx, float Sy, float Sz, float angle) {
    /*Shadow*/
    SetModel(x, -0.5, z, Sx, Sy, Sz, angle);
    SetMaterial(1);
         safe_glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, ShadowCubeBuffObj);
    safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

         safe_glEnableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, ShadowNormalBuffObj);
    safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* draw!*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SCIndxBuffObj);
    glDrawElements(GL_TRIANGLES, g_SCiboLen, GL_UNSIGNED_SHORT, 0);

    /* Disable the attributes used by our shader*/
    safe_glDisableVertexAttribArray(h_aPosition);
    safe_glDisableVertexAttribArray(h_aNormal);
}

void SetupCube(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ) {
   /*First Cube*/
    SetModel(x, y, z, scaleX, scaleY, scaleZ, angle);
    SetMaterial(material);
         safe_glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
    safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    safe_glEnableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
    safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* draw!*/
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CIndxBuffObj);

    glDrawElements(GL_TRIANGLES, g_CiboLen, GL_UNSIGNED_SHORT, 0);

    /* Disable the attributes used by our shader*/
    safe_glDisableVertexAttribArray(h_aPosition);
    safe_glDisableVertexAttribArray(h_aNormal);
    DrawShadow(x, z + 0.6, scaleX, scaleY, scaleZ + 0.4, angle);
}
/*void SetupRamp(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ) {
   /*First Cube* /
    SetModelCube(x, y, z, angle, scaleX, scaleY, scaleZ);
    SetMaterial(material);
         safe_glEnableVertexAttribArray(h_aPosition);
    glBindBuffer(GL_ARRAY_BUFFER, RampBuffObj);
    safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

    safe_glEnableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, RampNormalBuffObj);
    safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* draw!* /
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RIndxBuffObj);

    glDrawElements(GL_TRIANGLES, g_RiboLen, GL_UNSIGNED_SHORT, 0);

    /* Disable the attributes used by our shader* /
    safe_glDisableVertexAttribArray(h_aPosition);
    safe_glDisableVertexAttribArray(h_aNormal);
    DrawShadow(x, z + 0.3, scaleX - 0.5, scaleY, scaleZ, angle);
}*/
/*void SetupMesh(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ) {
   ShadeMode = 1;
   glUniform1f(h_uShadeMode, ShadeMode);
   SetModelMesh(x, y, z, angle, scaleX, scaleY, scaleZ);
   SetMaterial(material);

   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, MeshBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

  /* draw! * /
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshIndxBuffObj);
  glDrawElements(GL_TRIANGLES, g_MiboLen, GL_UNSIGNED_SHORT, 0);
  ShadeMode = 0;
  glUniform1f(h_uShadeMode, ShadeMode);

  DrawShadow(x, z + 0.1, scaleX - 0.1, scaleY, scaleZ - 0.2, angle);

}*/

/*void DrawMesh(float x, float y, float z) {
   ModelTrans.pushMatrix();
      SetupMesh(x, y - 0.2, z, 3, 0.0, 0.5, 0.5, 0.5);
      //SetupMesh(x, y - 0.3, z, 1, 0.0, 0.5, 0.5, 0.5);
   ModelTrans.popMatrix();
}
void DrawRamp(float x, float y, float z) {
   ModelTrans.pushMatrix();
      ModelTrans.pushMatrix();
      SetupRamp(x, y - 0.5, z + 0.25, 11, 0.0, 1.0, 1.0, 1.0);
      ModelTrans.popMatrix();


      ModelTrans.pushMatrix();
      SetupRamp(x, y - 0.5, z - 0.25, 11, 180.0, 1.0, 1.0, 1.0);
      ModelTrans.popMatrix();
   ModelTrans.popMatrix();
}

int CubeMaterials[4];

void DrawStackedCubes(float x, float y, float z) {
    ModelTrans.pushMatrix();
       ModelTrans.pushMatrix();
       SetupCube(x, y, z, CubeMaterials[0], 0.0, 1.0, 1.0, 1.0);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x + 0.2, y + 1.0, z + 0.2, CubeMaterials[1], 0.0, 1.0, 1.0, 1.0);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x + 0.45, y + 2.0, z + 0.35, CubeMaterials[2], 30.0, 1.0, 1.0, 1.0);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x + 0.2, y + 3.0, z + -0.1, CubeMaterials[3], 10.0, 1.0, 1.0, 1.0);
       ModelTrans.popMatrix();
    ModelTrans.popMatrix();
}
void DrawWallJump(float x, float y, float z) {
    ModelTrans.pushMatrix();

       //Main walls
       ModelTrans.pushMatrix();
       SetupCube(x - 1.0, y + 2.0, z, 7, 0.0, 0.2, 5.0, 2.0);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x + 1.0, y + 2.0, z, 7, 0.0, 0.2, 5.0, 2.0);
       ModelTrans.popMatrix();

       //support beams
       ModelTrans.pushMatrix();
       SetupCube(x, y + 4.3, z + 0.7, 7, 0.0, 3.0, 0.2, 0.1);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x, y + 4.3, z - 0.7, 7, 0.0, 3.0, 0.2, 0.1);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x, y + 2.3, z + 0.7, 7, 0.0, 3.0, 0.2, 0.1);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x, y + 2.3, z - 0.7, 7, 0.0, 3.0, 0.2, 0.1);
       ModelTrans.popMatrix();
       //ledges
       ModelTrans.pushMatrix();
       SetupCube(x - 1.5, y + 4.4, z, 7, 0.0, 1.0, 0.2, 2.0);
       ModelTrans.popMatrix();

       ModelTrans.pushMatrix();
       SetupCube(x + 1.5, y + 4.4, z, 7, 0.0, 1.0, 0.2, 2.0);
       ModelTrans.popMatrix();


    ModelTrans.popMatrix();
}*/

double spread;
int numOfStructs;
bool generated = false;
int hideIndex;
/*void GenerateStructures() {
    //printf("Num of position (according to linked list): %d\n", Table_Size());

    Node NodeTemp;
    glm::vec3 temp;

    int structOnes, structTwos, structThrees, i, division;
   /*
    for(int i = 0; i < numOfStructs; i++) {
       //memcpy(&temp, modelPositions + i, sizeof(glm::vec3));
      // temp = *(modelPositions + i);
       NodeTemp = Get_Index(i);
       temp = NodeTemp.Position;
       printf("Pos#%d: %f %f %f\n", i, temp.x, temp.y, temp.z); 
    }* /

   division = numOfStructs/3;
   structOnes = division;
   structTwos = division;
   structThrees = numOfStructs - (2 * division);

   for(i = 0; i < structOnes; i++) {
      NodeTemp = Get_Index(i);
      Set_ModelType(i, MODEL_TYPE_STACKED_CUBES);
      temp = NodeTemp.Position;
      DrawStackedCubes(temp.x, temp.y, temp.z);
   }
   for(i = structOnes; i < structOnes + structTwos; i++) {
      NodeTemp = Get_Index(i);
      Set_ModelType(i, MODEL_TYPE_WALL_JUMP);
      temp = NodeTemp.Position;
      DrawWallJump(temp.x, temp.y, temp.z);
   }
   for(i = structOnes + structTwos; i < numOfStructs; i++) {
      NodeTemp = Get_Index(i);
      Set_ModelType(i, MODEL_TYPE_RAMP);
      temp = NodeTemp.Position;
      DrawRamp(temp.x, temp.y, temp.z);
   }

   HideMesh();

   if(!generated) {
      printf("Generation Complete! Enjoy the scenary!\n");
      printf("  Number of Structs: %d\n", Table_Size());
      printf("    Stacked Cubes: %d\n", structOnes);
      printf("    Wall Jumps: %d\n", structTwos);
      printf("    Ramps: %d\n", structThrees);
      printf("  Spread: %f\n", spread);

      printf("\nWARNING! A Sapphire bunny is hidden somewhere! Can you find it?\n");
      generated = true;
   }
}*//*
void HideMesh() {

   Node NodeTemp = Get_Index(hideIndex);
   glm::vec3 temp = NodeTemp.Position;
   int type = NodeTemp.modelType;

   if(type == MODEL_TYPE_STACKED_CUBES) {
      DrawMesh(temp.x, temp.y + 3.95, temp.z);
   }
   else if(type == MODEL_TYPE_WALL_JUMP) {
      DrawMesh(temp.x + 1.5, temp.y - 0.05, temp.z);
   }
   //Else Ramp
   else {
      DrawMesh(temp.x + 0.5, temp.y - 0.05, temp.z);
   }

}*//*
float AdjustSpread(int structs, float spread) {
   int maxModels;
   float newSpread = spread;

   if(3.0 <= spread && spread < 7.0) {
      maxModels = spread * 20 - 20;

      //if the max number of structs that can fit is exceeded
      if(maxModels < structs) {
         newSpread = (structs + 20.0)/20.0;
      }
      else {
         newSpread = spread;
      }
   }
   return newSpread;
}*//*
void GeneratePositions() {
   //Two iterators, number of structures
   int i, j;
   glm::vec3 temp, temp2;
   //Used to check distance between model positions
   float distance;
   //Used to check inbetween models for spread
   int WithinSpread;
   Node NodeTemp;

   if(spread < 3.0) {
      spread = 3.0;
   }
   else if(spread > 10.0) {
      spread = 10.0;
   }

   //Adjust for min and max
     //Make sure numOfStructs is valid
    if(numOfStructs < 30) {
       numOfStructs = 30;
    }
    else if(numOfStructs > 100) {
       numOfStructs = 100;
    }

    spread = AdjustSpread(numOfStructs, spread);
   //Maximum distance any model can be from the origin
   double maxDistance = pow(spread, 2.0) + spread;
    //printf("NumOfStructs: %d\n", numOfStructs);

   //For each structure we want to generate
   for(i = 0; i < numOfStructs; i++) {
      WithinSpread = 0;

      //printf("Model#: %d\n", i);

      //While within spread
      while(!WithinSpread) {
         //Generate a random position
         temp.x = RandomFloat(-1.0 * maxDistance, maxDistance);
         //Change y later on if I want floating models
         temp.y = 0.0;
         temp.z = RandomFloat(-1.0 * maxDistance, maxDistance);

        // printf("RandomPoint: %f %f %f\n", temp.x, temp.y, temp.z);

         //Check each model generated so far to see if we're too close
         for(j = 0; j < i; j++) {
            //Get model j

            NodeTemp = Get_Index(j);
            temp2 = NodeTemp.Position;
            //printf("Stored Model#%d: %f %f %f\n", j, temp2.x, temp2.y, temp2.z);
            //Get distance between structures
            distance = glm::distance(temp, temp2);
            //If within spread
            if(distance < spread) {
        //       printf("Within spread!!\n");
               WithinSpread = 1;
            }
         }
         //Check to see if we're good
         if(WithinSpread) {
          // printf("Not good position, recalculating..\n");
            //Slightly unintuitive, but set to false to recalculate
            WithinSpread = 0;
         }
         else {
           // printf("Good position!\n");
            //Again unintuitive, set true to indicate we're good
            WithinSpread = 1;
         }
      }
    //  printf("Storing at position %d\n", i);
      //Store position
      Add_Entry(temp);
      //memcpy(modelPositions + i, &temp, sizeof(glm::vec3));
      //printf("Point stored!\n");
   }

   hideIndex = RandomFloat(0, numOfStructs);
   CubeMaterials[0] = RandomInt(2, 11);
   CubeMaterials[1] = RandomInt(2, 11);
   CubeMaterials[2] = RandomInt(2, 11);
   CubeMaterials[3] = RandomInt(2, 11);
}*/

/* Main display function */
void glfwDraw (GLFWwindow *window)
{
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        /*Start our shader      */
        glUseProgram(ShadeProg);

    SetProjectionMatrix();
    SetView();

    glUniform3f(h_uLightColor, 0.4, 0.4, 0.38);
    glUniform4f(h_uLightVec, 0.0, -1.0, 1.0, 0.0);
    glUniform3f(h_uCamPos, eye.x, eye.y, eye.z);

    ModelTrans.loadIdentity();
    SetModelStat();

       safe_glEnableVertexAttribArray(h_aPosition);

       glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
       safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
       glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

       SetMaterial(0);

    safe_glEnableVertexAttribArray(h_aNormal);
    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /* draw!*/
    glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);
    safe_glDisableVertexAttribArray(h_aPosition);

    //GenerateStructures();
//float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ
   //x,y,z,mat,sz
   SetupCube(3,0,6,2,45,1,1,2);
   SetupCube(3,1,6,2,45,1,.5,1);
   SetupCube(3,2,6,2,45,2,1,1);
   SetupCube(3,3,6,2,60,1,1,1);

	//Diisable the shader
	glUseProgram(0);	
	glfwSwapBuffers(window);

}



int main( int argc, char *argv[] )
{
   GLFWwindow *window;

   printf("sob1\n");
   glfwSetErrorCallback(glfwError);
   if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

   // Set up the main window.
   window = glfwCreateWindow(800, 800, "The Creation", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   if(argc != 3) {
      printf("Use: a.out [Spread double] [NumOfModles int]\n");
      return 0;
   }

   spread = atof(argv[1]);
   numOfStructs = atof(argv[2]);

   srand(time(0));
   //GeneratePositions();

   glfwMakeContextCurrent(window);
   glfwSetWindowPos(window, 80, 80);
   glfwSetWindowSizeCallback(window, glfwWindowResize);
	glfwSetWindowSize(window,800,800);
   g_height = g_width = 800;
   glfwSetKeyCallback(window, glfwKeyPress);
   glfwSetCursorPosCallback( window, glfwGetCursorPos );

   glewInit();
   glInitialize();
   //GeneratePositions();

   printf("sob2\n");

 //  glfwSetCursorEnterCallback( window, glfwGetMouseButton ); 

   printf("sob3\n");
   printf("%s\n", glewGetErrorString(glewInit()));
   glCullFace(GL_BACK);

   printf("sob4\n");

   //TheMesh = new CMesh();
   //assert(loadASCIIMesh("bunny10k.m", TheMesh));
   //TheMesh->resizeMesh(SVector3(1));
   //TheMesh->centerMeshByExtents(SVector3(0));
   
   InitGeom();

   // Start the main execution loop.
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      glfwDraw(window);
      glfwSetCursorPos(window,g_width/2,g_height/2);
   }

   // Clean up after GLFW.
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}

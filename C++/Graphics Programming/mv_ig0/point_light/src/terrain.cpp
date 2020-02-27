#include <fstream>
#include <cmath>
#include <iostream>

#include "terrain.h"
#include "example.h"

Terrain::Terrain()
{
    m_vertexBuffer = m_indexBuffer = m_colorBuffer = 0;
}

void Terrain::generateVertices(const vector<float> heights, int width)
{
    //Generate the vertices
    int i = 0;
    for (float z = float(-width / 2); z <= (width/2); ++z) 
    {
        for (float x = float(-width / 2); x <= (width/2); ++x) 
        {
            m_vertices.push_back(Vertex(x, heights[i++], z));
        }
    }

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Terrain::generateIndices(int width)
{
    /*
        We loop through building the triangles that
        make up each grid square in the heightmap

        (z*w+x) *----* (z*w+x+1)
                |   /| 
                |  / | 
                | /  |
     ((z+1)*w+x)*----* ((z+1)*w+x+1)
    */
    //Generate the triangle indices
    for (int z = 0; z < width - 1; ++z) //Go through the rows - 1
    {
        for (int x = 0; x < width - 1; ++x) //And the columns - 1
        {
            m_indices.push_back((z * width) + x); //Current point        
            m_indices.push_back(((z + 1) * width) + x); //Next row
            m_indices.push_back((z * width) + x + 1); //Same row, but next column

            m_indices.push_back(((z + 1) * width) + x); //Next row
            m_indices.push_back(((z + 1) * width) + x + 1); //Next row, next column
            m_indices.push_back((z * width) + x + 1); //Same row, but next column
        }
    }

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
}

Vertex* crossProduct(Vertex* out, Vertex* v1, Vertex* v2)
{
    Vertex v;
    v.x = (v1->y * v2->z) - (v1->z * v2->y);
	v.y = (v1->z * v2->x) - (v1->x * v2->z);
	v.z = (v1->x * v2->y) - (v1->y * v2->x);

    out->x = v.x;
    out->y = v.y;
    out->z = v.z;

    return out;
}

Vertex* normalize(Vertex* in) 
{
    float l = sqrtf(in->x * in->x + in->y * in->y + in->z * in->z);
    in->x = in->x / l;
    in->y = in->y / l;
    in->z = in->z / l;
    return in;
}

void Terrain::generateNormals() 
{
    vector<Vertex> faceNormals; //Temporary array to store the face normals
    vector<int> shareCount;

    m_normals.resize(m_vertices.size()); //We want a normal for each vertex
    shareCount.resize(m_vertices.size());

    for (unsigned int i = 0; i < shareCount.size(); ++i) 
    {
        shareCount[i] = 0;
    }

    unsigned int numTriangles = m_indices.size() / 3;

    faceNormals.resize(numTriangles); //One normal per triangle

    for (unsigned int i = 0; i < numTriangles; ++i)
    {
        Vertex* v1 = &m_vertices[m_indices[i*3]];
        Vertex* v2 = &m_vertices[m_indices[(i*3)+1]];
        Vertex* v3 = &m_vertices[m_indices[(i*3)+2]];

        Vertex vec1, vec2;

        vec1.x = v2->x - v1->x;
        vec1.y = v2->y - v1->y;
        vec1.z = v2->z - v1->z;

        vec2.x = v3->x - v1->x;
        vec2.y = v3->y - v1->y;
        vec2.z = v3->z - v1->z;

        Vertex* normal = &faceNormals[i];
        crossProduct(normal, &vec1, &vec2); //Calculate the normal
        normalize(normal);

        for (int j = 0; j < 3; ++j) 
        {
            int index = m_indices[(i*3)+j];
            m_normals[index].x += normal->x;
            m_normals[index].y += normal->y;
            m_normals[index].z += normal->z;
            shareCount[index]++;
        }
    }

    for (unsigned int i = 0; i < m_vertices.size(); ++i)
    {
        m_normals[i].x = m_normals[i].x / shareCount[i];
        m_normals[i].y = m_normals[i].y / shareCount[i];
        m_normals[i].z = m_normals[i].z / shareCount[i];
        normalize(&m_normals[i]);
    }

    glGenBuffers(1, &m_normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_normals.size() * 3, &m_normals[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Terrain::generateTexCoords(int width)
{
    for (int z = 0; z < width; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            float s = (float(x) / float(width)) * 8.0f;
            float t = (float(z) / float(width)) * 8.0f;
            m_texCoords.push_back(TexCoord(s, t));
        }
    }

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size() * 2, &m_texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool Terrain::loadHeightmap(const string& rawFile, int width) 
{
    const float HEIGHT_SCALE = 10.0f; 
    std::ifstream fileIn(rawFile.c_str(), std::ios::binary);

    if (!fileIn.good()) 
    {
        std::cout << "File does not exist" << std::endl;
        return false;
    }

    //This line reads in the whole file into a string
    string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));

    fileIn.close();

    if (stringBuffer.size() != (width * width)) 
    {
        std::cout << "Image size does not match passed width" << std::endl;
        return false;
    }

    vector<float> heights;
    heights.reserve(width * width); //Reserve some space (faster)

    //Go through the string converting each character to a float and scale it
    for (int i = 0; i < (width * width); ++i) 
    {
        //Convert to floating value, the unsigned char cast is importantant otherwise the values wrap at 128
        float value = (float)(unsigned char)stringBuffer[i] / 256.0f; 
    
        heights.push_back(value * HEIGHT_SCALE);
        m_colors.push_back(Color(value, value, value));
    }

    glGenBuffers(1, &m_colorBuffer); //Generate a buffer for the colors
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer); //Bind the color buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_colors.size() * 3, &m_colors[0], GL_STATIC_DRAW); //Send the data to OpenGL

    generateVertices(heights, width);
    generateIndices(width);
    generateTexCoords(width);
    generateNormals();

    return true;
}

void Terrain::render()
{
    //Bind the vertex array and set the vertex pointer to point at it
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    //glVertexPointer(3, GL_FLOAT, 0, 0);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    //Bind the color array
  //  glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    //glColorPointer(3, GL_FLOAT, 0, 0);
  //  glVertexAttribPointer((GLint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
    glVertexAttribPointer((GLint)2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Bind the index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    //Draw the triangles
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
}
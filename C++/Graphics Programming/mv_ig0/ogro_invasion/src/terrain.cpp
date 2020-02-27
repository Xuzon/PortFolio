#ifdef _WIN32
#include <windows.h>
#endif

#include <fstream>
#include <cmath>
#include <iostream>
#include <cassert>
#include <vector>

#include "glee/GLee.h"
#include "terrain.h"
#include "example.h"
#include "GL/glu.h"
#include "glslshader.h"

using std::vector;
using std::string;

Terrain::Terrain(const string& vertexShader, const string& fragmentShader, const string& waterVert, const string& waterFrag):
m_vertexBuffer(0),
m_indexBuffer(0),
m_colorBuffer(0),
m_isMultitextureEnabled(true),
m_width(0),
m_shaderProgram(NULL),
m_waterShaderProgram(NULL)
{
    m_shaderProgram = new GLSLProgram(vertexShader, fragmentShader);
    if (!waterFrag.empty() && !waterVert.empty())
    {
        m_waterShaderProgram = new GLSLProgram(waterVert, waterFrag);
    }
}

Terrain::~Terrain()
{
    delete m_shaderProgram;
    delete m_waterShaderProgram;
}

void Terrain::generateVertices(const vector<float> heights, int width)
{
    //Generate the vertices
    int i = 0;
    float halfWidth = float(width) * 0.5f;

    for (int z = 0; z < width; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            m_vertices.push_back(Vertex(float(x) - halfWidth, heights[i++], float(z) - halfWidth));
        }
    }

    /*for (float z = -halfWidth; z < halfWidth; z += 1.0f)
    {
        for (float x = -halfWidth; x < halfWidth; x += 1.0f)
        {
            m_vertices.push_back(Vertex(x, heights[i++], z));
        }
    }*/

    glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0], GL_STATIC_DRAW); //Send the data to OpenGL

    m_minX = -halfWidth;
    m_maxX = halfWidth;

    m_minZ = -halfWidth;
    m_maxZ = halfWidth;
}

void Terrain::generateWaterVertices(int width)
{
    const float waterHeight = 0.1f;
    m_waterVertices.push_back(Vertex((float)-width, waterHeight, (float)-width));
    m_waterVertices.push_back(Vertex((float)-width, waterHeight, (float) width));
    m_waterVertices.push_back(Vertex((float) width, waterHeight, (float) width));
    m_waterVertices.push_back(Vertex((float) width, waterHeight, (float)-width));

    /*const float waterStep = 4.0f;

    int i = 0;
    for (float z = float(-width); z <= float(width); z += waterStep)
    {
        for (float x = float(-width); x <= float(width); x += waterStep)
        {
            m_waterVertices.push_back(Vertex(x, waterHeight, z));
        }
    }*/

    glGenBuffers(1, &m_waterVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_waterVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_waterVertices.size() * 3, &m_waterVertices[0], GL_STATIC_DRAW);
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

void Terrain::generateWaterIndices(int width)
{
    m_waterIndices.push_back(0);
    m_waterIndices.push_back(1);
    m_waterIndices.push_back(2);

    m_waterIndices.push_back(0);
    m_waterIndices.push_back(2);
    m_waterIndices.push_back(3);

    /*int waterWidth = (int)sqrtf((float)m_waterVertices.size());

    //Generate the triangle indices
    for (int z = 0; z < waterWidth - 1; ++z) //Go through the rows - 1
    {
        for (int x = 0; x < waterWidth - 1; ++x) //And the columns - 1
        {
            m_waterIndices.push_back((z * waterWidth) + x); //Current point
            m_waterIndices.push_back(((z + 1) * waterWidth) + x); //Next row
            m_waterIndices.push_back((z * waterWidth) + x + 1); //Same row, but next column

            m_waterIndices.push_back(((z + 1) * waterWidth) + x); //Next row
            m_waterIndices.push_back(((z + 1) * waterWidth) + x + 1); //Next row, next column
            m_waterIndices.push_back((z * waterWidth) + x + 1); //Same row, but next column
        }
    }*/

    glGenBuffers(1, &m_waterIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_waterIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_waterIndices.size(), &m_waterIndices[0], GL_STATIC_DRAW);
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
    float maxHeight = -1000;
    float minHeight = 1000;

    for (int z = 0; z < width; ++z)
    {
        for (int x = 0; x < width; ++x)
        {
            float s = (float(x) / float(width)) * 8.0f;
            float t = (float(z) / float(width)) * 8.0f;
            m_texCoords.push_back(TexCoord(s, t));

            //Get the height at this vertex
            float h = m_vertices[(z * width) + x].y;
            if (h > maxHeight) maxHeight = h;
            if (h < minHeight) minHeight = h;
            m_heightTexCoords.push_back(h);
        }
    }

    //We scale the heights between 0.0 and 1.0 using the max height and min height
    for (vector<float>::iterator height = m_heightTexCoords.begin();
        height != m_heightTexCoords.end(); ++height)
    {
       (*height) = (minHeight + (*height)) / (maxHeight - minHeight);
    }

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer); //Bind the texcoord buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_texCoords.size() * 2, &m_texCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL

    glGenBuffers(1, &m_heightTexCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_heightTexCoordBuffer); //Bind the tex coord
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_heightTexCoords.size(), &m_heightTexCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

void Terrain::generateWaterTexCoords(int width)
{
    m_waterTexCoords.push_back(TexCoord(0.0f, 0.0f));
    m_waterTexCoords.push_back(TexCoord(0.0f, 16.0f));
    m_waterTexCoords.push_back(TexCoord(16.0f, 16.0f));
    m_waterTexCoords.push_back(TexCoord(16.0f, 0.0f));

    /*int waterWidth = (int)sqrtf((float)m_waterVertices.size());

    for (int z = 0; z < waterWidth; ++z)
    {
        for (int x = 0; x < waterWidth; ++x)
        {
            float s = (float(x) / float(width)) * 32.0f;
            float t = (float(z) / float(width)) * 32.0f;
            m_waterTexCoords.push_back(TexCoord(s, t));
        }
    }*/

    glGenBuffers(1, &m_waterTexCoordsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_waterTexCoordsBuffer); //Bind the vertex buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * m_waterTexCoords.size() * 2, &m_waterTexCoords[0], GL_STATIC_DRAW); //Send the data to OpenGL
}

bool Terrain::loadHeightmap(const string& rawFile, const string& grassTexture, const string& heightTexture, int width, bool generateWater, const string& waterTexture)
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

    if (stringBuffer.size() != (unsigned) (width * width))
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
        m_colors.push_back(Color(value, value, value, 1.0f));
    }

    glGenBuffers(1, &m_colorBuffer); //Generate a buffer for the colors
    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer); //Bind the color buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * m_colors.size(), &m_colors[0], GL_STATIC_DRAW); //Send the data to OpenGL

    generateVertices(heights, width);
    generateIndices(width);
    generateTexCoords(width);
    generateNormals();

    if (generateWater)
    {
        generateWaterVertices(width);
        generateWaterIndices(width);
        generateWaterTexCoords(width);

        if (!m_waterTexture.load(waterTexture))
        {
            std::cerr << "Could not load the water texture" << std::endl;
            return false;
        }

        if (!m_waterShaderProgram->initialize())
        {
            std::cerr << "Could not initialize the water shader" << std::endl;
            return false;
        }

        m_waterShaderProgram->bindShader();
        m_waterShaderProgram->bindAttrib(0, "a_Vertex");
        m_waterShaderProgram->bindAttrib(1, "a_TexCoord0");
        m_waterShaderProgram->linkProgram();
        m_waterShaderProgram->sendUniform("texture0", 0);

        glGenTextures(1, &m_waterTexID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_waterTexID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_waterTexture.getWidth(),
                          m_waterTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                          m_waterTexture.getImageData());



    }

    m_width = width;

    if (!m_grassTexture.load(grassTexture))
    {
        std::cerr << "Could not load the grass texture" << std::endl;
        return false;
    }

    if (!m_heightTexture.load(heightTexture))
    {
        std::cerr << "Could not load the height texture" << std::endl;
        return false;
    }


    glGenTextures(1, &m_grassTexID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_grassTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, m_grassTexture.getWidth(),
                      m_grassTexture.getHeight(), GL_RGB, GL_UNSIGNED_BYTE,
                      m_grassTexture.getImageData());

    glGenTextures(1, &m_heightTexID);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, m_heightTexID);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

    glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB8, m_heightTexture.getWidth(),
                      0, GL_RGB, GL_UNSIGNED_BYTE,
                      m_heightTexture.getImageData());

    if (!m_shaderProgram->initialize())
    {
        std::cerr << "Could not initialize the terrain's shader" << std::endl;
        return false;
    }

	m_shaderProgram->bindAttrib(0, "a_Vertex");
    m_shaderProgram->bindAttrib(1, "a_TexCoord0");
    m_shaderProgram->bindAttrib(2, "a_Normal");
    m_shaderProgram->bindAttrib(3, "a_TexCoord1");
    m_shaderProgram->linkProgram();
    m_shaderProgram->bindShader();

    m_shaderProgram->sendUniform("texture0", 0);
    m_shaderProgram->sendUniform("texture1", 1); //Set the uniform for the height texture

    //Send the lighting properties
    m_shaderProgram->sendUniform("material_ambient", 0.4f, 0.8f, 0.4f, 1.0f);
    m_shaderProgram->sendUniform("material_diffuse", 0.8f, 0.8f, 0.8f, 1.0f);
    m_shaderProgram->sendUniform("material_specular", 0.6f, 0.6f, 0.6f, 1.0f);
    m_shaderProgram->sendUniform("material_emissive", 0.0f, 0.0f, 0.0f, 1.0f);
    m_shaderProgram->sendUniform("material_shininess", 10.0f);
    m_shaderProgram->sendUniform("light0.ambient", 0.3f, 0.3f, 0.3f, 1.0f);
    m_shaderProgram->sendUniform("light0.diffuse", 1.0f, 1.0f, 1.0f, 1.0f);
    m_shaderProgram->sendUniform("light0.specular", 0.3f, 0.3f, 0.3f, 1.0f);
    m_shaderProgram->sendUniform("light0.position", 0.0f, 0.4f, 1.0f, 0.0f);

    return true;
}

Vertex Terrain::getPositionAt(int x, int z)
{
    assert((z * m_width) + x < (int)m_vertices.size());
    return m_vertices[(z * m_width) + x];
}

void Terrain::renderWater() const
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    m_waterShaderProgram->bindShader();

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    //Send the modelview and projection matrices to the shaders
    m_waterShaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_waterShaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

    glBindTexture(GL_TEXTURE_2D, m_waterTexID);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_waterVertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_waterTexCoordsBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_waterIndexBuffer);

    m_waterShaderProgram->bindShader();

    glDrawElements(GL_TRIANGLES, m_waterIndices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    //glDisable(GL_BLEND);
}

void Terrain::render() const
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    m_shaderProgram->bindShader();

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
    vector<float> normalMatrix = GLSLProgram::calculateNormalMatrix(modelviewMatrix);

    //Send the modelview and projection matrices to the shaders
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_shaderProgram->sendUniform3x3("normal_matrix", &normalMatrix[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_grassTexID);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, m_heightTexID);


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

    glBindBuffer(GL_ARRAY_BUFFER, m_heightTexCoordBuffer);
    glVertexAttribPointer((GLint)3, 1, GL_FLOAT, GL_FALSE, 0, 0);

    //Bind the index array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    //Draw the triangles
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);

    glActiveTexture(GL_TEXTURE0);
}

GLfloat Terrain::getHeightAt(GLfloat x, GLfloat z)
{
    float halfWidth = float(m_width) * 0.5f;

    float scaledX = x + halfWidth;
    float scaledZ = z + halfWidth;

/*
    Round down to get the x and z position near where we are
*/
    int x0 = (int)floor(scaledX);
    int z0 = (int)floor(scaledZ);

/*
    Get the 4 points surrounding the position passed in
*/
    int p0 = (z0 * m_width + x0);
    int p1 = ((z0 * m_width + x0) + 1);
    int p2 = ((z0 + 1) * m_width + x0);
    int p3 = ((z0 + 1) * m_width + x0 + 1);

    float fracX = scaledX - (float)x0;
    float fracZ = scaledZ - (float)z0;

/*
    If we are outside the bounds of the map, just return zero as the height
*/
    if (p0 >= (int)m_vertices.size() ||
		p1 >= (int)m_vertices.size() ||
		p2 >= (int)m_vertices.size() ||
		p3 >= (int)m_vertices.size() ||
		p0 < 0 || p1 < 0 || p2 < 0 || p3 < 0)
    {
        return 0.0f;
    }

/*
    Bilinearly interpolate the height values
*/
    float xInterp0 = m_vertices[p0].y + fracX * (m_vertices[p1].y - m_vertices[p0].y);
    float xInterp1 = m_vertices[p2].y + fracX * (m_vertices[p3].y - m_vertices[p2].y);
    return xInterp0 + fracZ * (xInterp1 - xInterp0);
}

void Terrain::normalizeTerrain()
{
    float miny = 1000, maxy = -1000;

    for (vector<Vertex>::iterator v = m_vertices.begin();
        v != m_vertices.end(); ++v)
    {
        if ((*v).y < miny) miny = (*v).y;
        if ((*v).y > maxy) maxy = (*v).y;
    }

    float h = maxy - miny;

    for (vector<Vertex>::iterator v = m_vertices.begin();
        v != m_vertices.end(); ++v)
    {
        (*v).y /= h;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0]);
}

void Terrain::scaleHeights(float scale)
{
    for (vector<Vertex>::iterator v = m_vertices.begin();
        v != m_vertices.end(); ++v)
    {
        (*v).y *= scale;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * m_vertices.size() * 3, &m_vertices[0]);
}

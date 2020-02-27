#ifndef BOGLGP_TERRAIN_H
#define BOGLGP_TERRAIN_H

#ifdef WIN32
#include <windows.h>
#endif

#include <string>
#include <vector>
#include "glee/GLee.h"
#include "targa.h"
#include "geom.h"

class GLSLProgram;

class Terrain
{
public:
    Terrain(const std::string& vertexShader, const std::string& fragmentShader, const std::string& waterVert="", const std::string& waterFrag="");
    virtual ~Terrain();

    bool loadHeightmap(const std::string& rawFile, const std::string& grassTexture, const std::string& heightTexture, int width, bool generateWater=false, const std::string& waterTexture="");
    void render() const;
    void renderWater() const;

    Vertex getPositionAt(int x, int z);
    GLfloat getHeightAt(GLfloat x, GLfloat z);

    void normalizeTerrain();
    void scaleHeights(float scale);

    float getMinX() { return m_minX; }
    float getMaxX() { return m_maxX; }
    float getMinZ() { return m_minZ; }
    float getMaxZ() { return m_maxZ; }
private:
    void generateVertices(const std::vector<float> heights, int width);
    void generateIndices(int width);
    void generateTexCoords(int width);
    void generateNormals();

    void generateWaterVertices(int width);
    void generateWaterIndices(int width);
    void generateWaterTexCoords(int width);

    GLuint m_vertexBuffer;
    GLuint m_indexBuffer;
    GLuint m_colorBuffer;
    GLuint m_texCoordBuffer;
    GLuint m_heightTexCoordBuffer;

    GLuint m_normalBuffer;

    GLuint m_waterVertexBuffer;
    GLuint m_waterIndexBuffer;
    GLuint m_waterTexCoordsBuffer;

    std::vector<Vertex> m_vertices;
    std::vector<Color> m_colors;
    std::vector<TexCoord> m_texCoords;
    std::vector<float> m_heightTexCoords;

    std::vector<GLuint> m_indices;
    std::vector<Vertex> m_normals;

    std::vector<Vertex> m_waterVertices;
    std::vector<GLuint> m_waterIndices;
    std::vector<TexCoord> m_waterTexCoords;

    TargaImage m_grassTexture;
    TargaImage m_heightTexture;
    TargaImage m_waterTexture;

    GLuint m_grassTexID;
    GLuint m_heightTexID;
    GLuint m_waterTexID;

    bool m_isMultitextureEnabled;
    int m_width;

    GLSLProgram* m_shaderProgram;
    GLSLProgram* m_waterShaderProgram;

    float m_minX;
    float m_maxX;
    float m_minZ;
    float m_maxZ;

};

#endif

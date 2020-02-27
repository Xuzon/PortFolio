#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>
#include <fstream>

#include "glslshader.h"
#include "md2model.h"

using std::ifstream;
using std::string;
using std::vector;


const Animation Animation::IDLE = Animation(0, 39);
const Animation Animation::RUN = Animation(40, 45);
const Animation Animation::ATTACK = Animation(46, 53);
const Animation Animation::PAIN1 = Animation(54, 57);
const Animation Animation::PAIN2 = Animation(58, 61);
const Animation Animation::PAIN3 = Animation(62, 65);
const Animation Animation::JUMP = Animation(66, 71);
const Animation Animation::FLIP_OFF = Animation(72, 83);
const Animation Animation::SAULTE = Animation(84, 94);
const Animation Animation::TAUNT = Animation(95, 111);
const Animation Animation::WAVE = Animation(112, 122);
const Animation Animation::POINT = Animation(123, 134);
const Animation Animation::CROUCH_IDLE = Animation(135, 153);
const Animation Animation::CROUCH_WALK = Animation(154, 159);
const Animation Animation::CROUCH_ATTACK = Animation(160, 168);
const Animation Animation::CROUCH_PAIN = Animation(169, 172);
const Animation Animation::CROUCH_DEATH = Animation(173, 177);
const Animation Animation::DEATH1 = Animation(178, 183, false);
const Animation Animation::DEATH2 = Animation(184, 189, false);
const Animation Animation::DEATH3 = Animation(190, 197, false);

MD2Model::MD2Model(const std::string vertexShader, const std::string fragmentShader):
m_startFrame(0),
m_endFrame(0),
m_currentFrame(0),
m_nextFrame(1),
m_interpolation(0.0f),
m_shaderProgram(NULL)
{
    m_shaderProgram = new GLSLProgram(vertexShader, fragmentShader);
}

MD2Model::~MD2Model()
{
    delete m_shaderProgram;
}

bool MD2Model::load(const string& filename)
{
    ifstream fileIn(filename.c_str(), std::ios::binary);

    if (!fileIn.good()) {
        return false;
    }

    MD2Header header;

    //Read the header
    fileIn.read(reinterpret_cast<char*>(&header), sizeof(MD2Header));

    //If the MD2 file is the wrong version then return false
    if (std::string(header.ident) == "IDP2" || header.version != 8) {
        return false;
    }

    //Allocate memory so that we can store the loaded data
    m_skins.resize(header.numSkins);

    //Allocate space for our final texture coordinates
    m_texCoords.resize(header.numTextureCoords);

    //And space for the ones read directly from the file before conversion
    m_md2TexCoords.resize(header.numTextureCoords);

    m_triangles.resize(header.numTriangles);

    //Allocate space for the key frames
    m_keyFrames.resize(header.numFrames);

    //Go through the key frames and allocate space for the vertices
    //in each one
    for (vector<KeyFrame>::iterator frame = m_keyFrames.begin();
        frame != m_keyFrames.end(); ++frame) {

        (*frame).vertices.resize(header.numVertices);
        (*frame).MD2verts.resize(header.numVertices);
    }


    fileIn.seekg(header.skinOffset);
    fileIn.read(reinterpret_cast<char*>(&m_skins[0]), sizeof(Skin) * header.numSkins);

    fileIn.seekg(header.texCoordOffset);
    fileIn.read(reinterpret_cast<char*>(&m_md2TexCoords[0]), sizeof(MD2TexCoord) * header.numTextureCoords);

    fileIn.seekg(header.triangleOffset);
    fileIn.read(reinterpret_cast<char*>(&m_triangles[0]), sizeof(Triangle) * header.numTriangles);

    fileIn.seekg(header.frameOffset);

    for (int i = 0; i < header.numFrames; ++i) {
        KeyFrame* f = &m_keyFrames[i];

        fileIn.read(reinterpret_cast<char*>(f->scale), sizeof(float) * 3);
        fileIn.read(reinterpret_cast<char*>(f->translate), sizeof(float) * 3);
        fileIn.read(reinterpret_cast<char*>(f->name), sizeof(char) * 16);
        fileIn.read(reinterpret_cast<char*>(&f->MD2verts[0]), sizeof(MD2Vertex) * header.numVertices);
    }



    // Scale the vertices
    m_radii.clear();
    for (vector<KeyFrame>::iterator frame = m_keyFrames.begin();
        frame != m_keyFrames.end(); ++frame) {

        float min = 10000, max = -10000;

        int k = 0;
        for (vector<Vertex>::iterator vertex = (*frame).vertices.begin();
                vertex != (*frame).vertices.end(); ++vertex) {

            (*vertex).x = ((*frame).scale[0] * (*frame).MD2verts[k].v[0] + (*frame).translate[0]) / 64.0f;
            (*vertex).z = ((*frame).scale[1] * (*frame).MD2verts[k].v[1] + (*frame).translate[1]) / 64.0f;
            (*vertex).y = ((*frame).scale[2] * (*frame).MD2verts[k].v[2] + (*frame).translate[2]) / 64.0f;
            k++;

            if ((*vertex).y < min) min = (*vertex).y;
            if ((*vertex).y > max) max = (*vertex).y;
        }

        float frameRadius = (max - min) / 2.0f;
        m_radii.push_back(frameRadius);
    }



    int i = 0;
    for (vector<TexCoord>::iterator texCoord = m_texCoords.begin();
        texCoord != m_texCoords.end(); ++texCoord) {

            (*texCoord).s = (float(m_md2TexCoords[i].s) / (float)header.skinWidth);
            (*texCoord).t = 1.0f - (float(m_md2TexCoords[i].t) / (float)header.skinHeight);
            ++i;
    }
    fileIn.close();

    reorganizeVertices();
    stripTextureNames();

    //The current frame is the interpolated frame being rendered
    //it will usually be midway between 2 frames of animation
    m_interpolatedFrame.vertices = m_keyFrames[0].vertices;

    generateBuffers();

    if (!m_shaderProgram->initialize())
    {
        std::cerr << "Could not load the shaders for the MD2 model" << std::endl;
        return false;
    }

    m_shaderProgram->bindAttrib(0, "a_Vertex");
    m_shaderProgram->bindAttrib(1, "a_TexCoord0");
    m_shaderProgram->linkProgram();

    return true;
}


void MD2Model::generateBuffers() {
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * m_interpolatedFrame.vertices.size(), &m_interpolatedFrame.vertices[0], GL_DYNAMIC_DRAW);

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * m_texCoords.size(), &m_texCoords[0], GL_STATIC_DRAW);
}

void MD2Model::update(float dt)
{
    const float FRAMES_PER_SECOND = 8.0f;
    m_interpolation += dt * FRAMES_PER_SECOND;
    if (m_interpolation >= 1.0f) {
        m_currentFrame = m_nextFrame;
        m_nextFrame++;
        if (m_nextFrame > m_endFrame) {
            if (m_loopAnimation)
            {
                m_nextFrame = m_startFrame;
            }
            else
            {
                m_nextFrame = m_endFrame;
                m_startFrame = m_endFrame;
            }
        }
        m_interpolation = 0.0f;
    }

    float t = m_interpolation;
    int i = 0;
    for (vector<Vertex>::iterator vertex = m_interpolatedFrame.vertices.begin();
        vertex != m_interpolatedFrame.vertices.end(); ++vertex) {

	    float x1 = m_keyFrames[m_currentFrame].vertices[i].x;
	    float x2 = m_keyFrames[m_nextFrame].vertices[i].x;
	    (*vertex).x = x1 + t * (x2 - x1);

	    float y1 = m_keyFrames[m_currentFrame].vertices[i].y;
	    float y2 = m_keyFrames[m_nextFrame].vertices[i].y;
	    (*vertex).y = y1 + t * (y2 - y1);

	    float z1 = m_keyFrames[m_currentFrame].vertices[i].z;
	    float z2 = m_keyFrames[m_nextFrame].vertices[i].z;
	    (*vertex).z = z1 + t * (z2 - z1);

        ++i;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * m_interpolatedFrame.vertices.size(), &m_interpolatedFrame.vertices[0]);
}

void MD2Model::render()
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    m_shaderProgram->bindShader();
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_shaderProgram->sendUniform("texture0", 0);

    //glBindTexture(GL_TEXTURE_2D, m_treeTexID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, m_interpolatedFrame.vertices.size());

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

/**
There is a slight problem rendering md2 models
using glDrawArrays or glDrawElements. The vertices
can be reused using indices, but because the same vertex
might have a different texture coordinate depending on the
triangle, the texture coordinates will not always be correct.
To fix this we go through each frame, duplicating some vertices
and removing the need to use indices. The texture coordinates
are inserted to match the vertices. */

void MD2Model::reorganizeVertices() {
    //Go through each frame
    vector<Vertex> tempVertices;
    vector<TexCoord> tempTexCoords;

    bool texCoordsDone = false;

	//Go through all the frames
    for (vector<KeyFrame>::iterator frame = m_keyFrames.begin(); frame != m_keyFrames.end(); ++frame) {
        tempVertices.clear();

		//Then go through the triangles
        for (unsigned int i = 0; i < m_triangles.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                //Push back the 3 vertices for this triangle
                tempVertices.push_back((*frame).vertices[m_triangles[i].vertexIndex[j]]);

				//If this is the first frame, do the same for the texture coordinates
                if (!texCoordsDone) {
                    tempTexCoords.push_back(m_texCoords[m_triangles[i].texCoordIndex[j]]);
                }
            }
        }
        texCoordsDone = true; //Texture coords are shared between frames, we only do it once

		//Copy the new vertex array over the original
        (*frame).vertices = tempVertices;
    }
	//Copy the new texture coordinate array over the original
    m_texCoords = tempTexCoords;
}

/**
    MD2 models can have multiple "skins". These are PCX images and tend to live in the folders
    that Quake 2 stored them in like players/blah/something.pcx. We
*/
void MD2Model::stripTextureNames()
{
    for(vector<Skin>::iterator skin = m_skins.begin(); skin != m_skins.end(); ++skin) {
        string texture = (*skin).name;

        string::size_type lastSlash = texture.find_last_of("/") + 1;
        string::size_type lastDot = texture.find_last_of(".");

        string textureName = texture.substr(lastSlash, lastDot - lastSlash);
        m_textureNames.push_back(textureName);
    }
}

#ifndef BOGLGP_MD2_H
#define BOGLGP_MD2_H

#include "glee/GLee.h"

#include <vector>
#include <string>
#include "geom.h"

class GLSLProgram;

struct Animation {
    int startFrame;
    int endFrame;
    bool loop;

    Animation(int startFrame, int endFrame, bool loop=true):
    startFrame(startFrame),
    endFrame(endFrame),
    loop(loop) { }

    static const Animation IDLE;
    static const Animation RUN;
    static const Animation ATTACK;
    static const Animation PAIN1;
    static const Animation PAIN2;
    static const Animation PAIN3;
    static const Animation JUMP;
    static const Animation FLIP_OFF;
    static const Animation SAULTE;
    static const Animation TAUNT;
    static const Animation WAVE;
    static const Animation POINT;
    static const Animation CROUCH_IDLE;
    static const Animation CROUCH_WALK;
    static const Animation CROUCH_ATTACK;
    static const Animation CROUCH_PAIN;
    static const Animation CROUCH_DEATH;
    static const Animation DEATH1;
    static const Animation DEATH2;
    static const Animation DEATH3;
};

class MD2Model
{
public:
    MD2Model(const std::string vertexShader, const std::string fragmentShader);
    virtual ~MD2Model();

    bool load(const std::string& filename);
    void update(float dt);
    void render();

    void setAnimation(int start, int end) {
        m_startFrame = start;
        m_endFrame = end;
        m_nextFrame = m_startFrame;
    }

    void setAnimation(const Animation& ani) {
        setAnimation(ani.startFrame, ani.endFrame);
        m_loopAnimation = ani.loop;
    }

    std::vector<std::string> getSkinNames() {
        return m_textureNames;
    }

    float getRadius()
    {
        if (m_currentFrame < 0 || m_currentFrame > (int) m_radii.size())
        {
            return m_radii[0];
        }
        return m_radii[m_currentFrame]; //Return the current radius
    }

private:

    void reorganizeVertices();
    void stripTextureNames();

    void generateBuffers();

    struct MD2Header {
        char ident[4];             // Must be equal to "IDP2"
        int version;           // MD2 version

        int skinWidth;         // texture width
        int skinHeight;        // height of the texture
        int frameSize;         // size of one frame in bytes

        int numSkins;          // number of textures
        int numVertices;       // number of vertices
        int numTextureCoords;  // number of texture coordinates
        int numTriangles;      // number of triangles
        int numGLCmds;         // number of opengl commands
        int numFrames;         // total number of frames

        int skinOffset;        // offset to skin names (64 bytes each)
        int texCoordOffset;    // offset to s-t texture coordinates
        int triangleOffset;    // offset to triangles
        int frameOffset;       // offset to frame data
        int GLCmdOffset;       // offset to opengl commands
        int eofOffset;         // offset to end of file
    };

    struct MD2Vertex {
        unsigned char v[3];
        unsigned char lightNormalIndex;
    };

    struct MD2TexCoord {
        short s;
        short t;
    };

    struct KeyFrame {
        float scale[3];
        float translate[3];
        char name[16];
        std::vector<MD2Vertex> MD2verts;
        std::vector<Vertex> vertices;
    };

    struct Triangle {
        short vertexIndex[3];
        short texCoordIndex[3];
    };

    struct Skin {
	    char name[64];  //texture file name
    };

    std::vector<Skin> m_skins;
    std::vector<MD2TexCoord> m_md2TexCoords;
    std::vector<Triangle> m_triangles; //These are loaded from the MD2 file

    std::vector<TexCoord> m_texCoords;
    std::vector<KeyFrame> m_keyFrames;
    std::vector<std::string> m_textureNames;

    KeyFrame m_interpolatedFrame;

    int m_startFrame;
    int m_endFrame;
    int m_currentFrame;
    int m_nextFrame;
    float m_interpolation;
    bool m_loopAnimation;

    GLuint m_vertexBuffer;
    GLuint m_texCoordBuffer;

    GLSLProgram* m_shaderProgram;

    std::vector<float> m_radii; //Store the radius for each frame
};

#endif

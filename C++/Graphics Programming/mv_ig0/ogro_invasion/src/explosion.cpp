#ifdef _WIN32
#include <windows.h>
#endif

#include <string>
#include <cstdlib>
#include "glee/GLee.h"
#include <GL/glu.h>

#include "explosion.h"
#include "glslshader.h"

using std::vector;
using std::string;

const string VERTEX_SHADER_120 = "data/shaders/glsl1.20/particle.vert";
const string FRAGMENT_SHADER_120 = "data/shaders/glsl1.20/particle.frag";

const string VERTEX_SHADER_130 = "data/shaders/glsl1.30/particle.vert";
const string FRAGMENT_SHADER_130 = "data/shaders/glsl1.30/particle.frag";

const string PARTICLE_TEXTURE = "data/textures/particle.tga";

GLuint Explosion::m_particleTexID = 0;
TargaImage Explosion::m_particleTexture;
std::auto_ptr<GLSLProgram> Explosion::m_shaderProgram;
GLuint Explosion::m_vertexBuffer = 0;
GLuint Explosion::m_colorBuffer = 0;

Explosion::Explosion(GameWorld* world, unsigned int numParticles):
Entity(world),
m_particles(vector<Particle>(numParticles))
{
}

Explosion::~Explosion()
{

}

bool Explosion::onInitialize()
{
    vector<Color> colors;
    colors.push_back(Color(1.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Color(1.0f, 1.0f, 0.0f, 1.0f));
    colors.push_back(Color(1.0f, 0.5f, 0.0f, 1.0f));
    colors.push_back(Color(0.5f, 0.5f, 0.5f, 1.0f));

    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        float randX = rand() / ((float)RAND_MAX + 1) - 0.5f;
        float randY = rand() / ((float)RAND_MAX + 1) - 0.5f;
        float randZ = rand() / ((float)RAND_MAX + 1) - 0.5f;
        float randSpeed = 1.5f; //(rand() / ((double)RAND_MAX + 1)) * 2.0f;

        (*it).life = 1.0f;
        (*it).velocity = Vector3(randX, randY, randZ);
        (*it).velocity.normalize();
        (*it).velocity = (*it).velocity * randSpeed;

        (*it).position = getPosition(); //Set to the same starting point

        int randColor = rand() % colors.size();
        (*it).color = colors[randColor];
    }

    if (m_particleTexID == 0)
    {
        const string vertexShader = (GLSLProgram::glsl130Supported()) ? VERTEX_SHADER_130 : VERTEX_SHADER_120;
        const string fragmentShader = (GLSLProgram::glsl130Supported()) ? FRAGMENT_SHADER_130 : FRAGMENT_SHADER_120;

        if (!m_particleTexture.load(PARTICLE_TEXTURE))
        {
            std::cerr << "Could not load the particle texture" << std::endl;
            return false;
        }

        glGenTextures(1, &m_particleTexID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_particleTexID);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_particleTexture.getWidth(),
                          m_particleTexture.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                          m_particleTexture.getImageData());


        m_shaderProgram = std::auto_ptr<GLSLProgram>(new GLSLProgram(vertexShader, fragmentShader));

        if (!m_shaderProgram->initialize())
        {
            std::cerr << "Could not load the particle shaders" << std::endl;
            return false;
        }

        m_shaderProgram->bindAttrib(0, "a_Vertex");
        m_shaderProgram->bindAttrib(1, "a_Color");
        m_shaderProgram->linkProgram();

        glGenBuffers(1, &m_vertexBuffer); //Generate a buffer for the vertices
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer); //Bind the vertex buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * m_particles.size(), NULL, GL_DYNAMIC_DRAW); //Send the data to OpenGL

        glGenBuffers(1, &m_colorBuffer); //Generate a buffer for the colors
        glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer); //Bind the vertex buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(Color) * m_particles.size(), NULL, GL_DYNAMIC_DRAW); //Send the data to OpenGL
    }
    return true;
}

void Explosion::onPrepare(float dT)
{
    int numDead = 0;
    for (ParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        if ((*it).life <= 0.0f)
        {
            numDead++;
            continue;
        }

        (*it).life -= 1.0f * dT;
        (*it).position += (*it).velocity * dT; //Update the particle's position
    }

    if (numDead == (int) m_particles.size())
    {
        destroy();
    }

}


void Explosion::onRender() const
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    static vector<Vector3> positions;
    static vector<Color> colors;

    m_shaderProgram->bindShader();

    //Get the current matrices from OpenGL
    glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
    glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

    //Send the modelview and projection matrices to the shaders
    m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
    m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);
    m_shaderProgram->sendUniform("point_size", 50.0f);
    m_shaderProgram->sendUniform("texture0", 0);

    positions.clear();
    colors.clear();

    //Enable the point sprite and the automatic texture coordinates
    glBindTexture(GL_TEXTURE_2D, m_particleTexID);
    for (ConstParticleIterator it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        if ((*it).life <= 0.0f)
        {
            continue; //Ignore dead particles
        }

        //Add to the list of points to be rendered
        positions.push_back((*it).position);
        Color c = (*it).color;
        c.a = (*it).life;
        colors.push_back(c);
    }

    if (positions.empty())
    {
        return;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vector3) * positions.size(), &positions[0]);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Color) * colors.size(), &colors[0]);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glVertexAttribPointer((GLint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer);
    glVertexAttribPointer((GLint)1, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Draw the points
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
    glDrawArrays(GL_POINTS, 0, positions.size());
    glDisable(GL_POINT_SPRITE);

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}

void Explosion::onPostRender()
{

}

void Explosion::onShutdown()
{

}


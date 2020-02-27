#ifndef GLSL_SHADER_H_INCLUDED
#define GLSL_SHADER_H_INCLUDED

#ifdef WIN32
#include <windows.h>
#endif

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "glee/GLee.h"

using std::string;
using std::ifstream;
using std::map;
using std::vector;

class GLSLProgram
{
public:
    struct GLSLShader
    {
        unsigned int id;
        string filename;
        string source;
    };

    GLSLProgram(const string& vertexShader, const string& fragmentShader)
    {
        m_vertexShader.filename = vertexShader;
        m_fragmentShader.filename = fragmentShader;
    }

    virtual ~GLSLProgram()
    {
        unload();
    }

    void unload()
    {
        glDetachShader(m_programID, m_vertexShader.id);
        glDetachShader(m_programID, m_fragmentShader.id);
        glDeleteShader(m_vertexShader.id);
        glDeleteShader(m_fragmentShader.id);
        glDeleteProgram(m_programID);
    }

    bool initialize()
    {
        m_programID = glCreateProgram();
        m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
        m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

        m_vertexShader.source = readFile(m_vertexShader.filename);
        m_fragmentShader.source = readFile(m_fragmentShader.filename);

        if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
        {
            return false;
        }

        const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
        glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, NULL);

        tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
        glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, NULL);

        if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
        {
			std::cerr << "Could not compile the shaders, they are invalid" << std::endl;
            return false;
        }

        glAttachShader(m_programID, m_vertexShader.id);
        glAttachShader(m_programID, m_fragmentShader.id);

        glLinkProgram(m_programID);
        return true;
    }

	void linkProgram()
	{
		glLinkProgram(m_programID);

        GLint result;
        glGetProgramiv(m_programID, GL_LINK_STATUS, &result);

		if (result == GL_FALSE) {
		    outputProgramLog(m_programID);
		    throw std::runtime_error("GLSL program did not link successfully, please check the logs");
		}

	}

    GLuint getUniformLocation(const string& name)
    {
        map<string, GLuint>::iterator i = m_uniformMap.find(name);
        if (i == m_uniformMap.end())
        {
            GLuint location = glGetUniformLocation(m_programID, name.c_str());
            m_uniformMap.insert(std::make_pair(name, location));
            return location;
        }

        return (*i).second;
    }

    GLuint getAttribLocation(const string& name)
    {
        map<string, GLuint>::iterator i = m_attribMap.find(name);
        if (i == m_attribMap.end())
        {
            GLuint location = glGetAttribLocation(m_programID, name.c_str());
            m_attribMap.insert(std::make_pair(name, location));
            return location;
        }

        return (*i).second;
    }

    void sendUniform(const string& name, const int id)
    {
        GLuint location = getUniformLocation(name);
        glUniform1i(location, id);
    }

    void sendUniform4x4(const string& name, const float* matrix, bool transpose=false)
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, transpose, matrix);
    }

    void sendUniform3x3(const string& name, const float* matrix, bool transpose=false)
    {
        GLuint location = getUniformLocation(name);
        glUniformMatrix3fv(location, 1, transpose, matrix);
    }

    void sendUniform(const string& name, const float red, const float green,
                     const float blue, const float alpha)
    {
        GLuint location = getUniformLocation(name);
        glUniform4f(location, red, green, blue, alpha);
    }

    void sendUniform(const string& name, const float x, const float y,
                     const float z)
    {
        GLuint location = getUniformLocation(name);
        glUniform3f(location, x, y, z);
    }

    void sendUniform(const string& name, const float scalar)
    {
        GLuint location = getUniformLocation(name);
        glUniform1f(location, scalar);
    }

    void bindAttrib(unsigned int index, const string& attribName)
    {
        glBindAttribLocation(m_programID, index, attribName.c_str());
    }

    void bindShader()
    {
        glUseProgram(m_programID);
    }

    /**
    Returns an array of 3x3 floats representing a suitable normal
    matrix. This returns the inverse transpose of the passed in matrix

    If you are going to normalize your normal after multiplication with
    this matrix then you can use the transpose(adjoint(M)) rather than
    the transpose(inverse(M)). The adjoint is like the inverse but isnt
    divided by the matrix determinate.
    */
    static std::vector<float> calculateNormalMatrix(const float* modelviewMatrix)
    {
        /*
            0   1   2
        0   0   3   6
        1   1   4   7
        2   2   5   8
        */

        //Grab the top 3x3 of the modelview matrix
        std::vector<float> M(3 * 3);
        M[0] = modelviewMatrix[0];
        M[1] = modelviewMatrix[1];
        M[2] = modelviewMatrix[2];
        M[3] = modelviewMatrix[4];
        M[4] = modelviewMatrix[5];
        M[5] = modelviewMatrix[6];
        M[6] = modelviewMatrix[8];
        M[7] = modelviewMatrix[9];
        M[8] = modelviewMatrix[10];

        //Work out the determinate
        float determinate = M[0] * M[4] * M[8] + M[1] * M[5] * M[6] + M[2] * M[3] * M[7];
        determinate -= M[2] * M[4] * M[6] + M[0] * M[5] * M[7] + M[1] * M[3] * M[8];

        //One division is faster than several
        float oneOverDet = 1.0f / determinate;

        std::vector<float> N(3 * 3);

        //Calculate the inverse and assign it to the transpose matrix positions
        N[0] = (M[4] * M[8] - M[5] * M[7]) * oneOverDet;
        N[3] = (M[2] * M[7] - M[1] * M[8]) * oneOverDet;
        N[6] = (M[1] * M[5] - M[2] * M[4]) * oneOverDet;

        N[1] = (M[5] * M[6] - M[3] * M[8]) * oneOverDet;
        N[4] = (M[0] * M[8] - M[2] * M[6]) * oneOverDet;
        N[7] = (M[2] * M[3] - M[0] * M[5]) * oneOverDet;

        N[2] = (M[3] * M[7] - M[4] * M[6]) * oneOverDet;
        N[5] = (M[1] * M[6] - M[8] * M[7]) * oneOverDet;
        N[8] = (M[0] * M[4] - M[1] * M[3]) * oneOverDet;

        return N;
    }

    static bool glsl130Supported()
    {
        std::string version = (char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
        if (version.find("1.30") != string::npos)
        {
            return true;
        }
        return false;
    }

private:
    string readFile(const string& filename)
    {
        ifstream fileIn(filename.c_str());

        if (!fileIn.good())
        {
            std::cerr << "Could not load shader: " << filename << std::endl;
            return string();
        }

        string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
        return stringBuffer;
    }

    bool compileShader(const GLSLShader& shader)
    {
        glCompileShader(shader.id);
        GLint result = 0xDEADBEEF;
        glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

        if (!result)
        {
            std::cout << "Could not compile shader: " << shader.id << std::endl;
            outputShaderLog(shader.id);
            return false;
        }

        return true;
    }

    void outputShaderLog(unsigned int shaderID)
    {
        vector<char> infoLog;
        GLint infoLen;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
		infoLog.resize(infoLen);

        std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
        glGetShaderInfoLog(shaderID, infoLog.size(), &infoLen, &infoLog[0]);

        std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
#ifdef WIN32
        MessageBox(NULL, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
#endif
    }

    void outputProgramLog(unsigned int programID)
    {
        vector<char> infoLog;
        GLint infoLen;
        GLint result;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);

		if (result == GL_TRUE) {
		    return;
		}

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLen);

		infoLog.resize(infoLen);

        std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
        glGetProgramInfoLog(programID, infoLog.size(), &infoLen, &infoLog[0]);

        std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
#ifdef _WIN32
        MessageBox(NULL, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
#endif

    }

    GLSLShader m_vertexShader;
    GLSLShader m_fragmentShader;
    unsigned int m_programID;

    map<string, GLuint> m_uniformMap;
    map<string, GLuint> m_attribMap;
};

#endif // GLSL_SHADER_H_INCLUDED

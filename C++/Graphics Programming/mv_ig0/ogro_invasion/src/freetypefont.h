#ifndef BOGLGP_FREETYPE_FONT_H
#define BOGLGP_FREETYPE_FONT_H

// FreeType Headers
#include <ft2build.h>
#include <freetype2/freetype/freetype.h>
#include <freetype2/freetype/ftglyph.h>
#include <freetype2/freetype/ftoutln.h>
#include <freetype2/freetype/fttrigon.h>
#include <string>
#include "glee/GLee.h"
#include "glslshader.h"
#include "uncopyable.h"

class FreeTypeFont : private Uncopyable {
public:
    FreeTypeFont(const std::string& fontName, int screenWidth, int screenHeight, int fontSize=16,
                 const std::string& vertShader="data/font.vert", const std::string& fragShader="data/font.frag");
    ~FreeTypeFont();

    bool initialize();
    void printString(const std::string& str, float x, float y);

private:
    GLuint m_textureID[128]; //Store room for the character textures

    int m_fontSize;
    int m_screenWidth;
    int m_screenHeight;
    std::string m_fontName;

    GLuint m_texCoordBuffer;
    GLuint m_vertexBuffer;

    void setOrthoMode();
    void unsetOrthoMode();
    bool generateCharacterTexture(unsigned char ch, FT_Face fontInfo);

    GLSLProgram* m_shaderProgram;

    std::map<char, std::pair<int, int> > m_glyphDimensions;
    std::map<char, std::pair<int, int> > m_glyphPositions;
    std::map<char, int> m_glyphAdvances;
};

#endif

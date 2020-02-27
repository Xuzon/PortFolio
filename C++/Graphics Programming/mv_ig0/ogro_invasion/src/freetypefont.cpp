#ifdef _WIN32
#include <windows.h>
#endif

#include "freetypefont.h"

using std::string;

FreeTypeFont::FreeTypeFont(const string& fontName, int screenWidth, int screenHeight, int fontSize,
                           const string& vertShader, const string& fragShader):
m_fontSize(fontSize),
m_screenWidth(screenWidth),
m_screenHeight(screenHeight),
m_fontName(fontName),
m_texCoordBuffer(0),
m_vertexBuffer(0),
m_shaderProgram(0)
{
    m_shaderProgram = new GLSLProgram(vertShader, fragShader);
}

FreeTypeFont::~FreeTypeFont()
{
    glDeleteTextures(128, m_textureID);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_texCoordBuffer);
}

bool FreeTypeFont::initialize()
{
    FT_Library library; //Create a freetype library instance

    if (FT_Init_FreeType(&library)) {
        std::cerr << "Could not initialize the freetype library" << std::endl;
        return false;
    }

    FT_Face fontInfo;  //Stores information on the loaded font

    //Now we attempt to load the font information
    if(FT_New_Face(library, m_fontName.c_str(), 0, &fontInfo)) {
        std::cerr << "Could not load the specified font" << std::endl;
        return false;
    }

    //FreeType uses heights which are one 64th of the size in pixels so
    //we set our font height by multiplying by 64. The 96x96 is the dots per inch
    FT_Set_Char_Size(fontInfo, (int)m_fontSize * 64, (int)m_fontSize * 64, 96, 96);

    //Generate 128 textures (each character gets its own texture)
    glGenTextures(128, m_textureID);

    for (unsigned char ch = 0; ch < 128; ++ch)
    {
        if (!generateCharacterTexture(ch, fontInfo))
        {
            std::cerr << "Could not generate the texture for character: " << ch << std::endl;
            return false;
        }
    }

    FT_Done_Face(fontInfo);
    FT_Done_FreeType(library);

    GLfloat vertices [] = {
        0.0f, 0.0f,
        (float)m_fontSize, 0.0f,
        (float)m_fontSize, (float)m_fontSize,
        0.0f, (float)m_fontSize
    };

    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &vertices[0], GL_STATIC_DRAW);

    //Just initialize with something for now, the tex coords are updated
    //for each character printed
    GLfloat texCoords [] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f
    };

    glGenBuffers(1, &m_texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &texCoords[0], GL_DYNAMIC_DRAW);

    if (!m_shaderProgram->initialize()) {
        std::cerr << "The shader program could not be initialized" << std::endl;
        return false;
    }

    m_shaderProgram->bindAttrib(0, "a_Vertex");
    m_shaderProgram->bindAttrib(1, "a_TexCoord0");
    m_shaderProgram->linkProgram();

    m_shaderProgram->sendUniform("texture0", 0);

    return true;
}

bool FreeTypeFont::generateCharacterTexture(unsigned char ch, FT_Face fontInfo)
{
    if(FT_Load_Glyph(fontInfo, FT_Get_Char_Index(fontInfo, ch), FT_LOAD_DEFAULT))
    {
        return false;
    }

  	FT_Glyph glyph;
    if(FT_Get_Glyph(fontInfo->glyph, &glyph))
    {
        return false;
    }

    if (FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1))
    {
        return false;
    }

    FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph) glyph;

    int width = (bitmapGlyph->bitmap.width) ? bitmapGlyph->bitmap.width : 16;
    int rows = (bitmapGlyph->bitmap.rows) ? bitmapGlyph->bitmap.rows : 16;

    //Allocate space for our font texture (R, G, B, A)
    int imageSize = width * rows * 4;

    vector<unsigned char> imageData(imageSize);

    for (int i = 0; i < imageSize / 4; i++)
    {
        unsigned char gray = 0;
        if (bitmapGlyph->bitmap.buffer)
        {
            gray = bitmapGlyph->bitmap.buffer[i];
        }

        imageData[i*4] = gray;
        imageData[(i*4)+1] = gray;
        imageData[(i*4)+2] = gray;
        imageData[(i*4)+3] = gray;
    }

    glBindTexture(GL_TEXTURE_2D, m_textureID[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, rows, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, &imageData[0]);

    m_glyphDimensions[ch] = std::make_pair(width, rows);
    m_glyphPositions[ch] = std::make_pair(bitmapGlyph->left, bitmapGlyph->top);
    m_glyphAdvances[ch] = fontInfo->glyph->advance.x / 64;
    return true;
}

void FreeTypeFont::printString(const std::string& str, float x, float y)
{
    static float modelviewMatrix[16];
    static float projectionMatrix[16];

    m_shaderProgram->bindShader();

    setOrthoMode();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glTranslatef(x, y, 0.0); //Position our text
    for(string::size_type i = 0; i < str.size(); ++i)
    {
        int ch = int(str[i]);

        float vertices [] = {
            0.0f, 0.0f,
            (float)m_glyphDimensions[ch].first, 0.0f,
            (float)m_glyphDimensions[ch].first, (float)m_glyphDimensions[ch].second,
            0.0f, (float)m_glyphDimensions[ch].second
        };

        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, &vertices[0]);
        glVertexAttribPointer((GLint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
        glVertexAttribPointer((GLint)1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glBindTexture(GL_TEXTURE_2D, m_textureID[ch]);
        glPushMatrix();
            glTranslatef((float)m_glyphPositions[ch].first, (float)m_glyphPositions[ch].second - m_glyphDimensions[ch].second, 0);

            glGetFloatv(GL_MODELVIEW_MATRIX, modelviewMatrix);
            glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);

            m_shaderProgram->sendUniform4x4("modelview_matrix", modelviewMatrix);
            m_shaderProgram->sendUniform4x4("projection_matrix", projectionMatrix);

            glDrawArrays(GL_QUADS, 0, 4);
        glPopMatrix();
        glTranslatef((float)m_glyphAdvances[ch], 0, 0); //Move along a bit for the next character
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    unsetOrthoMode();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void FreeTypeFont::setOrthoMode()
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, m_screenWidth, 0, m_screenHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void FreeTypeFont::unsetOrthoMode()
{
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

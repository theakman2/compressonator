//=====================================================================
// Copyright (c) 2016    Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
/// \file GPU_OpenGL.cpp
//
//=====================================================================

#include "GPU_OpenGL.h"
#include "MIPS.h"
#include <stdio.h>

#include "GPU_Decode.h"

#pragma comment(lib, "opengl32.lib")        // Open GL
#pragma comment(lib, "Glu32.lib")           // Glu 
#pragma comment(lib, "glew32.lib")          // glew 1.13.0

using namespace GPU_Decode;

GPU_OpenGL::GPU_OpenGL()
{

}

GPU_OpenGL::~GPU_OpenGL()
{

}

//====================================================================================

void GPU_OpenGL::GLRender()
{
    // OpenGL animation code goes here
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // setup texture mapping
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);

    glPushMatrix();
    glRotatef(theta, 0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);

    //for certain image format like bmp, the image texture is upside down, need to use coordinate as below
    //glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, -1.0);
    //glTexCoord2d(1.0, 1.0); glVertex2d(+1.0, -1.0);
    //glTexCoord2d(1.0, 0.0); glVertex2d(+1.0, +1.0);
    //glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, +1.0);

    //for dds use coordinate below
    glTexCoord2d(0.0, 0.0); glVertex2d(-1.0, -1.0);
    glTexCoord2d(1.0, 0.0); glVertex2d(+1.0, -1.0);
    glTexCoord2d(1.0, 1.0); glVertex2d(+1.0, +1.0);
    glTexCoord2d(0.0, 1.0); glVertex2d(-1.0, +1.0);

    glEnd();
    glPopMatrix();

    //for debug when showwindow is enable
    //SwapBuffers(m_hDC);
}

GLenum GPU_OpenGL::MIP2OLG_Format(const CMP_Texture* pSourceTexture)
{
    GLenum m_GLnum;
    switch (pSourceTexture->format)
    {
    case CMP_FORMAT_BC1:
    case CMP_FORMAT_DXT1:
        m_GLnum = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case CMP_FORMAT_BC2:
    case CMP_FORMAT_DXT3:
        m_GLnum = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case CMP_FORMAT_BC3:
    case CMP_FORMAT_DXT5:
        m_GLnum = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    case CMP_FORMAT_BC4:
    case CMP_FORMAT_ATI1N:
        m_GLnum = GL_COMPRESSED_RED_RGTC1;
        break;
    case CMP_FORMAT_BC5:
    case CMP_FORMAT_ATI2N:
    case CMP_FORMAT_ATI2N_XY:
    case CMP_FORMAT_ATI2N_DXT5:
        m_GLnum = GL_COMPRESSED_RG_RGTC2;
        break;
    case CMP_FORMAT_BC6H:
        m_GLnum = GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT;
        break;
    case CMP_FORMAT_BC7:
        m_GLnum = GL_COMPRESSED_RGBA_BPTC_UNORM;
        break;
    case CMP_FORMAT_ETC_RGB:
    case CMP_FORMAT_ETC2_RGB:
        m_GLnum = GL_COMPRESSED_RGB8_ETC2;
        break;
    case CMP_FORMAT_ASTC:  
        if ((pSourceTexture->nBlockWidth == 4) && (pSourceTexture->nBlockHeight == 4))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
        else if ((pSourceTexture->nBlockWidth == 5) && (pSourceTexture->nBlockHeight == 4))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_5x4_KHR;
        else if ((pSourceTexture->nBlockWidth == 5) && (pSourceTexture->nBlockHeight == 5))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_5x5_KHR;
        else if ((pSourceTexture->nBlockWidth == 6) && (pSourceTexture->nBlockHeight == 5))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_6x5_KHR;
        else if ((pSourceTexture->nBlockWidth == 6) && (pSourceTexture->nBlockHeight == 6))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_6x6_KHR;
        else if ((pSourceTexture->nBlockWidth == 8) && (pSourceTexture->nBlockHeight == 5))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_8x5_KHR;
        else if ((pSourceTexture->nBlockWidth == 8) && (pSourceTexture->nBlockHeight == 6))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_8x6_KHR;
        else if ((pSourceTexture->nBlockWidth == 8) && (pSourceTexture->nBlockHeight == 8))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_8x8_KHR;
        else if ((pSourceTexture->nBlockWidth == 10) && (pSourceTexture->nBlockHeight == 5))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_10x5_KHR;
        else if ((pSourceTexture->nBlockWidth == 10) && (pSourceTexture->nBlockHeight == 6))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_10x6_KHR;
        else if ((pSourceTexture->nBlockWidth == 10) && (pSourceTexture->nBlockHeight == 8))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_10x8_KHR;
        else if ((pSourceTexture->nBlockWidth == 10) && (pSourceTexture->nBlockHeight == 10))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_10x10_KHR;
        else if ((pSourceTexture->nBlockWidth == 12) && (pSourceTexture->nBlockHeight == 10))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_12x10_KHR;
        else if ((pSourceTexture->nBlockWidth == 12) && (pSourceTexture->nBlockHeight == 12))
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_12x12_KHR;
        else 
            m_GLnum = GL_COMPRESSED_RGBA_ASTC_4x4_KHR;
        break;
    default:
        m_GLnum = GL_INVALID_ENUM;
        break;
    }
    return m_GLnum;
}

// load pre-compressed texture
GLuint GPU_OpenGL::LoadTexture(const CMP_Texture* pSourceTexture, bool wrap)
{
    GLenum m_GLnum = MIP2OLG_Format(pSourceTexture);
    if (m_GLnum == GL_INVALID_ENUM)
    {
        fprintf(stderr, "Unsupported format.\n");
        return GLuint(-1);
    }

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW. Please make sure GLEW is downloaded.\n");
        return GLuint(-1);
    }

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
   
    //modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // bilinear filter the closest MIP map for small texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    // bilinear filter the first MIP map for large texture
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // if wrap is true, the texture wraps over at the edges (repeat)
    // false, the texture ends at the edges (clamp)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,wrap ? GL_REPEAT : GL_CLAMP);

    // build texture MIP maps
    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    //for uncompressed image
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //for compressed image (only for mip level 1)
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, m_GLnum, pSourceTexture->dwWidth, pSourceTexture->dwHeight, 0, pSourceTexture->dwDataSize, pSourceTexture->pData);
    
    return texture;
}

// Free Texture
void GPU_OpenGL::FreeTexture(GLuint texture)
{
    glDeleteTextures(1, &texture);
}

//=========================================================================================

int WINAPI GPU_OpenGL::Decompress(
    const CMP_Texture* pSourceTexture,
    CMP_Texture* pDestTexture
)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    // load our texture

    if (FAILED(InitWindow(hInstance, pDestTexture->dwWidth, pDestTexture->dwHeight)))
        return 0;

    EnableWindowContext(m_hWnd, &m_hDC, &m_hRC);

    GLint majVer = 0;
    GLint minVer = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &majVer);
    glGetIntegerv(GL_MINOR_VERSION, &minVer);

   if (majVer < 3 || (majVer < 3 && minVer < 2))
   {
       PrintStatusLine("Error: OpenGL 3.2 and up cannot be detected.\n");
       fprintf(stderr, "Error: OpenGL 3.2 and up cannot be detected.\n" );
       return 0;
   }

    texture = LoadTexture(pSourceTexture, false);

    //for debug, show window to view image
    //ShowWindow(g_hWnd2, SW_SHOW);

    //  Wait in Main message loop, until render is complete
    //  then exit
    MSG msg = { 0 };
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            GLRender();
            break;
        }
    }

    if (pSourceTexture->format == CMP_FORMAT_ETC_RGB || pSourceTexture->format == CMP_FORMAT_ETC2_RGB)
        glReadPixels(0, 0, pDestTexture->dwWidth, pDestTexture->dwHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pDestTexture->pData);
    else
        glReadPixels(0, 0, pDestTexture->dwWidth, pDestTexture->dwHeight, GL_RGBA, GL_UNSIGNED_BYTE, pDestTexture->pData);


    // free the texture
    FreeTexture(texture);

    // shutdown OpenGL
    DisableWindowContext(m_hWnd, m_hDC, m_hRC);

    return (int)0; // msg.wParam;
}



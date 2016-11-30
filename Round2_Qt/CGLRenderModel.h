#ifndef CGLRENDERMODEL_H
#define CGLRENDERMODEL_H
#include <GL/glew.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string>
#include <cstdlib>

#include <openvr.h>

#include "shared/lodepng.h"
#include "shared/Matrices.h"
#include "shared/pathtools.h"

class CGLRenderModel
{
public:
    CGLRenderModel( const std::string & sRenderModelName );
    ~CGLRenderModel();

    bool BInit( const vr::RenderModel_t & vrModel, const vr::RenderModel_TextureMap_t & vrDiffuseTexture );
    void Cleanup();
    void Draw();
    const std::string & GetName() const { return m_sModelName; }

private:
    GLuint m_glVertBuffer;
    GLuint m_glIndexBuffer;
    GLuint m_glVertArray;
    GLuint m_glTexture;
    GLsizei m_unVertexCount;
    std::string m_sModelName;
};

#endif // CGLRENDERMODEL_H

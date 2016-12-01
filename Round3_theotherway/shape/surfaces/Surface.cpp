#include "shape/surfaces/Surface.h"
#include <iostream>
#include "gl/util.h"

/**
 * @brief Surface::Surface
 * @param para the parameter settings
 * @param name the name of the surface
 */
Surface::Surface(Paramater para, std::string name) :
   m_para(para),
   m_name(name),
   m_ifTrans(false),
   m_transMat(glm::mat4()),
   m_vertices(std::vector<GLfloat>()) {}


/**
 * @brief Surface::Surface
 * @param para
 * @param name
 * @param transMat the transformation matrix if we see this surface as a whole
 */
Surface::Surface(Paramater para, std::string name, const glm::mat4& transMat) :
   m_para(para),
   m_name(name),
   m_ifTrans(true),
   m_transMat(transMat),
   m_vertices(std::vector<GLfloat>()) {}


// copy constructor
Surface::Surface(const Surface& sf) {
   m_para     = sf.m_para,
   m_name     = sf.m_name,
   m_ifTrans  = sf.m_ifTrans,
   m_transMat = sf.m_transMat;
   m_vertices = sf.m_vertices;
}

// copy assignment operator
Surface& Surface::operator=(const Surface& other) {
   m_para     = other.m_para,
   m_name     = other.m_name,
   m_ifTrans  = other.m_ifTrans,
   m_transMat = other.m_transMat;
   m_vertices = other.m_vertices;
   return *this;
}


void Surface::setParameters(const Paramater& para) {
   m_para = para;
   m_vertices.clear();
   computeVertices();
}


/**
 * @brief Surface::getData get all vertices on this surface
 * @return std::vector<GLfloat>
 */
std::vector<GLfloat> Surface::getData() {
   return m_vertices;
}


/**
 * @brief Surface::setData set the vertices on this surface to the givenset
 * @param vs std::vector<GLfloat>&the new vertices sets
 */
void Surface::setData(const std::vector<GLfloat>& vs) {
   m_vertices = vs;
}


void Surface::sentToVAO(bool nofill) {
   if (nofill && (static_cast<int>(m_name.find("Sierpinski")) < 0)) {
      setVertexData(m_vertices.data(), m_vertices.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_LINE_STRIP, m_vertices.size() / 6);
   } else{
      setVertexData(m_vertices.data(), m_vertices.size(), VBO::GEOMETRY_LAYOUT::LAYOUT_TRIANGLE_STRIP, m_vertices.size() / 6);
   }
   setAttribute(ShaderAttrib::POSITION, 3, 0, VBOAttribMarker::DATA_TYPE::FLOAT, false);
   setAttribute(ShaderAttrib::NORMAL, 3, 12, VBOAttribMarker::DATA_TYPE::FLOAT, false);
   buildVAO();
}


/**
 * @brief Surface::draw draw the vertices as well as their normals by sending them to VAO
 * This is where the points are actually drawn using LAYOUT_TRIANGLE_STRIP mode
 * The function is called by a Shape object
 */
void Surface::draw() {
   drawGeo();
}


/**
 * @brief Surface::push
 * @param v1 glm::vec4
 * @param v1n glm::vec4
 * @param v2 glm::vec4
 * @param v2n glm::vec4
 * Save two vertices to the list
 */
void Surface::push(glm::vec4 v1, glm::vec4 v1n, glm::vec4 v2, glm::vec4 v2n) {
   if (m_ifTrans) {
      v1  = m_transMat * v1;
      v2  = m_transMat * v2;
      v1n = m_transMat * v1n;
      v2n = m_transMat * v2n;
   }
   v1n = glm::normalize(v1n);
   v2n = glm::normalize(v2n);

   // save these points
   m_vertices.push_back(v1[0]), m_vertices.push_back(v1[1]), m_vertices.push_back(v1[2]);
   m_vertices.push_back(v1n[0]), m_vertices.push_back(v1n[1]), m_vertices.push_back(v1n[2]);
   m_vertices.push_back(v2[0]), m_vertices.push_back(v2[1]), m_vertices.push_back(v2[2]);
   m_vertices.push_back(v2n[0]), m_vertices.push_back(v2n[1]), m_vertices.push_back(v2n[2]);
}


/**
 * @brief Surface::push
 * @param v glm::vec4
 * @param vn glm::vec4
 * Save one vertice to the list
 */
void Surface::push(glm::vec4 v, glm::vec4 vn) {
   if (m_ifTrans) {
      v  = m_transMat * v;
      vn = m_transMat * vn;
      vn = glm::normalize(vn);
   }
   m_vertices.push_back(v[0]), m_vertices.push_back(v[1]), m_vertices.push_back(v[2]);
   m_vertices.push_back(vn[0]), m_vertices.push_back(vn[1]), m_vertices.push_back(vn[2]);
}

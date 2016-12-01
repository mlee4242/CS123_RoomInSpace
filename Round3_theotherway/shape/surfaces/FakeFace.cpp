#include "FakeFace.h"

#include "gl/util.h"
#include <iostream>
#include <math.h>

FakeFace::FakeFace(Paramater para, const std::string name)
   : Surface(para, name) {
}


FakeFace::FakeFace(Paramater para, const std::string name, const glm::mat4& transMat)
   : Surface(para, name, transMat) {
   m_ifTrans = true;
}


FakeFace::FakeFace(const FakeFace& sf) : Surface(sf) {
   m_para     = sf.m_para;
   m_name     = sf.m_name;
   m_transMat = sf.m_transMat;
   m_ifTrans  = sf.m_ifTrans;
   m_vertices = sf.m_vertices;
}

FakeFace::~FakeFace(){}

void FakeFace::computeVertices() {}

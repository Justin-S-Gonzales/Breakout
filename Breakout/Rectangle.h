#pragma once
#include "Mesh.h"
class Rectangle :
    public Mesh
{
private:
    float m_Vertices[8];
    unsigned int m_Indices[6];

public:
    Rectangle();

};


#include "object.h"

object::object(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uv, std::vector<int> indices) {
    vertexBuffer = 0;
	uvBuffer = 0;
	indiceBuffer = 0;
    textureID = 0;

    posX = 0; posY = 0;	posZ = 0;
    rotX = 0; rotY = 0; rotZ = 0;
    scaleX = 1; scaleY = 1; scaleZ = 1;


    this->vertices = vertices;
    this->uv = uv;
    this->indices = indices;

   
    glGenBuffers(1, &this->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	
  
    if (uv.size()) {
        glGenBuffers(1, &this->uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, this->uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(glm::vec2), &uv[0], GL_STATIC_DRAW);
    }

    
    if (indices.size()) {
        glGenBuffers(1, &this->indiceBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indiceBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);
    }
}

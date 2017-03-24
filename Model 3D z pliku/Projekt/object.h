#ifndef OBJECT_H
#define OBJECT_H

#include "glew.h"
#include "glm/glm.hpp"
#include <vector>
#include <iostream>

class object
{
    public:
		object() {}
        object(
			std::vector<glm::vec3> vertices, 
			std::vector<glm::vec2> uv = std::vector<glm::vec2>(), 
			std::vector<int> indices = std::vector<int>()
		);

        GLuint vertexBuffer, uvBuffer, indiceBuffer, textureID;

        float posX, posY, posZ,
              rotX, rotY, rotZ,
              scaleX, scaleY, scaleZ;

        std::vector<int> indices;
        std::vector<object> children;
    //private:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uv;
};

#endif // OBJECT_H

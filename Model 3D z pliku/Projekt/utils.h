#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include "glm/glm.hpp"
#include "glew.h"

namespace utils {
    bool load_obj(
        const char *filename,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals
    );

    GLuint load_bmp(const char *imagepath);

}

#endif // UTILS_H

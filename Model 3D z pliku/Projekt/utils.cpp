#include "utils.h"

namespace utils {


    bool load_obj(
        const char *filename,
        std::vector<glm::vec3> &out_vertices,
        std::vector<glm::vec2> &out_uvs,
        std::vector<glm::vec3> &out_normals
    ) {
        std::cout << "Loading OBJ file...\n";
        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;

        FILE *file = fopen(filename, "r");
        if(file == NULL) {
            std::cout << "Invalid file path...\n";
            return false;
        }

        while (1) {
            char lineHeader[128];

           
            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;

            if (strcmp(lineHeader, "v") == 0) {
              
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                temp_vertices.push_back(vertex);
            } else if (strcmp(lineHeader, "vt") == 0) {
               
                glm::vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y );
                uv.y = -uv.y; 
                temp_uvs.push_back(uv);
            } else if (strcmp(lineHeader, "vn") == 0) {
            
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
                temp_normals.push_back(normal);
            } else if (strcmp(lineHeader, "f") == 0) {
               
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
					&vertexIndex[0], &uvIndex[0], &normalIndex[0], 
					&vertexIndex[1], &uvIndex[1], &normalIndex[1], 
					&vertexIndex[2], &uvIndex[2], &normalIndex[2] 
				);
                if (matches != 9) {
                    std::cout << "File can't be read by parser...\n";
                    return false;
                }

           
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            } else {
               
                char commentBuffer[1000];
                fgets(commentBuffer, 1000, file);
            }
        }

       
        for (unsigned int i=0; i<vertexIndices.size(); i++) {
            
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];

            
            glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
            glm::vec2 uv = temp_uvs[ uvIndex-1 ];
            glm::vec3 normal = temp_normals[ normalIndex-1 ];

            
            out_vertices.push_back(vertex);
            out_uvs.push_back(uv);
            out_normals.push_back(normal);
        }

        return true;
    }

    
    GLuint load_bmp(const char *imagepath) {
        printf("Reading image %s...\n", imagepath);

        unsigned char header[54];
        unsigned int dataPos;
        unsigned int imageSize;
        unsigned int width, height;
        unsigned char *data;

       
        FILE *file = fopen(imagepath,"rb");
        if (!file) {
            printf("%s could not be opened. Invalid path.\n", imagepath);
            getchar();
            return 0;
        }

      
        if (fread(header, 1, 54, file) != 54) {
            std::cout << "Not a correct BMP file" << std::endl;
            return 0;
        }
 
        if (header[0] != 'B' || header[1] != 'M') {
            std::cout << "Not a correct BMP file" << std::endl;
            return 0;
        }
      
        if (*(int*)&(header[0x1E]) != 0) {
            std::cout << "Not a correct BMP file" << std::endl;
            return 0;
        }
        if (*(int*)&(header[0x1C]) != 24) {
            std::cout << "Not a correct BMP file" << std::endl;
            return 0;
        }

        
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

   
        if (imageSize == 0) {
            imageSize = width * height * 3;
        }
        if (dataPos == 0) {
            dataPos = 54;
        } 

      
        data = new unsigned char [imageSize];

       
        fread(data,1,imageSize,file);

       
        fclose(file);

    
        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

       
        delete[] data;

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

      
        return textureID;
    }
}

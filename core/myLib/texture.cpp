#include "texture.h"
#include "../ew/external/stb_image.h"
#include "../ew/external/glad.h"
#include <vector>
#include <string>

unsigned int loadTextures(std::vector<std::string> faces)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, numComponents;
	unsigned char* data;

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	for (int i = 0; i < faces.size(); i++) {
		data = stbi_load(faces[i].c_str(), &width, &height, &numComponents, 0);
		if (data == NULL)
		{
			printf("Failed to load image %s", faces[i].c_str());
			stbi_image_free(data);
			return 0;
		}
		else {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
	}



	
	


	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



	return texture;
}
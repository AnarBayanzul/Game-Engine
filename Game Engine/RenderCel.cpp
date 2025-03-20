#include "RenderCel.h"

#include <glm/ext.hpp>
#include <iostream>

void RenderCel::update(float delta) {
	glUseProgram(program);
	glUniformMatrix4fv(uniformIndexProj, 1, GL_FALSE, glm::value_ptr(perspective));

	for (int i = 0; i < objectCount; ++i) {
		objects[i]->update(delta);
		glBindVertexArray((meshes[objects[i]->getRenderElement()])->getVAO());
		glUniformMatrix4fv(uniformIndexTran, 1, GL_FALSE, glm::value_ptr(objects[i]->getModel()));
		//TODO bind texture
		glActiveTexture(GL_TEXTURE0); // TODO may be unneccessary
		if (objects[i]->getTextureElement() != -1) {
			glBindTexture(GL_TEXTURE_2D, (textures[objects[i]->getTextureElement()])->getTBO());
		}
		else {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		// TODO what does this do?
		// Cel shading
		glUniform4fv(uniformIndexColor, 1, glm::value_ptr(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)));
		glCullFace(GL_FRONT);
		glLineWidth(4.0f);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_POLYGON_OFFSET_LINE);
		glPolygonOffset(1.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, (meshes[objects[i]->getRenderElement()])->getVertexCount());
		glDisable(GL_POLYGON_OFFSET_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glCullFace(GL_BACK);
		glUniform4fv(uniformIndexColor, 1, glm::value_ptr(objects[i]->getColor()));

		if (objects[i]->show) {
			glDrawArrays(GL_TRIANGLES, 0, (meshes[objects[i]->getRenderElement()])->getVertexCount());
		}
	}
	glBindVertexArray(0);
	glUseProgram(0);
}


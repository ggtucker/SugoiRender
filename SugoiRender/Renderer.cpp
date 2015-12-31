#include "Renderer.h"

#include "GLError.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sr {

Renderer::Renderer() : Renderer(Shader(), Camera()) {}

Renderer::Renderer(const Shader& shader) : Renderer(shader, Camera()) {}

Renderer::Renderer(const Camera& camera) : Renderer(Shader(), camera) {}

Renderer::Renderer(const Shader& shader, const Camera& camera) : shader{ shader }, camera{ camera } {
	model.push(glm::mat4());
	glEnable(GL_DEPTH_TEST);
}

void Renderer::updateMVP() const {
	glm::mat4 _proj = camera.GetProjectionMatrix();
	glm::mat4 _view = camera.GetViewMatrix();
	glm::mat4 _model = model.top();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(_proj));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(_model));
}

void Renderer::Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) const {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Render(const Mesh& mesh) const {
	shader.Use();
	updateMVP();
	mesh.Render(this->shader);
}

void Renderer::PushMatrix() {
	model.push(model.top());
}

void Renderer::PopMatrix() {
	model.pop();
	if (model.empty()) {
		model.push(glm::mat4());
	}
}

void Renderer::Translate(GLfloat x, GLfloat y, GLfloat z) {
	model.top() = glm::translate(model.top(), glm::vec3(x, y, z));
}

void Renderer::Scale(GLfloat x, GLfloat y, GLfloat z) {
	model.top() = glm::scale(model.top(), glm::vec3(x, y, z));
}

void Renderer::Rotate(GLfloat degrees, glm::vec3 rotationAxis) {
	model.top() = glm::rotate(model.top(), degrees, rotationAxis);
}

void Renderer::RotateX(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Renderer::RotateY(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Renderer::RotateZ(GLfloat degrees) {
	model.top() = glm::rotate(model.top(), degrees, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Renderer::EnableImmediateMode(GLenum mode) {
	glBegin(mode);
}

void Renderer::DisableImmediateMode() {
	glEnd();
}

void Renderer::ImmediateColor(GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue);
}

void Renderer::ImmediateColorAlpha(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glColor4f(red, green, blue, alpha);
}

void Renderer::ImmediateVertex(GLfloat x, GLfloat y, GLfloat z) {
	glVertex3f(x, y, z);
}

void Renderer::ImmediateNormal(GLfloat nx, GLfloat ny, GLfloat nz) {
	glNormal3f(nx, ny, nz);
}

void Renderer::ImmediateTexCoord(GLfloat s, GLfloat t) {
	glTexCoord2f(s, t);
}

Shader& Renderer::GetShader() {
	return this->shader;
}

const Shader& Renderer::GetShader() const {
	return this->shader;
}

Camera& Renderer::GetCamera() {
	return this->camera;
}

const Camera& Renderer::GetCamera() const {
	return this->camera;
}

}
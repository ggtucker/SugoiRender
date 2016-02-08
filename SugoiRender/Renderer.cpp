#include "Renderer.h"

#include "GLError.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sr {

Renderer::Renderer() : Renderer(Shader(), Camera()) {}

Renderer::Renderer(const Renderer& other) :
	shader{ other.shader },
	camera{ other.camera },
	model{ other.model } {

}

Renderer::Renderer(Renderer&& other) {

}

Renderer::Renderer(const Shader& shader) : Renderer(shader, Camera()) {}

Renderer::Renderer(const Camera& camera) : Renderer(Shader(), camera) {}

Renderer::Renderer(const Shader& shader, const Camera& camera) : shader{ shader }, camera{ camera } {
	model.push(glm::mat4());
	glEnable(GL_DEPTH_TEST);
	check_gl_error();
}

void Renderer::updateMVP() {
	glm::mat4 _proj = camera.GetProjectionMatrix();
	glm::mat4 _view = camera.GetViewMatrix();
	glm::mat4 _model = model.top();
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(_proj));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "view"), 1, GL_FALSE, glm::value_ptr(_view));
	glUniformMatrix4fv(glGetUniformLocation(shader.GetProgram(), "model"), 1, GL_FALSE, glm::value_ptr(_model));
	check_gl_error();
}

void Renderer::Clear(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glClearColor(red, green, blue, alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	check_gl_error();
}

void Renderer::RenderMesh(GLuint meshId) {
	shader.Use();
	updateMVP();
	meshPool[meshId].Render();
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

void Renderer::SetRenderMode(GLenum mode) {
	glRenderMode(mode);
	check_gl_error();
}

void Renderer::EnableImmediateMode(GLenum mode) {
	glBegin(mode);
	check_gl_error();
}

void Renderer::DisableImmediateMode() {
	glEnd();
	check_gl_error();
}

void Renderer::ImmediateColor(GLfloat red, GLfloat green, GLfloat blue) {
	glColor3f(red, green, blue);
	check_gl_error();
}

void Renderer::ImmediateColorAlpha(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
	glColor4f(red, green, blue, alpha);
	check_gl_error();
}

void Renderer::ImmediateVertex(GLfloat x, GLfloat y, GLfloat z) {
	glVertex3f(x, y, z);
	check_gl_error();
}

void Renderer::ImmediateNormal(GLfloat nx, GLfloat ny, GLfloat nz) {
	glNormal3f(nx, ny, nz);
	check_gl_error();
}

void Renderer::ImmediateTexCoord(GLfloat s, GLfloat t) {
	glTexCoord2f(s, t);
	check_gl_error();
}

void Renderer::CreateMesh(GLuint* meshId) {
	*meshId = meshPool.create();
}

void Renderer::FinishMesh(GLuint meshId) {
	meshPool[meshId].Build();
}

GLuint Renderer::AddVertexToMesh(GLuint meshId, const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texCoords) {
	return meshPool[meshId].AddVertex(position, normal, texCoords);
}

void Renderer::AddTriangleToMesh(GLuint meshId, GLuint v1, GLuint v2, GLuint v3) {
	meshPool[meshId].AddTriangle(v1, v2, v3);
}

Shader& Renderer::GetShader() {
	return this->shader;
}

Camera& Renderer::GetCamera() {
	return this->camera;
}

}
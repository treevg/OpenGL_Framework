#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(GLFWwindow*);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();
bool getMouseMovement();

#endif
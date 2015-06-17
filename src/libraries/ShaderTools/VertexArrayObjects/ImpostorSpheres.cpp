#include "ImpostorSpheres.h"

float r_equ(float size) {
    return size * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - size;
}

float r_pos(float size) {
    return size * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

ImpostorSpheres::ImpostorSpheres():
num_balls(1000000){
    mode = GL_TRIANGLE_STRIP;

    glGenVertexArrays(1, &vertexArrayObjectHandle);
    glBindVertexArray(vertexArrayObjectHandle);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLfloat positions[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, -1.0f,
        1.0f, 1.0f
    };

    GLfloat* instance_colors = new GLfloat[num_balls*4];
    GLfloat* instance_positions = new GLfloat[num_balls*4];

    for (int i = 0; i < num_balls*4; i+=4) {
        instance_colors[i] = r_pos(1.0);
        instance_colors[i+1] = r_pos(1.0);
        instance_colors[i+2] = r_pos(1.0);
        instance_colors[i+3] = 1;

        instance_positions[i] = r_equ(30);
        instance_positions[i+1] = r_equ(30);
        instance_positions[i+2] = r_equ(30);
        instance_positions[i+3] = abs(1 + r_equ(0.4));
    }

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(positions) +
                 sizeof(GLfloat) * num_balls * 4 +
                 sizeof(GLfloat) * num_balls * 4, NULL, GL_STATIC_DRAW);

    GLuint offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(positions), positions);
    offset += sizeof(positions);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * num_balls * 4, instance_colors);
    offset += sizeof(GLfloat) * num_balls * 4;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * num_balls * 4, instance_positions);
    offset += sizeof(GLfloat) * num_balls * 4;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(positions));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(positions) + sizeof(GLfloat) * num_balls * 4));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(0,0);
    glVertexAttribDivisor(1,1);
    glVertexAttribDivisor(2,1);

    delete instance_colors;
    delete instance_positions;
}

void ImpostorSpheres::draw() {
    this->drawInstanced(num_balls);
}

void ImpostorSpheres::drawInstanced(int countInstances) {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArraysInstanced(mode, 0, 4, countInstances);
}


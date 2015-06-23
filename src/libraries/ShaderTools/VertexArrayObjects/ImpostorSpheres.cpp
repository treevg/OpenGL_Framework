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

    std::vector<GLfloat> instance_colors;
    std::vector<GLfloat> instance_positions;

        for (int i = 0; i < num_balls*4; i+=4) {
            instance_colors.push_back(r_pos(1.0));
            instance_colors.push_back(r_pos(1.0));
            instance_colors.push_back(r_pos(1.0));
            instance_colors.push_back(1);

            instance_positions.push_back(r_equ(30));
            instance_positions.push_back(r_equ(30));
            instance_positions.push_back(r_equ(30));
            instance_positions.push_back(1 + r_equ(0.5));
        }

    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(positions) +
                 sizeof(GLfloat) * instance_colors.size() +
                 sizeof(GLfloat) * instance_positions.size(), NULL, GL_STATIC_DRAW);

    GLuint offset = 0;
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(positions), positions);
    offset += sizeof(positions);
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * instance_colors.size(), &instance_colors[0]);
    offset += sizeof(GLfloat) * instance_colors.size();
    glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat) * instance_positions.size(), &instance_positions[0]);
    offset += sizeof(GLfloat) * instance_positions.size();

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(positions));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(sizeof(positions) + sizeof(GLfloat) * instance_colors.size()));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribDivisor(0,0);
    glVertexAttribDivisor(1,1);
    glVertexAttribDivisor(2,1);
}

void ImpostorSpheres::draw() {
    this->drawInstanced(num_balls);
}

void ImpostorSpheres::drawInstanced(int countInstances) {
    glBindVertexArray(vertexArrayObjectHandle);
    glDrawArraysInstanced(mode, 0, 4, countInstances);
}


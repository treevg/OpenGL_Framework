#ifndef ImpostorSpheres_H
#define ImpostorSpheres_H

#include "../VertexArrayObject.h"

class ImpostorSpheres : public VertexArrayObject {
public:
    ImpostorSpheres();
    ImpostorSpheres( int n );
    ImpostorSpheres( std::vector<GLfloat> instance_colors, std::vector<GLfloat> instance_positions );
    void draw();
    void drawInstanced(int countInstances);
    int num_balls;
    void init( bool generateRandom );

    bool m_generateRandom;
    std::vector<GLfloat> m_instance_colors;
    std::vector<GLfloat> m_instance_positions;

};

#endif // ImpostorSpheres_H

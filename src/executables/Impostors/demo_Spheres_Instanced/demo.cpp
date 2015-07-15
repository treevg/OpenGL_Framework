#include <vector>
#include "ShaderTools/Renderer.h"
#include "ShaderTools/RenderPass.h"
#include "AssetTools/Texture.h"
#include "AssetTools/Mesh.h"
#include "ShaderTools/VertexArrayObjects/Quad.h"
#include "ShaderTools/VertexArrayObjects/Grid.h"
#include "ShaderTools/VertexArrayObjects/ImpostorSpheres.h"
#include "vtkPDBReader.h"
#include "vtkDoubleArray.h"
#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkPoints.h"


using namespace glm;

float r(float size) {
    return size * 2 * static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - size;
}

float r2(float size) {
    return size * static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

int main(int argc, char *argv[]) {
    GLFWwindow* window = generateWindow();

    float rotX = 0.0f;
    float rotY = 0.0f;
    float distance = 40.0;
    float scale = 1.0;

    mat4 projection = perspective(45.0f, getRatio(window), 0.1f, 1000.0f);


    ShaderProgram spRenderImpostor = ShaderProgram("/Impostor/impostorSpheres_Instanced.vert", "/Filters/solidColor.frag");
    ShaderProgram spRenderDiscs = ShaderProgram("/Impostor/impostorSpheres_Instanced.vert", "/Impostor/impostorSpheres_discardFragments_Instanced.frag");
    ShaderProgram spRenderBalls = ShaderProgram("/Impostor/impostorSpheres_Instanced.vert", "/Impostor/impostorSpheres_Instanced.frag");

    // SSAO
    ShaderProgram spSSAO = ShaderProgram("/Filters/fullscreen.vert","/Filters/SSAO.frag");
    ShaderProgram spPlain = ShaderProgram("/Filters/fullscreen.vert","/Filters/toneMapperLinear.frag");


    // read molecule structure from pdb data
    vtkSmartPointer<vtkPDBReader> pdb =
            vtkSmartPointer<vtkPDBReader>::New();
    std::string MolFile = RESOURCES_PATH "/Molecules/1AON.pdb";
    pdb->SetFileName(MolFile.c_str());
    pdb->SetHBScale(1.0);
    pdb->SetBScale(1.0);
    pdb->Update();
    std::cout <<"# of atoms is: " << pdb->GetNumberOfAtoms() << std::endl;

    // prepare a vector that will contain xyz and radius
    std::vector<GLfloat> atomPositions;
    // prepare a vector that will contain atom colors rgba
    std::vector<GLfloat> atomColors;

    // Get array containing atom color
    vtkSmartPointer<vtkDataArray> colors =
            vtkDataArray::SafeDownCast(pdb->GetOutput()->GetPointData()->GetArray("rgb_colors"));

    // Get array containing atom radius
    vtkSmartPointer<vtkDataArray> radii =
            vtkDataArray::SafeDownCast(pdb->GetOutput()->GetPointData()->GetArray("radius"));

    double range[2];
    colors->GetRange(range);

    for(vtkIdType i = 0; i < pdb->GetOutput()->GetNumberOfPoints(); i++)
    {
        double p[3];
        pdb->GetOutput()->GetPoint(i, p);
        atomPositions.push_back(p[0]);
        atomPositions.push_back(p[1]);
        atomPositions.push_back(p[2]);
        atomPositions.push_back(radii->GetTuple(i)[0]);

        // get the colors and map them to 0..1
        //        atomColors.push_back((colors->GetTuple(i)[0] - range[0]) / (range[1] - range[0]));
        //        atomColors.push_back((colors->GetTuple(i)[1] - range[0]) / (range[1] - range[0]));
        //        atomColors.push_back((colors->GetTuple(i)[2] - range[0]) / (range[1] - range[0]));
        atomColors.push_back(colors->GetTuple(i)[0] / 255);
        atomColors.push_back(colors->GetTuple(i)[1] / 255);
        atomColors.push_back(colors->GetTuple(i)[2] / 255);
        atomColors.push_back(1);
        //std::cout << "Point " << i << " : " << p[0] << " " << p[1] << " " <<  p[2] << std::endl;
        //std::cout << "Color: " << atomColors->GetTuple(i)[0] << atomColors->GetTuple(i)[1] << atomColors->GetTuple(i)[2] << std::endl;
        //std::cout << "Radius: " << atomRadii->GetTuple(i)[0] << atomRadii->GetTuple(i)[1] << atomRadii->GetTuple(i)[2] << std::endl;
    }
    
    RenderPass* renderBalls = new RenderPass(
                new ImpostorSpheres(atomColors, atomPositions),
                &spRenderBalls
                ,getWidth(window),
                getHeight(window)
                );
    renderBalls->update("projection", projection);

    // define projection matrix for other shader programs
    renderBalls->setShaderProgram(&spRenderDiscs);
    renderBalls->update("projection", projection);
    renderBalls->setShaderProgram(&spRenderImpostor);
    renderBalls->update("projection", projection);

    mat4 projectionInv = inverse(projection);
    // SSAO
    // Create kernel and upload it to shader
    int kernelSize = 150;
    GLfloat kernel[3*kernelSize];

    for (int i = 0; i < kernelSize; ++i) {
        vec3 vec = vec3(r(1),r(1),r2(1));
        normalize(vec);
        float kernelScale = float(i) / float(kernelSize);
        kernelScale = lerp(0.1f, 1.0f, kernelScale * kernelScale);

        vec *= kernelScale;
        //        kernel.push_back(vec.x);
        //        kernel.push_back(vec.y);
        //        kernel.push_back(vec.z);
        kernel[i*3] = vec.x;
        kernel[i*3+1] = vec.y;
        kernel[i*3+2] = vec.z;
    }

    RenderPass* SSAO = new RenderPass(
                new Quad(),
                &spSSAO);
    SSAO->texture("tex", renderBalls->get("fragColor"));
    SSAO->update("invProj", projectionInv);
    SSAO->update("projection", projection);
    GLint handle = glGetUniformLocation(SSAO->getShaderProgram()->getProgramHandle(), "kernelArray");
    glUniform1fv(handle, 3*kernelSize, kernel);

    SSAO->setShaderProgram(&spPlain);
    SSAO->texture("tex", renderBalls->get("fragColor"));






    bool animate = false;
    float lastTime = 0;
    float elapsedTime = 0;



    glEnable(GL_DEPTH_TEST);
    render(window, [&] (float deltaTime) {
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) (rotY - deltaTime < 0)? rotY -= deltaTime + 6.283 : rotY -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) (rotY + deltaTime > 6.283)? rotY += deltaTime - 6.283 : rotY += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) (rotX - deltaTime < 0)? rotX -= deltaTime + 6.283 : rotX -= deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) (rotX + deltaTime > 6.283)? rotX += deltaTime - 6.283 : rotX += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS) distance += deltaTime * 10;
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS) distance = max(distance - deltaTime * 10, 0.0f);
        if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) scale += deltaTime;
        if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) scale = glm::max(scale - deltaTime, 0.01f);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) renderBalls->setShaderProgram(&spRenderImpostor);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) renderBalls->setShaderProgram(&spRenderDiscs);
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        { renderBalls->setShaderProgram(&spRenderBalls);
            SSAO->setShaderProgram(&spPlain);
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            renderBalls->setShaderProgram(&spRenderBalls);
            SSAO->setShaderProgram(&spSSAO);
        }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            {
                if(animate)
                {
                    animate = false;
                    lastTime = glfwGetTime();
                }
                else
                {
                    animate = true;
                    glfwSetTime(lastTime);
                }
            }


            if (animate)
            {
                elapsedTime = glfwGetTime();
                if (elapsedTime > 628)
                {
                    elapsedTime = 0;
                    glfwSetTime(0);
                }
            }

            mat4 view = translate(mat4(1), vec3(0,0,-distance)) * eulerAngleXY(-rotX, -rotY);
            vec3 xyzOffset = vec3(sin(elapsedTime),cos(elapsedTime/2),sin(elapsedTime/3));

            renderBalls->clear(0.2,0.2,0.2,999);
            renderBalls->clearDepth();
            renderBalls->update("scale", vec2(scale));
            renderBalls->update("view", view);
            //renderBalls->update("xyzOffset", xyzOffset);
            renderBalls->update("elapsedTime", elapsedTime);
            renderBalls->run();
            SSAO->clear();
            SSAO->run();
        });
    }



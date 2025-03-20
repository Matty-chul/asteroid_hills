#include "lab/tema/tema.h"

#include <vector>
#include <iostream>
#include<cmath>
#include <stb/stb_image.h>

using namespace std;
using namespace lab;

Tema::Tema(){

}

Tema::~Tema() {

}

Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* Tema::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);


    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    if (channels == 1) glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    if (channels == 2) glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
    if (channels == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (channels == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);
    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

Mesh* Tema::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

void Tema::Init() {
    
    buildtree(Tree1,2.0f, glm::vec3(0, 0.0f, -3.0f));
    buildtree(Tree2, 3.0f, glm::vec3(-8.5, 0, -2));
    buildtree(Tree3, 4.0f, glm::vec3(6, 0, 7));
    buildtree(Tree4, 5.0f, glm::vec3(8, 0, -7));
    buildtree(Tree5, 6.0f, glm::vec3(-1, 0, 3));
    buildtree(Tree6, 7.0f, glm::vec3(-7, 0, 6));
    //texture initialisation
    ismarker = false;
    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\texturi\\bark2.jpg");
        mapTextures["bark"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\texturi\\leaf.jpg");
        mapTextures["leaf"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\texturi\\heightmap.png");
        mapTextures["height"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\texturi\\rock.jpg");
        mapTextures["rock"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\texturi\\rock2.jpg");
        mapTextures["rock2"] = texture;
    }

    //box object initalisation


    {
        int dimension = 200;
        float step = 0.1f;
        vector<VertexFormat> vertices;
        float x = -static_cast<float>(dimension)*step/2.0f;
        float y = 0.0f;
        float z = -static_cast<float>(dimension) * step / 2.0f;
        float teX = 0.0f;
        float teY = -1.0f;
        for (int i = 0; i < dimension + 1; i++) {
            teX = 0.0f;

            for (int j = 0; j < dimension + 1; j++) {
                roundf(x); roundf(z);

                vertices.emplace_back(VertexFormat(glm::vec3(x,y,z),glm::vec3(0), glm::vec3(0),glm::vec2(teX,teY)));
                x += step;
                teX += 1.0f/static_cast<float>(dimension);

            }
            x = -static_cast<float>(dimension) * step / 2.0f;
            z +=step;
            teY += 1.0f / static_cast<float>(dimension);
        }
        vector<unsigned int> indices;
        for (int j = 0; j < dimension; j += 1) { 
            for (int i = 0; i < dimension; i++) {   

                indices.emplace_back(i + (dimension+1)* j);
                indices.emplace_back(i + (dimension +1) + (dimension +1) * j);
                indices.emplace_back(i + 1 + (dimension + 1)  *j);

                indices.emplace_back(i + 1 + (dimension + 1) *j);
                indices.emplace_back(i + (dimension + 1) + (dimension + 1) *j);
                indices.emplace_back(i + (dimension + 1) + 1 + (dimension + 1) *j);

            }
        }
        
        vector<VertexFormat> vd{
            VertexFormat(glm::vec3(-0.5f,1.5f,0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(0.0f,-1.0f)),
            VertexFormat(glm::vec3(-0.5f,-1.5f,0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(0.0f,0.0f)),
            VertexFormat(glm::vec3(0.5f,1.5f,0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(1.0f,-1.0f)),
            VertexFormat(glm::vec3(0.5f,-1.5f,0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(1.0f,0.0f)),

            VertexFormat(glm::vec3(-0.5f,1.5f,-0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(0.0f,-1.0f)),
            VertexFormat(glm::vec3(-0.5f,-1.5f,-0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(0.0f,0.0f)),
            VertexFormat(glm::vec3(0.5f,1.5f,-0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(1.0f,-1.0f)),
            VertexFormat(glm::vec3(0.5f,-1.5f,-0.5f),glm::vec3(0), glm::vec3(0),glm::vec2(0.0f,-1.0f)),
        };
       
        vector<unsigned int> vi{
            0,1,3,
            2,0,3,
            4,5,7,
            6,4,7,
            4,0,2,
            6,4,2,
            5,1,3,
            7,5,3,
            4,5,1,
            0,4,1,
            2,3,7,
            6,2,7

        };

        CreateMesh("asteroid", vertices, indices);
        CreateMesh("manual", vd, vi);
    }
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("paralelip");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "paralelip.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //shader initialisation
    {
        Shader* shader = new Shader("BasicShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "BasicShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "BasicShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Helicopter");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "HeliShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "BasicShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Tree1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree1.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree1.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Tree2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree1.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree2.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Tree3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree1.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Tree3.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("AsteroidShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "AsteroidShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "AsteroidShader.FS.glsl"), GL_FRAGMENT_SHADER);
        //shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "AsteroidShader.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("Marker");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Marker.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "BasicShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("DirectionShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Direction.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "Direction.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    {
        Shader* shader = new Shader("TreeID");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "TreeID.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "TreeID.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    CreateFramebuffer(window->GetResolution().x, window->GetResolution().y);
}

void Tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::buildtree(vector<TreeComponent>& Tree, float custom_id, glm::vec3 position, glm::vec3& helipos) {
    glm::vec3 modelscale(0.4f);
    glm::mat4 trunk = glm::mat4(1);

    trunk = glm::translate(trunk, position +glm::vec3(0.0f,1.5f,0.0f)*modelscale);
    trunk = glm::scale(trunk, modelscale);

    buildbranch(Tree, custom_id, trunk, 0, 1, helipos);
    buildbranch(Tree, custom_id, trunk, 1, 1, helipos);
    buildbranch(Tree, custom_id, trunk, 2, 1, helipos);
    Tree.emplace_back(TreeComponent(trunk, true, 0, custom_id));
    TreeCollection.emplace_back(Tree);

}

void Tema::buildbranch(vector<TreeComponent>& Tree, float custom_id, glm::mat4 prev_branch, int position, int level, glm::vec3& helipos) {
    if (level > 5) {
        return;
    }
    else {
        glm::mat4 cur_branch = glm::mat4(1);
        cur_branch = glm::rotate(prev_branch, glm::radians(120.0f * static_cast<float>(position)), glm::vec3(0, 1, 0));
        cur_branch = glm::translate(cur_branch, glm::vec3(0, 1.5f, 0.7f));
        cur_branch = glm::rotate(cur_branch, glm::radians(45.0f), glm::vec3(1, 0, 0));
        cur_branch = glm::scale(cur_branch, glm::vec3(0.6f));
        
        buildbranch(Tree, custom_id, cur_branch, 0, level + 1, helipos);
        buildbranch(Tree, custom_id, cur_branch, 1, level + 1, helipos);
        buildbranch(Tree, custom_id, cur_branch, 2, level + 1, helipos);

        Tree.emplace_back(TreeComponent(cur_branch, true, level, custom_id));
    }
}

void Tema::place_tree(gfxc::Camera* camera, vector<TreeComponent>& Tree, bool custom_buffer) {
    int size = Tree.size();
    for (int i = 0; i < size; i++) {
        if (Tree[i].level < 4) RenderMesh (camera, meshes["paralelip"], (custom_buffer? shaders["TreeID"] : shaders["Tree1"]), Tree[i].model,glm::vec3(0), mapTextures["height"], mapTextures["bark"], NULL, globalheli,Tree[i].object_id);
        if (Tree[i].level == 4) RenderMesh(camera, meshes["paralelip"], (custom_buffer ? shaders["TreeID"] : shaders["Tree2"]), Tree[i].model, glm::vec3(0), mapTextures["height"], mapTextures["bark"], mapTextures["leaf"], globalheli, Tree[i].object_id);
        if (Tree[i].level == 5) RenderMesh(camera, meshes["paralelip"], (custom_buffer ? shaders["TreeID"] : shaders["Tree3"]), Tree[i].model, glm::vec3(0),mapTextures["height"], NULL, mapTextures["leaf"], globalheli, Tree[i].object_id);
    }
}

void Tema::render_helicopter(gfxc::Camera* camera, float deltaTimeSeconds, glm::vec3 position, float rotationangle) {
    angle_displacement += angle_increment*deltaTimeSeconds;
    float rotation_speed = 400.0f;
    glm::vec3 helisize(0.25f);

    glm::mat4 heli_body = glm::mat4(1);
    heli_body = glm::translate(heli_body, position);
    heli_body = glm::rotate(heli_body, rotationangle, glm::vec3(0, 1, 0));
    heli_body = glm::translate(heli_body, glm::vec3(0, 1.0f, 0) * helisize);
    heli_body = glm::scale(heli_body, glm::vec3(1.0f, 1.0f, 2.0f)*helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_body, glm::vec3(1, 0.4, 0),mapTextures["height"], NULL, NULL, position);

    glm::mat4 heli_tail = glm::mat4(1);
    heli_tail = glm::translate(heli_tail, position);
    heli_tail = glm::rotate(heli_tail, rotationangle, glm::vec3(0, 1, 0));
    heli_tail = glm::translate(heli_tail, glm::vec3(0.0f, 1.0f, -2.0f)*helisize);
    heli_tail = glm::scale(heli_tail, glm::vec3(0.25f, 0.25f, 2.0f) * helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_tail, glm::vec3(1, 0.4, 0), mapTextures["height"], NULL, NULL, position);

    glm::mat4 heli_sblade1 = glm::mat4(1);
    heli_sblade1 = glm::translate(heli_sblade1, position);
    heli_sblade1 = glm::rotate(heli_sblade1, rotationangle, glm::vec3(0, 1, 0));
    heli_sblade1 = glm::translate(heli_sblade1, glm::vec3(0.0f, 1.1f, -3.0f) * helisize);
    heli_sblade1 = glm::rotate(heli_sblade1, glm::radians(angle_displacement*rotation_speed), glm::vec3(1, 0, 0));
    heli_sblade1 = glm::scale(heli_sblade1, glm::vec3(0.05f, 0.75f, 0.15f) * helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_sblade1, glm::vec3(0.2, 0.2, 0.2), mapTextures["height"], NULL, NULL, position);


    glm::mat4 heli_sblade2 = glm::mat4(1);

    heli_sblade2 = glm::translate(heli_sblade2, position);
    heli_sblade2 = glm::rotate(heli_sblade2, rotationangle, glm::vec3(0, 1, 0));
    heli_sblade2 = glm::translate(heli_sblade2,glm::vec3(0.0f, 1.1f, -3.0f) * helisize);
    heli_sblade2 = glm::rotate(heli_sblade2, glm::radians(angle_displacement*rotation_speed + 90.0f), glm::vec3(1, 0, 0));
    heli_sblade2 = glm::scale(heli_sblade2, glm::vec3(0.05f, 0.75f, 0.15f) * helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_sblade2, glm::vec3(0.2, 0.2, 0.2), mapTextures["height"], NULL, NULL, position);

    glm::mat4 heli_bblade1 = glm::mat4(1);
    heli_bblade1 = glm::translate(heli_bblade1, position);
    heli_bblade1 = glm::rotate(heli_bblade1, rotationangle, glm::vec3(0, 1, 0));
    heli_bblade1 = glm::translate(heli_bblade1, glm::vec3(0, 1.6f, 0.0f) * helisize);
    heli_bblade1 = glm::rotate(heli_bblade1, glm::radians(angle_displacement * rotation_speed), glm::vec3(0, 1, 0));
    heli_bblade1 = glm::scale(heli_bblade1, glm::vec3(0.25f, 0.1f, 3.5f) * helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_bblade1, glm::vec3(0.2, 0.2, 0.2), mapTextures["height"], NULL, NULL, position);

    glm::mat4 heli_bblade2 = glm::mat4(1);
    heli_bblade2 = glm::translate(heli_bblade2, position);
    heli_bblade2 = glm::rotate(heli_bblade2, rotationangle, glm::vec3(0, 1, 0));
    heli_bblade2 = glm::translate(heli_bblade2, glm::vec3(0, 1.6f, 0.0f) * helisize);
    heli_bblade2 = glm::rotate(heli_bblade2, glm::radians(angle_displacement*rotation_speed + 90.0f), glm::vec3(0, 1, 0));
    heli_bblade2 = glm::scale(heli_bblade2, glm::vec3(0.25f, 0.1f, 3.5f) * helisize);
    RenderMesh(camera, meshes["box"], shaders["Helicopter"], heli_bblade2, glm::vec3(0.2, 0.2, 0.2), mapTextures["height"], NULL, NULL, position);
}

void Tema::render_marker(gfxc::Camera* camera, glm::vec3 position, glm::vec3 heli_position) {
    float time = sin(static_cast<float>(Engine::GetElapsedTime() * 3.0));
    glm::vec3 offset = glm::vec3(0.0f, 0.02f, 0.0f);
    glm::vec3 markercolor = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, position + offset);
    model = glm::scale(model, glm::vec3(0.2f, 0.02f, 0.2f));
    RenderMesh(camera, meshes["teapot"], shaders["Marker"], model, markercolor, mapTextures["height"], NULL, NULL, globalheli);

    model = glm::mat4(1);
    model = glm::translate(model, position + glm::vec3(0.0f, 0.05f * time, 0.0f) +offset);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f * 0.1f, 0.0f));
    model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
    RenderMesh(camera, meshes["manual"], shaders["Marker"], model, markercolor +glm::vec3(0.2f), mapTextures["height"], NULL, NULL, globalheli);
}

void Tema::draw_scene(gfxc::Camera* camera) {

    glm::mat4 transform = glm::mat4(1);
    
    RenderMesh(camera, meshes["asteroid"], shaders["AsteroidShader"], transform, glm::vec3(0), mapTextures["height"], mapTextures["rock2"], mapTextures["rock"], globalheli);
   
    
    
}

void Tema::place_forest(gfxc::Camera* camera, vector<bool>& isTreeRenderable, bool custom_buffer) {
    int size = TreeCollection.size();
    for (int i = 0; i < size; i++) {
        if (isTreeRenderable[i] == true) place_tree(camera, TreeCollection[i], custom_buffer);
    }
}

void Tema::Update(float deltaTimeSeconds) {
    auto camera = GetSceneCamera();
    glm::vec3 relativeCameraPosition = glm::vec3(0,2.8,-3);

    camera->SetPositionAndRotation(globalheli + relativeCameraPosition, glm::quatLookAt(-glm::normalize(relativeCameraPosition), glm::vec3(0, 1, 0)));

    if (reachdestination) {
        globalheli += direction / length(direction) * deltaTimeSeconds*helistep;
       
        if (length(globalheli - destination) <= 0.1) {
            reachdestination = false;
            ismarker = false;
            
        }
    }

    glViewport(0, 0, window->GetResolution().x, window->GetResolution().y);
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    

    RenderMesh(camera,meshes["asteroid"], shaders["DirectionShader"], glm::mat4(1), glm::vec3(0), mapTextures["height"], NULL, NULL, globalheli, 0);
    place_forest(camera, isTreeRenderable, true);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen
    
    draw_scene(camera);
    place_forest(camera, isTreeRenderable);

    render_helicopter(camera, deltaTimeSeconds,globalheli, helirotation);
    //render_marker(camera, glm::vec3(0), glm::vec3(0));
    if (ismarker) render_marker(camera, marker_position, glm::vec3(0));
}

void Tema::FrameEnd()
{
    //DrawCoordinateSystem();
    if (enablebufferdisplay) {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer_object);
        glReadBuffer(GL_COLOR_ATTACHMENT1);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        float size_factor = 3.0;
        glm::ivec2 resolution = window->GetResolution();
        glBlitFramebuffer(0, 0, resolution.x, resolution.y, 0, 0, resolution.x / size_factor, resolution.y / size_factor, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
    
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::vec3& color, Texture2D* texture1, glm::vec3& position)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    int obj_color_location = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(obj_color_location, 1, glm::value_ptr(color));

    // Send the model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Send the view matrix unfirom
    glm::mat4 view = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    // Send the projection matrix uniform
    glm::mat4 projection = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

    // TODO(student): Send the application time, obtained by
    // calling Engine::GetElapsedTime(), in the form of a
    // uniform type attribute to the shader
    int time_location = glGetUniformLocation(shader->program, "ElapsedTime");

    glUniform1f(time_location, static_cast<float>(Engine::GetElapsedTime()));

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }
    glUniform3fv(glGetUniformLocation(shader->program,"heli_position"), 1, glm::value_ptr(position));
    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::RenderTreeMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, glm::vec3& helipos)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    float time = static_cast<float>(Engine::GetElapsedTime());
    int timeloc = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeloc, time);
    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    if (texture3)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    }
    // Draw the object
    glUniform3fv(glGetUniformLocation(shader->program, "heli_position"), 1, glm::value_ptr(helipos));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::RenderTreeMesh2(int treelevel, Mesh* mesh, Shader* shader, const glm::mat4& model, float treescale, Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, glm::vec3& helipos) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // TODO(student): Set any other shader uniforms that you need

    float time = static_cast<float>(Engine::GetElapsedTime());
    int timeloc = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeloc, time);
    if (texture1)
    {
        // TODO(student): Do these:
        // - activate texture location 0
        // - bind the texture1 ID
        // - send theuniform value

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    if (texture3)
    {
        // TODO(student): Do these:
        // - activate texture location 1
        // - bind the texture2 ID
        // - send the uniform value
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    }
    // Draw the object
    glUniform1f(glGetUniformLocation(shader->program, "treescale"), treescale);

    glUniform3fv(glGetUniformLocation(shader->program, "heli_position"), 1, glm::value_ptr(helipos));


    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

}

void Tema::RenderMesh(gfxc::Camera* camera, Mesh* mesh, Shader* shader, const glm::mat4& model, glm::vec3& color, Texture2D* texture1, Texture2D* texture2, Texture2D* texture3, glm::vec3& helipos, float object_id) {
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Bind view matrix
    glm::mat4 viewMatrix = camera->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (texture1)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    }

    if (texture2)
    {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    }

    if (texture3)
    {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    }
    //UNIFORM VARIABLES

    float time = static_cast<float>(Engine::GetElapsedTime()); //time
    int timeloc = glGetUniformLocation(shader->program, "time");
    glUniform1f(timeloc, time);

    glUniform3fv(glGetUniformLocation(shader->program, "heli_position"), 1, glm::value_ptr(helipos)); //helicopter_position
    glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color)); //monochromatic colour of an object
    glUniform1f(glGetUniformLocation(shader->program, "object_id"), object_id);


    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema::CreateFramebuffer(int width, int height)
{


    glGenFramebuffers(1, &framebuffer_object);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    
    glGenTextures(1, &color_texture);
    glBindTexture(GL_TEXTURE_2D, color_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);


    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);


    glGenTextures(1, &dir_texture);
    glBindTexture(GL_TEXTURE_2D, dir_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, dir_texture, 0);


    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);


    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    std::vector<GLenum> draw_textures;

    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    draw_textures.push_back(GL_COLOR_ATTACHMENT1);

    glDrawBuffers(draw_textures.size(), &draw_textures[0]);



    glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema::OnInputUpdate(float deltaTime, int mods)
{
}

void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        enablebufferdisplay = !enablebufferdisplay;
    }
}


void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

    float data[4];

    mouseY = window->props.resolution.y - mouseY;

    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object);
    glReadBuffer(GL_COLOR_ATTACHMENT1);

    glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_FLOAT, data);
    printf("PIXEL DATA: (%f, %f, %f, %f)\n", data[0], data[1], data[2], data[3]);

    if (button == 1) {
        if (static_cast<int>(data[3]) == 0) {
            ismarker = true;


            marker_position = glm::vec3(data[0], data[1], data[2]);
            helirotation = atan2(data[0] - globalheli.x, data[2] - globalheli.z + 0.0001);


            destination = glm::vec3(data[0], data[1], data[2]);
            direction = destination - globalheli;
            reachdestination = true;

        }
    }
    if (button == 2){
        if (static_cast<int>(data[3] > 1)) {
            isTreeRenderable[static_cast<int>(data[3]) - 2] = false;
        }

    }
    
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}

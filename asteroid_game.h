#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include <string>
#include<vector>
#include <unordered_map>


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
    public:
       Tema();
       ~Tema();

        void Init() override;

    private:
        class TreeComponent {
        public:
            glm::mat4 model;
            bool renderable;
            unsigned int level;
            float object_id;

            TreeComponent(glm::mat4 modelMatrix, bool boolvalue, unsigned int lvl, float id) {
                model = modelMatrix;
                renderable = boolvalue;
                level = lvl;
                object_id = id;
            }
        };

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        unsigned int framebuffer_object;
        unsigned int color_texture;
        unsigned int dir_texture;
        unsigned int depth_texture;
        void CreateFramebuffer(int width, int height);


        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::vec3& color = glm::vec3(1), Texture2D* texture1 = NULL, glm::vec3& helipos = glm::vec3(0));
        void RenderTreeMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, glm::vec3 & helipos = glm::vec3(0));
        void RenderTreeMesh2(int treelevel, Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix,float treescale = 1.0f, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, glm::vec3& helipos = glm::vec3(0));
        void RenderMesh(gfxc::Camera* camera,Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3&color = glm::vec3(1), Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, Texture2D* texture3 = NULL, glm::vec3& helipos = glm::vec3(0), float object_id = 10);

        std::unordered_map<std::string, Texture2D*> mapTextures;
        Texture2D* CreateTexture(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
        Texture2D* LoadTexture(const char* imagePath);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;


        std::vector<glm::mat4> leaves;
        std::vector<glm::mat4> branches;
        std::vector<glm::mat4> trunk;
        std::vector<bool> isTreeRenderable{ true,true,true,true,true,true };
   
        void render_helicopter(gfxc::Camera* camera, float deltaTimeSeconds,glm::vec3 position =  glm::vec3(0), float rotationangle = 0 );
        void render_marker(gfxc::Camera* camera, glm::vec3 position = glm::vec3(0), glm::vec3 heli_position = glm::vec3(0));
        void draw_scene(gfxc::Camera* camera);


        bool ismarker;
        glm::vec3 marker_position = glm::vec3(0);


        std::vector<TreeComponent> Tree1;
        std::vector<TreeComponent> Tree2;
        std::vector<TreeComponent> Tree3;
        std::vector<TreeComponent> Tree4;
        std::vector<TreeComponent> Tree5;
        std::vector<TreeComponent> Tree6;

        std::vector <std::vector<TreeComponent>> TreeCollection;

        void buildtree(std::vector<TreeComponent>& Tree, float custom_id, glm::vec3 postion = glm::vec3(0), glm::vec3& helipos = glm::vec3(0));
        void buildbranch(std::vector<TreeComponent>& Tree, float custom_id, glm::mat4 prev_branch, int position, int level, glm::vec3& helipos = glm::vec3(0));
        void place_tree(gfxc::Camera* camera, std::vector<TreeComponent>& Tree, bool custom_buffer = false);
        void place_forest(gfxc::Camera* camera, std::vector<bool>& isTreeRenderable, bool custom_buffer = false);

        float helirotation = 0 ;
        float helistep = 3.0f;
        glm::vec3 globalheli = glm::vec3(0);

        glm::vec3 destination = glm::vec3(0);
        glm::vec3 direction = glm::vec3(0);
        bool reachdestination = false;

        bool enablebufferdisplay = false;

        float angle_displacement = 0.0f;
        float angle_increment = 1.0f;
    };
}   // namespace lab
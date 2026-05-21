#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

// ImGui Headers
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// CSG Mathematics (Constructive Solid Geometry)
#include "csgjs.cpp"

// --- GODMODE RAW OPENGL INJECTION ---
#define GL_TRIANGLES 0x0004
#define GL_LINES 0x0001
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_ARRAY_BUFFER 0x8892
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MULTISAMPLE 0x809D
#define GL_DEPTH_TEST 0x0B71
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_FLOAT 0x1406

typedef void (* PFNGLGENVERTEXARRAYSPROC) (int n, unsigned int *arrays);
typedef void (* PFNGLGENBUFFERSPROC) (int n, unsigned int *buffers);
typedef void (* PFNGLBINDVERTEXARRAYPROC) (unsigned int array);
typedef void (* PFNGLBINDBUFFERPROC) (unsigned int target, unsigned int buffer);
typedef void (* PFNGLBUFFERDATAPROC) (unsigned int target, ptrdiff_t size, const void *data, unsigned int usage);
typedef void (* PFNGLVERTEXATTRIBPOINTERPROC) (unsigned int index, int size, unsigned int type, unsigned char normalized, int stride, const void *pointer);
typedef void (* PFNGLENABLEVERTEXATTRIBARRAYPROC) (unsigned int index);
typedef unsigned int (* PFNGLCREATESHADERPROC) (unsigned int type);
typedef void (* PFNGLSHADERSOURCEPROC) (unsigned int shader, int count, const char **string, const int *length);
typedef void (* PFNGLCOMPILESHADERPROC) (unsigned int shader);
typedef unsigned int (* PFNGLCREATEPROGRAMPROC) (void);
typedef void (* PFNGLATTACHSHADERPROC) (unsigned int program, unsigned int shader);
typedef void (* PFNGLLINKPROGRAMPROC) (unsigned int program);
typedef void (* PFNGLDELETESHADERPROC) (unsigned int shader);
typedef void (* PFNGLUSEPROGRAMPROC) (unsigned int program);
typedef int (* PFNGLGETUNIFORMLOCATIONPROC) (unsigned int program, const char *name);
typedef void (* PFNGLUNIFORMMATRIX4FVPROC) (int location, int count, unsigned char transpose, const float *value);
typedef void (* PFNGLUNIFORM3FPROC) (int location, float v0, float v1, float v2);

PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORM3FPROC glUniform3f;

void InjectGPUHooks() {
    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glfwGetProcAddress("glGenVertexArrays");
    glGenBuffers = (PFNGLGENBUFFERSPROC)glfwGetProcAddress("glGenBuffers");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glfwGetProcAddress("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glfwGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glfwGetProcAddress("glBufferData");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glfwGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glfwGetProcAddress("glEnableVertexAttribArray");
    glCreateShader = (PFNGLCREATESHADERPROC)glfwGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glfwGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glfwGetProcAddress("glCompileShader");
    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glfwGetProcAddress("glCreateProgram");
    glAttachShader = (PFNGLATTACHSHADERPROC)glfwGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glfwGetProcAddress("glLinkProgram");
    glDeleteShader = (PFNGLDELETESHADERPROC)glfwGetProcAddress("glDeleteShader");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glfwGetProcAddress("glUseProgram");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glfwGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glfwGetProcAddress("glUniformMatrix4fv");
    glUniform3f = (PFNGLUNIFORM3FPROC)glfwGetProcAddress("glUniform3f");
}

// --- EXPORT PROTOCOLS ---

void ExportToOBJ(const std::vector<float>& vertices, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    file << "# RealityEngine OBJ Export\n";
    for (size_t i = 0; i < vertices.size(); i += 6) {
        file << "v " << vertices[i] << " " << vertices[i+1] << " " << vertices[i+2] << "\n";
        file << "vn " << vertices[i+3] << " " << vertices[i+4] << " " << vertices[i+5] << "\n";
    }
    for (size_t i = 1; i <= vertices.size() / 6; i += 3) {
        file << "f " << i << "//" << i << " " << i+1 << "//" << i+1 << " " << i+2 << "//" << i+2 << "\n";
    }
    file.close();
}

// --- SHADERS ---
const char* gridVS = R"(#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model; uniform mat4 view; uniform mat4 projection;
void main() { gl_Position = projection * view * model * vec4(aPos, 1.0); })";

const char* gridFS = R"(#version 330 core
out vec4 FragColor; uniform vec3 gridColor;
void main() { FragColor = vec4(gridColor, 1.0); })";

const char* modelVS = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
out vec3 Normal;
uniform mat4 model; uniform mat4 view; uniform mat4 projection;
void main() {
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

const char* modelFS = R"(#version 330 core
out vec4 FragColor;
in vec3 Normal;
uniform vec3 objectColor; uniform vec3 lightDir;
void main() {
    float ambient = 0.25;
    vec3 norm = normalize(Normal);
    vec3 lDir = normalize(lightDir);
    float diff = max(dot(norm, lDir), 0.0);
    FragColor = vec4((ambient + diff) * objectColor, 1.0);
})";

unsigned int CompileShader(const char* v, const char* f) {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER); glShaderSource(vs, 1, &v, NULL); glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER); glShaderSource(fs, 1, &f, NULL); glCompileShader(fs);
    unsigned int p = glCreateProgram(); glAttachShader(p, vs); glAttachShader(p, fs); glLinkProgram(p);
    glDeleteShader(vs); glDeleteShader(fs); return p;
}

// --- PRIMITIVE LOGIC ---
csgjs_model CreateCube(glm::vec3 p, glm::vec3 s) {
    float v[] = { -0.5, -0.5, -0.5, 0, 0, -1, 0.5, -0.5, -0.5, 0, 0, -1, 0.5, 0.5, -0.5, 0, 0, -1, 0.5, 0.5, -0.5, 0, 0, -1, -0.5, 0.5, -0.5, 0, 0, -1, -0.5, -0.5, -0.5, 0, 0, -1, -0.5, -0.5, 0.5, 0, 0, 1, 0.5, -0.5, 0.5, 0, 0, 1, 0.5, 0.5, 0.5, 0, 0, 1, 0.5, 0.5, 0.5, 0, 0, 1, -0.5, 0.5, 0.5, 0, 0, 1, -0.5, -0.5, 0.5, 0, 0, 1, -0.5, 0.5, 0.5, -1, 0, 0, -0.5, 0.5, -0.5, -1, 0, 0, -0.5, -0.5, -0.5, -1, 0, 0, -0.5, -0.5, -0.5, -1, 0, 0, -0.5, -0.5, 0.5, -1, 0, 0, -0.5, 0.5, 0.5, -1, 0, 0, 0.5, 0.5, 0.5, 1, 0, 0, 0.5, 0.5, -0.5, 1, 0, 0, 0.5, -0.5, -0.5, 1, 0, 0, 0.5, -0.5, -0.5, 1, 0, 0, 0.5, -0.5, 0.5, 1, 0, 0, 0.5, 0.5, 0.5, 1, 0, 0, -0.5, -0.5, -0.5, 0, -1, 0, 0.5, -0.5, -0.5, 0, -1, 0, 0.5, -0.5, 0.5, 0, -1, 0, 0.5, -0.5, 0.5, 0, -1, 0, -0.5, -0.5, 0.5, 0, -1, 0, -0.5, -0.5, -0.5, 0, -1, 0, -0.5, 0.5, -0.5, 0, 1, 0, 0.5, 0.5, -0.5, 0, 1, 0, 0.5, 0.5, 0.5, 0, 1, 0, 0.5, 0.5, 0.5, 0, 1, 0, -0.5, 0.5, 0.5, 0, 1, 0, -0.5, 0.5, -0.5, 0, 1, 0 };
    csgjs_model m; for(int i=0; i<36; ++i) { csgjs_vertex vt; vt.pos.x=v[i*6]*s.x+p.x; vt.pos.y=v[i*6+1]*s.y+p.y; vt.pos.z=v[i*6+2]*s.z+p.z; vt.normal.x=v[i*6+3]; vt.normal.y=v[i*6+4]; vt.normal.z=v[i*6+5]; m.vertices.push_back(vt); m.indices.push_back(i); } return m;
}

csgjs_model CreateCyl(glm::vec3 p, float r, float h, int s) {
    csgjs_model m; float hh = h/2.0f;
    for(int i=0; i<s; ++i) {
        float t1 = (float)i/s*6.283185f, t2 = (float)(i+1)/s*6.283185f;
        glm::vec3 p1(cos(t1)*r, -hh, sin(t1)*r), p2(cos(t2)*r, -hh, sin(t2)*r), p3(cos(t1)*r, hh, sin(t1)*r), p4(cos(t2)*r, hh, sin(t2)*r);
        auto add = [&](glm::vec3 pos, glm::vec3 n) { csgjs_vertex v; v.pos.x=pos.x+p.x; v.pos.y=pos.y+p.y; v.pos.z=pos.z+p.z; v.normal.x=n.x; v.normal.y=n.y; v.normal.z=n.z; m.vertices.push_back(v); m.indices.push_back(m.vertices.size()-1); };
        add(p1, glm::vec3(cos(t1), 0, sin(t1))); add(p2, glm::vec3(cos(t2), 0, sin(t2))); add(p3, glm::vec3(cos(t1), 0, sin(t1))); add(p2, glm::vec3(cos(t2), 0, sin(t2))); add(p4, glm::vec3(cos(t2), 0, sin(t2))); add(p3, glm::vec3(cos(t1), 0, sin(t1)));
        add(p3, glm::vec3(0,1,0)); add(p4, glm::vec3(0,1,0)); add(glm::vec3(0,hh,0), glm::vec3(0,1,0));
        add(p2, glm::vec3(0,-1,0)); add(p1, glm::vec3(0,-1,0)); add(glm::vec3(0,-hh,0), glm::vec3(0,-1,0));
    } return m;
}

csgjs_model CreateSph(glm::vec3 p, float r, int st, int sl) {
    csgjs_model m;
    for(int i=0; i<st; ++i) {
        float ph1 = 3.14159f*i/st, ph2 = 3.14159f*(i+1)/st;
        for(int j=0; j<sl; ++j) {
            float th1 = 6.28318f*j/sl, th2 = 6.28318f*(j+1)/sl;
            auto get = [&](float t, float ph) { return glm::vec3(sin(ph)*cos(t), cos(ph), sin(ph)*sin(t)); };
            glm::vec3 v1=get(th1,ph1), v2=get(th2,ph1), v3=get(th1,ph2), v4=get(th2,ph2);
            auto add = [&](glm::vec3 pos) { csgjs_vertex vt; vt.pos.x=pos.x*r+p.x; vt.pos.y=pos.y*r+p.y; vt.pos.z=pos.z*r+p.z; vt.normal.x=pos.x; vt.normal.y=pos.y; vt.normal.z=pos.z; m.vertices.push_back(vt); m.indices.push_back(m.vertices.size()-1); };
            add(v1); add(v2); add(v3); add(v2); add(v4); add(v3);
        }
    } return m;
}

// --- GLOBAL THREADING STATE ---
std::atomic<bool> isCalculating(false);
std::atomic<bool> hasNewData(false);
std::vector<float> sharedGpuVertices;
std::mutex meshMutex;

void AsyncCSGWorker(int bShape, glm::vec3 bp, glm::vec3 bs, int sShape, glm::vec3 sp, glm::vec3 ss) {
    isCalculating = true;
    csgjs_model base, sub;
    if(bShape==0) base=CreateCube(bp, bs); else if(bShape==1) base=CreateSph(bp, bs.x, 16, 16); else base=CreateCyl(bp, bs.x/2, bs.y, 24);
    if(sShape==0) sub=CreateCube(sp, ss); else if(sShape==1) sub=CreateSph(sp, ss.x, 16, 16); else sub=CreateCyl(sp, ss.x/2, ss.y, 24);
    
    csgjs_model res = csgjs_difference(base, sub);
    
    std::vector<float> localVerts;
    for(int i : res.indices) {
        csgjs_vertex v = res.vertices[i];
        localVerts.push_back(v.pos.x); localVerts.push_back(v.pos.y); localVerts.push_back(v.pos.z);
        localVerts.push_back(v.normal.x); localVerts.push_back(v.normal.y); localVerts.push_back(v.normal.z);
    }
    
    meshMutex.lock();
    sharedGpuVertices = localVerts;
    hasNewData = true;
    meshMutex.unlock();
    isCalculating = false;
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "RealityEngine - Multithreaded Core", NULL, NULL);
    if (!window) return -1; glfwMakeContextCurrent(window); InjectGPUHooks();
    
    IMGUI_CHECKVERSION(); ImGui::CreateContext(); ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true); ImGui_ImplOpenGL3_Init("#version 330 core");

    unsigned int gridShader = CompileShader(gridVS, gridFS);
    unsigned int modelShader = CompileShader(modelVS, modelFS);

    std::vector<float> gv; int gs = 20;
    for (int i = -gs; i <= gs; ++i) {
        gv.push_back(-gs); gv.push_back(0); gv.push_back(i); gv.push_back(gs); gv.push_back(0); gv.push_back(i);
        gv.push_back(i); gv.push_back(0); gv.push_back(-gs); gv.push_back(i); gv.push_back(0); gv.push_back(gs);
    }
    unsigned int gVAO, gVBO; glGenVertexArrays(1, &gVAO); glGenBuffers(1, &gVBO);
    glBindVertexArray(gVAO); glBindBuffer(GL_ARRAY_BUFFER, gVBO); glBufferData(GL_ARRAY_BUFFER, gv.size()*4, gv.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,12,0); glEnableVertexAttribArray(0);

    unsigned int cVAO, cVBO; glGenVertexArrays(1, &cVAO); glGenBuffers(1, &cVBO);
    int cCount = 0;
    
    int bS=0, sS=2; glm::vec3 bp(0,0.5,0), bs(3,1,3), sp(0,0.5,0), ss(1,2,1);
    float mCol[3]={0.8,0.4,0.1}, camD=10, camA=45, camH=5;
    bool triggerUpdate = true;

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame(); ImGui::NewFrame();
        
        ImGui::Begin("RealityEngine Control Matrix");
        if(isCalculating) ImGui::Text("STATUS: RE-CALCULATING MESH..."); else ImGui::Text("STATUS: ENGINE READY");
        ImGui::Separator();
        ImGui::SliderFloat("Orbit", &camA, 0, 360); ImGui::SliderFloat("Zoom", &camD, 2, 50);
        if(ImGui::CollapsingHeader("Boolean Architecture", 32)) {
            const char* n[]={"Cube","Sphere","Cylinder"};
            if(ImGui::Combo("Base", &bS, n, 3)) triggerUpdate=true;
            if(ImGui::SliderFloat3("B-Scale", &bs.x, 0.1, 10)) triggerUpdate=true;
            ImGui::Separator();
            if(ImGui::Combo("Drill", &sS, n, 3)) triggerUpdate=true;
            if(ImGui::SliderFloat3("D-Pos", &sp.x, -5, 5)) triggerUpdate=true;
            if(ImGui::SliderFloat3("D-Scale", &ss.x, 0.1, 10)) triggerUpdate=true;
            ImGui::Separator();
            if(ImGui::Button("EXPORT OBJ", ImVec2(-1, 30))) {
                meshMutex.lock();
                ExportToOBJ(sharedGpuVertices, "invention_export.obj");
                meshMutex.unlock();
            }
        }
        ImGui::End();
        
        if (triggerUpdate && !isCalculating) {
            std::thread(AsyncCSGWorker, bS, bp, bs, sS, sp, ss).detach();
            triggerUpdate = false;
        }

        if (hasNewData) {
            meshMutex.lock();
            glBindVertexArray(cVAO); glBindBuffer(GL_ARRAY_BUFFER, cVBO);
            glBufferData(GL_ARRAY_BUFFER, sharedGpuVertices.size()*4, sharedGpuVertices.data(), GL_DYNAMIC_DRAW);
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,24,0); glEnableVertexAttribArray(0);
            glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,24,(void*)12); glEnableVertexAttribArray(1);
            cCount = sharedGpuVertices.size()/6;
            hasNewData = false;
            meshMutex.unlock();
        }

        glClearColor(0.1, 0.11, 0.13, 1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glm::mat4 v = glm::lookAt(glm::vec3(sin(glm::radians(camA))*camD, camH, cos(glm::radians(camA))*camD), glm::vec3(0), glm::vec3(0,1,0));
        glm::mat4 p = glm::perspective(0.78f, 1280.0f/720.0f, 0.1f, 100.0f);
        
        glUseProgram(gridShader);
        glUniformMatrix4fv(glGetUniformLocation(gridShader,"view"),1,0,glm::value_ptr(v));
        glUniformMatrix4fv(glGetUniformLocation(gridShader,"projection"),1,0,glm::value_ptr(p));
        glUniformMatrix4fv(glGetUniformLocation(gridShader,"model"),1,0,glm::value_ptr(glm::mat4(1)));
        glUniform3f(glGetUniformLocation(gridShader,"gridColor"),0.3,0.4,0.5);
        glBindVertexArray(gVAO); glDrawArrays(GL_LINES,0,gv.size()/3);

        if(cCount > 0) {
            glUseProgram(modelShader);
            glUniformMatrix4fv(glGetUniformLocation(modelShader,"view"),1,0,glm::value_ptr(v));
            glUniformMatrix4fv(glGetUniformLocation(modelShader,"projection"),1,0,glm::value_ptr(p));
            glUniformMatrix4fv(glGetUniformLocation(modelShader,"model"),1,0,glm::value_ptr(glm::mat4(1)));
            glUniform3f(glGetUniformLocation(modelShader,"objectColor"),mCol[0],mCol[1],mCol[2]);
            glUniform3f(glGetUniformLocation(modelShader,"lightDir"),0.5,1.0,0.3);
            glBindVertexArray(cVAO); glDrawArrays(GL_TRIANGLES,0,cCount);
        }

        ImGui::Render(); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    glfwTerminate(); return 0;
}

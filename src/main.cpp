#include <iostream>
#include "d_opengl.h"
#include <filesystem>
#include "shaders/d_Shader.h"

#include <iostream>
#include <numeric>

#ifndef D_REPEATS
#define D_REPEATS 10
#endif 

#ifndef D_INSTANCED
#define D_INSTANCED 0 // 0: non-instanced, 1: vertex attrib instanced, 2: SSBO instanced
#endif

#ifndef D_VEC
#define D_VEC 1  
#endif

#ifndef D_TEST_TIME
#define D_TEST_TIME 3
#endif

#ifndef D_ALLOCATE_PERSEC
#define D_ALLOCATE_PERSEC 5000
#endif

#ifndef D_INIT_SIZE
#define D_INIT_SIZE 1048576
#endif

#ifndef D_DEST_SIZE
#define D_DEST_SIZE 2048*1024*2
#endif

const int d_Stepper = D_INIT_SIZE;                  // first value to be evaluated.
const int d_StepperGoal = D_DEST_SIZE;
const int d_StepperCount = d_StepperGoal / d_Stepper;

const std::string d_specialAppend = "-hq";          // value to be appended to the output filenames.

#if D_INSTANCED == 1
const char* d_typeAppend = "_vertAttrib";           // method identifier for the output filename.
#elif D_INSTANCED == 2
const char* d_typeAppend = "_SSBO";
#else
const char* d_typeAppend = "";
#endif

int main() {
    GLFWwindow* d_Window;
    if (!(d_Window = d_setupGLFW())) {        
        return 0;
    };
    std::cout << "Current working directory: "
        << std::filesystem::current_path() << std::endl;


    d_Shader d_defaultShader(D_VEC, D_INSTANCED);

    GLint d_activeParams;
    glGetProgramiv(d_defaultShader.d_ShaderHandle, GL_ACTIVE_ATTRIBUTES, &d_activeParams);
    d_Logger::info("d_activeParams: " + std::to_string(d_activeParams));

    GLuint d_uniPosLocation = glGetUniformLocation(d_defaultShader.d_ShaderHandle, "d_uOffset");

    float t_time = 0;

    GLuint d_vao;
    glGenVertexArrays(1, &d_vao);
    glBindVertexArray(d_vao);

#if D_VEC == 1
    const std::string d_VecString = "vec1";
    struct d_Vertex {
        glm::vec3 val1;
    };
#elif D_VEC == 2
    const std::string d_VecString = "vec2";
    struct d_Vertex {
        glm::vec3 val1;
        glm::vec3 val2;
    };
#else 
    const std::string d_VecString = "vec3";
    struct d_Vertex {
        glm::vec3 val1;
        glm::vec3 val2;
        glm::vec3 val3;
    };
#endif

    const d_Vertex d_data[] = {
        { { -0.5f, -10.0f, 0.0f} },
        { { 0.0f,  -10.5f, 0.0f} },
        { { 0.5f,  -10.0f, 0.0f} }
    };

    std::vector<unsigned int> d_elem = { 0, 1, 2 };       

    std::vector<glm::vec3> d_instanced = {
    };

    int amount = d_Stepper;
    //std::cout << "amount: " << amount << std::endl;
    
    GLuint d_buffs[3];
    glGenBuffers(3, &d_buffs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, d_buffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(d_Vertex)*3, &d_data[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_buffs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * d_elem.size(), d_elem.data(), GL_STATIC_DRAW);

#if D_VEC == 1
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)0);
    glEnableVertexAttribArray(0);
#elif D_VEC == 2
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)(sizeof(glm::vec3)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
#else 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)(sizeof(glm::vec3)));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(d_Vertex), (const void*)(2 * sizeof(glm::vec3)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
#endif


    d_instanced.clear();
    for (int i = 0; i < d_StepperGoal; i++) {
        d_instanced.push_back({ 0.0f, 0.0f, 0.0f });
    };

#if D_INSTANCED == 1
    glBindBuffer(GL_ARRAY_BUFFER, d_buffs[2]);
    glBufferData(GL_ARRAY_BUFFER, d_instanced.size() * sizeof(glm::vec3), &d_instanced[0], GL_STATIC_DRAW);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (const void*)0);
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
#elif D_INSTANCED == 2
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, d_buffs[2]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, d_instanced.size() *sizeof(glm::vec3), &d_instanced[0], GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, d_buffs[2]);
#endif
   
    bool d_nextTest = false;
    std::vector<std::vector<float>> d_times(D_REPEATS);
    for (int i = 0; i < D_REPEATS; i++)
        d_times[i].reserve(D_TEST_TIME * D_ALLOCATE_PERSEC);

    struct d_log {
        int d_amount;
        float d_avg;
    };

    std::vector<d_log> d_entries;

    int d_repeat = 0;       
    size_t currIt = 0;
    while (!glfwWindowShouldClose(d_Window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwPollEvents();

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        t_time += deltaTime;
        d_times[d_repeat].push_back(deltaTime);
        
        if (!d_nextTest) {
            glUseProgram(d_defaultShader.d_ShaderHandle);
            glBindVertexArray(d_vao);

#if D_INSTANCED == 0
            for (int i = 0; i < amount; i++) { 
                glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
            }
#else
            glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL, amount);
#endif
        
            glfwSwapBuffers(d_Window);
        }
        else {
            std::vector<float> resAvgs;
            for (int i = 0; i < D_REPEATS; i++) {
                resAvgs.push_back(std::accumulate(d_times[i].begin(), d_times[i].end(), 0.0f) / d_times[i].size());
            }            
            float finalAvg = std::accumulate(resAvgs.begin(), resAvgs.end(), 0.0f) / (float)D_REPEATS * 1000.0;
#if D_INSTANCED == 1
            std::cout << "[" << currIt++ << "/" << d_StepperCount << "] glDrawElementsInstanced(); instances: " << amount << "; final avg ms: " << finalAvg << std::endl;
#else 
            std::cout << "[" << currIt++ << "/" << d_StepperCount << "] glDrawElements(); instances: " << amount << "; final avg ms: " << finalAvg << std::endl;
#endif
            d_entries.push_back({ amount, finalAvg });

            d_times.clear();        
            d_times.resize(D_REPEATS);
            amount += d_Stepper;
            d_nextTest = false;

            if (amount > d_StepperGoal) {
                break;
            }
        }

        if (t_time >= D_TEST_TIME) {            
            if (++d_repeat == D_REPEATS) {
                d_nextTest = true;
                d_repeat = 0;
            }
            t_time = 0.0f;
        }
    }
#if D_INSTANCED == 0
    auto d_csv = std::ofstream("csv/glDrawElements_" + d_VecString + "_" + d_typeAppend + d_specialAppend + ".csv");
#else
    auto d_csv = std::ofstream("csv/glDrawElementsInstanced_" + d_VecString + d_typeAppend + d_specialAppend + ".csv");
#endif

    d_csv << "amount,avgMs" << std::endl;
    for (auto el : d_entries) {
        d_csv << el.d_amount << "," << el.d_avg << std::endl;
    }

    glDeleteBuffers(2, &d_buffs[0]);    

    glfwDestroyWindow(d_Window);
    glfwTerminate();

	return 0;
}
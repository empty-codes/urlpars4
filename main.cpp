#include <imgui.h>                 
#include "imgui_impl_glfw.h"     
#include "imgui_impl_opengl3.h" 

#include <stdio.h>
#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

#include <iostream>
#include <regex>
#include <string>

using namespace std;

struct ParsedURL {
    string scheme;
    string host;
    string port;
    string path;
    string query;
    string fragment;
};

ParsedURL parseURL(const string& url) {
    ParsedURL parsed;

    regex url_regex(R"((https?|ftp)://([^/\s?#]+)(:(\d+))?(/[^?#]*)?(\?[^#]*)?(#\S*)?)");
    // regex url_regex(R"((https?|ftp)://([^:/\s]+)(:(\d+))?(/[\w/]+)?(\?[\w=&]+)?(#\w+)?)"); too strict
    smatch match;

    if (regex_match(url, match, url_regex)) {
        parsed.scheme = match[1];
        parsed.host = match[2];
        parsed.port = match[4].matched ? match[4].str() : ""; 
        parsed.path = match[5].matched ? match[5].str() : "";
        parsed.query = match[6].matched ? match[6].str() : "";
        parsed.fragment = match[7].matched ? match[7].str() : "";
    } else {
        parsed.scheme = "Invalid URL";
    }

    return parsed;
}

int main() {
    if (!glfwInit()) {
        cerr << "GLFW Initialization failed!" << endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(1280, 720, "URL Parser", NULL, NULL);
    if (!window) {
        cerr << "GLFW window creation failed!" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // enables v-sync, don't think i'll use it though

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    bool show_demo_window = true;
    char url_input[256] = "";
    ParsedURL parsed_url;
    bool is_parsed = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("URL Parser");

        ImGui::InputText("Enter URL", url_input, sizeof(url_input));

        string url_string(url_input);

        if (ImGui::Button("Parse")) {
            if (url_string.empty()) {
                cerr << "URL is empty!" << endl;
                is_parsed = false;
            } else {
                parsed_url = parseURL(url_input);
                is_parsed = true;
            }
        }

        if (is_parsed) {
            ImGui::Text("Scheme: %s", parsed_url.scheme.c_str());
            ImGui::Text("Host: %s", parsed_url.host.c_str());
            ImGui::Text("Port: %s", parsed_url.port.c_str());
            ImGui::Text("Path: %s", parsed_url.path.c_str());
            ImGui::Text("Query: %s", parsed_url.query.c_str());
            ImGui::Text("Fragment: %s", parsed_url.fragment.c_str());
        }

        ImGui::End();

        ImGui::Render();
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

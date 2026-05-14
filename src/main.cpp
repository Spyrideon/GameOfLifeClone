#include <glad/glad.h>      // MUST be before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "CellGrid.h"
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* init();

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 1000;
// for now grid width/height is a 10th of window width/height
constexpr int GRID_WIDTH = 100;
constexpr int GRID_HEIGHT = 100;

enum class DrawMode {
    ADD, REMOVE
};

int main() {

    GLFWwindow* window = init();
    if (!window) return -1;

    const Shader shader("../src/shader/vertex.shader", "../src/shader/fragment.shader");
    constexpr float vertices[] = {
        1.0f, 1.0f,         1.0f, 0.0f,
        -1.0f, 1.0f,        0.0f, 0.0f,
        -1.0f, -1.0f,       0.0f, 1.0f,
        1.0f, -1.0f,        1.0f, 1.0f
    };  // vertex points    // texture coords
    constexpr unsigned int indices[] = {
        0, 1, 2,
        0, 3, 2
    };// draw order

    unsigned int VBO, VAO, EBO;
    glGenBuffers(1, &VBO); glGenVertexArrays(1, &VAO); glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attr
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //texture coord attr
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0); // good practice

    CellGrid cell_grid(GRID_WIDTH, GRID_HEIGHT);

    double lastUpdateTime = glfwGetTime();
    const double updateInterval = 1.0 / 10.0;


    bool is_paused = false;
    bool space_was_pressed = false;
    auto draw_mode = DrawMode::ADD;
    // Render loop
    while (!glfwWindowShouldClose(window)) {

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        bool space_is_pressed = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
        if (space_is_pressed && !space_was_pressed)
            is_paused = !is_paused;
        space_was_pressed = space_is_pressed;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            draw_mode = DrawMode::ADD;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
            draw_mode = DrawMode::REMOVE;
        }

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            double x_coord, y_coord;
            glfwGetCursorPos(window, &x_coord, &y_coord);
            int win_width, win_height;
            glfwGetWindowSize(window, &win_width, &win_height);

            const bool cursor_in_window =   x_coord >= 0 && x_coord < win_width &&
                                            y_coord >= 0 && y_coord < win_height;
            if (cursor_in_window) {
                const int cell_x = static_cast<int>(x_coord / win_width  * GRID_WIDTH);
                const int cell_y = static_cast<int>(y_coord / win_height * GRID_HEIGHT);

                if   (draw_mode == DrawMode::ADD)
                    cell_grid.addCell(cell_x, cell_y);
                else
                    cell_grid.removeCell(cell_x, cell_y);
            }
        }


        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double currentTime = glfwGetTime();
        double elapsed = currentTime - lastUpdateTime;


        if (!is_paused) {
            if (elapsed >= updateInterval) {
                cell_grid.update();
                lastUpdateTime = currentTime;
            }
        }
        cell_grid.uploadTexToGPU();

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

GLFWwindow* init() {
    // Init GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game of Life", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize glad\n";
        glfwTerminate();
        return nullptr;
    }

    int fb_width, fb_height;
    glfwGetFramebufferSize(window, &fb_width, &fb_height);
    std::cout << fb_width << "x" << fb_height << "\n";

    return window;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
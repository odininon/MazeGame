//
// Created by Michael Stengel on 9/3/2022.
//

#ifndef MAZEGAMECONAN_GAMEMANAGER_H
#define MAZEGAMECONAN_GAMEMANAGER_H

class GLFWwindow;

class GameManager {

public:
    GameManager();
    ~GameManager() = default;

    void run();

private:
    GLFWwindow *window;
    unsigned int shaderProgram;
};


#endif //MAZEGAMECONAN_GAMEMANAGER_H

#pragma once

/*
to make new scene
1. add it to enum SceneType
2. go to Core/GameLoop/SceneManager.cpp to loadScene() add it to switch
3. go to existed scene and updateChangeScene() to make way to your new scene
*/

enum class SceneType { // all states of the game
    None,
    Start,
    Menu,
    WorldSelection,
    Game
};
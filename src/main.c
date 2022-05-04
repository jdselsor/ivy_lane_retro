#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "raylib/raylib.h"

#define DEFAULT_PROJECT_NAME "DEFAULT PROJECT NAME"
#define DEFAULT_SCREEN_WIDTH 800
#define DEFAULT_SCREEN_HEIGHT 600
#define DEFAULT_RENDER_TEXTURE_WIDTH 320
#define DEAFULT_RENDER_TEXTURE_HEIGHT 240
#define DEFAULT_GLSL_VERSION 330

struct Settings {
    char* project_name;
    size_t fps;

    size_t debug;

    size_t screenWidth;
    size_t screenHeight;
    size_t renderTextureWidth;
    size_t renderTextureHeight;

    size_t glslVersion;
} settings;

RenderTexture2D target;
Camera camera;

void Initalize ();
void Update ();
void Render ();
void CleanUp ();

void ReadConfigFile (char*);

int main(void) {
    Initalize();

    while (!WindowShouldClose())
    {
        Update();
        Render();
    }

    CleanUp();
    CloseWindow();

    return 0;
}

void Initalize ()  {
    SetConfigFlags (FLAG_MSAA_4X_HINT);

    settings.project_name = "PROJECT NAME";
    settings.fps = 60;
    settings.debug = false;
    settings.screenWidth = DEFAULT_SCREEN_WIDTH;
    settings.screenHeight = DEFAULT_SCREEN_HEIGHT;
    settings.renderTextureWidth = DEFAULT_RENDER_TEXTURE_WIDTH;
    settings.renderTextureHeight = DEAFULT_RENDER_TEXTURE_HEIGHT;

    ReadConfigFile(".config");

    InitWindow(settings.screenWidth, settings.screenHeight, settings.project_name);
    SetTargetFPS (settings.fps);

    camera.position = (Vector3) { 2.0f, 3.0f, 2.0f };
    camera.target = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.up = (Vector3) { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = 0;

    SetCameraMode(camera, CAMERA_ORBITAL);

    target = LoadRenderTexture(settings.renderTextureWidth, settings.renderTextureHeight);
}

void Update () {
    UpdateCamera(&camera);
}

void Render () {
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
            DrawGrid(20, 10.0f);
        EndMode3D();
    EndTextureMode();

    // Draw Render Texture
    BeginDrawing();

    ClearBackground(RAYWHITE);
        DrawTexturePro(
            target.texture,
            (Rectangle){
                0.0f,
                0.0f,
                target.texture.width,
                -target.texture.height
            },
            (Rectangle){
                0.0f,
                0.0f,
                GetScreenWidth(),
                GetScreenHeight()
            },
            (Vector2){
                0.0f,
                0.0f
            },
            0.0f,
            WHITE);
    EndDrawing();
}

void CleanUp () {
    UnloadRenderTexture(target);
}

void ReadConfigFile (char* configPath) {
    FILE *configFile;
    char buffer[256];

    configFile = fopen(configPath, "r");

    if (configFile == NULL) {
        printf("COULD NOT FIND .config at %s\n", configPath);
        return;
    }

    while (fgets(buffer, 256, configFile)) {
        // # is a comment in the .config file. This is just
        // ignored.
        if (buffer[0] == '#') {
            continue;    
        }

        char temp [256];
        strcpy(temp, buffer);
        // creates the initial "key" token.
        char* token = strtok(temp, ":"); 

        while (token) {
            if (strcasecmp(token, "project_name") == 0) {
                token = strtok(NULL, ":");
                strcpy(token, settings.project_name);
            } else if (strcasecmp(token, "fps") == 0) {
                token = strtok(NULL, ":");
                settings.fps = atoi(token);
            } else if (strcasecmp(token, "debug") == 0) {
                token = strtok(NULL, ":");
                settings.debug = atoi(token);
            } else if (strcasecmp(token, "screen_width") == 0) {
                token = strtok(NULL, ":");
                settings.screenWidth = atoi(token);
            } else if (strcasecmp(token, "screen_height") == 0) {
                token = strtok(NULL, ":");
                settings.screenHeight = atoi(token);
            } else if (strcasecmp(token, "render_texture_width") == 0) {
                token = strtok(NULL, ":");
                settings.renderTextureWidth = atoi(token);
            } else if (strcasecmp(token, "render_texture_height") == 0) {
                token = strtok(NULL, ":");
                settings.renderTextureHeight = atoi(token);
            } else if (strcasecmp(token, "glsl_version") == 0) {
                token = strtok(NULL, ":");
                settings.renderTextureHeight = atoi(token);
            }

            token = NULL;
        }
    }

    fclose(configFile);
}
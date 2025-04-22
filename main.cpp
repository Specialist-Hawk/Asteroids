#include "raylib.h"
#include <cmath>
#include <vector>

#define RAD2DEG 57.2957795
#define DEG2RAD 0.01745329

using namespace std;

// -------------------- SpaceObject --------------------
class SpaceObject {
protected:
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
    float rotation;
    Vector2 velocity;

public:
    SpaceObject(const char* texPath, Rectangle des = { 920, 500, 80, 80 }) {
        texture = LoadTexture(texPath);
        source = { 0, 0, (float)texture.width, (float)texture.height };
        dest = des;
        origin = { dest.width / 2.0f, dest.height / 2.0f };
        rotation = 0.0f;
        velocity = { 0.0f, 0.0f };
    }

    virtual ~SpaceObject() {
        UnloadTexture(texture);
    }

    virtual void Draw() {
        DrawTexturePro(texture, source, dest, origin, rotation, WHITE);
    }

    Rectangle& GetDest() { return dest; }
    float GetRotation() const { return rotation; }
};


// -------------------- PlayerShip --------------------
class PlayerShip : public SpaceObject {

public:
    PlayerShip(const char* texPath)
        : SpaceObject(texPath) {}

    void Move() {
        const float speed = 0.8f;
        const float friction = 0.95f;

        if (IsKeyDown(KEY_W)) velocity.y -= speed;
        if (IsKeyDown(KEY_S)) velocity.y += speed;
        if (IsKeyDown(KEY_A)) velocity.x -= speed;
        if (IsKeyDown(KEY_D)) velocity.x += speed;

        velocity.x *= friction;
        velocity.y *= friction;

        dest.x += velocity.x;
        dest.y += velocity.y;
    }

    void Rotate() {
        Vector2 mouse = GetMousePosition();

        float dy = mouse.y - dest.y;
        float dx = mouse.x - dest.x;

        float angle = atan2f(dy, dx) * RAD2DEG;
        rotation = angle-90;
    }
};

// -------------------- Custom Cursor --------------------
void CustomCursor(Texture2D const cursor) {
    HideCursor();
    Vector2 MousePos = GetMousePosition();
    DrawTexture(cursor, MousePos.x, MousePos.y, WHITE);
}

// -------------------- Main --------------------
int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(1920, 1080, "Asteroids");
    SetTargetFPS(60);

    PlayerShip player("../assets/ships/purple.png");
    Texture2D cursor = LoadTexture("../assets/cursor.png");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);
        CustomCursor(cursor);
        DrawText("PRESS 'ESC' TO EXIT. | 'SPACE' TO FIRE", 30, 30, 20, BLACK);

        player.Move();
        player.Rotate();
        player.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
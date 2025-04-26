#include "raylib.h"
#include <vector>
#include <cmath>

#define RAD2DEG 57.2957795
#define DEG2RAD 0.01745329

Texture2D bulletTexture;
Texture2D playerTexture;

using namespace std;

// -------------------- SpaceObject --------------------
class SpaceObject {
public:
    Texture2D texture;
    Rectangle source;
    Rectangle dest;
    Vector2 origin;
    float rotation;
    Vector2 velocity;

public:
    SpaceObject(Texture2D tex, Rectangle des = { 920, 500, 80, 80 }, float rot = 0.0f, Vector2 vel = {0, 0})
        : dest(des), rotation(rot), velocity(vel)
    {
        texture = tex;
        source = { 0, 0, (float)texture.width, (float)texture.height };
        origin = { dest.width / 2.0f, dest.height / 2.0f };
    }

    virtual void Draw() const {
        DrawTexturePro(this->texture, source, dest, origin, rotation, WHITE);
    }

    Rectangle& GetDest() { return dest; }
    float GetRotation() const { return rotation; }
};

// -------------------- Bullet --------------------
class Bullet : public SpaceObject {
public:
    Bullet( Texture2D bulletTexture, Rectangle des, float rot, Vector2 vel)
        : SpaceObject(bulletTexture, des, rot, vel) {}

    void Update() {
        dest.x += velocity.x * GetFrameTime();
        dest.y += velocity.y * GetFrameTime();
    }
};

// -------------------- PlayerShip --------------------
class PlayerShip : public SpaceObject {
public:
    vector<Bullet> bullets;

public:
    PlayerShip(Texture2D texture)
        : SpaceObject(texture) {}

    void Move() {
        const float speed = 80.0f;
        const float friction = 0.90f;

        if (IsKeyDown(KEY_W)) velocity.y -= speed * GetFrameTime();
        if (IsKeyDown(KEY_S)) velocity.y += speed * GetFrameTime();
        if (IsKeyDown(KEY_A)) velocity.x -= speed * GetFrameTime();
        if (IsKeyDown(KEY_D)) velocity.x += speed * GetFrameTime();

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
        rotation = angle - 90;
    }

    void Fire() {
        Vector2 bulletVel = {
            cosf((rotation + 90) * DEG2RAD) * 600.0f,
            sinf((rotation + 90) * DEG2RAD) * 600.0f
        };

        Rectangle bulletRect = { dest.x, dest.y, 16, 16 };

        bullets.push_back(Bullet(bulletTexture, bulletRect = {dest.x, dest.y, 64, 64}, rotation+90, bulletVel));
    }

    void UpdateBullets() {
        for (auto& bullet : bullets) {
            bullet.Update();
            bullet.Draw();
        }
    }
};

// -------------------- Custom Cursor --------------------
void CustomCursor(Texture2D const cursor) {
    HideCursor();
    Vector2 mousePos = GetMousePosition();
    DrawTexture(cursor, mousePos.x, mousePos.y, WHITE);
}

// -------------------- Main --------------------
int main() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(1920, 1080, "Asteroids");
    SetTargetFPS(60);

    playerTexture = LoadTexture("../assets/purple.png");
    Texture2D cursor = LoadTexture("../assets/cursor.png");
    bulletTexture = LoadTexture("../assets/bullet.png");
    PlayerShip player(playerTexture);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        player.Move();
        player.Rotate();
        player.Draw();
        player.UpdateBullets();

        if (IsKeyPressed(KEY_SPACE)) {
            player.Fire();
        }

        CustomCursor(cursor);
        DrawText("PRESS 'ESC' TO EXIT. | 'SPACE' TO FIRE", 30, 30, 20, BLACK);

        EndDrawing();
    }

    UnloadTexture(cursor);
    CloseWindow();
    return 0;
}

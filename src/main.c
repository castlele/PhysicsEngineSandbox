#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>

#define BG_COLOR (Color) { 20, 20, 20, 255, }
#define ACCENT_COLOR (Color) { 254, 255, 173, 255, }

#define WINDOW_W 800
#define WINDOW_H 600

#define GRAVITY 800

#define MAX_OBJS 24

typedef enum ColliderType
{
    STATIC, DYNAMIC,
} ColliderType;

typedef struct Collider
{
    float x;
    float y;
    float width;
    float height;
    float speedX;
    float speedY;
    char name[16];
    Color color;
    ColliderType type;
} Collider;

typedef enum GetCollidedSide
{
    LEFT, RIGHT, TOP, BOTTOM,
} CollidedSide;

Collider *objects[MAX_OBJS];
int currentIndex = -1;

void ListenEvents();
void UpdatePhysics();

void CreateWalls();
Collider *CreateCollider(float x, float y, float w, float h);
bool CheckCollision(Collider *lhs, Collider *rhs);
CollidedSide GetCollidedSide(bool isCollided, Collider *lhs, Collider *rhs);
void BumpColliders(CollidedSide side, Collider *lhs, Collider *rhs);
void DrawCollider(Collider *c);

int main()
{
    InitWindow(WINDOW_W, WINDOW_H, "Physics Engine Example");
    CreateWalls();

    while (!WindowShouldClose()) {
        ListenEvents();

        UpdatePhysics();

        BeginDrawing();
            ClearBackground(BG_COLOR);

            for (int i = 0; i <= currentIndex; i++) {
                DrawCollider(objects[i]);
            }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

// Private

void ListenEvents()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        currentIndex++;
        Vector2 pos = GetMousePosition();
        objects[currentIndex] = CreateCollider(pos.x, pos.y, 50, 50);
        snprintf(objects[currentIndex]->name, 6, "D: %i", currentIndex);
    }
}

void UpdatePhysics()
{
    float dt = GetFrameTime();

    for (int i = 0; i <= currentIndex; i++) {
        Collider *lhs = objects[i];

        if (lhs->type == STATIC) continue;

        for (int j = 0; j <= currentIndex; j++) {
            if (i == j) continue;

            Collider *rhs = objects[j];
            bool isCollided = CheckCollision(lhs, rhs);

            if (isCollided) {
                CollidedSide side = GetCollidedSide(isCollided, lhs, rhs);
                // BumpColliders(side, lhs, rhs);

                switch (side) {
                    case LEFT:
                    case RIGHT:
                        lhs->speedX = 0;
                        break;
                    case TOP:
                    case BOTTOM:
                        lhs->speedY = 0;
                        break;
                }
            }
        }

        lhs->speedY += dt * GRAVITY;

        lhs->x += dt * lhs->speedX;
        lhs->y += dt * lhs->speedY;
    }
}

void CreateWalls()
{
    currentIndex++;
    Collider *floor = CreateCollider(0, WINDOW_H - 50, WINDOW_W, 50);
    floor->type = STATIC;
    floor->color = WHITE;
    snprintf(floor->name, 2, "F");
    objects[currentIndex] = floor;

    currentIndex++;
    Collider *lWall = CreateCollider(0, 0, 50, WINDOW_H);
    lWall->type = STATIC;
    lWall->color = WHITE;
    snprintf(lWall->name, 3, "LW");
    objects[currentIndex] = lWall;

    currentIndex++;
    Collider *rWall = CreateCollider(WINDOW_W - 50, 0, 50, WINDOW_H);
    rWall->type = STATIC;
    rWall->color = WHITE;
    snprintf(rWall->name, 3, "RW");
    objects[currentIndex] = rWall;
}

Collider *CreateCollider(float x, float y, float w, float h)
{
    Collider *collider = malloc(sizeof(Collider));
    collider->x = x;
    collider->y = y;
    collider->width = w;
    collider->height = h;
    collider->speedX = 0;
    collider->speedY = 0;
    collider->color = ACCENT_COLOR;
    collider->type = DYNAMIC;

    return collider;
}

bool CheckCollision(Collider *lhs, Collider *rhs)
{
    float pmaxX = fmax(lhs->x, rhs->x);
    float pmaxY = fmax(lhs->y, rhs->y);
    float pminX = fmin(lhs->x + lhs->width, rhs->x + rhs->width);
    float pminY = fmin(lhs->y + lhs->height, rhs->y + rhs->height);

    return pminX - pmaxX >= 0 && pminY - pmaxY >= 0;
}

CollidedSide GetCollidedSide(bool isCollided, Collider *lhs, Collider *rhs)
{
    assert(isCollided);

    float right = (lhs->x + lhs->width) - rhs->x;
    float left = (rhs->x + rhs->width) - lhs->x;
    float top = (lhs->y + lhs->height) - rhs->y;
    float bottom = (rhs->y + rhs->height) - lhs->y;

    if (right < left && right < top && right < bottom) {
        return RIGHT;
    } else if (left < top && left < bottom) {
        return LEFT;
    } else if (top < bottom) {
        return TOP;
    } else {
        return BOTTOM;
    }
}

void BumpColliders(CollidedSide side, Collider *lhs, Collider *rhs)
{
    printf("Bumping of %s and %s is %i\n", lhs->name, rhs->name, side);
    switch (side) {
        case RIGHT:
            lhs->x = rhs->x - lhs->width;
            break;
        case LEFT:
            lhs->x = rhs->x + rhs->width;
            break;
        case TOP:
            lhs->y = rhs->y + rhs->height;
            break;
        case BOTTOM:
            lhs->y = rhs->y - lhs->height;
            break;
    }
}

void DrawCollider(Collider *c)
{
    DrawRectangleLines(c->x, c->y, c->width, c->height, c->color);
}

#ifndef RENDERING_H
#define RENDERING_H

#include <raylib.h>
#include "settings.h"
#include "types.h"
#include "vector_math.h"

// The world is built in a Z-up, right-handed coordinate system (x = width, y = depth,
// z = height), but raylib's Camera3D is Y-up. Moving height into y is just an axis swap,
// which on its own would flip handedness (mirroring left/right in the rendered view), so
// depth is also negated to keep the transform a rotation rather than a reflection.
inline Vector3 toRayVec(struct vector v) {
    return Vector3{ (float)v.x, (float)v.z, -(float)v.y };
}

// Builds a raylib perspective camera from the player position/facing.
// fovYDeg is the vertical field of view in degrees.
inline Camera3D buildCamera3D(struct vector C, struct vector Cf, double fovYDeg) {
    Camera3D camera = {0};
    camera.position   = toRayVec(C);
    camera.target     = toRayVec(vector{C.x + Cf.x, C.y + Cf.y, C.z + Cf.z});
    camera.up         = Vector3{0.0f, 1.0f, 0.0f};
    camera.fovy       = (float)fovYDeg;
    camera.projection = CAMERA_PERSPECTIVE;
    return camera;
}

// Draws a single world-space face as a 3D triangle. Backface culling is disabled
// globally (see main.cpp setup), so winding order doesn't matter here.
inline void drawFaceTriangle3D(const struct face& f, Color tint) {
    DrawTriangle3D(toRayVec(f.P1), toRayVec(f.P2), toRayVec(f.P3), tint);
}

inline void drawFaceWireframe3D(const struct face& f, Color tint) {
    Vector3 p1 = toRayVec(f.P1), p2 = toRayVec(f.P2), p3 = toRayVec(f.P3);
    DrawLine3D(p1, p2, tint);
    DrawLine3D(p2, p3, tint);
    DrawLine3D(p3, p1, tint);
}

// Draws thin wireframe edges along the world boundary.
inline void drawWorldBorder3D(Color color) {
    Vector3 center = { (float)worldWidth / 2.0f, (float)worldHeight / 2.0f, -(float)worldDepth / 2.0f };
    DrawCubeWires(center, (float)worldWidth, (float)worldHeight, (float)worldDepth, color);
}

#endif // RENDERING_H

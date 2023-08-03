#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

Model *model = NULL;
const int width = 800;
const int height = 800;

Vec3f barycentric(Vec2i *pts, Vec2i P);
void triangle(Vec2i *pts, TGAImage &image, TGAColor color);

int main(int argc, char **argv)
{
    if (2 == argc)
    {
        model = new Model(argv[1]);
    }
    else
    {
        model = new Model("../obj/african_head.obj");
    }

    TGAImage image(width, height, TGAImage::RGB);

    Vec3f light_dir(0, 0, -1);

    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            screen_coords[j] = Vec2i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2.);
            world_coords[j] = v;
        }
        Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;
        if (intensity > 0)
        {
            triangle(screen_coords, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
        // flat shader : random color
        // triangle(screen_coords, image, TGAColor(rand() % 255, rand() % 255, rand() % 255, 255));
        }

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}

Vec3f barycentric(Vec2i *pts, Vec2i P)
{
    Vec3f u = Vec3f(
                  pts[2][0] - pts[0][0],
                  pts[1][0] - pts[0][0],
                  pts[0][0] - P[0]) ^
              Vec3f(
                  pts[2][1] - pts[0][1],
                  pts[1][1] - pts[0][1],
                  pts[0][1] - P[1]);
    if (std::abs(u.z) < 1)
        return Vec3f(-1, 1, 1);

    return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
}

void triangle(Vec2i *pts, TGAImage &image, TGAColor color)
{
    Vec2i bboxmin(image.get_width() - 1, image.get_height() - 1);
    Vec2i bboxmax(0, 0);
    Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < 3; i++)
    {
        bboxmin.x = std::max(0, std::min(bboxmin.x, pts[i].x));
        bboxmin.y = std::max(0, std::min(bboxmin.y, pts[i].y));
        bboxmax.x = std::max(clamp.x, std::max(bboxmax.x, pts[i].x));
        bboxmax.y = std::max(clamp.y, std::max(bboxmax.y, pts[i].y));
    }

    Vec2i P;
    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {
        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vec3f bc_screen = barycentric(pts, P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            image.set(P.x, P.y, color);
        }
    }
}

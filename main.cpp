#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

#include <iostream>

const int width = 800;
const int height = 800;
const int depth = 255;

Model *model = NULL;
int *zbuffer = NULL;
Vec3f light_dir(0, 0, -1);
Vec3f camera(0, 0, 3);

Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
void triangle(Vec3i *pts, Vec2i *uv, TGAImage &image, float intensity, int *zbuffer);
Matrix viewport(int x, int y, int w, int h);

Vec3f world2screen(Vec3f v)
{
    return Vec3f(int((v.x + 1.) * width / 2. + .5), int((v.y + 1.) * height / 2. + .5), int((v.z + 1.) * depth / 2. + .5));
}

Vec3f m2v(Matrix m)
{
    return Vec3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

Matrix v2m(Vec3f v)
{
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}

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

    zbuffer = new int[width * height];
    for (int i = 0; i < width * height; i++)
    {
        zbuffer[i] = std::numeric_limits<int>::min();
    }

    {
        // draw the model
        Matrix Projection = Matrix::identity(4);
        Matrix ViewPort = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
        Projection[3][2] = -1.f / camera.z;

        TGAImage image(width, height, TGAImage::RGB);

        for (int i = 0; i < model->nfaces(); i++)
        {
            std::vector<int> face = model->face(i);
            Vec3i screen_coords[3];
            Vec3f world_coords[3];
            Vec2i uv[3];
            for (int j = 0; j < 3; j++)
            {
                Vec3f v = model->vert(face[j]);
                // screen_coords[j] = world2screen(v);
                screen_coords[j] = m2v(ViewPort * Projection * v2m(v));
                world_coords[j] = v;
                uv[j] = model->uv(i, j);
            }

            Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
            n.normalize();
            float intensity = n * light_dir;
            if (intensity > 0)
            {
                triangle(screen_coords, uv, image, intensity, zbuffer);
            }
        }

        image.flip_vertically();
        image.write_tga_file("output.tga");
    }

    {
        // dump z-buffer
        TGAImage zbimage(width, height, TGAImage::GRAYSCALE);
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                zbimage.set(i, j, TGAColor(zbuffer[i + j * width], 1));
            }
        }
        zbimage.flip_vertically();
        zbimage.write_tga_file("zbuffer.tga");
    }

    delete model;
    delete[] zbuffer;

    return 0;
}
Matrix viewport(int x, int y, int w, int h)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
}
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f s[2];
    for (int i = 2; i--;)
    {
        s[i][0] = C[i] - A[i];
        s[i][1] = B[i] - A[i];
        s[i][2] = A[i] - P[i];
    }
    Vec3f u = s[0] ^ s[1];
    if (std::abs(u[2]) > 1e-2)
        return Vec3f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
    return Vec3f(-1, 1, 1);
}

void triangle(Vec3i *pts, Vec2i *uv, TGAImage &image, float intensity, int *zbuffer)
{
    Vec2f bboxmin(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width() - 1, image.get_height() - 1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            bboxmin[j] = std::max(0.f, std::min(bboxmin[j], (float)pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], (float)pts[i][j]));
        }
    }

    Vec3f P;

    for (P.x = bboxmin.x; P.x <= bboxmax.x; P.x++)
    {

        for (P.y = bboxmin.y; P.y <= bboxmax.y; P.y++)
        {
            Vec3f bc_screen = barycentric(pts[0], pts[1], pts[2], P);
            if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
                continue;
            P.z = 0;
            Vec2i uvP;
            for (int i = 0; i < 3; i++)
            {
                P.z += pts[i][2] * bc_screen[i];
                uvP[0] += uv[i][0] * bc_screen[i];
                uvP[1] += uv[i][1] * bc_screen[i];
            }

            int idx = int(P.x + P.y * width);

            if (zbuffer[idx] < P.z)
            {
                zbuffer[idx] = P.z;
                TGAColor color = model->diffuse(uvP);
                image.set(P.x, P.y, TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255));
            }
        }
    }
}

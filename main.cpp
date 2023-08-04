#include <vector>
#include <iostream>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "our_gl.h"
#include "our_gl.cpp"

const int width = 800;
const int height = 800;

Model *model = NULL;
Vec3f light_dir(1, 1, 1);
Vec3f eye(1, 1, 3);
Vec3f center(0, 0, 0);
Vec3f up(0, 1, 0);

struct GoraudShader : public IShader
{
    Vec3f varying_intensity;

    virtual ~GoraudShader() {}

    virtual Vec3i vertex(int iface, int nthvert)
    {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));
        gl_Vertex = Projection * ModelView * gl_Vertex;

        varying_intensity[nthvert] = CLAMP(model->normal(iface, nthvert) * light_dir, 0.f, 1.f);
        return proj<3>(gl_Vertex / gl_Vertex[3]);
    }

    virtual bool fragment(Vec3f bar, TGAColor &color)
    {
        float intensity = varying_intensity * bar;
        color = TGAColor(255, 255, 255) * intensity;
        return false;
    }
};

struct ToonShader : public IShader
{
    Vec3f varying_ity;

    virtual ~ToonShader() {}

    virtual Vec3i vertex(int iface, int nthvert)
    {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));
        gl_Vertex = Viewport * Projection * ModelView * gl_Vertex;
        varying_ity[nthvert] = CLAMP(model->normal(iface, nthvert) * light_dir, 0.f, 1.f);

        return proj<3>(gl_Vertex / gl_Vertex[3]);
    }

    virtual bool fragment(Vec3f bar, TGAColor &color)
    {
        float intensity = varying_ity * bar;
        if (intensity > .85)
            intensity = 1;
        else if (intensity > .60)
            intensity = .80;
        else if (intensity > .45)
            intensity = .60;
        else if (intensity > .30)
            intensity = .45;
        else if (intensity > .15)
            intensity = .30;
        color = TGAColor(255, 155, 0) * intensity;
        return false;
    }
};

struct FlatShader : public IShader
{
    mat<3, 3, float> varying_tri;

    virtual ~FlatShader() {}

    virtual Vec3i vertex(int iface, int nthvert)
    {
        Vec4f gl_Vertex = embed<4>(model->vert(iface, nthvert));
        gl_Vertex = Projection * ModelView * gl_Vertex;
        varying_tri.set_col(nthvert, proj<3>(gl_Vertex / gl_Vertex[3]));
        gl_Vertex = Viewport * gl_Vertex;
        return proj<3>(gl_Vertex / gl_Vertex[3]);
    }

    virtual bool fragment(Vec3f bar, TGAColor &color)
    {
        Vec3f n = cross(varying_tri.col(1) - varying_tri.col(0), varying_tri.col(2) - varying_tri.col(0)).normalize();
        float intensity = CLAMP(n * light_dir, 0.f, 1.f);
        color = TGAColor(255, 255, 255) * intensity;
        return false;
    }
};

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

    lookat(eye, center, up);
    viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
    projection(-1. / (eye - center).norm());
    light_dir.normalize();

    TGAImage image(width, height, TGAImage::RGB);
    TGAImage zbuffer(width, height, TGAImage::GRAYSCALE);

    // GoraudShader shader;
    // ToonShader shader;
    FlatShader shader;
    for (int i = 0; i < model->nfaces(); i++)
    {
        Vec3i screen_coords[3];
        for (int j = 0; j < 3; j++)
        {
            screen_coords[j] = shader.vertex(i, j);
        }

        triangle(screen_coords, shader, image, zbuffer);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    zbuffer.flip_vertically();
    zbuffer.write_tga_file("zbuffer.tga");

    delete model;

    return 0;
}

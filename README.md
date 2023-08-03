# TinyRenderer

This is a repository to implement a tiny renderer followed by the [tutorial](https://github.com/ssloy/tinyrenderer/wiki)

### Lesson 3: Hidden faces removal (z buffer)

[tutorial](<https://github.com/ssloy/tinyrenderer/wiki/Lesson-3:-Hidden-faces-removal-(z-buffer)>)

branch: Lesson3

```
mkdir build && cd build && cmake ..
make && ./tinyrenderer && convert output.tga output.png && convert zbuffer.tga zbuffer.png
```

Output:
z-buffer : before VS after

<center class="half">
    <img src="./results/light.png" width="400"/>
    <space>
    <img src="./results/output.png" width="400"/>
</center>
zbuffer-gray
<center class="half">
    <img src="./results/zbuffer.png" width="400"/>
    <space>
</center>

### homework : Texture

<center class="half">
    <img src="./results/texture.png" width="400"/>
    <space>
</center>

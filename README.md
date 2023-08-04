# TinyRenderer

This is a repository to implement a tiny renderer followed by the [tutorial](https://github.com/ssloy/tinyrenderer/wiki)

### Lesson 4: Perspective projection

[tutorial](<https://github.com/ssloy/tinyrenderer/wiki/Lesson-4:-Perspective-projection)

branch: Lesson4

```
mkdir build && cd build && cmake ..
make && ./tinyrenderer && convert output.tga output.png && convert zbuffer.tga zbuffer.png
```

Output:
prospective projection : before VS after

<center class="half">
    <img src="./results/texture.png" width="400"/>
    <space>
    <img src="./results/output.png" width="400"/>
</center>
zbuffer-gray :  before VS after
<center class="half">
    <img src="./results/zbuffer.png" width="400"/>
    <space>
    <img src="./results/zbuffer-pro.png" width="400"/>
</center>

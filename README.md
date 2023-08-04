# TinyRenderer

This is a repository to implement a tiny renderer followed by the [tutorial](https://github.com/ssloy/tinyrenderer/wiki)

### Lesson 5: Moving the camera

[tutorial](<https://github.com/ssloy/tinyrenderer/wiki/Lesson-5:-Moving-the-camera)

branch: Lesson5

```
mkdir build && cd build && cmake ..
make && ./tinyrenderer && convert output.tga output.png && convert zbuffer.tga zbuffer.png
```

Output:
prospective projection : flat & goraund & cartoon

<center class="half">
    <img src="./results/Flat.png" width="400"/>
    <space>
    <img src="./results/output.png" width="400"/>
    <space>
    <img src="./results/Cartoon.png" width="400"/>
</center>
zbuffer-gray 
<center class="half">
    <img src="./results/zbuffer.png" width="400"/>
</center>

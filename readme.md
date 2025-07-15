# Sparrow Browser (Fork of liutgnu's project) #

## Introduction ##

This project, named Sparrow, is a fork of [liutgnu's web browser learning project](https://github.com/liutgnu/simple-browser-rendering-engine). Our goal is to continue development and build a functional browser. It is structured into 5 main parts:

1) HTML parser;
2) CSS parser;
3) Style generator;
4) Layout generator;
5) SDL2 painter (named *drawer* in this project);

The original project mainly took [mbrubeck/robinson](https://github.com/mbrubeck/robinson) as reference.

## Building the Project ##

This project now uses CMake for its build system. Each part can be compiled as a standalone program or as a library.

To build the project:

```bash
mkdir build
cd build
cmake ..
make
```

The dependencies are as follows:

*   **HTML Parser:** Self-contained.
*   **CSS Parser:** Self-contained.
*   **Style:** Depends on HTML and CSS.
*   **Layout:** Depends on Style, HTML, and CSS.
*   **Drawer:** Depends on Layout, Style, HTML, and CSS.

For rendering, the project uses FreeType2 for font rendering and SDL2 for painting. Therefore, SDL2, SDL2_image, and FreeType2 should be installed.

### Recent Improvements: ###

*   **Improved Font Rendering:** Enhanced text rendering to correctly display various characters and handle whitespace more robustly.
*   **Build System Migration:** The project has migrated from Makefiles to CMake, simplifying the build process.

## Running the Browser ##

After building the project, you can run the Sparrow browser with a test HTML and CSS file:

```bash
./build/sdl/sparrow testcase/test.html testcase/test.css
```

## DEMO ##

Let's take testcase/test.html and testcase/test.css as example.

**The output of htmlprog:**

```
$ ./htmlprog.out ../testcase/test.html
└─<ELEMENT> html
  ├─<ELEMENT> head
  │ └─<ELEMENT> title
  │   └─<TEXT> Test
  └─<ELEMENT> div
    ├─<ELEMENT> p
    │ ├─<TEXT> 简单
    │ ├─<ELEMENT> span
    │ │ └─<TEXT> 浏览器
    │ └─<TEXT> 演示

    └─<ELEMENT> p
      └─<TEXT> Hello, World!
```
**The output of css:**
```
$ ./cssprog.out ../testcase/test.css
├─span
│  └─display: inline
├─html
│  ├─width: 600.000000px
│  ├─padding: 10.000000px
│  ├─border-width: 1.000000px
│  ├─border-color: 0 0 0 255
│  ├─margin: auto
│  └─background: 255 255 255 255
├─head
│  └─display: none
├─.outer
│  ├─background: 0 204 255 255
│  ├─border-color: 102 102 102 255
│  ├─border-width: 2.000000px
│  ├─margin: 50.000000px
│  └─padding: 50.000000px
├─.inner
│  ├─border-color: 204 0 0 255
│  ├─border-width: 4.000000px
│  ├─height: 100.000000px
│  ├─margin-bottom: 20.000000px
│  └─width: 500.000000px
├─#bye .inner
│  └─background: 255 255 0 255
└─#name span
   ├─background: 204 0 0 255
   ├─color: 255 255 255 255
   └─font-size: 30.000000px
```
**The output of style:**
```
$ ./styleprog.out ../testcase/test.html ../testcase/test.css
└─<ELEMENT> html
  background: 255 255 255 255
  border-color: 0 0 0 255
  border-width: 1.000000px
  margin: auto
  padding: 10.000000px
  width: 600.000000px
  ├─<ELEMENT> head
  │ display: none
  │ └─<ELEMENT> title
  │   └─<TEXT> Test
  │     height: 21.000000px
  │     width: 30.000000px
  └─<ELEMENT> div
    background: 0 204 255 255
    border-color: 102 102 102 255
    border-width: 2.000000px
    margin: 50.000000px
    padding: 50.000000px
    ├─<ELEMENT> p
    │ border-color: 204 0 0 255
    │ border-width: 4.000000px
    │ height: 100.000000px
    │ margin-bottom: 20.000000px
    │ width: 500.000000px
    │ ├─<TEXT> 简单
    │ │ height: 21.000000px
    │ │ width: 32.000000px
    │ ├─<ELEMENT> span
    │ │ background: 204 0 0 255
    │ │ color: 255 255 255 255
    │ │ display: inline
    │ │ font-size: 30.000000px
    │ │ └─<TEXT> 浏览器
    │ │   color: 255 255 255 255
    │ │   font-size: 30.000000px
    │ │   height: 38.000000px
    │ │   width: 90.000000px
    │ └─<TEXT> 演示

    │   height: 21.000000px
    │   width: 52.000000px
    └─<ELEMENT> p
      background: 255 255 0 255
      border-color: 204 0 0 255
      border-width: 4.000000px
      height: 100.000000px
      margin-bottom: 20.000000px
      width: 500.000000px
      └─<TEXT> Hello, World!

        height: 21.000000px
        width: 111.000000px
```
**The output of layout:**
```
$ ./layoutprog.out ../testcase/test.html ../testcase/test.css
└─<ELEMENT> html
  INLINE
  content-xywh: 100.000000 11.000000 600.000000 460.000000
  margin-lrtb: 89.000000 89.000000 0.000000 0.000000
  border-lrtb: 1.000000 1.000000 1.000000 1.000000
  padding-lrtb: 10.000000 10.000000 10.000000 10.000000
  └─<ELEMENT> div
    BLOCK
    content-xywh: 202.000000 113.000000 396.000000 256.000000
    margin-lrtb: 50.000000 50.000000 50.000000 50.000000
    border-lrtb: 2.000000 2.000000 2.000000 2.000000
    padding-lrtb: 50.000000 50.000000 50.000000 50.000000
    ├─<ELEMENT> p
    │ BLOCK
    │ content-xywh: 206.000000 117.000000 500.000000 100.000000
    │ margin-lrtb: 0.000000 -112.000000 0.000000 20.000000
    │ border-lrtb: 4.000000 4.000000 4.000000 4.000000
    │ padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    │ └─<ELEMENT>
    │   ANONYMOUS
    │   content-xywh: 206.000000 117.000000 500.000000 38.000000
    │   margin-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   border-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   ├─<TEXT> 简单
    │   │ INLINE
    │   │ content-xywh: 206.000000 117.000000 32.000000 21.000000
    │   │ margin-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │ border-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │ padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   ├─<ELEMENT> span
    │   │ INLINE
    │   │ content-xywh: 238.000000 117.000000 90.000000 38.000000
    │   │ margin-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │ border-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │ padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │ └─<TEXT> 浏览器
    │   │   INLINE
    │   │   content-xywh: 238.000000 117.000000 90.000000 38.000000
    │   │   margin-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │   border-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   │   padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    │   └─<TEXT> 演示

    │     INLINE
    │     content-xywh: 328.000000 117.000000 52.000000 21.000000
    │     margin-lrtb: 0.000000 0.000000 0.000000 0.000000
    │     border-lrtb: 0.000000 0.000000 0.000000 0.000000
    │     padding-lrtb: 0.000000 0.000000 0.000000 0.000000
    └─<ELEMENT> p
      BLOCK
      content-xywh: 206.000000 245.000000 500.000000 100.000000
      margin-lrtb: 0.000000 -112.000000 0.000000 20.000000
      border-lrtb: 4.000000 4.000000 4.000000 4.000000
      padding-lrtb: 0.000000 0.000000 0.000000 0.000000
      └─<ELEMENT>
        ANONYMOUS
        content-xywh: 206.000000 245.000000 500.000000 21.000000
        margin-lrtb: 0.000000 0.000000 0.000000 0.000000
        border-lrtb: 0.000000 0.000000 0.000000 0.000000
        padding-lrtb: 0.000000 0.000000 0.000000 0.000000
        └─<TEXT> Hello, World!

          INLINE
          content-xywh: 206.000000 245.000000 111.000000 21.000000
          margin-lrtb: 0.000000 0.000000 0.000000 0.000000
          border-lrtb: 0.000000 0.000000 0.000000 0.000000
          padding-lrtb: 0.000000 0.000000 0.000000 0.000000
```
**The output of drawer:**
![](testcase/demo.png)
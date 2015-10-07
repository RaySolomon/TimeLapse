# TimeLapse

Goal of this project is to create set of tools for capturing series of images 
from camera (V4L or gphoto2), process them and assembly "time-lapse" video from them.

Currently, timelapse_assembly and timelapse_deflicker tool is available.

## Build

This project is written in C++ with usage of QT framework and ImageMagic. 
I invest effort to create tools multiplatform, but I tested it on Ubuntu Linux only.
It uses avconv or ffmpeg to build final videos.

#### Ubuntu

Install build tools and libraries:

```
sudo apt-get install cmake build-essential libmagick++-dev qtbase5-dev libav-tools
```

compile & install:

```
cmake . 
make
sudo make install
sudo ldconfig
```

## Tools
### TimeLapse Assembly tool

Tool for build timelapse video from series of images.

```
Usage: ./src/timelapse_assembly [options] source(s)
Tool for assembly time-lapse video from sequence of images.

Options:
  -h, --help               Displays this help.
  -v, --version            Displays version information.
  -o, --output <output>    Output video file (default is timelapse.mkv).
  --width <width>          Output video width. Default is 1920.
  --height <height>        Output video height. Default is 1080.
  --fps <fps>              Output video fps (frames per second). Default is 25.
  --length <length>        Output video length (in seconds). 
                           If this option is not defined, then length will be
                           computed from number of inputs images / fps.
  -b, --bitrate <bitrate>  Output video bitrate. Default 40000k.
  --codec <codec>          Output video codec. Default libx264.
  --no-strict-interval     Don't map input images to output video frames with
                           strict interval. Input image to output video frame
                           mapping will be computed from image timestamp (EXIF
                           metadata will be used or file modification time).
  --blend-frames           Blend frame transition.
  -V, --verbose            Verbose output.
  -d, --dryrun             Just parse arguments, check inputs and prints
                           informations.
  -f, --force              Overwrite existing output.
  --tmp <tmp>              Temp directory. Default is /tmp.
  -k, --keep-temp          Keep temporary files (Program will cleanup temporary
                           files at the end by default).

Arguments:
  source(s)                Source images (images or directories with images).
```

### TimeLapse Deflickering tool

Tool for "average" luminance of series of images.

```
Usage: ./src/timelapse_deflicker [options] source(s)
Tool for deflicker sequence of images.

Options:
  -h, --help                Displays this help.
  -v, --version             Displays version information.
  -o, --output <directory>  Output directory.
  -d, --dryrun              Just parse arguments, check inputs and prints
                            informations.
  -w, --debug-view          Composite one half of output image from original
                            and second half from updated image.
  -V, --verbose             Verbose output.

Arguments:
  source(s)                 Source images (images or directories with images).
```

## External tools

 - *timelapse-deflicker* - 
    Simple script to deflicker images taken for timelapses 
    https://github.com/cyberang3l/timelapse-deflicker
 - *gphoto2* -
    The gphoto2 commandline tool for accessing and controlling digital cameras. 
    https://github.com/gphoto/gphoto2

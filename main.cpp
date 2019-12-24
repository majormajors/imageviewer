#include <iostream>
#include <cassert>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <Magick++.h>

using namespace Magick;

int main() {
    Image image("pikachu.png");
    size_t width = image.columns();
    size_t height = image.rows();

    Display *dpy = XOpenDisplay(nullptr);
    int screen = DefaultScreen(dpy);

    int black = BlackPixel(dpy, screen);
    int white = WhitePixel(dpy, screen);

    Window root = DefaultRootWindow(dpy);
    Window w = XCreateSimpleWindow(dpy, root, 0, 0, width, height, 0, black, white);

    XSelectInput(dpy, w, StructureNotifyMask);

    XMapWindow(dpy, w);

    GC gc = XCreateGC(dpy, w, 0, nullptr);

    XSetForeground(dpy, gc, white);


    XEvent e;
    for(;;) {
        XNextEvent(dpy, &e);
        if (e.type == MapNotify)
            break;
    }

    Colormap cmap = XDefaultColormap(dpy, 0);

    XImage *ximage;

    char *data = static_cast<char *>(calloc(width * height, 4));

    ximage = XCreateImage(dpy,
            DefaultVisual(dpy, screen),
            DefaultDepth(dpy, screen),
            ZPixmap, 0, data, width, height, 32, 0);

    XColor xcolor;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ColorRGB pixel(image.pixelColor(x, y));
            xcolor.red = QuantumRange * (1 - pixel.red());
            xcolor.green = QuantumRange * (1 - pixel.green());
            xcolor.blue = QuantumRange * (1 - pixel.blue());
            xcolor.flags = DoRed | DoGreen | DoBlue;
            XAllocColor(dpy, cmap, &xcolor);
            XPutPixel(ximage, x, y, xcolor.pixel);
        }
    }

    XPutImage(dpy, w, gc, ximage, 0, 0, 0, 0, width, height);

    XFlush(dpy);

    for(;;) {
        XNextEvent(dpy, &e);
    }

    return 0;
}

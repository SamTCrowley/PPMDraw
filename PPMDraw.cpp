// PPMDraw v2
// COPYRIGHT 2019 SAM CROWLEY
// samtcrowley@gmail.com
// FOR EDUCATIONAL PURPOSES ONLY, NOT LIABLE FOR ANY HARM CAUSED BY CODE
// SERIOUSLY, IF YOU DID ANYTHING WITH THIS CODE THAT COULD HURT SOMEONE
// I DON'T KNOW HOW YOU DID IT. THIS IS CODE FOR DRAWING PICTURES.
// ALSO, DON'T USE THIS FOR COMMERCIAL PURPOSES WITHOUT PERMISSION.
// THERE ARE PROBABLY BETTER TOOLS OUT THERE ANYWAY.


#include "../include/PPMDraw.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <map>
#include <bitset>
#include <vector>

//  standard constructor
// purpose: To create a new PPMDraw instance and image of width w and height h
// Method: sets values for image width and height. Creates an array of pixel data to hold the image.
// also creates the checked vector and initializes the built-in font
// Parameters
// w width of image in pixels
// h height of image in pixels
PPMDraw::PPMDraw(int w, int h){
    width = w;
    height = h;

    // make an array to hold all the pixels, r, g, b for each
    image = new pixel[width * height];

    // a bitset to use for functions that have to check which pixels have been worked on
    checked = std::vector<bool>();
    for(int i = 0; i < width * height; i++){
        checked.push_back(false);
    }
    // initialize the built-in font
    init_alpha();
}
// copy constructor
// purpose: To create a copy of a given PPM instance
// method: copy the width and height of the original image, along with its draw and mask colors.
// then create an array of pixel data to hold the image and copy the pixel data. Also make copies
// of the checked vector and extended font data.
// Parameters:
// ppm reference to a PPMDraw instance to copy
PPMDraw::PPMDraw(const PPMDraw & ppm){
    width = ppm.width;
    height = ppm.height;

    red = ppm.red;
    green = ppm.green;
    blue = ppm.blue;

    mask_r = ppm.mask_r;
    mask_g = ppm.mask_g;
    mask_b = ppm.mask_b;

    ext_font_char_width = ppm.ext_font_char_width;
    ext_font_char_height = ppm.ext_font_char_height;

    // copy pixel data
    image = new pixel[width * height];
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            image[j * width + i] = ppm.image[j * width + i];
        }
    }
    // copy checked pixel data
    checked = std::vector<bool>();
    for(int i = 0; i < width * height; i++){
        checked.push_back(ppm.checked.at(i));
    }
    //copy extended font data
    ext_font = ppm.ext_font;

    // initialize the built-in font
    init_alpha();
}

// a second copy constructor
// purpose: copies a part of PPMDraw instance p to a new image. This portion of p
// starts at x, y and is w pixels wide and h pixels high.
// Method: sets the width and height. Creates a new array of pixel data.
// creates a new Checked vector rather than copying it. Initializes the built-in
// font but does not copy the extended font. Checks that the copied portion will
// actually fit into the image, then copies the image pixel data.
// Parameters:
// p a pointer to a PPMDraw instance to be copied from
// x,y position of upper-left corner of portion to copy
// w width of portion to copy
// h height of portion to copy
PPMDraw::PPMDraw(PPMDraw * p, int x, int y, int w, int h){
    width = w;
    height = h;

    // create image pixel array
    image = new pixel[width * height];

    //create checked vector
    checked = std::vector<bool>();
    for(int i = 0; i < width * height; i++){
        checked.push_back(false);
    }

    // initialize built-in font
    init_alpha();

    // checks that the region fits in the image
    if(x < 0){x = 0;}
    if(x >= width){x = width - 1;}
    if(y < 0){y = 0;}
    if(y >= height){y = height - 1;}
    if((x + w) >= width){w = width - x - 1;}
    if((y + h) >= height){h = height - y - 1;}


    // copy the data from image p
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            image[j * width + i] = p->image[(j + y) * p->width + (i + x)];
        }
    }
}

// pixel data constructor
// purpose: To create a PPMDraw instance from an array of pixel data created elsewhere.
// Must give width and height for the image.
// method: Set the width and height, create the image pixel data, create the checked vector,
// and initialize the built-in font. Then copy over the pixel data from the original array.
// Parameters:
// p a pointer to an array of pixels
// w width of image in pixels
// h height of image in pixels
PPMDraw::PPMDraw(pixel * p, int w, int h){
    width = w;
    height = h;
    image = new pixel[width * height];
    checked = std::vector<bool>();
    for(int i = 0; i < width * height; i++){
        checked.push_back(false);
    }
    init_alpha();

    // copy the data from image p
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            image[j * width + i] = p[j * w + i];
        }
    }
}

// destructor
// purpose: To destroy the PPMDraw instance
// Method: deletes the image data, which should be the only data on the heap.
PPMDraw::~PPMDraw(){
    if(image != nullptr){
        delete[] image;
    }
}

// assignment operator
// purpose: to set the values of one PPMDraw object to another.
// method: copy the width and height and color data. Copy the checked vector
// and extended font data. Erase the previous image pixel data and copy over
// the new data.
// Parameters:
// ppm a reference to the PPMDraw instance to copy
PPMDraw & PPMDraw::operator=(const PPMDraw & ppm){
    width = ppm.width;
    height = ppm.height;

    red = ppm.red;
    green = ppm.green;
    blue = ppm.blue;

    mask_r = ppm.mask_r;
    mask_g = ppm.mask_g;
    mask_b = ppm.mask_b;

    checked.clear();
    for(int i = 0; i < width * height; i++){
        checked.push_back(ppm.checked[i]);
    }

    ext_font_char_width = ppm.ext_font_char_width;
    ext_font_char_height = ppm.ext_font_char_height;
    ext_font = ppm.ext_font;

    delete image;
    image = new pixel[width * height];
    for(int j = 0; j < height; j++){
        for(int i = 0; i < width; i++){
            image[j * width + i] = ppm.image[j * width + i];
        }
    }
    return *this;
}

// fill
// Purpose: to fill the image with a single color
// Method: iterates through all of the pixels and sets their color to r, g, b.
// Parameters
// r the red value
// g the green value
// b the blue value
void PPMDraw::fill(unsigned char r, unsigned char g, unsigned char b){
    for(int i = 0; i < width * height; i++){
        image[i + 0] = pixel{r, g, b};
    }
}

// set_color
// Purpose: to set the color used for the draw functions
// Method: set the red, green, and blue values to r, g, b.
// Parameters
// r the red value
// g the green value
// b the blue value
void PPMDraw::set_color(unsigned char r, unsigned char g, unsigned char b){
    red = r;
    green = g;
    blue = b;
}

// set_mask_color
// Purpose: In this class masked draw functions only draw on top of the mask color,
// they will not draw on top of other colored pixels. This function sets the mask
// color for this purpose
// Method: set the mask_r, mask_g, and mask_b values to r, g, b
// parameters:
// r the red value
// g the green value
// b the blue value
void PPMDraw::set_mask_color(unsigned char r, unsigned char g, unsigned char b){
    mask_r = r;
    mask_g = g;
    mask_b = b;
}

// draw_point
// Purpose: To draw a single pixel at x, y
// Method: Checks if the point is within the image. By putting this here we don't
// need to check in the other drawing functions. sets the pixel at x, y to the draw color
// Parameters:
// x the x position
// y the y position
void PPMDraw::draw_point(int x, int y){
    if(x >= 0 && x < width && y >= 0 && y < height){
        image[y * width + x] = pixel{red, green, blue};
    }
}

// draw_point_masked
// Purpose: To draw a single pixel at x, y, IF that pixel matches the mask color.
// Method: Checks if the point is within the image. If so, checks if the existing
// pixel at that point matches the mask color. If so, draws the new pixel.
// Parameters:
// x the x position
// y the y position
void PPMDraw::draw_point_masked(int x, int y){
    if(x >= 0 && x < width && y >= 0 && y < height){
        pixel prev = image[y * width + x];
        if(prev.r == mask_r && prev.g == mask_g && prev.b == mask_b){
            image[y * width + x] = pixel{red, green, blue};
        }
    }
}


// draw_line
// Purpose: draws a line from x1, y1 to x2, y2
// Method: Implements the Bressenham line drawing algorithm. If mask is true,
// then it will only draw on top of pixels that match the mask color.
// parameters:
// x1 the x position of the first point
// y1 the y position of the first point
// x2 the x position of the second point
// y2 the y position of the second point
// mask whether to use masking, defaults to false
void PPMDraw::draw_line(int x1, int y1, int x2, int y2, bool mask){

    if(x1 == x2){
        if(y1 > y2){
            swap(y1, y2);
        }
        for(int i = y1; i <= y2; i++){
            if(mask){
                draw_point_masked(x1, i);
            }else{
                draw_point(x1, i);
            }
        }
    }else if(y1 == y2){
        if(x1 > x2){
            swap(x1, x2);
        }
        for(int i = x1; i <= x2; i++){
            if(mask){
                draw_point_masked(i, y1);
            }else{
                draw_point(i, y1);
            }
        }
    }else{
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = -1;
        int sy = -1;

        if(x1 < x2){
           sx = 1;
        }
        if(y1 < y2){
           sy = 1;
        }
        int err = dx-dy;
        int err2 = 0;

        do{
            if(mask){
                draw_point_masked(x1, y1);
            }else{
                draw_point(x1, y1);
            }
            err2 = 2*err;
            if(err2 > -dy){
                err = err - dy;
                x1 = x1 + sx;
            }
            if(err2 <  dx){
                err = err + dx;
                y1 = y1 + sy;
            }
        }while(x1 != x2 || y1 != y2);
        if(mask){
            draw_point_masked(x2, y2);
        }else{
            draw_point(x2, y2);
        }
    }
}

// draw_dash_line
// Purpose: to draw a dashed line from point x1, y1, to x2, y2.
// Method: implements Bressenham line drawing algorithm with a modification to allow dashes.
// A bool called draw is used to determine whether or not to draw pixels, and a counter d is
// used to keep track of how long each dash should be. Dashes have equal on and off lengths.
// Parameters:
// x1 the x position of the first point
// y1 the y position of the second point
// x2 the x position of the second point
// y2 the y position of the second point
// dash the length of the dashes
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_dash_line(int x1, int y1, int x2, int y2, int dash, bool mask){

    if(x1 == x2){
        if(y1 > y2){
            swap(y1, y2);
        }
        bool draw = true;
        int d = 0;
        for(int i = y1; i <= y2; i++){
            if(draw){
                if(mask){
                    draw_point_masked(x1, i);
                }else{
                    draw_point(x1, i);
                }
            }
            if(d < dash){
                d++;
            }else{
                d = 0;
                draw = !draw;
            }
        }
    }else if(y1 == y2){
        if(x1 > x2){
            swap(x1, x2);
        }
        bool draw = true;
        int d = 0;
        for(int i = x1; i <= x2; i++){
            if(draw){
                if(mask){
                    draw_point_masked(i, y1);
                }else{
                    draw_point(i, y1);
                }
            }
            if(d < dash){
                d++;
            }else{
                d = 0;
                draw = !draw;
            }
        }
    }else{
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = -1;
        int sy = -1;

        if(x1 < x2){
           sx = 1;
        }
        if(y1 < y2){
           sy = 1;
        }
        int err = dx-dy;
        int err2 = 0;

        bool draw = true;
        int d = 0;
        do{
            if(draw){
                if(mask){
                    draw_point_masked(x1, y1);
                }else{
                    draw_point(x1, y1);
                }
            }
            if(d < dash){
                d++;
            }else{
                d = 0;
                draw = !draw;
            }
            err2 = 2*err;
            if(err2 > -dy){
                err = err - dy;
                x1 = x1 + sx;
            }
            if(err2 <  dx){
                err = err + dx;
                y1 = y1 + sy;
            }
        }while(x1 != x2 || y1 != y2);
        if(mask){
            draw_point_masked(x2, y2);
        }else{
            draw_point(x2, y2);
        }
    }
}


// draw_rect
// Purpose: To draw a rectangle.
// Method: Draws 4 lines around the outside of the rectangle
// Parameters:
// x1 the x position of the upper-left corner
// y1 the y position of the upper-left corner
// x2 the x position of the lower-right corner
// y2 the y position of the lower-right corner
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_rect(int x1, int y1, int x2, int y2, bool mask){
    draw_line(x1, y1, x1, y2, mask);
    draw_line(x1, y1, x2, y1, mask);
    draw_line(x2, y1, x2, y2, mask);
    draw_line(x1, y2, x2, y2, mask);
}

// draw_dash_rect
// Purpose: To draw a rectangle using dashed lines.
// Method: Draws 4 dashed lines around the outside of the rectangle
// Parameters:
// x1 the x position of the upper-left corner
// y1 the y position of the upper-left corner
// x2 the x position of the lower-right corner
// y2 the y position of the lower-right corner
// dash length of dashes
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_dash_rect(int x1, int y1, int x2, int y2, int dash, bool mask){
    draw_dash_line(x1, y1, x1, y2, dash, mask);
    draw_dash_line(x1, y1, x2, y1, dash, mask);
    draw_dash_line(x2, y1, x2, y2, dash, mask);
    draw_dash_line(x1, y2, x2, y2, dash, mask);
}
// draw_fill_rect
// Purpose: To draw a filled rectangle.
// Method: Iterates down from y1 to y2 and draws a series of lines from x1 to x2, filling
// in the rectangle.
// Parameters:
// x1 the x position of the upper-left corner
// y1 the y position of the upper-left corner
// x2 the x position of the lower-right corner
// y2 the y position of the lower-right corner
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_fill_rect(int x1, int y1, int x2, int y2, bool mask){
    if(y1 > y2){
        swap(y1, y2);
    }
    for(int i = y1; i <= y2; i++){
        draw_line(x1, i, x2, i, mask);
    }
}

// draw_circle
// Purpose: to draw a circle centered at xC, yC with radius rad.
// Method: implements the Bressenham circle drawing algorithm in 4 quadrants.
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad the radius of the circle
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_circle(int xC, int yC, int rad, bool mask){
    int x = -rad;
    int y = 0;
    int err = 2-2*rad;
    do{
        if(mask){
            draw_point_masked(xC - x, yC + y);
            draw_point_masked(xC - y, yC - x);
            draw_point_masked(xC + x, yC - y);
            draw_point_masked(xC + y, yC + x);
        }else{
            draw_point(xC - x, yC + y);
            draw_point(xC - y, yC - x);
            draw_point(xC + x, yC - y);
            draw_point(xC + y, yC + x);
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}
    }while(x < 0);
}

// draw_fill_circle
// Purpose: to draw a filled circle centered at xC, yC with radius rad.
// Method: implements the Bressenham circle drawing algorithm in 4 quadrants,
// but draws a line across the circle from one quadrant to the next.
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad the radius of the circle
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_fill_circle(int xC, int yC, int rad, bool mask){
    int x = -rad;
    int y = 0;
    int yTrack = 0;
    int err = 2-2*rad;
    draw_line(xC + x, yC, xC - x, yC);
    do{
        if(y != yTrack){
            draw_line(xC + x, yC - y, xC - x, yC - y, mask);
            draw_line(xC + x, yC + y, xC - x, yC + y, mask);
            yTrack = y;
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}

    }while(x < 0);
}

// draw_dash_circle
// Purpose: to draw a dashed circle centered at xC, yC with radius rad.
// Method: implements the Bressenham circle drawing algorithm in 4 quadrants,
// but keeps track of whether or not to draw dashes.
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad the radius of the circle
// dash the length of the dashes
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_dash_circle(int xC, int yC, int rad, int dash, bool mask){
    int x = -rad;
    int y = 0;
    int err = 2-2*rad;
    int d = 0;
    bool draw = true;
    do{
        if(draw){
            if(mask){
                draw_point_masked(xC - x, yC + y);
                draw_point_masked(xC - y, yC - x);
                draw_point_masked(xC + x, yC - y);
                draw_point_masked(xC + y, yC + x);
            }else{
                draw_point(xC - x, yC + y);
                draw_point(xC - y, yC - x);
                draw_point(xC + x, yC - y);
                draw_point(xC + y, yC + x);
            }
        }
        if(d < dash){
            d++;
        }else{
            d = 0;
            draw = !draw;
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}
    }while(x < 0);
}


// draw_circle_sector
// purpose: to draw a section of a circle centered at xC, yC with radius rad. The portion
// of the circle will be drawn from angle a1 to angle a2.
// Method: Checks the values of a1 and a2 and calls the private circle_sector function
// with the appropriate parameters, because that function can only handle up 90 degrees
// of a circle, so it has to be broken up.
// Parameters:
// xC x position of center
// yC y position of center
// rad radius of circle
// a1 angle of starting point
// a2 angle of ending point
// mask whether or not use masking, defaults to false
void PPMDraw::draw_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask){

    if(between(a1, 0.0f, 180.0f) && between(a2, 180.0f, 360.0f)){
        circle_sector(xC, yC, rad, a1, 180, mask);
        circle_sector(xC, yC, rad, 180, a2, mask);
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 0.0f, 180.0f)){
        circle_sector(xC, yC, rad, a1, 360, mask);
        circle_sector(xC, yC, rad, 0, a2, mask);
    }else if(between(a1, 0.0f, 180.0f) && between(a2, 0.0f, 180.0f)){
        if(a1 < a2){
            circle_sector(xC, yC, rad, a1, a2, mask);
        }else if(a2 < a1){
            circle_sector(xC, yC, rad, a1, 180, mask);
            circle_sector(xC, yC, rad, 180, 360, mask);
            circle_sector(xC, yC, rad, 0, a2, mask);
        }
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 180.0f, 360.0f)){
        if(a1 < a2){
            circle_sector(xC, yC, rad, a1, a2, mask);
        }else if(a2 < a1){
            circle_sector(xC, yC, rad, a1, 360, mask);
            circle_sector(xC, yC, rad, 0, 180, mask);
            circle_sector(xC, yC, rad, 180, a2, mask);
        }
    }
}

// draw_dash_circle_sector
// purpose: to draw a dashed section of a circle centered at xC, yC with radius rad. The portion
// of the circle will be drawn from angle a1 to angle a2.
// Method: Checks the values of a1 and a2 and calls the private dash_circle_sector function
// with the appropriate parameters, because that function can only handle up 90 degrees
// of a circle, so it has to be broken up.
// Parameters:
// xC x position of center
// yC y position of center
// rad radius of circle
// a1 angle of starting point
// a2 angle of ending point
// dash length of dashes
// mask whether or not use masking, defaults to false
void PPMDraw::draw_dash_circle_sector(int xC, int yC, int rad, float a1, float a2, int dash, bool mask){

    if(between(a1, 0.0f, 180.0f) && between(a2, 180.0f, 360.0f)){
        dash_circle_sector(xC, yC, rad, a1, 180, dash, mask);
        dash_circle_sector(xC, yC, rad, 180, a2, dash, mask);
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 0.0f, 180.0f)){
        dash_circle_sector(xC, yC, rad, a1, 360, dash, mask);
        dash_circle_sector(xC, yC, rad, 0, a2, dash, mask);
    }else if(between(a1, 0.0f, 180.0f) && between(a2, 0.0f, 180.0f)){
        if(a1 < a2){
            dash_circle_sector(xC, yC, rad, a1, a2, dash, mask);
        }else if(a2 < a1){
            dash_circle_sector(xC, yC, rad, a1, 180, dash, mask);
            dash_circle_sector(xC, yC, rad, 180, 360, dash, mask);
            dash_circle_sector(xC, yC, rad, 0, a2, dash, mask);
        }
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 180.0f, 360.0f)){
        if(a1 < a2){
            dash_circle_sector(xC, yC, rad, a1, a2, dash, mask);
        }else if(a2 < a1){
            dash_circle_sector(xC, yC, rad, a1, 360, dash, mask);
            dash_circle_sector(xC, yC, rad, 0, 180, dash, mask);
            dash_circle_sector(xC, yC, rad, 180, a2, dash, mask);
        }
    }
}


// draw_fill_circle_sector
// purpose: to draw a filled section of a circle centered at xC, yC with radius rad. The portion
// of the circle will be drawn from angle a1 to angle a2.
// Method: Checks the values of a1 and a2 and calls the private circle_sector function
// with the appropriate parameters, because that function can only handle up 90 degrees
// of a circle, so it has to be broken up.
// Parameters:
// xC x position of center
// yC y position of center
// rad radius of circle
// a1 angle of starting point
// a2 angle of ending point
// mask whether or not use masking, defaults to false
void PPMDraw::draw_fill_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask){
    if(between(a1, 0.0f, 180.0f) && between(a2, 180.0f, 360.0f)){
        if(a1 < a2){
            fill_circle_sector(xC, yC, rad, a1, 180, mask);
            fill_circle_sector(xC, yC, rad, 180, a2, mask);
        }else if(a2 < a1){
            fill_circle_sector(xC, yC, rad, a1, 180, mask);
            fill_circle_sector(xC, yC, rad, 180, 360, mask);
            fill_circle_sector(xC, yC, rad, 0, a2, mask);
        }
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 0.0f, 180.0f)){
        if(a1 < a2){
            fill_circle_sector(xC, yC, rad, a1, 360, mask);
            fill_circle_sector(xC, yC, rad, 0, a2, mask);
        }else if(a2 < a1){
            fill_circle_sector(xC, yC, rad, a1, 360, mask);
            fill_circle_sector(xC, yC, rad, 0, a2, mask);
            //fill_circle_sector(xC, yC, rad, 180, a2);
        }
    }else if(between(a1, 0.0f, 180.0f) && between(a2, 0.0f, 180.0f)){
        if(a1 < a2){
            fill_circle_sector(xC, yC, rad, a1, a2, mask);
        }else if(a2 < a1){
            fill_circle_sector(xC, yC, rad, a1, 180, mask);
            fill_circle_sector(xC, yC, rad, 180, 360, mask);
            fill_circle_sector(xC, yC, rad, 0, a2, mask);
        }
    }else if(between(a1, 180.0f, 360.0f) && between(a2, 180.0f, 360.0f)){
        if(a1 < a2){
            fill_circle_sector(xC, yC, rad, a1, a2, mask);
        }else if(a2 < a1){
            fill_circle_sector(xC, yC, rad, a1, 360, mask);
            fill_circle_sector(xC, yC, rad, 0, 180, mask);
            fill_circle_sector(xC, yC, rad, 180, a2, mask);
        }
    }

}

// circle_sector
// Purpose: private function that actually handles the drawing of the circle sectors.
// Method: Implements the Bressenham circle algorithm, but checks if the point is between a1 and a2 before drawing.
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad radius of the circle
// a1 start angle
// a2 stop angle
// mask whether or not to use masking, defaults to false
void PPMDraw::circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask){
    if(a2 < a1){ swap(a1, a2);}

    // do the bressenham circle algorithm but check the angle of the point
    // to make sure it fits between the two angles
    int x = -rad;
    int y = 0;
    int err = 2-2*rad;
    // catch the points on ends of circle
    if(between(atan2((double)(0), (double)( x)) * 180./PI, (double)a1, (double)a2)){
        if(mask){
            draw_point_masked(xC + x, yC);
        }else{
            draw_point(xC + x, yC);
        }
    }
    if(between(atan2((double)(0), (double)(-x)) * 180./PI, (double)a1, (double)a2)){
        if(mask){
            draw_point_masked(xC - x, yC);
        }else{
            draw_point(xC - x, yC);
        }
    }
    do{
        if(between(atan2((double)( y), (double)(-x)) * 180./PI, (double)(a1), (double)(a2))){
            if(mask){
                draw_point_masked(xC - x, yC + y);
            }else{
                draw_point(xC - x, yC + y);
            }
        }
        if(between(atan2((double)(-x), (double)(-y)) * 180./PI, (double)(a1), (double)(a2))){
            if(mask){
                draw_point_masked(xC - y, yC - x);
            }else{
                draw_point(xC - y, yC - x);
            }
        }
        if(between(360 + atan2((double)(-y), (double)( x)) * 180./PI, (double)(a1), (double)(a2))){
            if(mask){
                draw_point_masked(xC + x, yC - y);
            }else{
                draw_point(xC + x, yC - y);
            }
        }
        if(between(360 + atan2((double)( x), (double)( y)) * 180./PI, (double)(a1), (double)(a2))){
            if(mask){
                draw_point_masked(xC + y, yC + x);
            }else{
                draw_point(xC + y, yC + x);
            }
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}
    }while(x < 0);


}


// dash_circle_sector
// Purpose: private function that actually handles the drawing of the dashed circle sectors.
// Method: Implements the Bressenham circle algorithm, but checks if the point is between a1 and a2 before drawing.
// Keeps track of whether or not to draw each dash
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad radius of the circle
// a1 start angle
// a2 stop angle
// dash length of dashes
// mask whether or not to use masking, defaults to false
void PPMDraw::dash_circle_sector(int xC, int yC, int rad, float a1, float a2, int dash, bool mask){
    if(a2 < a1){ swap(a1, a2);}




    // do the bressenham circle algorithm but check the angle of the point
    // to make sure it fits between the two angles
    int x = -rad;
    int y = 0;
    int err = 2-2*rad;

    bool draw = true;
    int d = 0;

    // catch the points on ends of circle
    if(between(atan2((double)(0), (double)( x)) * 180./PI, (double)a1, (double)a2)){
        if(draw){
            if(mask){
                draw_point_masked(xC + x, yC);
            }else{
                draw_point(xC + x, yC);
            }
        }
    }
    if(between(atan2((double)(0), (double)(-x)) * 180./PI, (double)a1, (double)a2)){
        if(draw){
            if(mask){
                draw_point_masked(xC - x, yC);
            }else{
                draw_point(xC - x, yC);
            }
        }
    }
    do{
        if(between(atan2((double)( y), (double)(-x)) * 180./PI, (double)(a1), (double)(a2))){
            if(draw){
                if(mask){
                    draw_point_masked(xC - x, yC + y);
                }else{
                    draw_point(xC - x, yC + y);
                }
            }
        }
        if(between(atan2((double)(-x), (double)(-y)) * 180./PI, (double)(a1), (double)(a2))){
            if(draw){
                if(mask){
                    draw_point_masked(xC - y, yC - x);
                }else{
                    draw_point(xC - y, yC - x);
                }
            }
        }
        if(between(360 + atan2((double)(-y), (double)( x)) * 180./PI, (double)(a1), (double)(a2))){
            if(draw){
                if(mask){
                    draw_point_masked(xC + x, yC - y);
                }else{
                    draw_point(xC + x, yC - y);
                }
            }
        }
        if(between(360 + atan2((double)( x), (double)( y)) * 180./PI, (double)(a1), (double)(a2))){
            if(draw){
                if(mask){
                    draw_point_masked(xC + y, yC + x);
                }else{
                    draw_point(xC + y, yC + x);
                }
            }
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}

        if(d < dash){
            d++;
        }else{
            draw = !draw;
            d = 0;
        }
    }while(x < 0);


}

// fill_circle_sector
// Purpose: private function that actually handles the drawing of the filled circle sectors.
// Method: Implements the Bressenham circle algorithm, but checks if the point is between a1 and a2 before drawing.
// Parameters:
// xC the x position of the center point
// yC the y position of the center point
// rad radius of the circle
// a1 start angle
// a2 stop angle
// mask whether or not to use masking, defaults to false
void PPMDraw::fill_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask){

    if(a2 < a1){ swap(a1, a2);}

    int x = -rad;
    int y = 0;
    int yTrack = 0;
    int err = 2-2*rad;
    for(int i = xC + x; i < xC - x; i++){
        if(between(atan2((double)(0), (double)(i-xC)) * 180./PI, (double)a1, (double)a2)){
            if(mask){
                draw_point_masked(i, yC);
            }else{
                draw_point(i, yC);
            }
        }
    }
    do{
        if(y != yTrack){
            for(int i = xC + x; i < xC - x; i++){
                if(a1 <= 180 && a2 <= 180){
                    if(between(atan2((double)(y), (double)(i-xC)) * 180./PI, (double)a1, (double)a2)){
                        if(mask){
                            draw_point_masked(i, yC + y);
                        }else{
                            draw_point(i, yC + y);
                        }
                    }
                }
                if(a1 >= 180 && a2 >= 180){
                    if(between(360 + atan2((double)(-y), (double)(i-xC)) * 180./PI, (double)(a1), (double)(a2))){
                        if(mask){
                            draw_point_masked(i, yC - y);
                        }else{
                            draw_point(i, yC - y);
                        }
                    }
                }
            }
            yTrack = y;
        }
        rad = err;
        if(rad <= y){ err += ++y*2+1;}
        if(rad > x || err > y){ err += ++x*2+1;}

    }while(x < 0);

}

// draw_ellipse
// Purpose: to draw an ellipse inside the rectangle from x1, y1, to x2, y2.
// Method: Implements Bressenhams ellipse drawing algorithm.
// Parameters:
// x1 x position of the upper-left corner of the bounding rectangle
// y1 y position of the upper-left corner of the bounding rectangle
// x2 x position of the lower-right corner of the bounding rectangle
// y2 y position of the lower-right corner of the bounding rectangle
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_ellipse(int x1, int y1, int x2, int y2, bool mask){
    // values of the diameter
    int a = abs(x2-x1);
    int b = abs(y2-y1);
    int b1 = b&1;
    // error increments
    long dx = 4*(1-a)*b*b;
    long dy = 4*(b1+1)*a*a;
    // error of 1 step
    long err = dx+dy+b1*a*a;
    long e2;

    // swap points
    if(x1 > x2){
        x1 = x2;
        x2 += a;
    }
    if(y1 > y2){
        y1 = y2;
    }
    // starting pixel
    y1 += (b+1)/2;
    y2 = y1-b1;
    a *= 8*a;
    b1 = 8*b*b;

   do {
       if(mask){
            draw_point_masked(x2, y1); /*   I. Quadrant */
            draw_point_masked(x1, y1); /*  II. Quadrant */
            draw_point_masked(x1, y2); /* III. Quadrant */
            draw_point_masked(x2, y2); /*  IV. Quadrant */
       }else{
           draw_point(x2, y1); /*   I. Quadrant */
           draw_point(x1, y1); /*  II. Quadrant */
           draw_point(x1, y2); /* III. Quadrant */
           draw_point(x2, y2); /*  IV. Quadrant */
       }
       e2 = 2*err;
       // y step
       if (e2 <= dy) {
            y1++; y2--;
            err += dy += a;
        }
        // x step
        if (e2 >= dx || 2*err > dy){
            x1++; x2--;
            err += dx += b1;
        }
   } while (x1 <= x2);

   while (y1-y2 < b) {  /* too early stop of flat ellipses a=1 */
       if(mask){
            draw_point_masked(x1-1, y1); /* -> finish tip of ellipse */
            draw_point_masked(x2+1, y1++);
            draw_point_masked(x1-1, y2);
            draw_point_masked(x2+1, y2--);
       }else{
            draw_point(x1-1, y1); /* -> finish tip of ellipse */
            draw_point(x2+1, y1++);
            draw_point(x1-1, y2);
            draw_point(x2+1, y2--);
       }
   }
}

// draw_fill_ellipse
// Purpose: to draw a filled ellipse inside the rectangle from x1, y1, to x2, y2.
// Method: Implements Bressenhams ellipse drawing algorithm. Draws a line across
// from one quadrant to the next to fill in the ellipse.
// Parameters:
// x1 x position of the upper-left corner of the bounding rectangle
// y1 y position of the upper-left corner of the bounding rectangle
// x2 x position of the lower-right corner of the bounding rectangle
// y2 y position of the lower-right corner of the bounding rectangle
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_fill_ellipse(int x1, int y1, int x2, int y2, bool mask){
    // to simplify things, check if it's a line
    if(x1 == x2 || y1 == y2){
        draw_line(x1, y1, x2, y2, mask);
    }else{

        // values of the diameter
        int a = abs(x2-x1);
        int b = abs(y2-y1);
        int b1 = b&1;
        // error increments
        long dx = 4*(1-a)*b*b;
        long dy = 4*(b1+1)*a*a;
        // error of 1 step
        long err = dx+dy+b1*a*a;
        long e2;

        // swap points
        if(x1 > x2){
            x1 = x2;
            x2 += a;
        }
        if(y1 > y2){
            y1 = y2;
        }
        // starting pixel
        y1 += (b+1)/2;
        y2 = y1-b1;
        a *= 8*a;
        b1 = 8*b*b;

       do {
            if(mask){
                draw_point_masked(x2, y1);   //I. Quadrant
                draw_point_masked(x1, y1);   //II. Quadrant
                draw_point_masked(x1, y2);   //III. Quadrant
                draw_point_masked(x2, y2);   //IV. Quadrant
            }else{
                draw_point(x2, y1);   //I. Quadrant
                draw_point(x1, y1);   //II. Quadrant
                draw_point(x1, y2);   //III. Quadrant
                draw_point(x2, y2);   //IV. Quadrant
            }

           draw_line(x1, y1, x2, y1, mask); // quadrant 1 to 4
           draw_line(x1, y2, x2, y2, mask); // quadrant 2 to 3
           e2 = 2*err;
           // y step
           if (e2 <= dy) {
                y1++; y2--;
                err += dy += a;
            }
            // x step
            if (e2 >= dx || 2*err > dy){
                x1++; x2--;
                err += dx += b1;
            }
       } while (x1 <= x2);

    }
}

// draw_dash_ellipse
// Purpose: to draw a dashed ellipse inside the rectangle from x1, y1, to x2, y2.
// Method: Implements Bressenhams ellipse drawing algorithm. Keeps track of whether
// or not to draw each dash.
// Parameters:
// x1 x position of the upper-left corner of the bounding rectangle
// y1 y position of the upper-left corner of the bounding rectangle
// x2 x position of the lower-right corner of the bounding rectangle
// y2 y position of the lower-right corner of the bounding rectangle
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_dash_ellipse(int x1, int y1, int x2, int y2, int dash, bool mask){
    // values of the diameter
    int a = abs(x2-x1);
    int b = abs(y2-y1);
    int b1 = b&1;
    // error increments
    long dx = 4*(1-a)*b*b;
    long dy = 4*(b1+1)*a*a;
    // error of 1 step
    long err = dx+dy+b1*a*a;
    long e2;

    int d = 0;
    bool draw = true;

    // swap points
    if(x1 > x2){
        x1 = x2;
        x2 += a;
    }
    if(y1 > y2){
        y1 = y2;
    }
    // starting pixel
    y1 += (b+1)/2;
    y2 = y1-b1;
    a *= 8*a;
    b1 = 8*b*b;

   do {
       if(draw){
           if(mask){
                draw_point_masked(x2, y1); /*   I. Quadrant */
                draw_point_masked(x1, y1); /*  II. Quadrant */
                draw_point_masked(x1, y2); /* III. Quadrant */
                draw_point_masked(x2, y2); /*  IV. Quadrant */
           }else{
                draw_point(x2, y1); /*   I. Quadrant */
                draw_point(x1, y1); /*  II. Quadrant */
                draw_point(x1, y2); /* III. Quadrant */
                draw_point(x2, y2); /*  IV. Quadrant */
           }
       }
        if(d < dash){
            d++;
        }else{
            d = 0;
            draw = !draw;
        }
       e2 = 2*err;
       // y step
       if (e2 <= dy) {
            y1++; y2--;
            err += dy += a;
        }
        // x step
        if (e2 >= dx || 2*err > dy){
            x1++; x2--;
            err += dx += b1;
        }
   } while (x1 <= x2);

   while (y1-y2 < b) {  /* too early stop of flat ellipses a=1 */
       if(draw){
           if(mask){
                draw_point_masked(x1-1, y1); /* -> finish tip of ellipse */
                draw_point_masked(x2+1, y1++);
                draw_point_masked(x1-1, y2);
                draw_point_masked(x2+1, y2--);
           }else{
               draw_point(x1-1, y1); /* -> finish tip of ellipse */
               draw_point(x2+1, y1++);
               draw_point(x1-1, y2);
               draw_point(x2+1, y2--);
           }
       }
       if(d < dash){
            d++;
        }else{
            d = 0;
            draw = !draw;
        }
   }
}

// draw_triangle
// Purpose: to draw a triangle from x1, y1, x2, y2, and x3, y3.
// Method: draws three lines to connect the points.
// Parameters:
// x1 x position of point 1
// y1 y position of point 1
// x2 x position of point 2
// y2 y position of point 2
// x3 x position of point 3
// y3 y position of point 3
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool mask){
    draw_line(x1, y1, x2, y2, mask);
    draw_line(x2, y2, x3, y3, mask);
    draw_line(x3, y3, x1, y1, mask);
}

// draw_dash_triangle
// Purpose: to draw a triangle from x1, y1, x2, y2, and x3, y3.
// Method: draws three dashed lines to connect the points.
// Parameters:
// x1 x position of point 1
// y1 y position of point 1
// x2 x position of point 2
// y2 y position of point 2
// x3 x position of point 3
// y3 y position of point 3
// dash length of dashes
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_dash_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int dash, bool mask){
    draw_dash_line(x1, y1, x2, y2, dash, mask);
    draw_dash_line(x2, y2, x3, y3, dash, mask);
    draw_dash_line(x3, y3, x1, y1, dash, mask);
}

// draw_fill_triangle
// Purpose: to draw a filled triangle from x1, y1, x2, y2, and x3, y3.
// Method: Orders the points from top to bottom and splits the triangle into two
// triangles. Iterates through each triangle's y values and draws lines across
// from one side to the other.
// Parameters:
// x1 x position of point 1
// y1 y position of point 1
// x2 x position of point 2
// y2 y position of point 2
// x3 x position of point 3
// y3 y position of point 3
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool mask){
        // order the points from top to bottom
        if(y1 > y2){
            swap(y1, y2);
            swap(x1, x2);
        }
        if(y1 > y3){
            swap(y1, y3);
            swap(x1, x3);
        }
        if(y2 > y3){
            swap(y2, y3);
            swap(x2, x3);
        }

        int xS = x1;
        int xE = x1;
        //draw the bottom triangle
        for(int y = y1; y < y2; y++){
            if(y2 == y1){
                xS = x1;
            }else{
                xS = ((x2 - x1)*(y - y1))/(y2 - y1) + x1;
            }
            if(y3 == y1){
                xS = x3;
            }else{
                xE = ((x3 - x1)*(y - y1))/(y3 - y1) + x1;
            }

            draw_line(xS, y, xE, y, mask);

        }
        // draw top triangle
        for(int y = y2; y <= y3; y++){
            if(y3 == y2){
                xS = x2;
            }else{
                xS = ((x3 - x2)*(y - y2))/(y3 - y2) + x2;
            }
            if(y3 == y1){
                xS = x1;
            }else{
                xE = ((x3 - x1)*(y - y1))/(y3 - y1) + x1;
            }
            draw_line(xS, y, xE, y, mask);
        }
}


// draw_char
// Purpose: to draw a single character using the built-in font.
// Method: gets the correct bitset based on the character to draw. Iterates
// through the bitset and draws a point if the bit is 1.
// Parameters:
// x x position of the upper-left corner
// y y position of the upper-left corner
// c the character to draw
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_char(int x, int y, char c, bool mask){
    std::bitset<48> letter = font[c];
    int n = 47;
    for(int i = 0; i < 6; i++){
        for(int j = 0; j < 8; j++){
            if(letter[n]){
                if(mask){
                    draw_point_masked(x + i, y + j);
                }else{
                    draw_point(x + i, y + j);
                }
            }
            n--;
        }
    }
}

// draw_string
// Purpose: to draw a string of text using the built-in font.
// Method: Iterates through the string and draws one character at a time.
// Parameters:
// x x position of upper-left corner of string
// y position of upper-left corner of string
// text the string to draw
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_string(int x, int y, std::string text, bool mask){
        for(unsigned int i = 0; i < text.length(); i++){
            draw_char(x + 6 * i, y, text[i], mask);
        }

}

// draw_text_box
// Purpose: Draws text with a set width. Will break the text into lines so that they
// fit within the width. Uses built-in font.
// Method: Calculates how many characters can fit into the width of the box. Breaks the
// text into lines of appropriate width at spaces. Draws each line.
// Parameters
// x x position of the upper-left corner of the text box
// y y position of the upper-left corner of the text box
// w the width of the text box in pixels.
// text the text to draw
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_text_box(int x, int y, int w, std::string text, bool mask){
    // keep track of where to draw the next line
    int line_x = x;
    int line_y = y + 8;

    // determine the number of characters that fit on 1 line
    unsigned int char_limit = w / 6;

    std::string line = "";

    while(char_limit < text.length()){

        // get a substring from current char to char limit

        int i = text.find_last_of(' ', char_limit);
        int j = text.find('\n');
        if( j != -1 && j < i){
            line = text.substr(0, j);
            text = text.substr(j+1);
        }else{
            line = text.substr(0, i);
            text = text.substr(i+1);
        }

        // draw the line and update the y position
        draw_string(line_x, line_y, line, mask);
        line_y += 8;
    }
    // draw rest of text
    draw_string(line_x, line_y, text);
}

// recursive_fill
// purpose: to call the recursive fill function while clearing the checked vector
// Method: clears the checked vector and calls the recursive fill function.
// Parameters:
// x x position of start point
// y y position of start point
void PPMDraw::recursive_fill(int x, int y){
    // this is a wrapper function to call the real recursive draw function
    // while clearing the checked vector
    clear_checked();
    do_recursive_fill(x, y);
}


// copy
// Purpose: to copy a portion of the image to another location on the image.
// Method: Copies data from the copied portion to a temporary pixel array. Then
// copies the data back from the temp array to the image.
// Parameters:
// x x position of upper-left corner of region to be copied
// y y position of upper-left corner of region to be copied
// w width of region to be copied
// h height of region to be copied
// x2 x position of upper-left corner of region to copy to
// y2 y position of upper-left corner of region to copy to
void PPMDraw::copy(int x, int y, int w, int h, int x2, int y2){
    // this copies the rectangular region from x, y with width w and height h
    // and pastes it to x2, y2, it starts with upper right corner

    // checks that the region fits in the image
    if(x < 0){x = 0;}
    if(x >= width){x = width - 1;}
    if(y < 0){y = 0;}
    if(y >= height){y = height - 1;}
    if((x + w) >= width){w = width - x - 1;}
    if((y + h) >= height){h = height - y - 1;}
    // copy data to a temp array
    pixel * temp = new pixel[h * w];
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            temp[j * w + i] = image[(y + j) * width + (x + i)];
        }
    }

    // copy from temp array back to image
    for(int i = 0; i < w; i++){
        for(int j = 0; j < h; j++){
            image[(y2 + j) * width + (x2 + i)] = temp[j * w + i];
        }
    }
    delete temp;

}

// copy
// Purpose: to copy a region of pixels to another region in a second PPMDraw instance
// Method: Copies the pixel data to a temporary array, then copies the data from the
// temp array to the destination PPMDraw instance.
// Parameters
// p pointer to the destination PPMDraw instance
// x x position of upper-left corner of region to be copied
// y y position of upper-left corner of region to be copied
// w width of region to be copied
// h height of region to be copied
// x2 x position of region to copied to in destination image
// y2 y position of region to be copied to in destination image
void PPMDraw::copy(PPMDraw * p, int x, int y, int w, int h, int x2, int y2){
    // this copies the rectangular region from x, y with width w and height h
    // and pastes it to x2, y2, it starts with upper left corner

    // checks that the region fits in the image
    if(x < 0){x = 0;}
    if(x >= width){x = width - 1;}
    if(y < 0){y = 0;}
    if(y >= height){y = height - 1;}
    if((x + w) >= width){w = width - x - 1;}
    if((y + h) >= height){h = height - y - 1;}
    if((x2 + w) >= p->width){w = p->width - x - 1;}
    if((y2 + h) >= p->height){h = p->height - y - 1;}
    // copy data to a temp array
    pixel * temp = new pixel[h * w];
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            temp[j * w + i] = image[(y + j) * width + (x + i)];
        }
    }

    // copy from temp array back to image
    for(int j = 0; j < h; j++){
        for(int i = 0; i < w; i++){
            p->image[(y2 + j) * p->width + (x2 + i)] = temp[j * w + i];
        }
    }
    delete temp;

}


// h_flip
// Purpose: To flip the image horizontally.
// Method: Copies the flipped image data to a temporary pixel array,
// then copies the data back from the temporary array to the image.
void PPMDraw::h_flip(){
    pixel * tmp = new pixel;
	// this for loop counts the rows from top to bottom of the image
	for(int i = 0; i < height; i++){
		// this for loop reverses the pixels in that row
		for(int j = 0, k = j + width - 1 ; j <= k && k >= j; j++, k--){
			// save the first element in the temp pixel
			tmp->r = image[i * width + j].r;
			tmp->g = image[i * width + j].g;
			tmp->b = image[i * width + j].b;
			// move the last element into the first element
			image[i * width + j].r = image[i * width + k].r;
			image[i * width + j].g = image[i * width + k].g;
			image[i * width + j].b = image[i * width + k].b;
			// save the temp pixel to the last element
			image[i * width + k].r = tmp->r;
			image[i * width + k].g = tmp->g;
			image[i * width + k].b = tmp->b;
		}
	}
	delete tmp;
}

// v_flip
// Purpose: to flip an image vertically
// Method: Stores the flipped image into a temporary array, then
// copies the data back to the image.
void PPMDraw::v_flip(){
    pixel * temp = new pixel[width];
    for(int i = 0, j = height - 1 ; i <= j && j >= i; i++, j--){
		// copy upper row to the temp array
		for(int k = 0; k < width; k++){
			temp[k].r = image[i * width + k].r;
			temp[k].g = image[i * width + k].g;
			temp[k].b = image[i * width + k].b;
		}
		//copy lower row to the upper row
		for(int k = 0; k < width; k++){
			image[i * width + k].r = image[j * width + k].r;
			image[i * width + k].g = image[j * width + k].g;
			image[i * width + k].b = image[j * width + k].b;
		}
		// copy from temp array to lower row
		for(int k = 0; k < width; k++){
			image[j * width + k].r = temp[k].r;
			image[j * width + k].g = temp[k].g;
			image[j * width + k].b = temp[k].b;
		}
	}
    delete temp;
}

// rotate
// Purpose: To rotate the image in 90 degree increments.
// Method: Stores the rotated image in a temporary array of pixel data.
// Copies the stored data back into the image.
// Parameters:
// angle can be -3, -2, -1, 1, 2, or 3, which correspond to -270, -180, -90, 90, 180, or 270 degrees
void PPMDraw::rotate(int angle){
    // convert counterclockwise to clockwise rotation
    if(angle == -1){
        angle = 3;
    }else if(angle == -2){
        angle = 2;
    }else if(angle == -3){
        angle = 1;
    }
    switch(angle){
        case 1: {
                // rotate 90 degrees clockwise
                pixel * temp = new pixel[height * width];
                for(int y = 0; y < height; y++){
                    for(int x = 0; x < width; x++){
                        int w_p = height;
                        int x_p = w_p - y - 1;
                        int y_p = x;
                        temp[y_p * w_p + x_p].r = image[y * width + x].r;
                        temp[y_p * w_p + x_p].g = image[y * width + x].g;
                        temp[y_p * w_p + x_p].b = image[y * width + x].b;
                    }
                }
                delete image;
                image = temp;
                temp = 0;
                swap(width, height);
                break;
                }

        case 2: {
                // rotate 180 degrees clockwise
                pixel * temp = new pixel[height * width];
                for(int y = 0; y < height; y++){
                    for(int x = 0; x < width; x++){
                        int x_p = width - x - 1;
                        int y_p = height - y - 1;
                        temp[y_p * width + x_p].r = image[y * width + x].r;
                        temp[y_p * width + x_p].g = image[y * width + x].g;
                        temp[y_p * width + x_p].b = image[y * width + x].b;
                    }
                }
                delete image;
                image = temp;
                temp = 0;
                break;
                }
        case 3: {
                // rotate 270 degrees clockwise
                pixel * temp = new pixel[height * width];
                for(int y = 0; y < height; y++){
                    for(int x = 0; x < width; x++){
                        int w_p = height;
                        int h_p = width;
                        int x_p = y;
                        int y_p = h_p - x - 1;
                        temp[y_p * w_p + x_p].r = image[y * width + x].r;
                        temp[y_p * w_p + x_p].g = image[y * width + x].g;
                        temp[y_p * w_p + x_p].b = image[y * width + x].b;
                    }
                }
                delete image;
                image = temp;
                temp = 0;
                swap(width, height);
                break;
                }

        default: break;
    }
}


// scale
// Purpose: To scale an image up or down.
// Method: When scaling down, the image is broken into squares and the average color of each
// square is copied to a single pixel in the shrunken image. When scaling up, the color of each
// pixel is applied to several pixels in the enlarged image.
// Parameters
// s the scale of the image
void PPMDraw::scale(float s){
    // code for shrinking the image
    // break image into squares, where each square represents 1 pixel in smaller image
    // average the color in each square to color the new pixel
    if(s < 1.0){
        // size of new image
        int w_p = width * s;
        int h_p = height * s;
        // size of squares to break the original image into
        int pix_w = width / w_p;
        int pix_h = height / h_p;
        // number of squares to break the original image into
        int num_w = width / pix_w;
        int num_h = height / pix_h;

        // create the new pixel array
        pixel * new_image = new pixel[w_p * h_p];

        for(int j = 0; j < num_h; j++){
            for(int i = 0; i < num_w; i++){
                // variables to get average color in each square
                int sum_r = 0;
                int sum_g = 0;
                int sum_b = 0;
                int num = 0;
                for(int y = j * pix_h; y < (j + 1) * pix_h; y++){
                    for(int x = i * pix_w; x < (i + 1) * pix_w; x++){
                        // sum up the colors
                        sum_r += image[y * width + x].r;
                        sum_g += image[y * width + x].g;
                        sum_b += image[y * width + x].b;
                        num++;
                    }
                }
                // calculate average and assign the color to the right pixel in the new image
                new_image[j * w_p + i].r = sum_r / num;
                new_image[j * w_p + i].g = sum_g / num;
                new_image[j * w_p + i].b = sum_b / num;
            }
        }

    // swap the pixel arrays and set the width and height
    pixel * old_image = image;
    image = new_image;
    new_image = nullptr;
    delete old_image;
    width = w_p;
    height = h_p;
    }
    // code for enlarging the image
    // scale up each pixel by s.
    if(s > 1.0){
        // size of new image
        int w_p = width * s;
        int h_p = height * s;

        // create the new pixel array
        pixel * new_image = new pixel[w_p * h_p];

        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                for(int j = int(y * s); j < int((y + 1) * s); j++ ){
                    for(int i = int(x * s); i < int((x + 1) * s); i++ ){

                        new_image[j * w_p + i].r = image[y * width + x].r;
                        new_image[j * w_p + i].g = image[y * width + x].g;
                        new_image[j * w_p + i].b = image[y * width + x].b;
                    }
                }
            }
        }

        // swap the pixel arrays and set the width and height
        pixel * old_image = image;
        image = new_image;
        new_image = nullptr;
        delete old_image;
        width = w_p;
        height = h_p;
    }
}


// do_recursive_fill
// Purpose: Private function that actually does the recursive fill operation
// Method: checks the color of the 4 surrounding pixels around x,y and if the
// color is the same, it calls the function on that pixel too, hence the recursion.
// It avoids too much double-checking of pixels by adding checked pixels to the
// checked vector.
// Parameters
// x x position to fill
// y y position to fill
void PPMDraw::do_recursive_fill(int x, int y){


    // get the color of the first pixel, this is the color that will be changed
    pixel pix = image[y * width + x ];

    // fill the current pixel with the new color
    draw_point(x, y);
    checked[y * width + x] = true;

    //check if the color of neighboring pixels matches the first pixel
    // if so, call this function on them
    // Left Pixel
    if(x > 0 && !(checked[y * width + (x - 1)])){
        pixel p2 = image[y * width + (x - 1)];

        if(pix.equals(p2)){
            recursive_fill(x - 1, y);
        }
    }

    if(x < width && !(checked[y * width + (x + 1)])){
        pixel p2 = image[y * width + (x + 1)];

        if(pix.equals(p2)){
            recursive_fill(x + 1, y);
        }
    }

    if(y > 0 && !(checked[(y - 1) * width + x])){
        pixel p2 = image[(y - 1) * width + x];

        if(pix.equals(p2)){
            recursive_fill(x, y - 1);
        }
    }

    if(y < height && !(checked[(y + 1) * width + x])){
        pixel p2 = image[(y + 1) * width + x];

        if(pix.equals(p2)){
            recursive_fill(x, y + 1);
        }
    }


}

// save
// Purpose: to save the image to a ppm file
// Method: Create an ostream instance and save the image parameters as ASCII.
// Then save the pixel data as binary to save space.
// Parameters:
// file the filename of the image
bool PPMDraw::save(std::string file){
    std::ofstream save(file.c_str(), std::ios_base::out | std::ios_base::binary);
    if(save.is_open()){
        save << "P6" << std::endl;
        save << width << " " << height << std::endl;
        save << "255" << std::endl;
        unsigned char * temp = new unsigned char[height * width * 3];
        for(int i  = 0; i < height * width; i++){
            temp[i * 3 + 0] = image[i].r;
            temp[i * 3 + 1] = image[i].g;
            temp[i * 3 + 2] = image[i].b;
        }
        save.write(reinterpret_cast<const char *> (temp), height*width*3*sizeof(unsigned char));
        delete temp;
        save.close();
        return true;
    }else{
        return false;
    }


}

// load
// Purpose: to load data from a ppm file into a PPMDraw image
// Method: Opens a file as ASCII to get the image parameters, then
// reopens the file as binary to load the data into the image.
// Parameters
// file the name of the file to load
bool PPMDraw::load(std::string file){
    // load the file as ASCII to read the header
    std::ifstream load(file.c_str(), std::ios_base::in);
    if(load.is_open()){
        int offset = 0;     // this will be used to determine how long the header section is
        int i = 0;
        std::string token;  // holds the header data

        while(load >> token && i < 4)
        {
            // skip comments, which have to follow a # char
            if(token[0] == '#')
            {
                load.ignore(256,'\n');
                continue;
            }

            switch(i)
            {
                case 0: break;//m_magic_num = int(token[1]-'0');

                case 1: width = std::atoi(token.c_str());
                        break;

                case 2: height = std::atoi(token.c_str());
                        break;

                case 3: break;//m_color_depth = atoi(token.c_str());
            }

            ++i;
            offset = load.tellg();
        }

        load.close();
        offset += 1;
        // open the file again as a binary file to read the data
        std::ifstream biload(file.c_str(), std::ios_base::in | std::ios_base::binary);
        // jump past header information
        biload.seekg(offset);

        // create the array to hold the data, clear out the old data
        delete image;
        image = new pixel[ width * height];
        unsigned char * temp = new unsigned char[width * height * 3];
        biload.read( (char *)temp, height*width*3*sizeof(unsigned char));
        for(int i = 0; i < width * height; i++){
            image[i] = pixel{temp[3*i + 0], temp[3*i + 1], temp[3*i + 2]};
        }
        delete temp;

        biload.close();
        return true;
    }else{
        return false;
    }

}

// clear_checked
// Purpose: Private function to clear the checked vector before doing a recursive fill
// Method Iterates through the checked vector and sets everything to false
void PPMDraw::clear_checked(){
    for(int i = 0; i < width * height; i++){
        checked[i] = false;
    }
}

// abs
// Purpose: absolute value function
int PPMDraw::abs(int i){
    if(i < 0){
        return -i;
    }else{
        return i;
    }
}

// swap
// Purpose: swaps the values of two variables
template<typename T> void PPMDraw::swap(T & a, T & b){
    T c = a;
    a = b;
    b = c;
}

// min
// Purpose returns the lesser of the values
template<typename T> T PPMDraw::min(T a, T b){
    if(a < b){
        return a;
    }else{
        return b;
    }
}

// max
// Purpose: returns the greater of the two values
template<typename T> T PPMDraw::max(T a, T b){
    if(a > b){
        return a;
    }else{
        return b;
    }
}

// between
// Purpose: returns true if the value i is between lower and upper
// i value to be evaluated
// lower lower bound
// upper upper bound
template<typename T> bool PPMDraw::between(T i, T lower, T upper){
    if((i >= lower && i <= upper)){ // || (i <= lower && i >= upper)){
        return true;
    }else{
        return false;
    }
}

// lerp
// Purpose: linear interpolation between a and b
// Parameters:
// a initial value, t = 0
// b final value, t = 1
// t "time" parameter, from 0 to 1
float PPMDraw::lerp(float a, float b, float t){
    // t must be 0.0 to 1.0
    return a + (b - a)*t;
}

// init_alpha
// Purpose: To initialize the built-in font.
// Method: Creates a map of characters to bitsets, where each bitset represents
// the pixel data for a character. Imagine a 6 wide by 8 tall grid. Each character
// is designed to fit in that grid. Pixels are drawn for 1, and not drawn for 0.
void PPMDraw::init_alpha(){
    // Define a simple font for drawing text
    font[' '] = std::bitset<48>  (std::string("000000000000000000000000000000000000000000000000"));
    font['!'] = std::bitset<48>  (std::string("000000000000000011110110000000000000000000000000"));
    font['"'] = std::bitset<48>  (std::string("000000001100000000000000110000000000000000000000"));
    font['#'] = std::bitset<48>  (std::string("001010001111111000101000111111100010100000000000"));
    font['$'] = std::bitset<48>  (std::string("001001000101010011111110010101000100100000000000"));
    font['%'] = std::bitset<48>  (std::string("000000100100110000010000011000001000010000000000"));
    font['&'] = std::bitset<48>  (std::string("000111001110001010110010110011000000001000000000"));
    font['\\'] = std::bitset<48>  (std::string("100000000110000000010000000011000000001000000000"));
    font['('] = std::bitset<48>  (std::string("000000000000000001111100100000100000000000000000"));
    font[')'] = std::bitset<48>  (std::string("000000001000001001111100000000000000000000000000"));
    font['*'] = std::bitset<48>  (std::string("010010000011000011100000001100000100100000000000"));
    font['+'] = std::bitset<48>  (std::string("000100000001000001111100000100000001000000000000"));
    font[','] = std::bitset<48>  (std::string("000000000000000000000110000000000000000000000000"));
    font['-'] = std::bitset<48>  (std::string("000100000001000000010000000100000001000000000000"));
    font['.'] = std::bitset<48>  (std::string("000000000000000000000100000000000000000000000000"));
    font['/'] = std::bitset<48>  (std::string("000000100000110000010000011000001000000000000000"));
    font['0'] = std::bitset<48>  (std::string("011111001000001010000010100000100111110000000000"));
    font['1'] = std::bitset<48>  (std::string("000000001000001011111110000000100000000000000000"));
    font['2'] = std::bitset<48>  (std::string("010011101001001010010010100100100111001000000000"));
    font['3'] = std::bitset<48>  (std::string("010001001000001010000010100100100110110000000000"));
    font['4'] = std::bitset<48>  (std::string("111100000001000000010000000100001111111000000000"));
    font['5'] = std::bitset<48>  (std::string("111001001001001010010010100100101001110000000000"));
    font['6'] = std::bitset<48>  (std::string("011111001001001010010010100100101000110000000000"));
    font['7'] = std::bitset<48>  (std::string("100000001000000010000110100110001110000000000000"));
    font['8'] = std::bitset<48>  (std::string("011011001001001010010010100100100110110000000000"));
    font['9'] = std::bitset<48>  (std::string("011000001001000010010000100100000111111000000000"));
    font[':'] = std::bitset<48>  (std::string("000000000000000001000100000000000000000000000000"));
    font[';'] = std::bitset<48>  (std::string("000000000000000001000110000000000000000000000000"));
    font['<'] = std::bitset<48>  (std::string("000000000001000000101000010001000000000000000000"));
    font['='] = std::bitset<48>  (std::string("001010000010100000101000001010000000000000000000"));
    font['>'] = std::bitset<48>  (std::string("000000000100010000101000000100000000000000000000"));
    font['?'] = std::bitset<48>  (std::string("010000001000000010001010100100000110000000000000"));
    font['@'] = std::bitset<48>  (std::string("011111001000001010111010101010100111001000000000"));
    font['A'] = std::bitset<48>  (std::string("011111101001000010010000100100000111111000000000"));
    font['B'] = std::bitset<48>  (std::string("111111101001001010010010100100100110110000000000"));
    font['C'] = std::bitset<48>  (std::string("011111001000001010000010100000100100010000000000"));
    font['D'] = std::bitset<48>  (std::string("111111101000001010000010100000100111110000000000"));
    font['E'] = std::bitset<48>  (std::string("111111101001001010010010100100101000001000000000"));
    font['F'] = std::bitset<48>  (std::string("111111101001000010010000100100001000000000000000"));
    font['G'] = std::bitset<48>  (std::string("011111001000001010000010100010100100110000000000"));
    font['H'] = std::bitset<48>  (std::string("111111100001000000010000000100001111111000000000"));
    font['I'] = std::bitset<48>  (std::string("100000101000001011111110100000101000001000000000"));
    font['J'] = std::bitset<48>  (std::string("000011000000001000000010000000101111110000000000"));
    font['K'] = std::bitset<48>  (std::string("111111100001000000010000001010001100011000000000"));
    font['L'] = std::bitset<48>  (std::string("111111100000001000000010000000100000001000000000"));
    font['M'] = std::bitset<48>  (std::string("111111101000000001100000100000001111111000000000"));
    font['N'] = std::bitset<48>  (std::string("111111100100000000100000000100001111111000000000"));
    font['O'] = std::bitset<48>  (std::string("011111001000001010000010100000100111110000000000"));
    font['P'] = std::bitset<48>  (std::string("111111101001000010010000100100001111000000000000"));
    font['Q'] = std::bitset<48>  (std::string("011111001000001010001010100001000111101000000000"));
    font['R'] = std::bitset<48>  (std::string("111111101001000010010000100110001111011000000000"));
    font['S'] = std::bitset<48>  (std::string("011000101001001010010010100100101000110000000000"));
    font['T'] = std::bitset<48>  (std::string("100000001000000011111110100000001000000000000000"));
    font['U'] = std::bitset<48>  (std::string("111111000000001000000010000000101111110000000000"));
    font['V'] = std::bitset<48>  (std::string("111110000000010000000010000001001111100000000000"));
    font['W'] = std::bitset<48>  (std::string("111111100000001000001100000000101111111000000000"));
    font['X'] = std::bitset<48>  (std::string("110001100010100000010000001010001100011000000000"));
    font['Y'] = std::bitset<48>  (std::string("110000000010000000011110001000001100000000000000"));
    font['Z'] = std::bitset<48>  (std::string("100001101000101010010010101000101100001000000000"));
    font['['] = std::bitset<48>  (std::string("000000001111111010000010100000100000000000000000"));
    font['\''] = std::bitset<48>  (std::string("000000000000000011100000000000000000000000000000"));
    font[']'] = std::bitset<48>  (std::string("000000001000001010000010111111100000000000000000"));
    font['^'] = std::bitset<48>  (std::string("001000000100000010000000010000000010000000000000"));
    font['_'] = std::bitset<48>  (std::string("000000100000001000000010000000100000001000000000"));
    font['`'] = std::bitset<48>  (std::string("000000001000000001000000000000000000000000000000"));
    font['a'] = std::bitset<48>  (std::string("000001000010101000101010001010100001111000000000"));
    font['b'] = std::bitset<48>  (std::string("111111100001001000010010000100100000110000000000"));
    font['c'] = std::bitset<48>  (std::string("000111000010001000100010001000100001010000000000"));
    font['d'] = std::bitset<48>  (std::string("000011000001001000010010000100101111111000000000"));
    font['e'] = std::bitset<48>  (std::string("000111000010101000101010001010100001101000000000"));
    font['f'] = std::bitset<48>  (std::string("000100000111111010010000100100000000000000000000"));
    font['g'] = std::bitset<48>  (std::string("001100100100100101001001010010010011111000000000"));
    font['h'] = std::bitset<48>  (std::string("111111100001000000010000000100000000111000000000"));
    font['i'] = std::bitset<48>  (std::string("000000000000000001011110000000000000000000000000"));
    font['j'] = std::bitset<48>  (std::string("000000100000000100000001010111100000000000000000"));
    font['k'] = std::bitset<48>  (std::string("111111100000100000010100001000100000000000000000"));
    font['l'] = std::bitset<48>  (std::string("000000000000000011111110000000000000000000000000"));
    font['m'] = std::bitset<48>  (std::string("000111100001000000001000000100000001111000000000"));
    font['n'] = std::bitset<48>  (std::string("001111100001000000010000000100000001111000000000"));
    font['o'] = std::bitset<48>  (std::string("000111000010001000100010001000100001110000000000"));
    font['p'] = std::bitset<48>  (std::string("001111110010010000100100001001000001100000000000"));
    font['q'] = std::bitset<48>  (std::string("000110000010010000100100001001000011111100000000"));
    font['r'] = std::bitset<48>  (std::string("000000000011111000010000000100000000000000000000"));
    font['s'] = std::bitset<48>  (std::string("000000000001001000101010001010100010010000000000"));
    font['t'] = std::bitset<48>  (std::string("000000000010000011111110001000000000000000000000"));
    font['u'] = std::bitset<48>  (std::string("000111000000001000000010000000100001110000000000"));
    font['v'] = std::bitset<48>  (std::string("000110000000010000000010000001000001100000000000"));
    font['w'] = std::bitset<48>  (std::string("000111100000001000000100000000100001111000000000"));
    font['x'] = std::bitset<48>  (std::string("001000100001010000001000000101000010001000000000"));
    font['y'] = std::bitset<48>  (std::string("001100000000100000000111000010000011000000000000"));
    font['z'] = std::bitset<48>  (std::string("010001100100101001010010011000100000000000000000"));
    font['{'] = std::bitset<48>  (std::string("000000000000000001101100100100100000000000000000"));
    font['|'] = std::bitset<48>  (std::string("000000000000000011111110000000000000000000000000"));
    font['}'] = std::bitset<48>  (std::string("000000000000000010010010011011000000000000000000"));
    font['~'] = std::bitset<48>  (std::string("000100000010000000010000000010000001000000000000"));
}

// load_extended_font
// Purpose: to load an extra font from a pbm file.
// Method: Loads the pbm file as ASCII and reads in all of the image data into a vector of strings.
// Then it creates a map of characters to vectors of bools instead of bitsets, because bitsets can't
// be set with dynamic sizes at runtime.
// Parameters:
// file the name of the pbm file to load
// num_chars the number of characters to load
// chars_wide the width of the character grid in the pbm file
// chars_high the height of the character grid in the pbm file
// char_width the width of each character in pixels
// char_height the height of each character in pixels
bool PPMDraw::load_extended_font(std::string file, int num_chars, int chars_wide, int chars_high, int char_width, int char_height){
    // load the file as ASCII to read the header

    std::ifstream load(file.c_str(), std::ios_base::in);
    if(load.is_open()){
        int offset = 0;     // this will be used to determine how long the header section is
        int i = 0;
        std::string token;  // holds the header data
        std::stringstream input_data;
        int font_width;
        int font_height;

        std::vector<std::string> data = std::vector<std::string>();

        ext_font_char_height = char_height;
        ext_font_char_width = char_width;

        while(load >> token && i < 3)
        {
            // skip comments, which have to follow a # char
            if(token[0] == '#')
            {
                load.ignore(256,'\n');
                continue;
            }

            switch(i)
            {
                case 0: break;//m_magic_num = int(token[1]-'0');

                case 1: font_width = std::atoi(token.c_str());
                        break;

                case 2: font_height = std::atoi(token.c_str());
                        break;

            }

            ++i;
            offset = load.tellg();
        }

        load.close();
        offset += 1;
        // open the file again to read the data
        std::ifstream biload(file.c_str(), std::ios_base::in);
        // jump past header information
        biload.seekg(offset);
        // load font data into a single string
        while(std::getline(biload, token)){
            input_data << token;
        }
        token = input_data.str();

        // now split the single string into substrings so that the string length is equal to the width of the image in pixels
        // the number of strings in the vector will be equal to the height of the image in pixels
        // this creates a 2D "array" of 0 and 1...
        for(int i = 0; i < font_height; i++){
            std::string substring = token.substr(0, font_width);
            token = token.substr(font_width);
            data.push_back(substring);
        }

        // iterate through the characters we need to load
        for(int c = 0; c < num_chars; c++){
            // find the x and y for the upper left hand corner of each character
            int start_y = char_width * (c % chars_wide);
            int start_x = char_height * (c / chars_wide);
            // add the vector<bool> to the appropriate spot in the map...
            ext_font[(unsigned char)c] = std::vector<bool>();
            // iterate through the x and y
            for(int x = start_x; x < start_x + char_width; x++){
                for(int y = start_y; y < start_y + char_height; y++){
                    // check the value in the data array we made earlier
                    // if 1, add true to indicate a pixel is drawn.
                    if(data.at(x).at(y) == '1'){
                        ext_font.at((unsigned char)c).push_back(true);

                    }else{
                        ext_font.at((unsigned char)c).push_back(false);
                    }
                }
            }
        }


        biload.close();
        return true;
    }else{
        return false;
    }
}

// draw_ext_char
// Purpose: to draw a single character from the extended font
// Method: chooses the correct vector of bools based on the input character c.
// iterates through the bools and draws pixels accordingly.
// Parameters:
// x x position of upper-left corner of the character
// y y position of the upper-left corner of the character
// c the character to draw
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_ext_char(int x, int y, unsigned char c, bool mask){
    // similarish to the draw_char function, but generalized to account for extended character sizes.
    std::vector<bool> letter = ext_font[c];
    for(int i = 0; i < ext_font_char_width; i++){
        for(int j = 0; j < ext_font_char_height; j++){
            if(letter[j * ext_font_char_width + i]){
                if(mask){
                    draw_point_masked(x + i, y + j);
                }else{
                    draw_point(x + i, y + j);
                }
            }
        }
    }
}

// draw_ext_string
// Purpose: to draw a string of text using the extended font
// Method: similar to draw_string, but adapted to use the extended font width and height
// Parameters:
// x x position of upper-left corner of the string
// y y position of the upper-left corner of the string
void PPMDraw::draw_ext_string(int x, int y, std::string text, bool mask){
        for(unsigned int i = 0; i < text.length(); i++){
            draw_ext_char(x + ext_font_char_width * i, y, (unsigned char)text[i], mask);
        }
}

// draw_ext_text_box
// Purpose: to draw text with a set width using the extended font
// Method: Similar to draw_text_box, but adapted to use extended font width and height
// Parameters:
// x x position of upper-left corner of text box
// y y position of upper-left corner of text box
// text the text to draw
// mask whether or not to use masking, defaults to false
void PPMDraw::draw_ext_text_box(int x, int y, int w, std::string text, bool mask){
    // keep track of where to draw the next line
    int line_x = x;
    int line_y = y + ext_font_char_height;

    // determine the number of characters that fit on 1 line
    unsigned int char_limit = w / ext_font_char_width;

    std::string line = "";

    while(char_limit < text.length()){

        // get a substring from current char to char limit

        int i = text.find_last_of(' ', char_limit);
        int j = text.find('\n');
        if( j != -1 && j < i){
            line = text.substr(0, j);
            text = text.substr(j+1);
        }else{
            line = text.substr(0, i);
            text = text.substr(i+1);
        }

        // draw the line and update the y position
        draw_ext_string(line_x, line_y, line, mask);
        line_y += ext_font_char_height;
    }
    // draw rest of text
    draw_ext_string(line_x, line_y, text, mask);
}

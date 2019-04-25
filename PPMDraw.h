// PPMDraw v2
// COPYRIGHT 2019 SAM CROWLEY
// samtcrowley@gmail.com
// FOR EDUCATIONAL PURPOSES ONLY, NOT LIABLE FOR ANY HARM CAUSED BY CODE
// SERIOUSLY, IF YOU DID ANYTHING WITH THIS CODE THAT COULD HURT SOMEONE
// I DON'T KNOW HOW YOU DID IT. THIS IS CODE FOR DRAWING PICTURES.
// ALSO, DON'T USE THIS FOR COMMERCIAL PURPOSES WITHOUT PERMISSION.
// THERE ARE PROBABLY BETTER TOOLS OUT THERE ANYWAY.

#ifndef PPMDRAW_H
#define PPMDRAW_H

#include <fstream>
#include <sstream>
#include <map>
#include <bitset>
#include <vector>


struct pixel{
    unsigned char r;
    unsigned char g;
    unsigned char b;

    bool equals(pixel p){
        return (r == p.r && g == p.g && b == p.b);
    }
};

class PPMDraw
{
    public:

        int width;
        int height;

        pixel * image; // pointer to an array of pixel data

        PPMDraw(int w, int h); // basic constructor for new images
        PPMDraw(const PPMDraw & ppm); // copy constructor
        PPMDraw(PPMDraw * p, int x, int y, int w, int h); // creates a new image from a portion of an existing image p starting at x,y and w wide and h high
        PPMDraw(pixel * p, int w, int h); // creates an image from an existing array of pixel data
        virtual ~PPMDraw();

        PPMDraw & operator=(const PPMDraw & ppm); // assignment operator

        void fill(unsigned char r, unsigned char g, unsigned char b); // fills the image with the given RGB color

        void set_color(unsigned char r, unsigned char g, unsigned char b); // this is the color that used for drawing to the screen
        void set_mask_color(unsigned char r, unsigned char g, unsigned char b); // when using a masked drawing function, it only draws on these color pixels

        void draw_point(int x, int y); // draws a single pixel at x, y
        void draw_point_masked(int x, int y); // draws a single pixel at x, y IF the color of that pixel matches the mask color

        void draw_line(int x1, int y1, int x2, int y2, bool mask = false); // draws a line
        void draw_dash_line(int x1, int y1, int x2, int y2, int dash, bool mask = false); // draws a dashed line. dash sets the length of the dashes

        void draw_rect(int x1, int y1, int x2, int y2, bool mask = false); // draws a rectangle
        void draw_dash_rect(int x1, int y1, int x2, int y2, int dash, bool mask = false); // draws a rectangle from dashed lines
        void draw_fill_rect(int x1, int y1, int x2, int y2, bool mask = false); // draws a filled rectangle

        void draw_circle(int xC, int yC, int rad, bool mask = false); // draws a circle at xC, yC with radius rad
        void draw_fill_circle(int xC, int yC, int rad, bool mask = false); // draws a dashed circle
        void draw_dash_circle(int xC, int yC, int rad, int dash, bool mask = false); // draws a filled circle

        void draw_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask = false); // draws part of a circle from angle a1 to a2
        void draw_dash_circle_sector(int xC, int yC, int rad, float a1, float a2, int dash, bool mask = false); // draws a dashed part of a circle
        void draw_fill_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask = false); // draws part of a filled in circle

        void draw_ellipse(int x1, int y1, int x2, int y2, bool mask = false); // draws an ellipse inside the rectangular space from x1, y1 to x2, y2
        void draw_fill_ellipse(int x1, int y1, int x2, int y2, bool mask = false); // draws a dashed ellipse
        void draw_dash_ellipse(int x1, int y1, int x2, int y2, int dash, bool mask = false); // draws a filled ellipse

        void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool mask = false); // draws a triangle
        void draw_dash_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int dash, bool mask = false); // draws a dashed triangle
        void draw_fill_triangle(int x1, int y1, int x2, int y2, int x3, int y3, bool mask = false); // draws a filled triangle


        void draw_char(int x, int y, char c, bool mask = false); // draws a character using the built-in font
        void draw_string(int x, int y, std::string text, bool mask = false); // draws a string using the built-in font
        void draw_text_box(int x, int y, int w, std::string text, bool mask = false); // draws text with a set width using the built-in font

        bool load_extended_font(std::string file, int num_chars, int chars_wide, int chars_high, int char_width, int char_height); // loads an extended font from a pbm file, for when the built-in font isn't good enough
        void draw_ext_char(int x, int y, unsigned char c, bool mask = false); // draws a character using the extended font
        void draw_ext_string(int x, int y, std::string text, bool mask = false); // draws a string using the extended font
        void draw_ext_text_box(int x, int y, int w, std::string text, bool mask = false); // draws text with a set width using the extended font

        void recursive_fill(int x, int y); // starting at x,y, fills in all adjacent pixels with the same color ( flood fill ) MEMORY INTENSIVE AND SLOW

        void copy(int x, int y, int w, int h, int x2, int y2); // copies the region at x,y of w width and h height to a new region at x2, y2
        void copy(PPMDraw * p, int x, int y, int w, int h, int x2, int y2); // copies the region from x,y of w width and h high to a new region at x2, y2 in another instance of PPMDraw p

        void h_flip(); // flips the image horizontally
        void v_flip(); // flips the image vertically
        void rotate(int angle); // rotates the image in 90 degree increments, arguments are -3, -2, -1, 1, 2, 3, where 1 = 90, 2 = 180, 3 = 270

        void scale(float s); // scales the image up or down

        bool save(std::string file); // saves the image to a file
        bool load(std::string file); // loads data from a file

        float const PI = 3.14159265;




    protected:
    private:



        std::vector<bool> checked; // this is used in the recursive fill function to keep track of what pixels have been checked for coloring.

        // data that holds the draw color
        unsigned char red;
        unsigned char green;
        unsigned char blue;

        // data that holds the mask color
        unsigned char mask_r;
        unsigned char mask_g;
        unsigned char mask_b;

        void clear_checked(); // before performing the recursive fill function, this is called to reset the checked vector
        void do_recursive_fill(int x, int y); // this is the recursive function called to do the recursive fill operation

        // these functions are used to help draw circle sectors
        void circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask);
        void dash_circle_sector(int xC, int yC, int rad, float a1, float a2, int dash, bool mask);
        void fill_circle_sector(int xC, int yC, int rad, float a1, float a2, bool mask);

        // some helper functions for math
        int abs(int i);
        template<typename T> void swap(T & a, T & b);
        template<typename T> T min(T a, T b);
        template<typename T> T max(T a, T b);
        template<typename T> bool between(T i, T lower, T upper);
        float lerp(float a, float b, float t);

        // initializes the built-in font
        void init_alpha();
        std::map<char, std::bitset<48> > font; // holds the built-in font data

        // holds the extended font data
        std::map<unsigned char, std::vector<bool> > ext_font;
        int ext_font_char_width; // width of characters in the extended font
        int ext_font_char_height; // height of characters in the extended font


};

#endif // PPMDRAW_H

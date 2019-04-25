#include <iostream>
#include <PPMDraw.h>


int main(){
    std::cout << "Running PPMDRaw Test" << std::endl;

    std::cout << "Testing Draw Functions" << std::endl;
    PPMDraw test = PPMDraw(300, 300);
    // fill the image with black
    test.fill(0, 0, 0);
    // set the draw color to white
    test.set_color(255, 255, 255);
    // set the mask color to black, this allows us to draw "masked" pixels on top of black pixels
    test.set_mask_color(0, 0, 0);

    // test the point drawing functions
    test.draw_point(5, 5);
    test.draw_point_masked(10, 5);
    //set mask color to red
    test.set_mask_color(255, 0, 0);
    test.draw_point_masked(15, 5); // THIS POINT SHOULD NOT APPEAR

    // test line drawing functions
    test.draw_line(5, 15, 25, 15);
    for(int i = 5; i > 0; i--){
        test.draw_dash_line(5, 15 + 3 * i, 25, 15 + 3 * i, i);
    }

    // test the rectangle draw functions
    test.draw_rect(5, 30, 25, 50);
    test.draw_dash_rect(5, 60, 25, 80, 0);
    test.draw_fill_rect(5, 90, 25, 110);

    // test the circle draw functions
    test.draw_circle(50, 40, 10);
    test.draw_dash_circle(50, 70, 10, 0);
    test.draw_fill_circle(50, 100, 10);

    // test triangle draw functions
    test.draw_triangle(75, 50, 100, 50, 88, 30);
    test.draw_dash_triangle(75, 80, 100, 80, 88, 60, 0);
    test.draw_fill_triangle(75, 110, 100, 110, 88, 90);

    // test ellipse drawing functions
    test.draw_ellipse(125, 30, 150, 50);
    test.draw_dash_ellipse(125, 60, 150, 80, 0);
    test.draw_fill_ellipse(125, 90, 150, 110);

    // test circle sector drawing functions
    test.draw_circle_sector(175, 40, 10, 0, 270);
    test.draw_dash_circle_sector(175, 70, 10, 0, 270, 0);
    test.draw_fill_circle_sector(175, 100, 10, 0, 270);

    // test recursive fill function
    // draw lines to contain the fill
    test.draw_line(5, 210, 15, 200);
    test.draw_line(15, 200, 25, 210);
    test.draw_line(25, 210, 15, 220);
    test.draw_line(15, 220, 5, 210);
    // start the fill in the center of the space
    test.recursive_fill(15, 210);

    // test the text functions
    test.draw_text_box(50, 150, 175, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");

    // load an extended font
    test.load_extended_font("ASCII.pbm", 256, 16, 8, 16, 16);
    test.draw_ext_text_box(50, 200, 175, "Lorem ipsum dolor sit amet.");


    test.save("test.ppm");
    std::cout << "Test image saved to test.ppm" << std::endl;

    std::cout << "Drawing a House" << std::endl;

    PPMDraw house = PPMDraw(300, 300);

    // fill the background with blue sky
    house.fill(0, 0, 200);

    // add green grass
    house.set_color(0, 200, 0);
    house.draw_fill_rect(0, 200, 300, 300);

    // add a red rectangle for the house
    house.set_color(200, 0, 0);
    house.draw_fill_rect(50, 100, 250, 250);

    // add a roof
    house.set_color(100, 100, 0);
    house.draw_fill_triangle(40, 100, 260, 100, 150, 50);

    // add a door
    house.draw_fill_rect(100, 150, 140, 250);
    house.set_color(50, 50, 50);
    house.draw_fill_circle(110, 200, 5);

    // add a window
    house.set_color(150, 150, 255);
    house.draw_fill_rect(175, 150, 225, 200);
    house.set_color(100, 100, 0);
    house.draw_rect(175, 150, 225, 200);
    house.draw_line(200, 150, 200, 200);
    house.draw_line(175, 175, 225, 175);

    // add a cloud BEHIND the roof
    // set mask color to the sky color
    house.set_mask_color(0, 0, 200);
    house.set_color(255, 255, 255);
    house.draw_fill_ellipse(100, 25, 200, 75, true); // TRUE so that we use the mask color.

    // save the image
    house.save("house.ppm");

    std::cout << "Image saved to house.ppm" << std::endl;

    std::cout << "Reloading house.ppm" << std::endl;
    PPMDraw house2 = PPMDraw(300, 300);
    house2.load("house.ppm");
    std::cout << "house.ppm reloaded." << std::endl;

    std::cout << "Shrinking image by 75%" << std::endl;
    house2.scale(0.5);
    std::cout << "Creating an image to paste to..." << std::endl;
    PPMDraw destination = PPMDraw(300, 300);
    house2.copy(&destination, 0, 0, 150, 150, 0, 0);
    house2.rotate(1);
    house2.copy(&destination, 0, 0, 150, 150, 150, 0);
    house2.rotate(1);
    house2.copy(&destination, 0, 0, 150, 150, 0, 150);
    house2.rotate(1);
    house2.copy(&destination, 0, 0, 150, 150, 150, 150);
    destination.save("copied.ppm");
}

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "C_numbers.h"

int main() {
    int x_length = 800;
    int y_length = 600;
    int max_n_iterations = 500;

    // Initial complex plane mapping
    double x_center = -0.5;
    double y_center = 0.0;
    double scale = 0.005;  // pixel-to-plane scale

    

    // Create a palette with gamma correction
    sf::Color* palette = new sf::Color[max_n_iterations];
    for(int k = 0; k < max_n_iterations; k++){
        double t = pow(k / (double)max_n_iterations, 0.3);
        palette[k] = sf::Color(
            std::min(255,int(255*t)),
            std::min(255,int(128*t)),
            std::min(255,int(255*(1.0-t)))
        );
    }


    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    // Variables for zoom rectangle
    sf::Vector2i startPos, endPos;
    bool selecting = false;

    auto Mandelbrot = [&]() {
        image.create(x_length, y_length, sf::Color::Black);
        int progress = 0;
        //int total = x_length * y_length;
        C_number zero(0,0);

        for(int px_y = 0; px_y < y_length; px_y++) {
            for(int px_x = 0; px_x < x_length; px_x++) {
                double x = (px_x - x_length/2.0) * scale + x_center;
                double y = (px_y - y_length/2.0) * scale + y_center;
                C_number z(x, y);
                C_number iterator[2] {zero, zero};
                int count = 0;

                while(iterator[1].square_norm() <= 4 && count < max_n_iterations){
                    iterator[0] = iterator[1];
                    iterator[1] = iterator[0]*iterator[0] + z;
                    count++;
                }

                // Smoothed coloring
                int color_index;
                if(count < max_n_iterations){
                    double mu = count - log2(log2(iterator[1].square_norm()));
                    color_index = int(mu);
                    if(color_index >= max_n_iterations) color_index = max_n_iterations - 1;
                    image.setPixel(px_x, px_y, palette[color_index]);
                }

                progress++;
            }
        }
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    };

    auto Julia = [&](C_number seed) {
        image.create(x_length, y_length, sf::Color::Black);
        int progress = 0;


        for(int px_y = 0; px_y < y_length; px_y++) {
            for(int px_x = 0; px_x < x_length; px_x++) {

                double x = (px_x - x_length/2.0) * scale + x_center;
                double y = (px_y - y_length/2.0) * scale + y_center;
                C_number z(x, y);
                C_number iterator[2] {z, z};
                int count = 0;

                while(iterator[1].square_norm() <= 4 &&  count < max_n_iterations){
                    iterator[0] = iterator[1];
                    iterator[1] = iterator[0]*iterator[0] + seed;
                    count++;
                }

                // Smoothed coloring
                int color_index;
                if(count < max_n_iterations){
                    double mu = count - log2(log2(iterator[1].square_norm()));
                    color_index = (int)((mu / max_n_iterations) * (max_n_iterations-1));;
                    if(color_index >= max_n_iterations) color_index = max_n_iterations - 1;
                    image.setPixel(px_x, px_y, palette[color_index]);
                }

                progress++;
            }
        }
        texture.loadFromImage(image);
        sprite.setTexture(texture);
    };






    std::cout<<"::::::::::::::::::::::::::::::::::::\n";
    std::cout<<"::::::::::FRACTALS FACTORY::::::::::\n";
    std::cout<<"::::::::::::::::::::::::::::::::::::\n";

    std::cout<<"\nWhich fractal do you want to draw?\n"
        <<"1: The Mandelbrot set.\n"
        <<"2: A Julia set.\n";

    int choice;
    C_number seed;

    do{
        std::cout<<"Pick an option,\n";
        std::cin>>choice;
    }while(choice!=1 && choice!=2);

    if(choice==2){
        double re,im;
        std::cout<<"I need a complex number as seed (maybe close to the boundary of the Mandelbrot set)\n";
        std::cout<<"For example: -0.7+0.27015 i, −0.8+0.156 i, −0.4+0.6 i, 0.285+0.01 i, ...\n";
        std::cout<<"Give me the real and imaginary part.\n";
        std::cin>>re>>im;
        seed.set_re(re);
        seed.set_im(im);
    };

    sf::RenderWindow window(sf::VideoMode(x_length, y_length), "Fractals");

    switch(choice){
        case 1:
            Mandelbrot(); // initial render of Mandelbrot set
            std::cout<<"Select an area to zoom in.\n";
            break;
        case 2:
            Julia(seed);
            std::cout<<"Select an area to zoom in.\n";
            break;
    };


    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();

            else if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
                startPos = sf::Mouse::getPosition(window);
                selecting = true;
            }

            else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
                if(selecting){
                    endPos = sf::Mouse::getPosition(window);
                    selecting = false;

                    // Map pixel rectangle to complex plane
                    double x0 = (std::min(startPos.x, endPos.x) - x_length/2.0)*scale + x_center;
                    double x1 = (std::max(startPos.x, endPos.x) - x_length/2.0)*scale + x_center;
                    double y0 = (std::min(startPos.y, endPos.y) - y_length/2.0)*scale + y_center;
                    double y1 = (std::max(startPos.y, endPos.y) - y_length/2.0)*scale + y_center;

                    // Update center and scale
                    x_center = (x0 + x1)/2.0;
                    y_center = (y0 + y1)/2.0;
                    scale = std::max((x1 - x0)/x_length, (y1 - y0)/y_length);

                    if(choice==1)
                        Mandelbrot();
                    else
                        Julia(seed);
                }
            }
        }

        window.clear();
        window.draw(sprite);

        // Draw selection rectangle
        if(selecting){
            sf::Vector2i currentPos = sf::Mouse::getPosition(window);
            sf::RectangleShape rect;
            rect.setPosition(std::min(startPos.x, currentPos.x), std::min(startPos.y, currentPos.y));
            rect.setSize(sf::Vector2f(std::abs(currentPos.x - startPos.x), std::abs(currentPos.y - startPos.y)));
            rect.setFillColor(sf::Color::Transparent);
            rect.setOutlineColor(sf::Color::White);
            rect.setOutlineThickness(1);
            window.draw(rect);
        }

        window.display();
    }

    delete[] palette;
    return 0;
}

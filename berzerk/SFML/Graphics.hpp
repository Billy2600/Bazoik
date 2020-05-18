#ifndef __sfml_graphics_hpp__
#define __sfml_graphics_hpp__

#include <string>
#include "System.hpp"

namespace sf
{
    enum Style
    {
        None = 0,
        Default = 1
    };

    class VideoMode
    {
        public:
            VideoMode(const int x, const int y)
            {

            }

            static VideoMode getDesktopMode()
            {
                return VideoMode(640, 480);
            }
    };

    class View
    {
        public:
            void setSize(Vector2f size)
            {

            }

            void setCenter(Vector2f center)
            {

            }
    };

    class Image
    {
        private:
            int8_t *pixels;

        public:
            Image()
            {
                pixels = new int8_t[4];
            }

            void loadFromFile(std::string filename)
            {
                
            }

            const int8_t* getPixelsPtr() const
            {
                return pixels;
            }

            ~Image()
            {
                delete pixels;
            }
    };

    class Texture
    {
        public:
            Texture()
            {

            }
    };

    class Font
    {
        public:
            Font()
            {

            }
    };

    class RenderWindow
    {
        private:


        public:
            RenderWindow()
            {

            }

            void create(VideoMode videoMode, std::string title, Style style)
            {

            }

            void setView(View view)
            {

            }

            void setFramerateLimit(int limit)
            {

            }

            void setIcon(const int width, const int height, const int8_t *pixels)
            {

            }

            bool isOpen()
            {
                return true;
            }

            void clear()
            {

            }

            void display()
            {

            }

            void close()
            {

            }

            static View getDefaultView()
            {
                return View();
            }
    };
}

#endif
#ifndef __sfml_graphics_hpp__
#define __sfml_graphics_hpp__

namespace sf
{
    enum Style
    {
        None = 0,
        Default = 1
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

            static View getDefaultView()
            {
                return View();
            }
    };

    class VideoMode
    {
        public:
            VideoMode(int x, int y)
            {

            }

            static VideoMode getDesktopMode()
            {
                return VideoMode();
            }
    };

    class View
    {

    };
}

#endif
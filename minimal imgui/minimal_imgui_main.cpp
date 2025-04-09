#include "minimal_imgui_pge_backend.h"

#include "olcPixelGameEngine.h"

class Example : public olc::PixelGameEngine
{
public:
    Example()
    {
        sAppName = "Example";
    }

public:
    bool OnUserCreate() override
    {
        Context* ctx = get_current_context();
        if (auto root_guard = Panel("root"))
        {
            if (auto child_guard = Panel("child_panel"))
            {
                if (auto btn1_guard = Button("button1"))
                {
                    std::cout << "Button1 clicked!\n";
                }
                if (auto btn2_guard = Button("button2"))
                {
                    std::cout << "Button2 clicked!\n";
                }
                if (auto btn3_guard = Button("button3"))
                {
                    std::cout << "Button2 clicked!\n";
                }
            }
            if (auto root_btn_guard = Button("root_button"))
            {
                std::cout << "Root button clicked!\n";
            }
            if (auto root_btn_guard2 = Button("root_button_2"))
            {
                std::cout << "Root button clicked!\n";
            }
        }

        std::cout << "\nRendering hierarchy:\n";
        iterate_widgets(ctx, draw_widget, this);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        return true;
    }
};


int main()
{
    Example demo;
    if (demo.Construct(256, 240, 4, 4))
        demo.Start();

    return 0;
}
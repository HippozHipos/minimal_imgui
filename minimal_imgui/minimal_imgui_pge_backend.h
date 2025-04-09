#pragma once

#ifndef OLC_PGE_APPLICATION
    #define OLC_PGE_APPLICATION
#endif

#include "olcPixelGameEngine.h"
#include "minimal_imgui.h"

void draw_widget(Context* ctx, const WidgetObject& wb, olc::PixelGameEngine* pge)
{
    std::string type = (wb.uitype == WidgetObject::Panel) ? "Panel" : "Button";
    std::cout << type << " '" << wb.relative_name << "' at ("
        << wb.position.x << ", " << wb.position.y << "), size ("
        << wb.size.x << ", " << wb.size.y << ")\n";
    pge->DrawRect(wb.position.x, wb.position.y, wb.size.x, wb.size.y);
    pge->DrawString(wb.position.x, wb.position.y, wb.name);
}

void process_widget_and_children(Context* ctx, const WidgetObject& wb, 
    void (*action)(Context*, const WidgetObject&, olc::PixelGameEngine*), olc::PixelGameEngine* pge)
{
    action(ctx, wb, pge);
    for (WidgetObject* child : wb.stack)
    {
        process_widget_and_children(ctx, *child, action, pge);
    }
}

void iterate_widgets(Context* ctx, void (*action)(Context* , const WidgetObject&, olc::PixelGameEngine*), olc::PixelGameEngine* pge)
{
    for (const auto& [key, wb] : ctx->widget_lookup)
    {
        if (key.find("->") == std::string::npos)
        {
            process_widget_and_children(ctx, wb, action, pge);
        }
    }
}
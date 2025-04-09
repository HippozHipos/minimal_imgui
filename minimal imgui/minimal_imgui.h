#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>

struct Vec2
{
    float x, y;
    Vec2 operator+(Vec2 other) { return Vec2{ x + other.x, y + other.y }; }
};

struct LayoutPolicyPack
{
    float paddingx = 2, paddingy = 2;
    float vertical_spacing = 5, horizontal_spacing = 5;
};

struct WidgetObject;

struct Context
{
    std::unordered_map<std::string, WidgetObject> widget_lookup;
    std::stack<std::string> parent_stack;
    bool same_line = false;
};

Context* get_current_context()
{
    static Context ctx;
    return &ctx;
}

struct WidgetObject
{
    std::string name;
    std::string relative_name;
    Vec2 position;
    Vec2 size;
    bool active = true;
    Vec2 cursor;
    std::vector<WidgetObject*> stack;
    LayoutPolicyPack layout_policy;
    enum UIType { Panel, Button } uitype;
    Context* context;
    bool (*execute_if)(Context*, WidgetObject*);
};

bool execute_if_always(Context*, WidgetObject*) { return true; }
//button2 ends up at y=29 so simulating pressing button2
bool execute_if_clicked(Context*, WidgetObject* wb) { return wb->position.y == 29; }

struct ScopeGuard
{
    Context* ctx;
    bool should_execute;
    ScopeGuard(Context* c, bool exec) : ctx(c), should_execute(exec)
    {
        if (!should_execute && ctx)
        {
            ctx->parent_stack.pop();
        }
    }
    ~ScopeGuard()
    {
        if (should_execute && ctx)
        {
            std::cout << "Guard destroyed for: " << ctx->parent_stack.top() << "\n";
            ctx->parent_stack.pop();
        }
    }
    operator bool() const { return should_execute; }
};

ScopeGuard Panel(std::string name, LayoutPolicyPack layout = LayoutPolicyPack())
{
    Context* ctx = get_current_context();
    std::string key = ctx->parent_stack.empty() ? name : ctx->parent_stack.top() + "->" + name;
    WidgetObject& wb = ctx->widget_lookup[key];

    wb.name = name;
    wb.relative_name = key;
    wb.uitype = WidgetObject::Panel;
    wb.layout_policy = layout;
    wb.cursor = { layout.paddingx, layout.paddingy };
    wb.stack.clear();
    wb.size = { 100, 100 };
    wb.context = ctx;
    wb.execute_if = execute_if_always;

    if (!ctx->parent_stack.empty())
    {
        WidgetObject& parent = ctx->widget_lookup[ctx->parent_stack.top()];
        wb.position = parent.position + parent.cursor;
        parent.stack.push_back(&wb);
        if (ctx->same_line)
        {
            parent.cursor.x += wb.size.x + parent.layout_policy.horizontal_spacing;
            ctx->same_line = false;
        }
        else
        {
            parent.cursor.y += wb.size.y + parent.layout_policy.vertical_spacing;
        }
    }
    else
    {
        wb.position = { 0, 0 };
    }

    ctx->parent_stack.push(wb.relative_name);
    bool executed = wb.execute_if(ctx, &wb);
    return ScopeGuard(ctx, executed);
}

ScopeGuard Button(std::string name, LayoutPolicyPack layout = LayoutPolicyPack())
{
    Context* ctx = get_current_context();
    std::string key = ctx->parent_stack.empty() ? name : ctx->parent_stack.top() + "->" + name;
    WidgetObject& wb = ctx->widget_lookup[key];

    wb.name = name;
    wb.relative_name = key;
    wb.uitype = WidgetObject::Button;
    wb.size = { 80, 20 };
    wb.context = ctx;
    wb.execute_if = execute_if_clicked;

    if (!ctx->parent_stack.empty())
    {
        WidgetObject& parent = ctx->widget_lookup[ctx->parent_stack.top()];
        wb.position = parent.position + parent.cursor;
        parent.stack.push_back(&wb);
        if (ctx->same_line)
        {
            parent.cursor.x += wb.size.x + parent.layout_policy.horizontal_spacing;
            ctx->same_line = false;
        }
        else
        {
            parent.cursor.y += wb.size.y + parent.layout_policy.vertical_spacing;
        }
    }
    else
    {
        wb.position = { layout.paddingx, layout.paddingy };
    }

    ctx->parent_stack.push(wb.relative_name);
    bool executed = wb.execute_if(ctx, &wb);
    return ScopeGuard(ctx, executed);
}


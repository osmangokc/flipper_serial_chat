#include "../serial_chat.h"

void serial_chat_scene_about_on_enter(void* context) {
    SerialChatApp* app = context;
    Widget* widget = app->widget;
    
    widget_add_text_box_element(
        widget,
        0,
        0,
        128,
        14,
        AlignCenter,
        AlignCenter,
        "\e#Serial Chat\e#",
        false);
    
    widget_add_text_scroll_element(
        widget,
        0,
        16,
        128,
        48,
        "Version: 1.0\n"
        "\n"
        "Developer:\n"
        "Osman Gokce\n"
        "\n"
        "GitHub:\n"
        "github.com/osmangokc\n"
        "\n"
        "UART chat application\n"
        "for Flipper Zero\n"
        "\n"
        "Press Back to return");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, SerialChatViewAbout);
}

bool serial_chat_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void serial_chat_scene_about_on_exit(void* context) {
    SerialChatApp* app = context;
    widget_reset(app->widget);
}

#include "../serial_chat.h"

static void text_input_callback(void* context) {
    SerialChatApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, SerialChatCustomEventTextEntered);
}

void serial_chat_scene_text_input_on_enter(void* context) {
    SerialChatApp* app = context;
    TextInput* text_input = app->text_input;
    
    text_input_set_header_text(text_input, "Enter message:");
    text_input_set_result_callback(
        text_input,
        text_input_callback,
        app,
        app->text_input_buffer,
        TEXT_INPUT_SIZE,
        true);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, SerialChatViewTextInput);
}

bool serial_chat_scene_text_input_on_event(void* context, SceneManagerEvent event) {
    SerialChatApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == SerialChatCustomEventTextEntered) {
            // Message is in text_input_buffer, just go back
            // Chat scene will handle it via search_and_switch mechanism
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, SerialChatSceneChat);
            consumed = true;
        }
    }
    
    return consumed;
}

void serial_chat_scene_text_input_on_exit(void* context) {
    SerialChatApp* app = context;
    text_input_reset(app->text_input);
}

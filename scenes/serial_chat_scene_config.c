#include "serial_chat.h"

void (* const serial_chat_scene_on_enter_handlers[])(void*) = {
    serial_chat_scene_menu_on_enter,
    serial_chat_scene_chat_on_enter,
    serial_chat_scene_settings_on_enter,
    serial_chat_scene_text_input_on_enter,
    serial_chat_scene_about_on_enter,
};

bool (* const serial_chat_scene_on_event_handlers[])(void*, SceneManagerEvent) = {
    serial_chat_scene_menu_on_event,
    serial_chat_scene_chat_on_event,
    serial_chat_scene_settings_on_event,
    serial_chat_scene_text_input_on_event,
    serial_chat_scene_about_on_event,
};

void (* const serial_chat_scene_on_exit_handlers[])(void*) = {
    serial_chat_scene_menu_on_exit,
    serial_chat_scene_chat_on_exit,
    serial_chat_scene_settings_on_exit,
    serial_chat_scene_text_input_on_exit,
    serial_chat_scene_about_on_exit,
};

const SceneManagerHandlers serial_chat_scene_handlers = {
    .on_enter_handlers = serial_chat_scene_on_enter_handlers,
    .on_event_handlers = serial_chat_scene_on_event_handlers,
    .on_exit_handlers = serial_chat_scene_on_exit_handlers,
    .scene_num = SerialChatSceneNum,
};

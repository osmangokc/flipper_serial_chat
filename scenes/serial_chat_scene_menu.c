#include "../serial_chat.h"

void serial_chat_scene_menu_submenu_callback(void* context, uint32_t index) {
    SerialChatApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void serial_chat_scene_menu_on_enter(void* context) {
    SerialChatApp* app = context;
    Submenu* submenu = app->submenu;
    
    submenu_add_item(
        submenu,
        "Start Chat",
        SubmenuIndexChat,
        serial_chat_scene_menu_submenu_callback,
        app);
    
    submenu_add_item(
        submenu,
        "Settings",
        SubmenuIndexSettings,
        serial_chat_scene_menu_submenu_callback,
        app);
    
    submenu_add_item(
        submenu,
        "About",
        SubmenuIndexAbout,
        serial_chat_scene_menu_submenu_callback,
        app);
    
    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, SerialChatSceneMenu));
    
    view_dispatcher_switch_to_view(app->view_dispatcher, SerialChatViewSubmenu);
}

bool serial_chat_scene_menu_on_event(void* context, SceneManagerEvent event) {
    SerialChatApp* app = context;
    
    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, SerialChatSceneMenu, event.event);
        
        if(event.event == SubmenuIndexChat) {
            scene_manager_next_scene(app->scene_manager, SerialChatSceneChat);
            return true;
        } else if(event.event == SubmenuIndexSettings) {
            scene_manager_next_scene(app->scene_manager, SerialChatSceneSettings);
            return true;
        } else if(event.event == SubmenuIndexAbout) {
            scene_manager_next_scene(app->scene_manager, SerialChatSceneAbout);
            return true;
        }
    }
    
    return false;
}

void serial_chat_scene_menu_on_exit(void* context) {
    SerialChatApp* app = context;
    submenu_reset(app->submenu);
}

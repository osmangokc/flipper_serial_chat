#include "serial_chat.h"

static bool serial_chat_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    SerialChatApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool serial_chat_back_event_callback(void* context) {
    furi_assert(context);
    SerialChatApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static SerialChatApp* serial_chat_app_alloc() {
    SerialChatApp* app = malloc(sizeof(SerialChatApp));
    
    // Load config from storage
    serial_chat_load_config(app);
    
    // Initialize message buffer
    app->message_count = 0;
    app->message_scroll_offset = 0;
    memset(app->messages, 0, sizeof(app->messages));
    memset(app->text_input_buffer, 0, TEXT_INPUT_SIZE);
    
    // Initialize RX line buffer
    app->rx_line_pos = 0;
    memset(app->rx_line_buffer, 0, MESSAGE_MAX_LEN);
    
    // Create RX stream buffer
    app->rx_stream = furi_stream_buffer_alloc(512, 1);
    app->rx_timer = NULL;
    
    // GUI
    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    
    // Scene Manager - View Dispatcher'dan önce oluşturulmalı
    app->scene_manager = scene_manager_alloc(&serial_chat_scene_handlers, app);
    
    // View Dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(
        app->view_dispatcher, 
        serial_chat_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(
        app->view_dispatcher, 
        serial_chat_back_event_callback);
    view_dispatcher_attach_to_gui(
        app->view_dispatcher, 
        app->gui, 
        ViewDispatcherTypeFullscreen);
    
    // Submenu
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher, 
        SerialChatViewSubmenu, 
        submenu_get_view(app->submenu));
    
    // Variable Item List (Settings)
    app->variable_item_list = variable_item_list_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        SerialChatViewSettings,
        variable_item_list_get_view(app->variable_item_list));
    
    // Text Input
    app->text_input = text_input_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        SerialChatViewTextInput,
        text_input_get_view(app->text_input));
    
    // Widget (About)
    app->widget = widget_alloc();
    view_dispatcher_add_view(
        app->view_dispatcher,
        SerialChatViewAbout,
        widget_get_view(app->widget));
    
    // Chat View
    serial_chat_view_alloc(app);
    view_dispatcher_add_view(
        app->view_dispatcher,
        SerialChatViewChat,
        serial_chat_view_get(app));
    
    return app;
}

static void serial_chat_app_free(SerialChatApp* app) {
    furi_assert(app);
    
    // Deinit UART if active
    serial_chat_uart_deinit(app);
    
    // Stop and free timer if active
    if(app->rx_timer) {
        furi_timer_stop(app->rx_timer);
        furi_timer_free(app->rx_timer);
    }
    
    // Free stream buffer
    furi_stream_buffer_free(app->rx_stream);
    
    // Views
    view_dispatcher_remove_view(app->view_dispatcher, SerialChatViewSubmenu);
    submenu_free(app->submenu);
    
    view_dispatcher_remove_view(app->view_dispatcher, SerialChatViewSettings);
    variable_item_list_free(app->variable_item_list);
    
    view_dispatcher_remove_view(app->view_dispatcher, SerialChatViewTextInput);
    text_input_free(app->text_input);
    
    view_dispatcher_remove_view(app->view_dispatcher, SerialChatViewAbout);
    widget_free(app->widget);
    
    view_dispatcher_remove_view(app->view_dispatcher, SerialChatViewChat);
    serial_chat_view_free(app);
    
    // View Dispatcher
    view_dispatcher_free(app->view_dispatcher);
    
    // Scene manager
    scene_manager_free(app->scene_manager);
    
    // Records
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_GUI);
    
    free(app);
}

int32_t serial_chat_app(void* p) {
    UNUSED(p);
    
    SerialChatApp* app = serial_chat_app_alloc();
    
    scene_manager_next_scene(app->scene_manager, SerialChatSceneMenu);
    
    view_dispatcher_run(app->view_dispatcher);
    
    serial_chat_app_free(app);
    
    return 0;
}

#include "../serial_chat.h"

typedef struct {
    SerialChatApp* app;
} ChatViewModel;

static void rx_timer_callback(void* context) {
    SerialChatApp* app = context;
    
    uint8_t data;
    bool updated = false;
    
    // Process all available data
    while(furi_stream_buffer_receive(app->rx_stream, &data, 1, 0) > 0) {
        if(data == '\n' || data == '\r') {
            if(app->rx_line_pos > 0) {
                app->rx_line_buffer[app->rx_line_pos] = '\0';
                serial_chat_add_message(app, app->rx_line_buffer, false);
                app->rx_line_pos = 0;
                updated = true;
            }
        } else if(app->rx_line_pos < MESSAGE_MAX_LEN - 1) {
            app->rx_line_buffer[app->rx_line_pos++] = data;
        }
    }
    
    // Update view if we got new messages
    if(updated) {
        with_view_model(
            app->chat_view,
            ChatViewModel * model,
            { UNUSED(model); },
            true);
    }
}

void serial_chat_scene_chat_on_enter(void* context) {
    SerialChatApp* app = context;
    
    // Initialize RX line buffer
    app->rx_line_pos = 0;
    memset(app->rx_line_buffer, 0, MESSAGE_MAX_LEN);
    
    // Initialize UART if not already done
    if(!app->serial_handle) {
        serial_chat_uart_init(app);
    }
    
    // Check if we're returning from text input with a message
    if(strlen(app->text_input_buffer) > 0) {
        // Send the message
        serial_chat_uart_send(app, app->text_input_buffer);
        serial_chat_add_message(app, app->text_input_buffer, true);
        
        // Clear buffer
        memset(app->text_input_buffer, 0, TEXT_INPUT_SIZE);
        
        // Update view
        with_view_model(
            app->chat_view,
            ChatViewModel * model,
            { UNUSED(model); },
            true);
    }
    
    // Start RX processing timer (check every 100ms)
    app->rx_timer = furi_timer_alloc(rx_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->rx_timer, 100);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, SerialChatViewChat);
}

bool serial_chat_scene_chat_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    // Timer handles RX processing now
    return false;
}

void serial_chat_scene_chat_on_exit(void* context) {
    SerialChatApp* app = context;
    
    // Stop RX timer
    if(app->rx_timer) {
        furi_timer_stop(app->rx_timer);
        furi_timer_free(app->rx_timer);
        app->rx_timer = NULL;
    }
    
    // Deinitialize UART
    serial_chat_uart_deinit(app);
}

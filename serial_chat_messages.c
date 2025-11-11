#include "serial_chat.h"

void serial_chat_add_message(SerialChatApp* app, const char* text, bool is_sent) {
    if(!text || strlen(text) == 0) return;
    
    // Shift messages if buffer is full
    if(app->message_count >= MESSAGE_BUFFER_SIZE) {
        for(int i = 0; i < MESSAGE_BUFFER_SIZE - 1; i++) {
            app->messages[i] = app->messages[i + 1];
        }
        app->message_count = MESSAGE_BUFFER_SIZE - 1;
    }
    
    // Add new message
    ChatMessage* msg = &app->messages[app->message_count];
    strncpy(msg->text, text, MESSAGE_MAX_LEN - 1);
    msg->text[MESSAGE_MAX_LEN - 1] = '\0';
    msg->is_sent = is_sent;
    
    app->message_count++;
    
    // Auto-scroll to bottom
    if(app->message_count > CHAT_MAX_VISIBLE_LINES) {
        app->message_scroll_offset = app->message_count - CHAT_MAX_VISIBLE_LINES;
    }
}

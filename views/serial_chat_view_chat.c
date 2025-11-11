#include "../serial_chat.h"
#include <gui/elements.h>

#define CHAT_LINE_HEIGHT 10
#define CHAT_VIEW_HEIGHT 53
#define CHAT_MAX_VISIBLE_LINES 5

typedef struct {
    SerialChatApp* app;
} ChatViewModel;

static void chat_view_draw_callback(Canvas* canvas, void* _model) {
    furi_assert(_model);
    ChatViewModel* model = _model;
    SerialChatApp* app = model->app;
    
    canvas_clear(canvas);
    canvas_set_font(canvas, FontSecondary);
    
    // Draw messages
    int y_offset = 0;
    int visible_count = 0;
    
    if(app->message_count > 0) {
        // Calculate which messages to show
        int start_index = 0;
        if(app->message_count > CHAT_MAX_VISIBLE_LINES) {
            start_index = app->message_count - CHAT_MAX_VISIBLE_LINES + app->message_scroll_offset;
            if(start_index < 0) start_index = 0;
            if(start_index > app->message_count - CHAT_MAX_VISIBLE_LINES) {
                start_index = app->message_count - CHAT_MAX_VISIBLE_LINES;
            }
        }
        
        for(int i = start_index; i < app->message_count && visible_count < CHAT_MAX_VISIBLE_LINES; i++) {
            ChatMessage* msg = &app->messages[i];
            
            // Draw arrow indicating direction
            if(msg->is_sent) {
                canvas_draw_str(canvas, 0, y_offset + 8, ">");
                canvas_draw_str(canvas, 8, y_offset + 8, msg->text);
            } else {
                canvas_draw_str(canvas, 0, y_offset + 8, "<");
                canvas_draw_str(canvas, 8, y_offset + 8, msg->text);
            }
            
            y_offset += CHAT_LINE_HEIGHT;
            visible_count++;
        }
    } else {
        canvas_draw_str(canvas, 10, 20, "No messages yet");
    }
    
    // Draw separator line
    canvas_draw_line(canvas, 0, CHAT_VIEW_HEIGHT, 127, CHAT_VIEW_HEIGHT);
    
    // Draw instructions
    canvas_draw_str(canvas, 2, 62, "OK:Type Back:Menu");
}

static bool chat_view_input_callback(InputEvent* event, void* context) {
    SerialChatApp* app = context;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyOk) {
            // Open text input
            scene_manager_next_scene(app->scene_manager, SerialChatSceneTextInput);
            return true;
        } else if(event->key == InputKeyBack) {
            // Return to menu
            scene_manager_search_and_switch_to_previous_scene(
                app->scene_manager, SerialChatSceneMenu);
            return true;
        } else if(event->key == InputKeyUp) {
            // Scroll up
            if(app->message_scroll_offset > 0) {
                app->message_scroll_offset--;
                with_view_model(
                    app->chat_view,
                    ChatViewModel * model,
                    { UNUSED(model); },
                    true);
            }
            return true;
        } else if(event->key == InputKeyDown) {
            // Scroll down
            if(app->message_count > CHAT_MAX_VISIBLE_LINES) {
                if(app->message_scroll_offset < app->message_count - CHAT_MAX_VISIBLE_LINES) {
                    app->message_scroll_offset++;
                    with_view_model(
                        app->chat_view,
                        ChatViewModel * model,
                        { UNUSED(model); },
                        true);
                }
            }
            return true;
        }
    }
    
    return false;
}

View* serial_chat_view_get(SerialChatApp* app) {
    return app->chat_view;
}

void serial_chat_view_alloc(SerialChatApp* app) {
    app->chat_view = view_alloc();
    view_allocate_model(app->chat_view, ViewModelTypeLocking, sizeof(ChatViewModel));
    
    with_view_model(
        app->chat_view,
        ChatViewModel * model,
        { model->app = app; },
        false);
    
    view_set_context(app->chat_view, app);
    view_set_draw_callback(app->chat_view, chat_view_draw_callback);
    view_set_input_callback(app->chat_view, chat_view_input_callback);
}

void serial_chat_view_free(SerialChatApp* app) {
    view_free(app->chat_view);
}

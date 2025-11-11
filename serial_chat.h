#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_serial.h>
#include <furi_hal_serial_control.h>
#include <furi_hal_serial_types.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/variable_item_list.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <toolbox/stream/stream.h>
#include <toolbox/stream/buffered_file_stream.h>
#include <storage/storage.h>

#define UART_CH (FuriHalSerialIdUsart)
#define SERIAL_CHAT_CONFIG_PATH APP_DATA_PATH("serial_chat.conf")
#define TEXT_INPUT_SIZE 128
#define MESSAGE_BUFFER_SIZE 32
#define MESSAGE_MAX_LEN 256

typedef enum {
    SerialChatViewSubmenu,
    SerialChatViewChat,
    SerialChatViewSettings,
    SerialChatViewTextInput,
    SerialChatViewAbout,
} SerialChatView;

typedef enum {
    SerialChatSceneMenu,
    SerialChatSceneChat,
    SerialChatSceneSettings,
    SerialChatSceneTextInput,
    SerialChatSceneAbout,
    SerialChatSceneNum,
} SerialChatScene;

typedef enum {
    SerialChatCustomEventNone,
    SerialChatCustomEventStartChat,
    SerialChatCustomEventSettings,
    SerialChatCustomEventTextEntered,
    SerialChatCustomEventRxData,
} SerialChatCustomEvent;

typedef struct {
    uint32_t baudrate;
    FuriHalSerialDataBits data_bits;
    FuriHalSerialParity parity;
    FuriHalSerialStopBits stop_bits;
} SerialChatConfig;

typedef struct {
    char text[MESSAGE_MAX_LEN];
    bool is_sent; // true = sent, false = received
} ChatMessage;

typedef struct {
    Gui* gui;
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    Submenu* submenu;
    VariableItemList* variable_item_list;
    TextInput* text_input;
    View* chat_view;
    Widget* widget;
    NotificationApp* notifications;
    
    SerialChatConfig config;
    FuriHalSerialHandle* serial_handle;
    FuriStreamBuffer* rx_stream;
    FuriTimer* rx_timer;
    
    ChatMessage messages[MESSAGE_BUFFER_SIZE];
    uint8_t message_count;
    uint8_t message_scroll_offset;
    
    char text_input_buffer[TEXT_INPUT_SIZE];
    char rx_line_buffer[MESSAGE_MAX_LEN];
    size_t rx_line_pos;
} SerialChatApp;

typedef enum {
    SubmenuIndexChat,
    SubmenuIndexSettings,
    SubmenuIndexAbout,
} SubmenuIndex;

// Scene handlers
void serial_chat_scene_menu_on_enter(void* context);
bool serial_chat_scene_menu_on_event(void* context, SceneManagerEvent event);
void serial_chat_scene_menu_on_exit(void* context);

void serial_chat_scene_chat_on_enter(void* context);
bool serial_chat_scene_chat_on_event(void* context, SceneManagerEvent event);
void serial_chat_scene_chat_on_exit(void* context);

void serial_chat_scene_settings_on_enter(void* context);
bool serial_chat_scene_settings_on_event(void* context, SceneManagerEvent event);
void serial_chat_scene_settings_on_exit(void* context);

void serial_chat_scene_text_input_on_enter(void* context);
bool serial_chat_scene_text_input_on_event(void* context, SceneManagerEvent event);
void serial_chat_scene_text_input_on_exit(void* context);

void serial_chat_scene_about_on_enter(void* context);
bool serial_chat_scene_about_on_event(void* context, SceneManagerEvent event);
void serial_chat_scene_about_on_exit(void* context);

// Scene handlers array
extern const SceneManagerHandlers serial_chat_scene_handlers;

// Scene on_enter handlers
extern void (* const serial_chat_scene_on_enter_handlers[])(void*);

// Scene on_event handlers  
extern bool (* const serial_chat_scene_on_event_handlers[])(void*, SceneManagerEvent);

// Scene on_exit handlers
extern void (* const serial_chat_scene_on_exit_handlers[])(void*);

// View get methods
View* serial_chat_view_get(SerialChatApp* app);
void serial_chat_view_alloc(SerialChatApp* app);
void serial_chat_view_free(SerialChatApp* app);

#define CHAT_MAX_VISIBLE_LINES 5

// UART functions
void serial_chat_uart_init(SerialChatApp* app);
void serial_chat_uart_deinit(SerialChatApp* app);
void serial_chat_uart_send(SerialChatApp* app, const char* text);
void serial_chat_uart_rx_callback(
    FuriHalSerialHandle* handle,
    FuriHalSerialRxEvent event,
    void* context);

// Message handling
void serial_chat_add_message(SerialChatApp* app, const char* text, bool is_sent);

// Config handling
void serial_chat_save_config(SerialChatApp* app);
void serial_chat_load_config(SerialChatApp* app);

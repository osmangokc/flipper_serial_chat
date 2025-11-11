#include "../serial_chat.h"
#include <furi.h>
#include <gui/elements.h>

static const char* const baudrate_values[] = {
    "9600",
    "19200",
    "38400",
    "57600",
    "115200",
    "230400",
};

static const uint32_t baudrate_values_num[] = {
    9600,
    19200,
    38400,
    57600,
    115200,
    230400,
};

static const char* const data_width_values[] = {
    "8 bits",
    "9 bits",
};

static const char* const parity_values[] = {
    "None",
    "Even",
    "Odd",
};

static const char* const stop_bits_values[] = {
    "1 bit",
    "2 bits",
};

static void serial_chat_scene_settings_baudrate_change(VariableItem* item) {
    SerialChatApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, baudrate_values[index]);
    app->config.baudrate = baudrate_values_num[index];
}

static void serial_chat_scene_settings_data_width_change(VariableItem* item) {
    SerialChatApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, data_width_values[index]);
    app->config.data_bits = (index == 0) ? FuriHalSerialDataBits8 : FuriHalSerialDataBits9;
}

static void serial_chat_scene_settings_parity_change(VariableItem* item) {
    SerialChatApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, parity_values[index]);
    
    if(index == 0) {
        app->config.parity = FuriHalSerialParityNone;
    } else if(index == 1) {
        app->config.parity = FuriHalSerialParityEven;
    } else {
        app->config.parity = FuriHalSerialParityOdd;
    }
}

static void serial_chat_scene_settings_stop_bits_change(VariableItem* item) {
    SerialChatApp* app = variable_item_get_context(item);
    uint8_t index = variable_item_get_current_value_index(item);
    variable_item_set_current_value_text(item, stop_bits_values[index]);
    app->config.stop_bits = (index == 0) ? FuriHalSerialStopBits1 : FuriHalSerialStopBits2;
}

void serial_chat_scene_settings_on_enter(void* context) {
    SerialChatApp* app = context;
    VariableItemList* variable_item_list = app->variable_item_list;
    VariableItem* item;
    
    // Baudrate
    item = variable_item_list_add(
        variable_item_list,
        "Baudrate",
        COUNT_OF(baudrate_values),
        serial_chat_scene_settings_baudrate_change,
        app);
    
    uint8_t baudrate_index = 4; // Default 115200
    for(uint8_t i = 0; i < COUNT_OF(baudrate_values_num); i++) {
        if(baudrate_values_num[i] == app->config.baudrate) {
            baudrate_index = i;
            break;
        }
    }
    variable_item_set_current_value_index(item, baudrate_index);
    variable_item_set_current_value_text(item, baudrate_values[baudrate_index]);
    
    // Data Width
    item = variable_item_list_add(
        variable_item_list,
        "Data Width",
        COUNT_OF(data_width_values),
        serial_chat_scene_settings_data_width_change,
        app);
    uint8_t data_width_index = (app->config.data_bits == FuriHalSerialDataBits8) ? 0 : 1;
    variable_item_set_current_value_index(item, data_width_index);
    variable_item_set_current_value_text(item, data_width_values[data_width_index]);
    
    // Parity
    item = variable_item_list_add(
        variable_item_list,
        "Parity",
        COUNT_OF(parity_values),
        serial_chat_scene_settings_parity_change,
        app);
    uint8_t parity_index = 0;
    if(app->config.parity == FuriHalSerialParityEven) {
        parity_index = 1;
    } else if(app->config.parity == FuriHalSerialParityOdd) {
        parity_index = 2;
    }
    variable_item_set_current_value_index(item, parity_index);
    variable_item_set_current_value_text(item, parity_values[parity_index]);
    
    // Stop Bits
    item = variable_item_list_add(
        variable_item_list,
        "Stop Bits",
        COUNT_OF(stop_bits_values),
        serial_chat_scene_settings_stop_bits_change,
        app);
    uint8_t stop_bits_index = (app->config.stop_bits == FuriHalSerialStopBits1) ? 0 : 1;
    variable_item_set_current_value_index(item, stop_bits_index);
    variable_item_set_current_value_text(item, stop_bits_values[stop_bits_index]);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, SerialChatViewSettings);
}

bool serial_chat_scene_settings_on_event(void* context, SceneManagerEvent event) {
    SerialChatApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void serial_chat_scene_settings_on_exit(void* context) {
    SerialChatApp* app = context;
    
    // Save config when leaving settings
    serial_chat_save_config(app);
    
    // Reinitialize UART with new settings if it's already initialized
    if(app->serial_handle) {
        serial_chat_uart_deinit(app);
        serial_chat_uart_init(app);
    }
    
    variable_item_list_reset(app->variable_item_list);
}

#include "serial_chat.h"
#include <storage/storage.h>

#define CONFIG_VERSION 1

void serial_chat_save_config(SerialChatApp* app) {
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);
    
    if(storage_file_open(file, SERIAL_CHAT_CONFIG_PATH, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        uint8_t version = CONFIG_VERSION;
        storage_file_write(file, &version, sizeof(version));
        storage_file_write(file, &app->config.baudrate, sizeof(app->config.baudrate));
        storage_file_write(file, &app->config.data_bits, sizeof(app->config.data_bits));
        storage_file_write(file, &app->config.parity, sizeof(app->config.parity));
        storage_file_write(file, &app->config.stop_bits, sizeof(app->config.stop_bits));
        storage_file_close(file);
    }
    
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

void serial_chat_load_config(SerialChatApp* app) {
    // Set defaults
    app->config.baudrate = 115200;
    app->config.data_bits = FuriHalSerialDataBits8;
    app->config.parity = FuriHalSerialParityNone;
    app->config.stop_bits = FuriHalSerialStopBits1;
    
    Storage* storage = furi_record_open(RECORD_STORAGE);
    File* file = storage_file_alloc(storage);
    
    if(storage_file_open(file, SERIAL_CHAT_CONFIG_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        uint8_t version = 0;
        if(storage_file_read(file, &version, sizeof(version)) == sizeof(version) &&
           version == CONFIG_VERSION) {
            storage_file_read(file, &app->config.baudrate, sizeof(app->config.baudrate));
            storage_file_read(file, &app->config.data_bits, sizeof(app->config.data_bits));
            storage_file_read(file, &app->config.parity, sizeof(app->config.parity));
            storage_file_read(file, &app->config.stop_bits, sizeof(app->config.stop_bits));
        }
        storage_file_close(file);
    }
    
    storage_file_free(file);
    furi_record_close(RECORD_STORAGE);
}

#include "serial_chat.h"

void serial_chat_uart_rx_callback(
    FuriHalSerialHandle* handle,
    FuriHalSerialRxEvent event,
    void* context) {
    SerialChatApp* app = (SerialChatApp*)context;
    
    if(event == FuriHalSerialRxEventData) {
        uint8_t data = furi_hal_serial_async_rx(handle);
        // Just buffer the data, don't send events from ISR
        furi_stream_buffer_send(app->rx_stream, &data, 1, 0);
    }
}

void serial_chat_uart_init(SerialChatApp* app) {
    // If already initialized, deinit first
    if(app->serial_handle) {
        serial_chat_uart_deinit(app);
    }
    
    // Acquire serial handle
    app->serial_handle = furi_hal_serial_control_acquire(UART_CH);
    furi_check(app->serial_handle);
    
    // Initialize UART with current config
    furi_hal_serial_init(app->serial_handle, app->config.baudrate);
    
    // Configure framing (data bits, parity, stop bits)
    furi_hal_serial_configure_framing(
        app->serial_handle,
        app->config.data_bits,
        app->config.parity,
        app->config.stop_bits);
    
    // Enable RX and TX
    furi_hal_serial_async_rx_start(
        app->serial_handle,
        serial_chat_uart_rx_callback,
        app,
        false);
}

void serial_chat_uart_deinit(SerialChatApp* app) {
    if(app->serial_handle) {
        furi_hal_serial_async_rx_stop(app->serial_handle);
        furi_hal_serial_deinit(app->serial_handle);
        furi_hal_serial_control_release(app->serial_handle);
        app->serial_handle = NULL;
    }
}

void serial_chat_uart_send(SerialChatApp* app, const char* text) {
    if(app->serial_handle && text) {
        size_t len = strlen(text);
        furi_hal_serial_tx(app->serial_handle, (uint8_t*)text, len);
        // Send newline
        furi_hal_serial_tx(app->serial_handle, (uint8_t*)"\r\n", 2);
    }
}

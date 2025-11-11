# Serial Chat for Flipper Zero

A full-featured UART serial communication application for Flipper Zero that enables bidirectional text messaging over serial ports.

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-Flipper%20Zero-orange.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)

## Features

- 📨 **Bidirectional Communication**: Send and receive text messages over UART
- ⚙️ **Configurable Serial Settings**: Customize baudrate, data bits, parity, and stop bits
- 💾 **Persistent Configuration**: Settings are saved and restored automatically
- 📜 **Message History**: Scrollable chat view with message buffer (up to 32 messages)
- 🎯 **User-Friendly Interface**: Intuitive navigation and text input
- 🔄 **Real-Time Updates**: Incoming messages appear instantly (100ms polling)

## Supported Serial Configurations

### Baudrate Options
- 9600 bps
- 19200 bps
- 38400 bps
- 57600 bps
- **115200 bps** (default)
- 230400 bps

### Data Bits
- 8 bits (default)
- 9 bits

### Parity
- None (default)
- Even
- Odd

### Stop Bits
- 1 bit (default)
- 2 bits

## Hardware Connections

Connect your external device to Flipper Zero's GPIO pins:

```
Flipper Zero          External Device
━━━━━━━━━━━━          ━━━━━━━━━━━━━━━
GPIO 13 (TX)   ────►  RX
GPIO 14 (RX)   ◄────  TX
GND            ────   GND
3.3V           ────   VCC (if needed)
```

**⚠️ Important**: Ensure voltage levels are compatible (3.3V). Do not connect 5V signals directly!

## Installation

### Method 1: Pre-built FAP (Recommended)

1. Download `serial_chat.fap` from the releases page
2. Copy to SD card: `/ext/apps/GPIO/serial_chat.fap`
3. Navigate to **Applications** → **GPIO** → **Serial Chat** on Flipper Zero

### Method 2: Build from Source

1. Clone the Flipper Zero firmware repository:
```bash
git clone https://github.com/flipperdevices/flipperzero-firmware.git
cd flipperzero-firmware
```

2. Copy this application to `applications_user/serial_chat/`

3. Build the application:
```bash
./fbt fap_serial_chat
```

4. The compiled FAP will be at: `build/f7-firmware-D/.extapps/serial_chat.fap`

5. Install to Flipper:
```bash
./fbt launch_app APPSRC=applications_user/serial_chat
```

Or copy manually to SD card.

## Usage

### Starting a Chat Session

1. Launch **Serial Chat** from **GPIO** category
2. Select **Settings** to configure serial port (optional)
3. Select **Start Chat** to begin communication
4. Press **OK** button to type and send messages
5. Use **↑** and **↓** to scroll through message history
6. Press **Back** to return to menu

### Sending Messages

1. In chat view, press **OK** button
2. Type your message using the text input screen
3. Press **Save** to send
4. Message appears in chat view with `>` prefix

### Receiving Messages

- Incoming messages appear automatically with `<` prefix
- Messages are displayed in real-time (100ms latency)
- Line breaks are detected on `\n` or `\r\n`
- Up to 256 characters per message

## Menu Structure

```
Main Menu
├── Start Chat      → Open chat interface
├── Settings        → Configure serial parameters
└── About           → Application information
```

## Configuration

Settings are automatically saved to: `/ext/apps_data/serial_chat/serial_chat.conf`

The configuration persists between application restarts and firmware updates.

## Technical Details

### Memory Usage
- Stack Size: 8KB
- Message Buffer: 32 messages × 256 bytes
- RX Stream Buffer: 512 bytes

### Architecture
- **Scene-based Navigation**: Clean state management
- **Timer-based RX Processing**: Thread-safe message reception
- **FuriTimer**: 100ms polling for incoming data
- **Stream Buffer**: ISR-safe data buffering

### Dependencies
- Furi HAL Serial API
- GUI Framework
- Storage API

## Troubleshooting

### No Messages Received
- Check wiring (TX ↔ RX crossover)
- Verify baudrate matches remote device
- Ensure GND is connected
- Check data format settings (8N1 is most common)

### Messages Corrupted
- Try different baudrate
- Check parity and stop bit settings
- Verify voltage levels (3.3V)
- Reduce cable length if possible

### Application Crashes
- Reset Flipper Zero
- Check for firmware updates
- Rebuild application from latest source

## Development

### Project Structure
```
applications_user/serial_chat/
├── application.fam              # Application manifest
├── serial_chat.h                # Main header
├── serial_chat.c                # Application entry point
├── serial_chat_uart.c           # UART communication
├── serial_chat_config.c         # Configuration management
├── serial_chat_messages.c       # Message handling
├── scenes/                      # Scene implementations
│   ├── serial_chat_scene_menu.c
│   ├── serial_chat_scene_chat.c
│   ├── serial_chat_scene_settings.c
│   ├── serial_chat_scene_text_input.c
│   ├── serial_chat_scene_about.c
│   └── serial_chat_scene_config.c
└── views/                       # Custom views
    └── serial_chat_view_chat.c
```

### Building

```bash
# Clean build
./fbt -c fap_serial_chat

# Build only
./fbt fap_serial_chat

# Build and launch on connected Flipper
./fbt launch_app APPSRC=applications_user/serial_chat
```

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Use Cases

- 🔧 **Debugging**: Monitor serial output from microcontrollers
- 🤖 **IoT Communication**: Chat with ESP32, Arduino, Raspberry Pi
- 📡 **Radio Modules**: Configure LoRa, RF modules via AT commands
- 🔌 **Hardware Testing**: Test UART devices and protocols
- 🎓 **Education**: Learn serial communication protocols

## Roadmap

- [ ] Hex view mode for binary data
- [ ] Custom line endings configuration
- [ ] Message timestamps
- [ ] Export chat history
- [ ] Hardware flow control (RTS/CTS)
- [ ] Macro/command shortcuts

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Author

**Osman Gokce**
- GitHub: [@osmangokc](https://github.com/osmangokc)

## Acknowledgments

- Flipper Devices team for the amazing platform
- Flipper Zero community for inspiration and support

## Support

If you find this useful, please give it a ⭐️ on GitHub!

For issues and questions, please use the GitHub Issues page.

---

**Disclaimer**: This is a community-developed application and is not officially supported by Flipper Devices. Use at your own risk.

#!/usr/bin/env python3
import struct
import zlib
import binascii

def create_chunk(chunk_type, data):
    length = struct.pack('>I', len(data))
    crc = struct.pack('>I', binascii.crc32(chunk_type + data) & 0xffffffff)
    return length + chunk_type + data + crc

def create_minimal_png():
    # PNG signature
    png_signature = b'\x89PNG\r\n\x1a\n'
    
    # IHDR chunk (10x10, grayscale, 1 bit depth)
    width = 10
    height = 10
    bit_depth = 1
    color_type = 0  # Grayscale
    
    ihdr_data = struct.pack('>IIBBBBB', width, height, bit_depth, color_type, 0, 0, 0)
    ihdr_chunk = create_chunk(b'IHDR', ihdr_data)
    
    # Image data - speech bubble pattern
    # Each row is 2 bytes (10 pixels padded to 16 bits)
    # Black = 1, White = 0
    image_rows = [
        0b0000000000000000,  # Row 0: Empty
        0b0011111100000000,  # Row 1: Top of bubble
        0b0100000010000000,  # Row 2: Left/right sides
        0b0100000010000000,  # Row 3: Left/right sides
        0b0100000010000000,  # Row 4: Left/right sides
        0b0100000010000000,  # Row 5: Left/right sides
        0b0011111100000000,  # Row 6: Bottom of bubble
        0b0000100000000000,  # Row 7: Tail middle
        0b0001000000000000,  # Row 8: Tail tip
        0b0000000000000000,  # Row 9: Empty
    ]
    
    # Create raw image data with filter bytes
    raw_data = b''
    for row in image_rows:
        raw_data += b'\x00'  # Filter type: None
        raw_data += struct.pack('>H', row)
    
    # Compress with zlib
    compressed = zlib.compress(raw_data, 9)
    idat_chunk = create_chunk(b'IDAT', compressed)
    
    # IEND chunk
    iend_chunk = create_chunk(b'IEND', b'')
    
    # Combine all
    png_data = png_signature + ihdr_chunk + idat_chunk + iend_chunk
    
    return png_data

# Create the icon
png_data = create_minimal_png()

# Write to file
with open('icons/serial_chat_10px.png', 'wb') as f:
    f.write(png_data)

print("✓ Created speech bubble icon: icons/serial_chat_10px.png")

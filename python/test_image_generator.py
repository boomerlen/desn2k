#!/usr/bin/env python3
"""
Script to verify the integrity of .iso files produced by image_generator.py
and reconstruct original files to confirm the conversion process is reversible.

This script performs:
1. Header validation - ensures correct file table structure
2. Data integrity checks - verifies all data matches original files
3. Format validation - confirms WAV and BMP format requirements
4. Reconstruction testing - converts ISO data back to original formats
5. Optional file output - saves reconstructed files for manual inspection

Usage examples:
  # Use debug configuration from .vscode/launch.json
  python test_image_generator.py --use-debug-config
  
  # Verify custom ISO file
  python test_image_generator.py --iso file.iso --files input1.wav input2.bmp
  
  # Save reconstructed files for inspection
  python test_image_generator.py --use-debug-config --reconstruct-to output_dir
"""

# This is vibe coded

import argparse
import struct
from pathlib import Path
from PIL import Image
import wave
import tempfile
import os

def read_iso_header(iso_data):
    """Read the header table from the ISO file"""
    if len(iso_data) < 4:
        raise ValueError("ISO file too small to contain valid header")
    
    # Determine number of entries by looking at first address
    first_addr = struct.unpack('<I', iso_data[0:4])[0]
    n_entries = first_addr // 4
    
    if n_entries == 0:
        raise ValueError("No entries found in ISO file")
    
    print(f"Found {n_entries} entries in ISO file")
    
    # Read all addresses from header
    addresses = []
    for i in range(n_entries):
        addr = struct.unpack('<I', iso_data[i*4:(i+1)*4])[0]
        addresses.append(addr)
    
    return addresses

def reconstruct_wav_file(iso_data, start_addr, output_path, original_wav_path=None):
    """Reconstruct original WAV file from ISO data using original file's parameters"""
    print(f"Reconstructing WAV file to: {output_path}")
    
    # Read size and data from ISO
    data_size = struct.unpack('<I', iso_data[start_addr:start_addr+4])[0]
    wav_data = iso_data[start_addr+4:start_addr+4+data_size]
    
    # If we have the original file, read its header parameters
    if original_wav_path:
        with open(original_wav_path, 'rb') as f:
            original_header = f.read(44)  # Read the full WAV header
        
        # Extract key parameters from original header
        # Bytes 22-23: Number of channels
        # Bytes 24-27: Sample rate
        # Bytes 28-31: Byte rate  
        # Bytes 32-33: Block align
        # Bytes 34-35: Bits per sample
        channels = struct.unpack('<H', original_header[22:24])[0]
        sample_rate = struct.unpack('<I', original_header[24:28])[0]
        byte_rate = struct.unpack('<I', original_header[28:32])[0]
        block_align = struct.unpack('<H', original_header[32:34])[0]
        bits_per_sample = struct.unpack('<H', original_header[34:36])[0]
        
        print(f"Original WAV parameters: {sample_rate}Hz, {bits_per_sample}-bit, {channels} channel(s)")
    else:
        # Fallback to assumed values (this shouldn't happen in normal usage)
        channels = 1
        sample_rate = 44100
        bits_per_sample = 16
        byte_rate = sample_rate * channels * (bits_per_sample // 8)
        block_align = channels * (bits_per_sample // 8)
        print("Warning: No original file provided, using default parameters")
    
    # Create WAV header with correct parameters
    wav_header = bytearray()
    
    # RIFF header
    wav_header.extend(b'RIFF')
    wav_header.extend((data_size + 36).to_bytes(4, 'little'))  # File size - 8
    wav_header.extend(b'WAVE')
    
    # fmt chunk
    wav_header.extend(b'fmt ')
    wav_header.extend((16).to_bytes(4, 'little'))  # fmt chunk size
    wav_header.extend((1).to_bytes(2, 'little'))   # PCM format
    wav_header.extend(channels.to_bytes(2, 'little'))   # Number of channels
    wav_header.extend(sample_rate.to_bytes(4, 'little'))  # Sample rate
    wav_header.extend(byte_rate.to_bytes(4, 'little'))  # Byte rate
    wav_header.extend(block_align.to_bytes(2, 'little'))   # Block align
    wav_header.extend(bits_per_sample.to_bytes(2, 'little'))  # Bits per sample
    
    # data chunk
    wav_header.extend(b'data')
    wav_header.extend(data_size.to_bytes(4, 'little'))
    
    # Write reconstructed WAV file
    with open(output_path, 'wb') as f:
        f.write(wav_header)
        f.write(wav_data)
    
    print(f"✓ WAV file reconstructed successfully with correct parameters")

def verify_wav_data(iso_data, start_addr, original_wav_path):
    """Verify WAV data in ISO file against original"""
    print(f"\n--- Verifying WAV data at address {start_addr} ---")
    
    if start_addr >= len(iso_data):
        raise ValueError(f"Start address {start_addr} exceeds ISO file size")
    
    # Read size from first 4 bytes of data chunk
    data_size = struct.unpack('<I', iso_data[start_addr:start_addr+4])[0]
    print(f"WAV data size in ISO: {data_size} bytes")
    
    # Read the audio data
    wav_data = iso_data[start_addr+4:start_addr+4+data_size]
    
    # Verify against original WAV file
    with open(original_wav_path, 'rb') as f:
        original_data = f.read()
    
    # Check WAV header validation
    if len(original_data) < 45:
        raise ValueError("Original WAV file is too short")
    
    if original_data[0:4] != b'RIFF':
        raise ValueError("Original WAV doesn't start with 'RIFF'")
    
    if original_data[8:12] != b'WAVE':
        raise ValueError("Original WAV doesn't contain 'WAVE' format")
    
    if original_data[20:22] != b'\x01\x00':
        raise ValueError("Original WAV is not PCM format")
    
    if original_data[22:24] != b'\x01\x00':
        raise ValueError("Original WAV is not mono")
    
    # Compare the actual audio data (skip 44-byte WAV header)
    original_audio_data = original_data[44:]
    expected_size = len(original_audio_data)
    
    print(f"Expected WAV data size: {expected_size} bytes")
    print(f"Actual WAV data size: {data_size} bytes")
    
    if data_size != expected_size:
        raise ValueError(f"WAV data size mismatch: expected {expected_size}, got {data_size}")
    
    if wav_data != original_audio_data:
        raise ValueError("WAV data content mismatch")
    
    print("✓ WAV data verification passed")
    
    # Reconstruct the WAV file for additional verification
    with tempfile.NamedTemporaryFile(suffix='.wav', delete=False) as temp_wav:
        reconstruct_wav_file(iso_data, start_addr, temp_wav.name, original_wav_path)
        
        # Verify the reconstructed file has the same audio data
        with open(temp_wav.name, 'rb') as f:
            reconstructed_data = f.read()
        
        if reconstructed_data[44:] != original_audio_data:
            os.unlink(temp_wav.name)
            raise ValueError("Reconstructed WAV audio data doesn't match original")
        
        print("✓ WAV reconstruction verification passed")
        os.unlink(temp_wav.name)
    
    return start_addr + 4 + data_size

def reconstruct_bmp_file(iso_data, start_addr, output_path):
    """Reconstruct original BMP file from ISO RGB565 data"""
    print(f"Reconstructing BMP file to: {output_path}")
    
    # Read image header from ISO
    width_bytes = struct.unpack('<I', iso_data[start_addr:start_addr+4])[0]
    total_size_bytes = struct.unpack('<I', iso_data[start_addr+4:start_addr+8])[0]
    
    # Calculate dimensions
    width_pixels = width_bytes // 2
    height_pixels = total_size_bytes // width_bytes
    
    print(f"Reconstructing {width_pixels}x{height_pixels} image")
    
    # Read RGB565 data
    rgb565_data = iso_data[start_addr+8:start_addr+8+total_size_bytes]
    
    # Convert RGB565 back to RGB
    rgb_pixels = []
    for i in range(0, len(rgb565_data), 2):
        # Read RGB565 value (little endian)
        rgb565 = struct.unpack('<H', rgb565_data[i:i+2])[0]
        
        # Extract RGB components
        r5 = (rgb565 >> 11) & 0x1F
        g6 = (rgb565 >> 5) & 0x3F
        b5 = rgb565 & 0x1F
        
        # Convert back to 8-bit values using standard RGB565 expansion
        # Now that image_generator.py uses correct RGB565 conversion, we can use standard expansion
        r8 = (r5 << 3) | (r5 >> 2)  # Expand 5-bit to 8-bit with bit replication
        g8 = (g6 << 2) | (g6 >> 4)  # Expand 6-bit to 8-bit with bit replication
        b8 = (b5 << 3) | (b5 >> 2)  # Expand 5-bit to 8-bit with bit replication
        
        rgb_pixels.append((r8, g8, b8))
    
    # Create PIL Image
    reconstructed_image = Image.new('RGB', (width_pixels, height_pixels))
    reconstructed_image.putdata(rgb_pixels)
    
    # Save as BMP
    reconstructed_image.save(output_path, 'BMP')
    print(f"✓ BMP file reconstructed successfully")

def verify_bmp_data(iso_data, start_addr, original_bmp_path):
    """Verify BMP data in ISO file against original"""
    print(f"\n--- Verifying BMP data at address {start_addr} ---")
    
    if start_addr >= len(iso_data):
        raise ValueError(f"Start address {start_addr} exceeds ISO file size")
    
    # Read image header from ISO
    width_bytes = struct.unpack('<I', iso_data[start_addr:start_addr+4])[0]
    total_size_bytes = struct.unpack('<I', iso_data[start_addr+4:start_addr+8])[0]
    
    print(f"Image width (bytes): {width_bytes}")
    print(f"Total image size (bytes): {total_size_bytes}")
    
    # Calculate expected dimensions
    width_pixels = width_bytes // 2  # 2 bytes per pixel in RGB565
    height_pixels = total_size_bytes // width_bytes
    
    print(f"Image dimensions: {width_pixels}x{height_pixels} pixels")
    
    # Read the image data
    image_data = iso_data[start_addr+8:start_addr+8+total_size_bytes]
    
    # Verify against original BMP
    with Image.open(original_bmp_path) as im:
        expected_width = im.width
        expected_height = im.height
        
        print(f"Expected dimensions: {expected_width}x{expected_height} pixels")
        
        if width_pixels != expected_width:
            raise ValueError(f"Width mismatch: expected {expected_width}, got {width_pixels}")
        
        if height_pixels != expected_height:
            raise ValueError(f"Height mismatch: expected {expected_height}, got {height_pixels}")
        
        expected_total_bytes = expected_width * expected_height * 2
        if total_size_bytes != expected_total_bytes:
            raise ValueError(f"Total size mismatch: expected {expected_total_bytes}, got {total_size_bytes}")
        
        # Verify pixel data by converting original to RGB565 and comparing
        original_data = im.getdata()
        expected_data = bytearray()
        
        for rgb in original_data:
            r, g, b = rgb[0], rgb[1], rgb[2]
            
            # Convert to RGB565 using corrected standard conversion
            r5 = (r >> 3) & 0x1F
            g6 = (g >> 2) & 0x3F
            b5 = (b >> 3) & 0x1F
            
            rgb565 = (r5 << 11) | (g6 << 5) | b5
            expected_data.extend(rgb565.to_bytes(2, byteorder="little"))
        
        if image_data != expected_data:
            raise ValueError("Image pixel data mismatch")
    
    print("✓ BMP data verification passed")
    
    # Reconstruct the BMP file for additional verification
    with tempfile.NamedTemporaryFile(suffix='.bmp', delete=False) as temp_bmp:
        reconstruct_bmp_file(iso_data, start_addr, temp_bmp.name)
        
        # Verify the reconstructed image has the same dimensions
        with Image.open(temp_bmp.name) as reconstructed_im:
            if reconstructed_im.size != (width_pixels, height_pixels):
                os.unlink(temp_bmp.name)
                raise ValueError("Reconstructed BMP dimensions don't match")
            
            print("✓ BMP reconstruction verification passed")
        
        os.unlink(temp_bmp.name)
    
    return start_addr + 8 + total_size_bytes

def verify_iso_file(iso_path, expected_files):
    """Main verification function"""
    print(f"Verifying ISO file: {iso_path}")
    print(f"Expected input files: {expected_files}")
    
    # Read the entire ISO file
    with open(iso_path, 'rb') as f:
        iso_data = f.read()
    
    print(f"ISO file size: {len(iso_data)} bytes")
    
    # Read header
    addresses = read_iso_header(iso_data)
    
    if len(addresses) != len(expected_files):
        raise ValueError(f"Number of entries mismatch: expected {len(expected_files)}, got {len(addresses)}")
    
    # Verify each file
    for i, (addr, expected_file) in enumerate(zip(addresses, expected_files)):
        file_path = Path(expected_file)
        print(f"\nEntry {i}: {file_path.name} at address {addr}")
        
        if file_path.suffix.lower() == ".wav":
            next_addr = verify_wav_data(iso_data, addr, file_path)
        elif file_path.suffix.lower() == ".bmp":
            next_addr = verify_bmp_data(iso_data, addr, file_path)
        else:
            raise ValueError(f"Unsupported file type: {file_path.suffix}")
        
        # Check that next address matches expectation (if not last entry)
        if i < len(addresses) - 1:
            expected_next = addresses[i + 1]
            if next_addr != expected_next:
                print(f"Warning: Address calculation mismatch. Expected next at {expected_next}, calculated {next_addr}")

def main():
    parser = argparse.ArgumentParser(description="Verify ISO file integrity and test reconstruction")
    parser.add_argument("--iso", type=str, help="Path to ISO file to verify")
    parser.add_argument("--files", nargs="+", type=str, help="Original input files used to generate ISO")
    parser.add_argument("--use-debug-config", action="store_true", 
                       help="Use the debug configuration from .vscode/launch.json")
    parser.add_argument("--reconstruct-to", type=str, 
                       help="Directory to save reconstructed files (for manual inspection)")
    
    args = parser.parse_args()
    
    if args.use_debug_config:
        # Use the configuration from .vscode/launch.json
        iso_file = "python/test.iso"
        input_files = ["assets/HL1_song11.wav", "assets/TestImgGimp.bmp"]
        print("Using debug configuration from .vscode/launch.json")
    else:
        if not args.iso or not args.files:
            parser.error("Either use --use-debug-config or provide --iso and --files")
        iso_file = args.iso
        input_files = args.files
    
    try:
        # Check if files exist
        iso_path = Path(iso_file)
        if not iso_path.exists():
            raise FileNotFoundError(f"ISO file not found: {iso_file}")
        
        for file_path in input_files:
            if not Path(file_path).exists():
                raise FileNotFoundError(f"Input file not found: {file_path}")
        
        # Run verification
        verify_iso_file(iso_path, input_files)
        
        # If reconstruction directory is specified, save reconstructed files
        if args.reconstruct_to:
            reconstruct_directory = Path(args.reconstruct_to)
            reconstruct_directory.mkdir(exist_ok=True)
            
            print(f"\n--- Saving reconstructed files to {reconstruct_directory} ---")
            
            # Read ISO data again
            with open(iso_path, 'rb') as f:
                iso_data = f.read()
            
            # Read header
            addresses = read_iso_header(iso_data)
            
            # Reconstruct each file
            for i, (addr, expected_file) in enumerate(zip(addresses, input_files)):
                file_path = Path(expected_file)
                output_name = f"reconstructed_{i}_{file_path.name}"
                output_path = reconstruct_directory / output_name
                
                if file_path.suffix.lower() == ".wav":
                    reconstruct_wav_file(iso_data, addr, output_path, file_path)
                elif file_path.suffix.lower() == ".bmp":
                    reconstruct_bmp_file(iso_data, addr, output_path)
                
                print(f"Saved: {output_path}")
        
        print("\n" + "="*50)
        print("✅ ISO file verification PASSED!")
        print("All data matches expected values and reconstruction successful.")
        if args.reconstruct_to:
            print(f"Reconstructed files saved to: {args.reconstruct_to}")
        print("="*50)
        
    except Exception as e:
        print("\n" + "="*50)
        print("❌ ISO file verification FAILED!")
        print(f"Error: {e}")
        print("="*50)
        return 1
    
    return 0

if __name__ == "__main__":
    exit(main())

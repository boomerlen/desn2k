import argparse
from pathlib import Path
from PIL import Image

def convert_wav(bytes_in):
    # Step through bytes ensure it is correctly formatted
    bytes_out = bytearray() 
    if len(bytes_in) < 45:
        raise ValueError("Input data is too short to be a valid WAV file.")
    if bytes_in[0:4] != b'RIFF':
        raise ValueError("Input data does not start with 'RIFF'.")
    if bytes_in[8:12] != b'WAVE':
        raise ValueError("Input data does not contain 'WAVE' format.")
    if bytes_in[20:22] != b'\x01\x00':
        raise ValueError("Input data does not have PCM format.")
    if bytes_in[22:24] != b'\x01\x00':
        raise ValueError("Input data does not have a single channel (mono).")

    # Rest of this function should create a byte array which contains the size of the data chunk
    # in the first 4 bytes and then the data itself
    data_size = len(bytes_in) - 44 
    bytes_out.extend(data_size.to_bytes(4, byteorder="little"))
    bytes_out.extend(bytes_in[44:])
    return bytes_out

def convert_bmp(path_in):
    im = Image.open(path_in)

    width = im.width
    total_out_size_bytes = im.width * im.height * 2 # 565 format uses 2 bytes per pixel

    data = im.getdata()
    
    out_data = bytearray()

    # Recall: image header is 
    # 0x0: width_bytes (4 bytes)
    # 0x4: whole_size_bytes (4 bytes)
    # 0x8: data

    out_data.extend((im.width * 2).to_bytes(4, byteorder="little"))
    out_data.extend(total_out_size_bytes.to_bytes(4, byteorder="little"))

    for rgb in data:
        # I think PIL converts to RGB (BMP is GBR _of course_)
        r = rgb[0]
        g = rgb[1]
        b = rgb[2]

        # Convert r and b to a number between 0 and 31
        # g has 6 bits so can go up to 63
        r = int((r / 256) * 32)
        g = int((g / 256) * 64)
        b = int((b / 256) * 32)
        
        r5 = (r >> 3) & 0x1F  # 8-bit -> 5-bit (keep upper 5 bits)
        g6 = (g >> 2) & 0x3F  # 8-bit -> 6-bit (keep upper 6 bits)  
        b5 = (b >> 3) & 0x1F

        # Make two bytes out of this
        rgb565 = (r5 << 11) | (g6 << 5) | b5

        out_data.extend(rgb565.to_bytes(2, byteorder="little"))

    return out_data 

def main(args):
    data_names = []
    data_bytes = []
    
    for file in args.infile:
        f_path = Path(file)

        if f_path.suffix == ".wav":
            with open(f_path, 'rb') as infile:
                data = infile.read()
            result = convert_wav(data)

            data_names.append(f_path.name)
            data_bytes.append(result)
        elif f_path.suffix == ".bmp":
            result = convert_bmp(f_path)

            data_names.append(f_path.name)
            data_bytes.append(result)
        else:
            raise ValueError(f"Unrecognised extention: {f_path.suffix}")
    
    # build header
    data_out = bytearray()
    n_entries = len(data_names)
    start_addr = n_entries * 4 # 4 bytes per entry in the file table
    curr_addr = start_addr
    for i in range(n_entries):
        data_out.extend(curr_addr.to_bytes(length=4, byteorder="little"))
        curr_addr = curr_addr + len(data_bytes[i])

    for i in range(len(data_names)):
        print(f"{i}: {data_names[i]}")
        data_out.extend(data_bytes[i])

    with open(args.outfile, 'wb') as outfile:
        outfile.write(data_out)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="SD Card Image Generator CLI")
    parser.add_argument("--infile", nargs="+", type=str, required=True, help="Input file paths")
    parser.add_argument("--outfile", type=str, required=True, help="Output file path")
    args = parser.parse_args()
    main(args)

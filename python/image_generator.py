import argparse

def convert_bytes(bytes_in):
    # Step through bytes ensure it is correctly formatted
    bytes_out = bytearray() 
    if len(bytes_in) < 45:
        raise ValueError("Input data is too short to be a valid WAV file.")
    if bytes_in[0:4] != b'RIFF':
        raise ValueError("Input data does not start with 'RIFF'.")
    if bytes_in[8:12] != b'WAVE':
        raise ValueError("Input data does not contain 'WAVE' format.")
    if bytes_in[18:20] != b'\x01\x00':
        raise ValueError("Input data does not have PCM format.")
    if bytes_in[22:24] != b'\x01\x00':
        raise ValueError("Input data does not have a single channel (mono).")

    # Rest of this function should create a byte array which contains the size of the data chunk
    # in the first 4 bytes and then the data itself
    data_size = len(bytes_in) - 44 
    bytes_out.extend(data_size.to_bytes(4, 'little'))
    bytes_out.extend(bytes_in[44:])
    return bytes_out

def main(args):
    with open(args.infile, 'rb') as infile:
        data = infile.read()
    result = convert_bytes(data)
    with open(args.outfile, 'wb') as outfile:
        outfile.write(result)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="SD Card WAV Image Generator CLI")
    parser.add_argument("--infile", type=str, required=True, help="Input file path")
    parser.add_argument("--outfile", type=str, required=True, help="Output file path")
    args = parser.parse_args()
    main(args)
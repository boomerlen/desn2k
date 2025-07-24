import requests
import sys 

if len(sys.argv != 3):
    print("Invalid args")
    sys.exit(-1)

file_in = sys.argv[1]
file_out = sys.argv[2]

export_url = "http://drawio-export:8000/export"

with open(file_in, "r") as f:
    data = f.read()

request_data = {
    'xml': data,
    'format': 'png',
    'bg': 'white',
    'w': 0,
    'h': 0,
    'border': 0
}

resp = requests.post(export_url, data=request_data)

if resp.status_code != 200:
    print(f"Export failed: {resp.status_code}")
    print(f"Error: {resp.text}")
    sys.exit(-1)

with open(file_out, "wb") as f:
    f.write(resp.content)


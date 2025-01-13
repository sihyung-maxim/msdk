###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################

from pathlib import Path
import re
from imgConverter import convert
import traceback
import argparse

def batchconvert(input_dir, output_dir = "converted"):
    if not Path(output_dir).exists():
        Path(output_dir).mkdir()

    for file in sorted(Path(input_dir).iterdir()):
        with open(file, "rb") as f:
            header = f.readline() # First line in a valid image should contain the image header
            try:
                header = header.decode(encoding="ascii").strip()
                if "*IMG*" not in header:
                    print(f"Bad image header in {file}, skipping")
                else:

                    expr = re.compile("\*IMG\* (\w+) (\d+) (\d+) (\d+)")
                    match = expr.findall(header)

                    if len(match) == 1:
                        # Received expected header, parse parameters from regex
                        values = match[0]
                        pixel_format = values[0]
                        imglen = int(values[1])
                        w = int(values[2])
                        h = int(values[3])

                        content = f.read() # Read the rest of the file content

                        out_file = f"{Path(output_dir).joinpath(file.name)}.png"
                        convert(content, out_file, w, h, pixel_format)
                        print(f"Successfully converted {file} -> {out_file}")
            except:
                print(f"Error converting {file}, skipping...")
                print(traceback.format_exc())

argparser = argparse.ArgumentParser(description="Utility conversion script for converting batches of raw images captured by ImgCapture firmware into .pngs")
argparser.add_argument("input_dir", type=str, help="Input directory containing the raw images to convert.")
argparser.add_argument("-o", type=str, help="Output directory to save the converted images.", default="converted")

if __name__ == "__main__":
    args = argparser.parse_args()
    batchconvert(args.input_dir, args.o)
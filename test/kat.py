#!/usr/bin/env python3
import argparse
import serial
import re

def get_keys():
    keys = None
    with open("pk", "r") as pk_file, open("sk", "r") as sk_file:
        # Obtaining the public key
        pk_content = pk_file.read()
        pk_key_string = re.sub(r".* = ", r"", pk_content)
        # Obtaining the secret key
        sk_content = sk_file.read()
        sk_key_string = re.sub(r".* = ", r"", sk_content)
        keys = (pk_key_string, sk_key_string)
    return keys

def parse_arguments():
    parser = argparse.ArgumentParser(description="IO Demo")
    parser.add_argument("-u", "--uart", help="UART Device", required=True)
    parser.add_argument("-b", "--baud", help="BAUD Rate", default=115200, type=int)
    return parser.parse_args()

def test_sign():
    pass

def test_gen():
    pass

def test_verify():
    pass

def main():
    args = parse_arguments()
    keys = get_keys()
    with serial.Serial(args.uart, args.baud) as ser:
        ser.flushInput()
        # for i in range(256):
            # print(f"Sending {i}...", end="", flush=True)
            # try:
                # ser.write([i])
                # c = ser.read(1)
                # if len(c) != 1:
                    # print("read failed!")
                    # break
                # c = c[0]
                # print(f"got {c} (should be {i})")
            # except TimeoutError:
                # print("timed out!")
                # break

if __name__ == "__main__":
    main()

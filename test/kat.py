#!/usr/bin/env python3
import argparse
import serial
import re
import secrets
import subprocess
import ast
from string import ascii_letters, punctuation, digits
from random import choice, randint

def parse_arguments():
    parser = argparse.ArgumentParser(description="IO Demo")
    parser.add_argument("-u", "--uart", help="UART Device", required=True)
    parser.add_argument("-b", "--baud", help="BAUD Rate", default=115200, type=int)
    parser.add_argument("-i", "--implementation", help="The version of Rainbow to compare against (Optimized, Reference, etc.). standard is the reference implementation")
    return parser.parse_args()

def random_message(min_len, max_len):
    string_format = ascii_letters
    return "".join(choice(string_format) for _ in range(randint(min_len,max_len)))

def get_keys(pk, sk):
    keys = None
    with open(pk, "r") as pk_file, open(sk, "r") as sk_file:
        # Obtaining the public key
        pk_content = pk_file.read()
        pk_key_string = re.sub(r".* = ", r"", pk_content)
        # Obtaining the secret key
        sk_content = sk_file.read()
        sk_key_string = re.sub(r".* = ", r"", sk_content)
        keys = (bytes.fromhex(pk_key_string), bytes.fromhex(sk_key_string))
    return keys

def run_ref_gen(seed, implementation):
    subprocess.run(['make', '-C', 'rainbow-submission-round2/' + implementation])
    with open('rseed', 'wb') as bfile:
        bfile.write(seed)
    genkey = subprocess.run(['rainbow-submission-round2/' + implementation + '/rainbow-genkey', 'KATpk', 'KATsk', 'rseed'])

def run_ref_verify(keys):
    
def run_ref_sign(keys):
    pass

def test_gen(args):
    seed = secrets.token_bytes(48)
    # run_ref_gen(seed, args.implementation)
    # pk, sk = get_keys('KATpk', 'KATsk')
    with serial.Serial(args.uart, args.baud) as ser:
        bytearr = bytearray(seed)
        ser.flushInput()
        try:
            for i in range(48):
                print(f"Sending {bytearr[i]}")
                ser.write([bytearr[i]])
                c = ser.read(1)[0]
                print(f"Received {c}")
            b_output = ser.read_until(bytes('}', 'ascii'))
            s_output = b_output.decode('ascii')
            s_output = s_output.replace("\n", " ")
            print(repr(s_output))
            data = ast.literal_eval(s_output)
            # print("Received:\n", data)
        except TimeoutError:
            print("timed out!")

def test_sign(sk):
    run_ref_sign(sk)
    with serial.Serial(args.uart, args.baud) as ser:


def test_verify(pk, args):
    pk_size = 161600
    message = bytes(random_message(20, 200), "utf-8")
    ref_verified = run_ref_verify(pk)
    with serial.Serial(args.uart, args.baud) as ser:
        ser.flushInput()
        try:
            for i in range(pk_size):
                ser.write([pk[i]])
            # Send message length to the device
            currlen = len(message)
            if currlen > 255:
                write_length(ser, currlen)
            else:
                ser.write([currlen])
            for i in range(len(message)):
                ser.write([message[i]])
            # same procedure for signature
        except TimeoutError:
            print("Timed out!")


def write_length(ser, currlen):
    first_iter = True
    ser.write([0])
    while currlen > 0:
        ser.write([255])
        currlen -= 255
    ser.write([0])




def main():
    args = parse_arguments()
    if args.implementation == None:
        args.implementation = "Reference_Implementation"
    # keys = get_keys()
    test_gen(args)

# with serial.Serial(args.uart, args.baud) as ser:
        # ser.flushInput()
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

#!/usr/bin/env python3
import argparse
import serial
import re
import secrets
import subprocess
import ast
import os
from datetime import datetime
from string import ascii_letters, punctuation, digits
from random import choice, randint
from time import time

# I feel sorry for anyone reading this. One does not simply read other people's code

static_output = '''RAINBOW(16,36,32,32) - classic
sk size: 103648
pk size: 161600
hash size: 32
signature size: 66

RAINBOW(16,36,32,32) - classic signature = '''

def parse_arguments():
    parser = argparse.ArgumentParser(description="Script to test implementations")
    parser.add_argument("-u", "--uart", help="UART Device", required=True)
    parser.add_argument("-r", "--rate", help="BAUD Rate", default=115200, type=int)
    parser.add_argument("-i", "--implementation", help="The version of Rainbow to compare against (Optimized, Reference, etc.). standard is the reference implementation", type=str)
    parser.add_argument("-c", "--customkat", help="Use a custom KAT folder (id) for testing (includes signature file, bad signature file, message file, key files and the random seed file). If not specified, the script generates a KAT at random for testing", type=str)
    parser.add_argument("-w", "--wrongsign", help="Use the wrong signature for testing", default=False, type=bool)
    parser.add_argument("-b", "--bench", help="Set if benchmarking", default=False, action='store_true')
    parser.add_argument("-f", "--function", help="What functionality to test (genkey/verify/sign)", type=str, default="verify")
    return parser.parse_args()

##########################
##  Hostside reference  ##
##########################

def run_ref_gen(implementation, kat_id):
    rseed_path = f"KAT_{kat_id}/rseed" 
    pk_path = f"KAT_{kat_id}/KATpk"
    sk_path = f"KAT_{kat_id}/KATsk"
    subprocess.run(["rainbow-submission-round2/" + implementation + "/rainbow-genkey", pk_path, sk_path, rseed_path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

def run_ref_verify(implementation, kat_id):
    output = subprocess.run(["rainbow-submission-round2/" + implementation + "/rainbow-verify", f"KAT_{kat_id}/KATpk", f"KAT_{kat_id}/signature", f"KAT_{kat_id}/message"], capture_output=True)
    regex = re.compile(".*Correct.*")
    if regex.search(output.stdout.decode("utf-8")):
        return True
    else:
        return False
    
def run_ref_sign(implementation, kat_id):
    sign_status = subprocess.run(["rainbow-submission-round2/" + implementation + "/rainbow-sign", f"KAT_{kat_id}/KATsk", f"KAT_{kat_id}/message"], capture_output=True)
    with open(f"KAT_{kat_id}/signature", "w") as sign_file:
        sign_file.write(sign_status.stdout.decode("ascii"))
    return re.search(r"(?<== )\w*", sign_status.stdout.decode("ascii")).group(0)

###############################
## General testing functions ##
###############################

def test_gen(args):
    seed = get_kat_file(args.kat_id, "rseed", "rb") 
    pk, sk = args.keys
    with serial.Serial(args.uart, args.rate) as ser:
        bytearr = bytearray(seed)
        ser.flushInput()
        try:
            for i in range(48):
                ser.write([bytearr[i]])
            data = read_uart(ser)
            data["pk_value"], data["sk_value"] = bytes.fromhex(data["pk_value"]), bytes.fromhex(data["sk_value"])
            if (data["pk_value"] == pk) and (data["sk_value"] == sk):
                print("Keys generated correctly")
            else:
                print("Error")
        except TimeoutError:
            print("timed out!")

def test_sign(args):
    sk_size = 103648
    sk = args.keys[1]
    run_ref_sign(args.implementation, args.kat_id)
    signature = re.search(r"(?<== )\w*", get_kat_file(args.kat_id, "signature", "r")).group(0)
    signature_b = bytes.fromhex(signature)
    message = get_kat_file(args.kat_id, "message", "r")
    with serial.Serial(args.uart, args.rate) as ser:
        ser.flushInput()
        try:
            for i in range(sk_size):
                ser.write([sk[i]])
            init_send(message, ser)
            data = read_uart(ser)
            if bytes.fromhex(data["Signature"]) == signature_b:
                print("Correct signature")
            else:
                print("Error")
        except TimeoutError:
            print("Timed out!")

def test_verify(args):
    pk_size = 161600
    pk = args.keys[0]
    message = get_kat_file(args.kat_id, "message", "r")
    signature = get_kat_file(args.kat_id, "signature", "r") if not args.wrongsign else get_kat_file(args.kat_id, "bad_signature", "r")
    signature = re.search(r"(?<== )\w*", signature).group(0)
    ref_verified = run_ref_verify(args.implementation, args.kat_id)
    with serial.Serial(args.uart, args.rate) as ser:
        ser.flushInput()
        try:
            for i in range(pk_size):
                ser.write([pk[i]])
                if (i & 0xffff) == 0xffff:
                  print(i)
            # Send message
            init_send(message, ser)
            # Send signature
            sign_b = bytes.fromhex(signature)
            currlen = len(sign_b)
            write_length(ser, currlen)
            for i in range(currlen):
                ser.write([sign_b[i]])
            # Read output
            data = read_uart(ser)
            # print(data)
            check_verify(data, args.wrongsign, ref_verified, args)
        except TimeoutError:
            print("Timed out!")

#########################
## Helpers for testing ##
#########################

def check_verify(data, wrong_signature, ref_verified, args):
    if wrong_signature:
        if data["Status"] == "Success":
            print("Houston, we've had a problem here")
        else:
            print("Bad signature did not work (phew)")
    else:
        verify_success(data, ref_verified, args)

def verify_success(data, ref_verified, args):
    if (data["Status"] == "Success") and ref_verified:
        print("Verification success!")
        with open(f"KAT_{args.kat_id}/verify_succes", "w") as status_file:
            status_file.write("Success\n")
        if args.bench:
            with open(f"KAT_{args.kat_id}/benchmarks", "w") as bench_file:
                bench_file.write("Cycles: " + str(data["Cycles"]) + "\n")
                bench_file.write("Instructions: " + str(data["Instructions"]) + "\n")
        print("Cycles: " + str(data["Cycles"]))
        print("Instructions: " + str(data["Instructions"]))
    else:
        print("These aren't the droids you are looking for.")
        with open(f"KAT_{args.kat_id}/verify_failure", "w") as status_file:
            status_file.write("Failure\n")

def init_send(message, ser):
    b_message = bytes(message, "ascii") 
    currlen = len(b_message)
    write_length(ser, currlen)
    for i in range(len(b_message)):
        ser.write([b_message[i]])

def write_length(ser, currlen):
    long_length = currlen > 255
    if long_length: ser.write([0])
    while currlen > 255:
        ser.write([255])
        currlen -= 255
    ser.write([currlen])
    if long_length: ser.write([0])

def read_uart(ser):
        b_output = ser.read_until(bytes("}", "ascii"))
        s_output = b_output.decode("ascii")
        s_output = "".join(s_output.split())
        s_output = re.sub(r"bytes.fromhex\((\"\w*\")\)", r"\1", s_output)
        data = ast.literal_eval(s_output)
        return data

def get_kat_file(kat_id, f, f_type):
    content = None
    with open(f"KAT_{kat_id}/{f}", f_type) as msg_f:
        content = msg_f.read()
    return content

def get_keys(pk, sk):
    keys = None
    with open(pk, "r") as pk_file, open(sk, "r") as sk_file:
        # Obtaining the public key
        pk_content = pk_file.read()
        pk_key_string = re.search(r"(?<== )\w*", pk_content).group(0)
        # Obtaining the secret key
        sk_content = sk_file.read()
        sk_key_string = re.search(r"(?<== )\w*", sk_content).group(0)
        keys = (bytes.fromhex(pk_key_string), bytes.fromhex(sk_key_string))
    return keys

###########################
##     KAT GENERATION    ##
###########################

def generate_kat(args):
    seed = secrets.token_bytes(48)
    message_content = random_message(20, 200) 
    kat_id = int(time())
    os.mkdir(f"KAT_{kat_id}")
    with open(f"KAT_{kat_id}/message", "w") as msg_f, open(f"KAT_{kat_id}/rseed", "wb") as seed_f:
        msg_f.write(message_content)
        seed_f.write(seed)
    run_ref_gen(args.implementation, kat_id)
    signature = run_ref_sign(args.implementation, kat_id)
    with open(f"KAT_{kat_id}/bad_signature", "w") as b_sign_f:
        b_sign_f.write(bad_signature(signature))
    return kat_id

def bad_signature(signature):
    while True:
        bad_signature = secrets.token_bytes(len(signature))
        if bad_signature != signature:
            return static_output + bad_signature.hex()
    
def random_message(min_len, max_len):
    string_format = ascii_letters
    return "".join(choice(string_format) for _ in range(randint(min_len,max_len)))

def setup(args):
    if args.implementation == None:
        args.implementation = "Reference_Implementation"
    # keys = get_keys()
    subprocess.run(["make", "-C", "rainbow-submission-round2/" + args.implementation], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    if args.customkat == None:
        print("Starting KAT generation..")
        args.kat_id = generate_kat(args)
        print("KAT generation done")
    else:
        args.kat_id = args.customkat
    args.keys = get_keys(f"KAT_{args.kat_id}/KATpk", f"KAT_{args.kat_id}/KATsk")

# Do all the things
def main():
    args = parse_arguments()
    setup(args)
    print("Using KAT with ID:", args.kat_id)
    # Setup done. Start testing
    if args.function == "genkey":
        print("Testing key generation..")
        test_gen(args)
        print("Key generation done")
    elif args.function == "sign":
        print("Testing signing..")
        test_sign(args)
        print("Signing done")
    elif args.function == "verify":
        print("Testing verification..")
        test_verify(args)
        print("Verification done")
    else:
        print("Functionality not recognized")

if __name__ == "__main__":
    main()

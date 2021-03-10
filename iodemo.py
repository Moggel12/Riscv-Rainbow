#!/usr/bin/env python3
import argparse
import serial


def parse_arguments():
    parser = argparse.ArgumentParser(description="IO Demo")
    parser.add_argument("-u", "--uart", help="UART Device", required=True)
    parser.add_argument("-b", "--baud", help="BAUD Rate", default=115200, type=int)
    return parser.parse_args()

def main():
    args = parse_arguments()
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
        msg_sent = "hello"
        for c in msg_sent:
            print(f"Sending {c} (ASCII {ord(c)}). ", end="", flush=True)
            ser.write([ord(c)])
            c_rcv = ser.read(1)[0]
            print(f"Got {c_rcv}, should be {ord(c)}", flush=True)
        print(f"Sent {msg_sent}")
        ser.write([0])
        c = ser.read(1)[0]
        msg_rcv = ""
        while (c != 0):
            print("in loop")
            char = chr(c) 
            print(f"Got character {char}", flush=True)
            msg_rcv += c
            c = ser.read(1)[0]
        print(f"Total message received: {msg_rcv}", flush=True)

if __name__ == "__main__":
    main()

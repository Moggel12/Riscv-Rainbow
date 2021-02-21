#!/usr/bin/env python3
import argparse
import serial


def parse_arguments():
    parser = argparse.ArgumentParser(description="IO Demo")
    parser.add_argument("-u", "--uart", help="UART Device", required=True)
    parser.add_argument("-b", "--baud", help="BAUD Rate", default=115200, type=int)
    return parser.parse_args()


def fn(s, i):
    i %= 256
    return (s * 0x42 + i) % 0x66


def main():
    args = parse_arguments()
    with serial.Serial(args.uart, args.baud) as ser:
        ser.flushInput()
        s = 0x42
        for i in range(256):
            print(f"Sending 0x{i:02X}...", end="", flush=True)
            try:
                ser.write([i])
                s = fn(s, i)
                c = ser.read(1)
                if len(c) != 1:
                    print("read failed!")
                    break
                c = c[0]
                print(f"got 0x{c:02X} (should be 0x{s:02X})")
            except TimeoutError:
                print("timed out!")
                break


if __name__ == "__main__":
    main()

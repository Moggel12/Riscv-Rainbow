#! /usr/bin/env python3

import pandas as pd
from matplotlib import pyplot as plt
plt.rcParams['font.family'] = 'serif'
plt.rcParams['font.serif'] = ['Times New Roman'] + plt.rcParams['font.serif']
import numpy as np
import os


def obtain_stats():
    template = {"standard": [], "bitslice": [], "lookup": []}
    cycles = pd.DataFrame(template)
    instructions = pd.DataFrame(template)
    types = ["standard", "bitslice", "lookup"]
    for f in os.listdir():
        if "KAT_" in f:
            cycle_values = {}
            instr_values = {}
            for t in types:
                with open(f"{f}/{t}/benchmarks", "r") as bench:
                    line = bench.read()
                    line_arr = line.split("\n")
                    cycle_values[t] = line_arr[0].split(" ")[1]
                    instr_values[t] = line_arr[1].split(" ")[1]
            cycles = cycles.append(cycle_values, ignore_index=True)
            instructions = instructions.append(instr_values, ignore_index=True)
    cycles = cycles.astype({"standard": np.int64, "bitslice": np.int64, "lookup": np.int64})
    instructions = instructions.astype({"standard": np.int64, "bitslice": np.int64, "lookup": np.int64})
    return cycles, instructions

# def bar_all_cycles(cycles):
#     x = np.arange(len(cycles))
#     width = 0.25
#     fig, ax = plt.subplots()
#     ax.bar(x - width, cycles["bitslice"], width, label="Bitslice cycles", color="blue")
#     ax.set_ylabel("Bitslicing cycles")
#     ax.set_xlabel("Test number")
#     ax.set_xticks(x)
#     ax.legend(loc="center right", bbox_to_anchor=(-0.10, 1))
#     ax2 = ax.twinx()
#     ax2.bar(x, cycles["standard"], width, label="Standard cycles", color="red")
#     ax2.bar(x + width, cycles["lookup"], width, label="Lookup cycles", color="green")
#     ax2.set_ylabel("Standard & Lookup table cycles")
#     ax2.legend(loc="center left", bbox_to_anchor=(1.10, 1))
#     fig.savefig("plots/bar_all_cycles.png", bbox_inches="tight")

def bar_avg_cycles(cycle_avg):
    x = np.arange(len(cycle_avg))
    fig, ax = plt.subplots()
    bsl = ax.bar("Bitslice", cycle_avg["bitslice"], label="Bitslice", color="blue")
    std = ax.bar("Standard", cycle_avg["standard"], label="Standard", color="red")
    lkp = ax.bar("Lookup", cycle_avg["lookup"], label="Lookup", color="green")
    ax.set_ylabel("Mega cycle average")
    ax.set_xlabel("Implementation")
    ax.legend()
    ax.bar_label(bsl, padding=2)
    ax.bar_label(std, padding=2)
    ax.bar_label(lkp, padding=2)
    fig.savefig("plots/bar_avg_cycle.png", bbox_inches="tight")

# def bar_all_ratio(ratio):
#     x = np.arange(len(ratio))
#     width = 0.25
#     fig, ax = plt.subplots()
#     ax.bar(x - width, ratio["bitslice"], width, label="Bitslice", color="blue")
#     ax.bar(x, ratio["standard"], width, label="Standard", color="red")
#     ax.bar(x + width, ratio["lookup"], width, label="Lookup", color="green")
#     ax.set_ylabel("Mega cycles per instruction")
#     ax.set_xlabel("Test number")
#     ax.set_xticks(x)
#     ax.legend()
#     fig.savefig("plots/bar_all_ratio.png", bbox_inches="tight")

def bar_avg_ratio(ratio_avg):
    fig, ax = plt.subplots()
    bsl = ax.bar("Bitslice", ratio_avg["bitslice"], label="Bitslice", color="blue")
    std = ax.bar("Standard", ratio_avg["standard"], label="Standard", color="red")
    lkp = ax.bar("Lookup", ratio_avg["lookup"], label="Lookup", color="green")
    ax.set_ylabel("Average cycles per instruction")
    ax.set_xlabel("Implementation")
    ax.legend(loc="center right")
    ax.bar_label(bsl, padding=2)
    ax.bar_label(std, padding=2)
    ax.bar_label(lkp, padding=2)
    fig.savefig("plots/bar_avg_ratio.png", bbox_inches="tight")

def main():
    if not os.path.exists("plots"):
        os.mkdir("plots")
    cycles, instructions = obtain_stats()
    cycle_avg = cycles.sum()/len(cycles)
    cycle_avg = cycle_avg/1000000 # Mega cycles
    ratio = cycles/instructions
    ratio_avg = ratio.sum()/len(ratio)
    # ratio_avg = ratio_avg/1000000 # Mega cycles
    bar_avg_cycles(cycle_avg)
    bar_avg_ratio(ratio_avg)
    # print(ratio_avg)
    # print(cycle_avg)


if __name__ == "__main__":
    main()
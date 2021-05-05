#! /usr/bin/env python3

import re

M = 64
N = 100
j = 0
with open("KAT_1619004850/KATpk", "r") as file:
    content = file.read()
    i = 0
    l = [ [] for i in range(M)]
    content = re.search(r"(?<== )\w*", content).group(0)
    b_content = bytes.fromhex(content)
    for b in b_content:
        if i == 32:
            i = 0
        l[i*2].append((b >> 4) & 15)
        l[i*2+1].append(b & 15)
        i = (i+1)%32

# print(j)
print(l[0][0:10])
print(((b_content[0] >> 4) & 15))
print(len(b_content)/M)
# print(len(b_content)/M)
# print((N*(N+1)/2))
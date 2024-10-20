#!/usr/bin/env python3
import numpy as np;

import matplotlib
import matplotlib.pyplot as plt;

import sys;

import re;

def plot(lines):
    for idx,line in enumerate(lines):
        x = [_[0] for _ in line];
        y = [_[1] for _ in line];

        plt.plot(x,y,label=f"line {idx}");
        plt.legend();
    plt.show();

def convertStringToTuple(string):
    pattern = r"-?\d+";
    x = re.findall(pattern,string);
    return (int(x[0]),int(x[1]));

def driver(lines):
    pattern = r"(\(-?\d+,-?\d+\))\s+?";

    buffer = [];
    for line in lines:
        x = re.findall(pattern,line);
        if(len(x) > 0):
            buffer.append([convertStringToTuple(_) for _ in x]);

    return buffer;

if __name__ == "__main__":
    
    buffer = [];
    while True:
        line = sys.stdin.readline();
        
        if line == "":
            break;

        buffer.append(line);
    
    buffer = driver(buffer);
    plot(buffer);
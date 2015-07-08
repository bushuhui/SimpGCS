#!/usr/bin/env python

import os,sys
import random
import time

g_n  = 10000

def gen_random_string():
    s = ""
    n = random.randint(4, 60);
    for i in range(n):
        r = random.randint(0,26-1)
        s = s + chr(ord('a') + r)

    return s

def term_bench(n):
    """
        perform terminal benchmark

        parameters:
            n           [in] line number
            cn          [in] column number
    """

    for i in range(n):
        s = gen_random_string()
        print "%s" % s


if( __name__ == "__main__" ):
    n  = g_n

    if( len(sys.argv) >= 2 ):
        n = int(sys.argv[1])
    if( len(sys.argv) >= 3 ):
        cn = int(sys.argv[2])

    term_bench(n)


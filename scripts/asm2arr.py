#!/usr/bin/env python3

import os
from sys import argv

if len(argv) != 2 or not argv[1].endswith('a65'):
    print('Usage: {argv[1]} [filename.a65]')
    exit(1)

out = argv[1].replace('.a65', '.o65')
code = os.system(f'xa {argv[1]} -o {out}')

if code != 0:
    print(f'Error: {code}')
else:
    with open(out, 'rb') as f:
        bs = f.read()
        a = '{' + ', '.join(f'0x{b:02x}' for b in bs) + '}'
        print(a)
        code = os.system(f'echo {a} | xclip -sel c')
        if code == 0:
            print('Output copied to clipboard')
        else:
            exit(1)

# 6502-constexpr

## Extended Instruction Set

### HLT (0x02) - 1 byte

When using `exec_until_hlt()`, it halts the machine. Otherwise does nothing.

#### Usage

```asm
ADC #$01
...
.byt $02 ; HLT
```

## Using `scripts/asm2arr.py`

```bash
sudo apt install xa65
./scripts/asm2arr.py [filename.a65]
```

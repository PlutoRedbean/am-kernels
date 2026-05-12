#!/usr/bin/env python3
"""Patch direct calls to RV32 soft mul/div helpers into RV32M instructions."""

import argparse
import struct
from collections import Counter
from pathlib import Path


UDIV_SIG = (
    0x00058613,  # mv a2, a1
    0x00050593,  # mv a1, a0
    0xFFF00513,  # li a0, -1
    0x02060C63,  # beqz a2, ...
    0x00100693,  # li a3, 1
    0x00B67A63,  # bgeu a2, a1, ...
)

UMOD_SIG = (
    0x00008293,  # mv t0, ra
    0xFB5FF0EF,  # jal __udivdi3
    0x00058513,  # mv a0, a1
    0x00028067,  # jr t0
)

MOD_SIG = (
    0x00008293,  # mv t0, ra
    0x0005CA63,  # bltz a1, ...
    0x00054C63,  # bltz a0, ...
    0xF79FF0EF,  # jal __udivdi3
)

MUL_SIG = (
    0x00050613,  # mv a2, a0
    0x00000513,  # li a0, 0
    0x0015F693,  # andi a3, a1, 1
    0x00068463,  # beqz a3, ...
    0x00C50533,  # add a0, a0, a2
    0x0015D593,  # srli a1, a1, 1
    0x00161613,  # slli a2, a2, 1
    0xFE0596E3,  # bnez a1, ...
)


def rv32m(funct3):
    """Encode OP a0, a0, a1 with funct7=1."""
    opcode = 0x33
    rd = 10
    rs1 = 10
    rs2 = 11
    funct7 = 0x01
    return (
        (funct7 << 25)
        | (rs2 << 20)
        | (rs1 << 15)
        | (funct3 << 12)
        | (rd << 7)
        | opcode
    )


RV32M = {
    "div": rv32m(0x4),
    "divu": rv32m(0x5),
    "rem": rv32m(0x6),
    "remu": rv32m(0x7),
    "mul": rv32m(0x0),
}


def sign_extend(value, bits):
    sign = 1 << (bits - 1)
    return (value ^ sign) - sign


def decode_jal_target(word, pc):
    if (word & 0x7F) != 0x6F:
        return None
    imm = (
        ((word >> 31) & 0x1) << 20
        | ((word >> 12) & 0xFF) << 12
        | ((word >> 20) & 0x1) << 11
        | ((word >> 21) & 0x3FF) << 1
    )
    return pc + sign_extend(imm, 21)


def find_signature(words, sig):
    size = len(sig)
    for idx in range(0, len(words) - size + 1):
        if tuple(words[idx : idx + size]) == sig:
            return idx
    return None


def find_helpers(words, base):
    helpers = {}
    helper_ends = []

    udiv_idx = find_signature(words, UDIV_SIG)
    if udiv_idx is not None:
        helpers[base + udiv_idx * 4] = ("divu", RV32M["divu"])
        helper_ends.append(base + (udiv_idx + len(UDIV_SIG)) * 4)

        div_idx = udiv_idx - 2
        if div_idx >= 0 and words[div_idx : div_idx + 2] == [0x06054063, 0x0605C663]:
            helpers[base + div_idx * 4] = ("div", RV32M["div"])

    for name, sig, opcode in (
        ("remu", UMOD_SIG, RV32M["remu"]),
        ("rem", MOD_SIG, RV32M["rem"]),
        ("mul", MUL_SIG, RV32M["mul"]),
    ):
        idx = find_signature(words, sig)
        if idx is not None:
            helpers[base + idx * 4] = (name, opcode)
            helper_ends.append(base + (idx + len(sig) + 1) * 4)

    if not helpers:
        return helpers, None

    return helpers, (min(helpers), max(helper_ends))


def patch_binary(path, base, dry_run):
    data = bytearray(path.read_bytes())
    if len(data) % 4 != 0:
        raise SystemExit(f"{path}: binary size is not 4-byte aligned")

    words = list(struct.unpack("<" + "I" * (len(data) // 4), data))
    helpers, helper_region = find_helpers(words, base)
    if not helpers:
        raise SystemExit("no known RV32 soft mul/div helper pattern was found")

    counts = Counter()
    for idx, word in enumerate(words):
        rd = (word >> 7) & 0x1F
        if rd != 1:
            continue

        pc = base + idx * 4
        if helper_region is not None and helper_region[0] <= pc < helper_region[1]:
            continue

        target = decode_jal_target(word, pc)
        if target not in helpers:
            continue

        name, opcode = helpers[target]
        words[idx] = opcode
        counts[name] += 1

    if not counts:
        raise SystemExit("soft mul/div helpers were found, but no direct call sites were patched")

    if not dry_run:
        path.write_bytes(struct.pack("<" + "I" * len(words), *words))

    return counts, helpers


def main():
    parser = argparse.ArgumentParser(
        description="Replace direct jal calls to GCC RV32 soft mul/div helpers with RV32M ops."
    )
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--base", default="0x80000000", type=lambda x: int(x, 0))
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    counts, helpers = patch_binary(args.binary, args.base, args.dry_run)
    helper_desc = ", ".join(f"{name}@0x{addr:08x}" for addr, (name, _) in sorted(helpers.items()))
    count_desc = ", ".join(f"{name}={counts[name]}" for name in sorted(counts))
    print(f"patched soft mul/div calls: {count_desc} ({helper_desc})")


if __name__ == "__main__":
    main()

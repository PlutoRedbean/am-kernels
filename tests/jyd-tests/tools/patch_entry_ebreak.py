#!/usr/bin/env python3
"""Patch the terminal self-loop in the entry block to ebreak.

The jyd COE images start at RESET_PC, run a small entry block that calls one
or more test routines, then park in a `j .` self-loop.  Patch that entry
self-loop, not an arbitrary matching word elsewhere in the image.
"""

from __future__ import annotations

import argparse
import re
from pathlib import Path


def parse_word(value: str) -> int:
    word = int(value, 0)
    if not 0 <= word <= 0xFFFFFFFF:
        raise argparse.ArgumentTypeError(f"word out of 32-bit range: {value}")
    return word


def sign_extend(value: int, bits: int) -> int:
    sign = 1 << (bits - 1)
    return (value ^ sign) - sign


def jal_target(word: int, pc: int) -> int:
    imm = (
        ((word >> 31) & 0x1) << 20
        | ((word >> 12) & 0xFF) << 12
        | ((word >> 20) & 0x1) << 11
        | ((word >> 21) & 0x3FF) << 1
    )
    return pc + sign_extend(imm, 21)


def is_unconditional_jal(word: int) -> bool:
    return (word & 0x7F) == 0x6F and ((word >> 7) & 0x1F) == 0


def find_entry_self_loop(words: list[int], base: int, self_loop: int, limit: int) -> int:
    idx = 0
    seen: set[int] = set()

    for _ in range(limit):
        if not 0 <= idx < len(words):
            raise SystemExit("entry scan left image before finding self-loop")
        if idx in seen:
            raise SystemExit("entry scan looped before finding self-loop")

        seen.add(idx)
        word = words[idx]
        pc = base + idx * 4

        if word == self_loop:
            return idx * 4

        if is_unconditional_jal(word):
            target = jal_target(word, pc)
            if (target - base) % 4 != 0:
                raise SystemExit(f"unaligned entry jump target 0x{target:x}")
            idx = (target - base) // 4
            continue

        idx += 1

    raise SystemExit(f"entry self-loop 0x{self_loop:08x} not found within {limit} instructions")


def patch_asm(path: Path, word_index: int, from_word: int, to_word: int) -> None:
    lines = path.read_text(encoding="utf-8").splitlines()
    word_lines = [
        idx for idx, line in enumerate(lines)
        if re.search(r"\.word\s+0x[0-9a-fA-F]{8}", line)
    ]
    if word_index >= len(word_lines):
        raise SystemExit(f"{path}: cannot map word index {word_index} to .word line")

    line_idx = word_lines[word_index]
    old = f"0x{from_word:08x}"
    new = f"0x{to_word:08x}"
    if old.lower() not in lines[line_idx].lower():
        raise SystemExit(f"{path}: .word at index {word_index} is not {old}")

    lines[line_idx] = re.sub(old, new, lines[line_idx], count=1, flags=re.IGNORECASE)
    path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--binary", type=Path, required=True)
    parser.add_argument("--asm", type=Path, required=True)
    parser.add_argument("--from-word", type=parse_word, required=True)
    parser.add_argument("--to-word", type=parse_word, required=True)
    parser.add_argument("--base", type=parse_word, required=True)
    parser.add_argument("--scan-limit", type=int, default=256)
    args = parser.parse_args()

    data = bytearray(args.binary.read_bytes())
    words = [
        int.from_bytes(data[offset : offset + 4], "little")
        for offset in range(0, len(data), 4)
    ]

    offset = find_entry_self_loop(words, args.base, args.from_word, args.scan_limit)
    data[offset : offset + 4] = args.to_word.to_bytes(4, "little")
    args.binary.write_bytes(data)
    patch_asm(args.asm, offset // 4, args.from_word, args.to_word)

    print(f'+ PATCH "entry self-loop -> ebreak @ 0x{offset:x} (pc 0x{args.base + offset:x})"')


if __name__ == "__main__":
    main()

include $(MUNDUS_HOME)/common.mk

WORK_DIR  := $(CURDIR)
BUILD_DIR := $(WORK_DIR)/build
DATA_DIR  := $(WORK_DIR)/data
USE_RV32M ?= 0
PURE      ?= 0

CASE ?= default
ifeq ($(CASE),default)
  CASE_DIR := $(DATA_DIR)
  CASE_TAG := root
else
  CASE_DIR := $(DATA_DIR)/$(CASE)
  CASE_TAG := $(CASE)
endif

TRUE_VALUES := 1 y yes true
RV32M_ENABLED := $(filter $(TRUE_VALUES),$(USE_RV32M))
PURE_ENABLED  := $(filter $(TRUE_VALUES),$(PURE))
BUILD_TAG := $(CASE_TAG)$(if $(RV32M_ENABLED),_rv32m,)$(if $(PURE_ENABLED),_pure,)

IROM_COE  := $(CASE_DIR)/irom.coe
DRAM_COE  := $(CASE_DIR)/dram.coe
HAS_DRAM  := $(wildcard $(DRAM_COE))

IROM_ASM_SRC    := $(BUILD_DIR)/$(BUILD_TAG)_irom_data.S
IROM_BIN        := $(BUILD_DIR)/$(BUILD_TAG)_irom_data.bin
IROM_RAW_DISASM := $(BUILD_DIR)/$(BUILD_TAG)_irom_data.raw.asm
IROM_DISASM     := $(BUILD_DIR)/$(BUILD_TAG)_irom_data.asm
DRAM_BIN        := $(BUILD_DIR)/$(BUILD_TAG)_dram_data.bin
IMAGE_BIN       := $(BUILD_DIR)/$(BUILD_TAG)_jyd_image.bin

COE2IMAGE_SCRIPT := $(AM_HOME)/tools/coe2image.py
COE2IMAGE        := python3 $(COE2IMAGE_SCRIPT)

PATCH_SOFT_M_SCRIPT := $(WORK_DIR)/tools/patch_soft_m.py
PATCH_SOFT_M        := python3 $(PATCH_SOFT_M_SCRIPT)

EBREAK_WORD    ?= 0x00100073
SELF_LOOP_WORD ?= 0x0000006f
DRAM_OFFSET    ?= 0x100000

CROSS_COMPILE ?= riscv32-unknown-linux-gnu-
OBJDUMP       := $(CROSS_COMPILE)objdump

ISA      ?= riscv32
PLATFORM ?= jyd
MODE     ?= batch
RESET_PC ?= 0x80000000

NPC_BUILD_DIR := $(NPC_HOME)/build
NPCFLAGS      += --diff=$(NEMU_HOME)/build/riscv32-nemu-interpreter-so
NPCFLAGS      += --log=$(NPC_BUILD_DIR)/jyd-log.txt
NPCFLAGS      += --csv=$(NPC_BUILD_DIR)/mtrace.csv

MAKE_DEPS := $(MAKEFILE_LIST)

CONFIG_FILE := $(BUILD_DIR)/$(BUILD_TAG).config

.PHONY: all image asm run clean cases

all: image asm

image: $(IMAGE_BIN) $(IROM_DISASM)

asm: $(IROM_DISASM)

cases:
	@echo default
	@find $(DATA_DIR) -mindepth 1 -maxdepth 1 -type d -printf '%f\n' | sort

$(BUILD_DIR):
	@mkdir -p $@

$(CONFIG_FILE): $(MAKE_DEPS) | $(BUILD_DIR)
	@tmp="$@.tmp"; \
	{ \
		echo "CASE=$(CASE)"; \
		echo "CASE_DIR=$(CASE_DIR)"; \
		echo "CASE_TAG=$(CASE_TAG)"; \
		echo "USE_RV32M=$(USE_RV32M)"; \
		echo "PURE=$(PURE)"; \
		echo "EBREAK_WORD=$(EBREAK_WORD)"; \
		echo "SELF_LOOP_WORD=$(SELF_LOOP_WORD)"; \
		echo "DRAM_OFFSET=$(DRAM_OFFSET)"; \
		echo "CROSS_COMPILE=$(CROSS_COMPILE)"; \
		echo "OBJDUMP=$(OBJDUMP)"; \
		echo "RESET_PC=$(RESET_PC)"; \
		echo "COE2IMAGE_SCRIPT=$(COE2IMAGE_SCRIPT)"; \
		echo "PATCH_SOFT_M_SCRIPT=$(PATCH_SOFT_M_SCRIPT)"; \
	} > "$$tmp"; \
	if ! cmp -s "$$tmp" "$@"; then \
		mv "$$tmp" "$@"; \
	else \
		rm "$$tmp"; \
	fi

$(IROM_ASM_SRC) $(IROM_BIN) $(IROM_RAW_DISASM) &: \
	$(IROM_COE) \
	$(COE2IMAGE_SCRIPT) \
	$(PATCH_SOFT_M_SCRIPT) \
	$(CONFIG_FILE) | $(BUILD_DIR)
	@echo + COE2IMAGE $<
	@$(COE2IMAGE) \
		--input $< \
		--asm-out $(IROM_ASM_SRC) \
		--bin-out $(IROM_BIN) \
		--section .boot \
		--symbol _start \
		--entry
	@echo + OBJDUMP "raw ->" $(patsubst $(WORK_DIR)/%,%,$(IROM_RAW_DISASM))
	@$(OBJDUMP) -D -b binary -m riscv:rv32 --adjust-vma=$(RESET_PC) $(IROM_BIN) > $(IROM_RAW_DISASM)
	@if [ -n "$(PURE_ENABLED)" ]; then \
		echo + SKIP "pure mode: leave IROM unchanged"; \
	else \
		python3 -c 'from pathlib import Path; import re, sys; bin_path = Path(sys.argv[1]); asm_path = Path(sys.argv[2]); from_word = int(sys.argv[3], 0); to_word = int(sys.argv[4], 0); data = bytearray(bin_path.read_bytes()); off = data.find(from_word.to_bytes(4, "little")); assert off >= 0, f"{bin_path}: word 0x{from_word:08x} not found"; data[off:off + 4] = to_word.to_bytes(4, "little"); bin_path.write_bytes(data); text = asm_path.read_text(encoding="utf-8"); text, count = re.subn(f"0x{from_word:08x}", f"0x{to_word:08x}", text, count=1, flags=re.IGNORECASE); assert count == 1, f"{asm_path}: word 0x{from_word:08x} not found"; asm_path.write_text(text, encoding="utf-8"); print(f"+ PATCH \"self-loop -> ebreak @ 0x{off:x}\"")' \
			$(IROM_BIN) $(IROM_ASM_SRC) $(SELF_LOOP_WORD) $(EBREAK_WORD); \
	fi
	@if [ -n "$(RV32M_ENABLED)" ] && [ -z "$(PURE_ENABLED)" ]; then \
		echo + PATCH "soft mul/div calls -> RV32M instructions"; \
		$(PATCH_SOFT_M) --binary $(IROM_BIN) --base $(RESET_PC); \
	fi

$(DRAM_BIN): $(DRAM_COE) $(COE2IMAGE_SCRIPT) $(CONFIG_FILE) | $(BUILD_DIR)
	@echo + COE2IMAGE $<
	@$(COE2IMAGE) \
		--input $< \
		--asm-out $(BUILD_DIR)/$(BUILD_TAG)_dram_data.S \
		--bin-out $(DRAM_BIN) \
		--section .data \
		--symbol dram_data \
		--data-only

$(IMAGE_BIN): $(IROM_BIN) $(if $(HAS_DRAM),$(DRAM_BIN),) $(CONFIG_FILE) | $(BUILD_DIR)
	@echo + PACK "->" $(patsubst $(WORK_DIR)/%,%,$@)
	@truncate -s $$(( $(DRAM_OFFSET) )) $@
	@dd if=$(IROM_BIN) of=$@ bs=1 seek=0 conv=notrunc status=none
	@if [ -f "$(DRAM_BIN)" ]; then \
		dd if=$(DRAM_BIN) of=$@ bs=1 seek=$$(( $(DRAM_OFFSET) )) conv=notrunc status=none; \
	fi

$(IROM_DISASM): $(IROM_BIN) $(CONFIG_FILE) | $(BUILD_DIR)
	@echo + OBJDUMP "->" $(patsubst $(WORK_DIR)/%,%,$@)
	@$(OBJDUMP) -D -b binary -m riscv:rv32 --adjust-vma=$(RESET_PC) $< > $@

run: image
	@$(MAKE) -s -C $(NPC_HOME) wave PLATFORM=$(PLATFORM) MODE=$(MODE) ISA=$(ISA) \
		ARGS="$(NPCFLAGS)" PROGRAM=$(abspath $(IMAGE_BIN))

clean:
	rm -rf $(BUILD_DIR)

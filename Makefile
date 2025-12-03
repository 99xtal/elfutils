CC		:= cc
CFLAGS 	:= -std=c17 -Wall -Wextra -Wpedantic -O2
LDFLAGS	:=
SRC_DIR := src
BIN_DIR := bin

PROGRAMS := jolt locdiff prodeval safecode

jolt_SRC := $(SRC_DIR)/jolt.c
locdiff_SRC := $(SRC_DIR)/locdiff.c
prodeval_SRC := $(SRC_DIR)/prodeval.c
safecode_SRC := $(SRC_DIR)/safecode.c

BINS := $(addprefix $(BIN_DIR)/, $(PROGRAMS))

.PHONY: all clean
all: $(BINS)

debug: CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -g -O0
debug: all

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

define BUILD_RULE
$(BIN_DIR)/$(1): $($(1)_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$^ -o $$@ $(LDFLAGS)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call BUILD_RULE,$(prog))))

clean:
	rm -rf $(BIN_DIR)
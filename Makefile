CC		:= cc
CFLAGS 	:= -std=c17 -Wall -Wextra -Wpedantic
LDFLAGS	:=
SRC_DIR := src
BIN_DIR := bin

PROGRAMS := locdiff

locdiff_SRC := $(SRC_DIR)/locdiff.c

BINS := $(addprefix $(BIN_DIR)/, $(PROGRAMS))

.PHONY: all clean
all: $(BINS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

define BUILD_RULE
$(BIN_DIR)/$(1): $($(1)_SRC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $$^ -o $$@ $(LDFLAGS)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call BUILD_RULE,$(prog))))

clean:
	rm -rf $(BIN_DIR)
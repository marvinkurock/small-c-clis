# Compiler
CC = gcc

# Directories
SRCS = main.c
OUTDIR = bins
OUTFILE = $(OUTDIR)/resw.exe
DEBUG_OUTFILE = resw.exe

# Flags
DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -s -DNDEBUG

# Default build target
.PHONY: all
all: debug

# Debug build
.PHONY: debug
debug: $(OUTDIR)
	$(CC) $(SRCS) $(DEBUG_FLAGS) -o $(DEBUG_OUTFILE)
	@echo "Build complete: Debug -> $(DEBUG_OUTFILE)"

.PHONY: run
run: debug
	.\resw.exe -h

# Release build
.PHONY: release
release: $(OUTDIR)
	$(CC) $(SRCS) $(RELEASE_FLAGS) -o $(OUTFILE)
	@echo "Build complete: Release -> $(OUTFILE)"

# Create output directory if it doesn't exist
$(OUTDIR):
	mkdir $(OUTDIR)

# Clean build artifacts
.PHONY: clean
clean:
	@echo "Cleaning up..."
	rm -rf $(OUTDIR)
	@echo "Clean complete!"

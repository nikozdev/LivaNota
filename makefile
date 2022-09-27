# basic

NAME:=livanota
VERS:=0.0.0

# files

## source

SLSRC:=./$(NAME).cxx
SLOBJ:=./$(NAME).obj
SLBIN:=./$(NAME)-$(VERS).bin
SLMAN:=./$(NAME)-$(VERS).man

## target

TROOT?=$(HOME)/.local/bin

TDBIN:=$(TROOT)/bin
TDMAN:=$(TROOT)/share/man/man1

TLBIN:=./$(patsubst %.bin,$(TDBIN)/%,$(SLBIN))
TLMAN:=./$(patsubst %,$(TDMAN)/%,$(SLMAN))

# build

## compiler

CMAKER?= $(shell which clang++) -c -o
CFLAGS+= -std=c++17
CFLAGS+= -O0 -g
CFLAGS+= -D_NAME_=\"$(NAME)\" -D_VERS_=\"$(VERS)\"

## linker

LMAKER?= $(shell which clang++) -o

# shell

SHSU:= $(shell which sudo)
SHCO:= $(shell which chown) -R $(USER):$(USER)
SHCM:= $(shell which chmod) -R
SHCP:= $(shell which cp) -riv
SHRM:= $(shell which rm) -rfv
SHMV:= $(shell which mv) -iv
SHMD:= $(shell which mkdir) -p
SHDB:= $(shell which lldb)

# rules

## internal

build: build-head $(SLOBJ) $(SLBIN)
build-head:
	$(info "--[[build]]--")

clean: clean-head
	$(SHRM) $(SLOBJ) $(SLBIN)
clean-head:
	$(info "--[[clean]]--")

## external

setup: setup-head $(TLBIN) $(TLMAN)
setup-head:
	$(info "--[=[setup]=]--")

reset: reset-head
	$(SHRM) $(TLBIN) $(TLMAN)
reset-head:
	$(info "--[=[reset]=]--")

## addition

again: again-head clean build
again-head:
	$(info "--[=[again]=]--")

start: start-head build
	$(SLBIN)
start-head:
	$(info "--[=[start]=]--")

rerun: rerun-head clean build
	$(SLBIN)
rerun-head:
	$(info "--[=[rerun]=]--")

debug: debug-head clean build
	$(SHDB) $(SLBIN)
debug-head:
	$(info "--[=[debug]=]--")

print: print-head
	$(info "--[=[basic]=]--")
	$(info "[NAME]=$(NAME)")
	$(info "[VERS]=$(VERS)")
	$(info "--[=[files]=]--")
	$(info "--[[source]]--")
	$(info "[SLSRC]=$(SLSRC)")
	$(info "[SLOBJ]=$(SLOBJ)")
	$(info "[SLBIN]=$(SLBIN)")
	$(info "--[[target]]--")
	$(info "[TROOT]=$(TROOT)")
	$(info "[TDBIN]=$(TDBIN)")
	$(info "[TDMAN]=$(TDMAN)")
	$(info "[TLBIN]=$(TLBIN)")
	$(info "[TLMAN]=$(TLMAN)")
	$(info "--[=[build]=]--")
	$(info "--[[compiler]]--")
	$(info "[CMAKER]=$(CMAKER)")
	$(info "[CFLAGS]=$(CFLAGS)")
	$(info "--[[linker]]--")
	$(info "[LMAKER]=$(LMAKER)")
	$(info "[LFLAGS]=$(LFLAGS)")
	$(info "--[=[shell]=]--")
	$(info "[SHSU]=$(SHSU)")
	$(info "[SHCO]=$(SHCO)")
	$(info "[SHCM]=$(SHCM)")
	$(info "[SHCP]=$(SHCP)")
	$(info "[SHRM]=$(SHRM)")
	$(info "[SHMV]=$(SHMV)")
	$(info "[SHMD]=$(SHMD)")
	$(info "[SHDB]=$(SHDB)")
	$(info "--[=[rules]=]--")
	$(info "--[build]--")
	$(info "link binary file from object code compiled from source code")
	$(info "--[clean]--")
	$(info "remove compiled object code and linked binary file")
	$(info "--[setup]--")
	$(info "copy binary and manual files into the system")
	$(info "--[reset]--")
	$(info "remove binary and manual files from the system")
	$(info "--[again]--")
	$(info "clean and rebuild the project again")
	$(info "--[start]--")
	$(info "build and run the binary file")
	$(info "--[rerun]--")
	$(info "clean, rebuild and run the binary file with the shell")
	$(info "--[debug]--")
	$(info "clean, rebuild and run the binary file with the debugger")
print-head:
	$(info "--[==[print]==]--")

## source

%.cxx:
	$(info "[source]=$@")

%.obj: %.cxx
	$(info "[object]=$@")
	$(CMAKER) $@ $^ $(CFLAGS)

%.bin: $(SLOBJ)
	$(info "[source-binary]=$@")
	$(LMAKER) $@ $^ $(LFLAGS)

%.man:
	$(info "[source-manual]=$@")

## target

$(TDBIN)/%: %.bin
	$(info "[target-binary]=$@")
	$(SHCP) $< $@
	$(SHCO) $@
	$(SHCM) 744 $@

$(TDMAN)/%: %.man
	$(info "[target-manual]=$@")
	$(SHCP) $< $@
	$(SHCM) 644 $@

# endof

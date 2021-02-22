IDIR =../include
CC=g++
CFLAGS=-I$(IDIR) -std=c++17

all: switch_basic switch_lambda switch_hooks switch_custom_state switch_permissioned

run_all: switch_basic switch_lambda switch_hooks switch_custom_state switch_permissioned run_switch_basic run_switch_lambda run_switch_hooks run_switch_custom_state run_switch_permissioned

dir:
	mkdir -p build

clean:
	rm -rf build


run_switch_basic: switch_basic
	@echo -e ""
	@echo -e "\u001b[33mRunning switch_basic\u001b[0m"
	@build/switch_basic
	@echo -e ""

run_switch_lambda: switch_lambda
	@echo -e ""
	@echo -e "\u001b[33mRunning switch_lambda\u001b[0m"
	@build/switch_lambda
	@echo -e ""

run_switch_hooks: switch_hooks
	@echo -e ""
	@echo -e "\u001b[33mRunning switch_hooks\u001b[0m"
	@build/switch_hooks
	@echo -e ""

run_switch_custom_state: switch_custom_state
	@echo -e ""
	@echo -e "\u001b[33mRunning switch_custom_state\u001b[0m"
	@build/switch_custom_state
	@echo -e ""

run_switch_permissioned: switch_permissioned
	@echo -e ""
	@echo -e "\u001b[33mRunning switch_permissioned\u001b[0m"
	@build/switch_permissioned
	@echo -e ""

switch_basic: dir
	$(CC) $(CFLAGS) switch_basic.cpp -o build/switch_basic

switch_lambda: dir
	$(CC) $(CFLAGS) switch_lambda.cpp -o build/switch_lambda

switch_hooks: dir
	$(CC) $(CFLAGS) switch_hooks.cpp -o build/switch_hooks

switch_custom_state: dir
	$(CC) $(CFLAGS) switch_custom_state.cpp -o build/switch_custom_state

switch_permissioned: dir
	$(CC) $(CFLAGS) switch_permissioned.cpp -o build/switch_permissioned

.PHONY: dir all run_all clean switch_basic switch_lambda switch_hooks switch_custom_state switch_permissioned run_switch_basic run_switch_lambda run_switch_hooks run_switch_custom_state run_switch_permissioned

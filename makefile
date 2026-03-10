# -------------------------------
# Paths
# -------------------------------
ENGINE_DIR   := engine
BUILD_NATIVE := $(ENGINE_DIR)/build/native-debug
BUILD_WASM   := $(ENGINE_DIR)/build/wasm-release
EMSDK_ENV    := $(ENGINE_DIR)/emsdk/emsdk_env.sh

# -------------------------------
# Tools
# -------------------------------
CMAKE := /usr/bin/cmake
CTEST := /usr/bin/ctest

# -------------------------------
# Phony targets
# -------------------------------
.PHONY: all native test wasm clean clean-native clean-wasm reconfigure-native reconfigure-wasm

all: native

# -------------------------------
# Native build
# -------------------------------
native:
	cd $(ENGINE_DIR) && $(CMAKE) --preset native-debug
	cd $(ENGINE_DIR) && $(CMAKE) --build --preset native-debug

# -------------------------------
# Native tests
# -------------------------------
test: native
	cd $(ENGINE_DIR) && $(CTEST) --preset native-debug

# -------------------------------
# WASM build
# -------------------------------
wasm:
	@test -f "$(EMSDK_ENV)" || (echo "ERROR: $(EMSDK_ENV) not found" && exit 1)
	cd $(ENGINE_DIR) && bash -c "source emsdk/emsdk_env.sh && $(CMAKE) --preset wasm-release && $(CMAKE) --build --preset wasm-release"

# -------------------------------
# Reconfigure from scratch
# -------------------------------
reconfigure-native:
	rm -rf $(BUILD_NATIVE)
	cd $(ENGINE_DIR) && $(CMAKE) --preset native-debug

reconfigure-wasm:
	rm -rf $(BUILD_WASM)
	@test -f "$(EMSDK_ENV)" || (echo "ERROR: $(EMSDK_ENV) not found" && exit 1)
	cd $(ENGINE_DIR) && bash -c "source emsdk/emsdk_env.sh && $(CMAKE) --preset wasm-release"

# -------------------------------
# Cleaning
# -------------------------------
clean-native:
	rm -rf $(BUILD_NATIVE)

clean-wasm:
	rm -rf $(BUILD_WASM)

clean:
	rm -rf $(ENGINE_DIR)/build
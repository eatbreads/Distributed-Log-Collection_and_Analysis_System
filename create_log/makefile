# 设置构建目录
BUILD_DIR = build

# vcpkg 目录（请根据实际路径修改）
VCPKG_TOOLCHAIN_FILE = /root/project/vcpkg/scripts/buildsystems/vcpkg.cmake

# 默认目标
all:
	@cmake -S . -B $(BUILD_DIR) -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN_FILE)
	@cmake --build $(BUILD_DIR) -- -j$(shell nproc)

# 清理构建目录
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean

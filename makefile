COMPILER = cl
LINKER = link

COMMON_FLAGS = -nologo
COMPILER_FLAGS = /FC /Z7 /EHsc

LIBS = user32.lib D2d1.lib Dwrite.lib

BUID_DIR = build
CODE_DIR = code

setup_dirs:
	@if not exist $(BUID_DIR) mkdir $(BUID_DIR)

clean:
	rd /s /q $(BUID_DIR)

run:
	run.bat

main: setup_dirs kninja
	cd $(BUID_DIR)
	$(LINKER) $(COMMON_FLAGS) /DEBUG \
	KNinja.obj $(LIBS) \
	/Fe:KNinja.exe

kninja: setup_dirs $(CODE_DIR)/KNinja.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/KNinja.cpp /Fo:$(BUID_DIR)/KNinja.obj


tests: setup_dirs tests.obj
	cd $(BUID_DIR)
	$(LINKER) $(COMMON_FLAGS) /DEBUG tests.obj /Fe:Tests.exe

tests.obj: setup_dirs $(CODE_DIR)/Tests.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/Tests.cpp /Fo:$(BUID_DIR)/Tests.obj

test:
	build\Tests.exe
COMPILER = cl
LINKER = link

COMMON_FLAGS = -nologo
COMPILER_FLAGS = /FC /Z7 /EHsc

LIBS = user32.lib D2d1.lib Dwrite.lib Winmm.lib Ole32.lib 

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
	KNinja.obj FileIO.obj ErrorMessage.obj Shapes.obj Keyboard.obj Image.obj $(LIBS) \
	/Fe:KNinja.exe

kninja: setup_dirs $(CODE_DIR)/KNinja.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/KNinja.cpp /Fo:$(BUID_DIR)/KNinja.obj

fileio: setup_dirs $(CODE_DIR)/FileIO.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/FileIO.cpp /Fo:$(BUID_DIR)/FileIO.obj

error: setup_dirs $(CODE_DIR)/ErrorMessage.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/ErrorMessage.cpp /Fo:$(BUID_DIR)/ErrorMessage.obj

shapes: setup_dirs $(CODE_DIR)/Shapes.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/Shapes.cpp /Fo:$(BUID_DIR)/Shapes.obj

keyboard: setup_dirs $(CODE_DIR)/Keyboard.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/Keyboard.cpp /Fo:$(BUID_DIR)/Keyboard.obj

image: setup_dirs $(CODE_DIR)/Image.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/Image.cpp /Fo:$(BUID_DIR)/Image.obj

modules: fileio error shapes keyboard image


tests: setup_dirs tests.obj fileio error shapes keyboard image
	cd $(BUID_DIR)
	$(LINKER) $(COMMON_FLAGS) /DEBUG \
	Tests.obj FileIO.obj ErrorMessage.obj Shapes.obj Keyboard.obj Image.obj \
	/Fe:Tests.exe

tests.obj: setup_dirs $(CODE_DIR)/Tests.cpp
	$(COMPILER) $(COMMON_FLAGS) $(COMPILER_FLAGS) -c $(CODE_DIR)/Tests.cpp /Fo:$(BUID_DIR)/Tests.obj

test:
	build\Tests.exe

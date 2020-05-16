SOURCE = main.tex
TARGET = main.pdf
PACKAGE_FOLDER = utilities

DEPENDENCIES = $(shell find . -name "*.tex")
DEPENDENCIES += $(shell find $(PACKAGE_FOLDER)/ -name "*")
DEPENDENCIES += ${wildcard *.bib}
SOURCE_BASE = $(basename $(SOURCE))
TEMPORARY = $(addsuffix .pdf,$(SOURCE_BASE))
SOURCE_CACHE = $(filter-out ./$(SOURCE) ./$(TARGET),$(shell find . -maxdepth 1 -name "$(SOURCE_BASE)*"))
SOURCE_CACHE := $(addsuffix ",$(addprefix ",$(SOURCE_CACHE)))

$(info $(DEPENDENCIES))
$(info $(SOURCE_CACHE))

.PHONY: all clean clear_cache

all: $(TARGET)

$(TARGET): $(DEPENDENCIES)
	$(MAKE) -C $(PACKAGE_FOLDER)
	latexmk -f -pdf -cd -latexoption="-synctex=1" $(SOURCE)
ifneq ($(TARGET),$(TEMPORARY))
	mv $(TEMPORARY) $(TARGET)
endif

clean: clear_cache
	rm -f $(TARGET)

clear_cache:
	rm -f $(SOURCE_CACHE)
.PHONY: install

FILES = $(wildcard *.sty)
FILES += $(wildcard *.cls)
TEX_DIR = $(HOME)/texmf/tex/latex
COPY_FILES = $(addprefix $(TEX_DIR)/, $(FILES))

install: $(COPY_FILES)

$(TEX_DIR)/%.sty: %.sty $(TEX_DIR)
	cp $< $@

$(TEX_DIR)/%.cls: %.cls $(TEX_DIR)
	cp $< $@

$(TEX_DIR):
	mkdir -p $@
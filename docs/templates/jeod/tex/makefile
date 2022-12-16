################################################################################
#
# This is the standard makefile for building the LaTeX-based documentation
# for a JEOD model.
#
# To get a desription of the arguments accepted by this makefile, type
# 'make help'
#
# To bootstrap the document build process type
# '$JEOD_HOME/bin/jeod/build_document'
#
#
# Tailoring:
# This makefile is not intended to be modified. It instead includes two make
# include files, 'model_name.mk' and 'overrides.mk'.
#
# The first of the two, model_name.mk, must exist. This file is included just
# just after defining the default make target and must define the make
# variables MODEL_TYPE, MODEL_NAME, and MODEL_TITLE.
# The entire model_name.mk file for the memory model is
#   MODEL_TYPE  = utils
#   MODEL_NAME  = memory
#   MODEL_TITLE = 'Memory Model'
#
# The makefile also includes the file 'overrides.mk', if it exists. This file
# is included just before the final dependencies are specified. You can override
# definitions in this file or create new targets. You can hook your newly-
# defined targets to the build process by specifying those targets as the
# value of the make variable EXTRA_DEPENDS.
#
#
# Path assumptions:
# This makefile assumes the following executables are in your path:
#   pdflatex
#   bibtex
#   perl
#   make
#
################################################################################


####################
## Non Parallel.   #
####################

.NOTPARALLEL:


####################
## Default target. #
####################

default: all


#
# Build the standard documentation files from the model name.
#
# DOCUMENTS  = dynenv notes # notes.tex is not ready for prime time
DOCUMENTS  = dynenv


######################################################
# Specify template, source and document directories. #
######################################################
TEMPLATE_DIR = ${JEOD_HOME}/docs/templates/jeod

# INST_DIR: directory in which the generated pdf files will be installed
INST_DIR = ..

# PDF_DIR: directory in which the generated pdf files will be tested.
# The install target changes this to INST_DIR
PDF_DIR  = .


######################################
# Define commands and command paths. #
######################################
PDFLATEX = $(BIN_DIR)pdflatex -output-format pdf


###########################################
# Derive the document product file names. #
###########################################
# Dynamics environment LaTeX style, bibliography, perl scripts, etc.
# (listed alphabetically)
JEOD_LATEX_FILES = \
	dyncover.sty \
	dynenv.bib \
	dynenv.sty \
	dynenvboilerplate.sty \
	dynenvcode.sty \
	dynenvlinks.sty \
	dynenvmatter.sty \
	dynenvreqt.sty \
	dynenvupfront.sty \
	dynmath.sty \
	jeodspec.sty \
	ifvtex.sty \
	longtable.dvi \
	longtable.sty \
	make_labels.pl \
	Meatball.pdf \
	ps4pdf.sty \
	relpath.pl

# Model documenation style sheet
STYLE_SHEET = ${MODEL_NAME}.sty

# LaTeX file names are derived from the document names
TEX_FILES = $(addsuffix .tex,$(DOCUMENTS))

# Auxiliary information file names are derived from the document names
AUX_FILES = $(addsuffix .aux,$(DOCUMENTS))

# PDF file names are derived from the LaTeX file names
PDF_FILES = $(subst .tex,.pdf,$(TEX_FILES))


###################################
# Define the compilation targets. #
###################################


all: pdfs
	@echo " "
	@echo Generated documents: $(DOCUMENTS)

install: all
	@echo "Installing documents in $(INST_DIR)"
	@cp $(PDF_FILES) $(INST_DIR)

pdfs: links $(PDF_FILES)

$(DOCUMENTS): % : %.pdf
	@echo " "
	@echo Generated document: $@

#
# Generate the PDF files from the LaTeX files.
#
$(PDF_FILES): %.pdf : %.tex %.aux
	cat /dev/null | $(PDFLATEX) $(basename $(@F)).tex

force:
	cat /dev/null | $(PDFLATEX) $(MODEL_NAME).tex

# Force a recompile if the style sheet or any top-level files have changed.
$(PDF_FILES): $(JEOD_LATEX_FILES)

#
# The auxiliary files are needed by BibTeX to build .bbl files.
#
$(AUX_FILES): %.aux : %.tex links
	cat /dev/null | $(PDFLATEX) $(<F)
	rm $(basename $(@F)).pdf


#
# Link in the standard suite of scripts, style sheets, ...
#
links: $(JEOD_LATEX_FILES)
$(JEOD_LATEX_FILES):
	ln -s $(TEMPLATE_DIR)/$@ $@

################################
# Define the cleaning targets. #
################################
clean_aux:
	@echo Removing the LaTeX auxiliary files.
	rm -f *.aux *.bbl *.blg *.lof *.log *.lot *.out *.toc

clean_gen:
	@echo Auto-generated LaTeX files.
	rm -f coarse_metrics.tex metrix_metrics.tex inventory.tex

clean_links:
	@echo Removed linked dynamics environment LaTeX files.
	@rm -f $(JEOD_LATEX_FILES)

clean: clean_aux
	@echo Removed generated DVI files
	@rm -f *.dvi

clean_pdf:
	@echo Removing output PDF files
	@rm -f $(PDF_FILES)

real_clean: clean_links clean_gen clean clean_pdf
	@echo Removed all LaTeX and BibTeX produced files.

spotless: real_clean


###################################
# Define the information targets. #
###################################
help:
	@echo -e "\n\
Documentation Make Options:\n\
    make            - Processes all LaTeX document files:\n\
                      $(TEX_FILES)\n\
\n\
    make clean_aux  - Deletes all auxiliary files\n\
\n\
    make clean_gen  - Deletes the generated inventory and metrics files\n\
\n\
    make clean      - Deletes all auxiliary files and dvi files\n\
\n\
    make clean_pdf  - Deletes all PDF files\n\
\n\
    make coverage   - Generate the file code_coverage.tex\n\
\n\
    make install    - Install the document (default INST_DIR=../)\n\
\n\
    make real_clean - Deletes almost everything, leaving only the makefile\n\
\n\
    make spotless   - Deletes all linked and generated files.\n"



####################################################
# Include model-specific overrides, if they exist. #
####################################################

-include overrides.mk


##################################
# Specify document dependencies. #
##################################

$(AUX_FILES) : $(EXTRA_DEPENDS)
$(PDF_FILES) : $(AUX_FILES)

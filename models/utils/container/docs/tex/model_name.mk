# MODEL_TYPE  = utils
# MODEL_NAME  = container
# MODEL_TITLE = 'Container Model'


DOT_FILES = $(wildcard *.dot)

# Figures generated by dot
DOT_PDF_FILES = $(subst .dot,.pdf,$(DOT_FILES))

EXTRA_DEPENDS = $(DOT_PDF_FILES)

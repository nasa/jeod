
#
# Generate PDF figure files from the dot files.
#
$(DOT_PDF_FILES): %.pdf : %.dot
	perl ${JEOD_HOME}/dgh_scripts/make_dot_figure.pl $< $@

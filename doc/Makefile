OUTDIR = ../../build-lextype-Documentation/

.ONESHELL:
all:
	rm -f $(OUTDIR)*
	mkdir -p $(OUTDIR)
	cp * $(OUTDIR)
	cd $(OUTDIR)
	pdflatex *.tex
	bibtex *.aux
	pdflatex *.tex
	pdflatex *.tex
	make clean
	rm Makefile
	rm *.tex
	rm *.bib

clean:
	rm -f *\(busy\)
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.gz
	rm -f *.log
	rm -f *.out
	rm -f *.toc

remove:
	make clean
	rm -f *.pdf

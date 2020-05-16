OUTDIR = ../../build-lextype-Documentation/

.ONESHELL:
all:
	python3 listing.py
	rm -f $(OUTDIR)*
	mkdir -p $(OUTDIR)
	cp * $(OUTDIR)
	cp -r img $(OUTDIR)
	rm listing.tex
	cd $(OUTDIR)
	for f in *.tex; do pdflatex $$f; done
	for f in *.aux; do bibtex $$f; done
	for f in *.tex; do pdflatex $$f; done
	for f in *.tex; do pdflatex $$f; done
	make clean
	rm Makefile
	rm -r img
	rm *.tex
	rm *.bib
	rm *.py

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

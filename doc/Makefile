OUTDIR = ../../build-lextype-Documentation/

all:
	rm -f $(OUTDIR)*
	mkdir -p $(OUTDIR)
	pdflatex *.tex
	pdflatex *.tex
	bibtex *.aux | true
	pdflatex *.tex
	mv *.pdf $(OUTDIR)
	make clean

clean:
	rm -f *\(busy\)
	rm -f *.aux
	rm -f *.bbl
	rm -f *.blg
	rm -f *.gz
	rm -f *.log
	rm -f *.pdf
	rm -f *.toc


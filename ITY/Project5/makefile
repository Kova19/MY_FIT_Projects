########################################
# K suboru: proj5.tex                  #
# Datum:    03.05.2024                 #
# Autor:    Martin Kovacik (xkovacm01) #
# Projekt:  ITY projekt 5              #
########################################


PROJ=proj5

$(PROJ).pdf: $(PROJ).tex $(PROJ).bib
	pdflatex $(PROJ).tex
	bibtex $(PROJ)
	pdflatex $(PROJ).tex
	pdflatex $(PROJ).tex

clean:
	rm -f $(PROJ).aux $(PROJ).bbl $(PROJ).blg $(PROJ).dvi $(PROJ).log $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf
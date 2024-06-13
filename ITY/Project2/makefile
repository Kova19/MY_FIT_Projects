########################################
# K suboru: proj2.tex                  #
# Datum:    15.3.2024                  #
# Autor:    Martin Kovacik (xkovacm01) #
# Projekt:  ITY projekt 2              #
########################################

PROJ=proj2

$(PROJ).pdf: $(PROJ).tex
	pdflatex $(PROJ).tex
	pdflatex $(PROJ).tex

clean:
	rm -f $(PROJ).aux $(PROJ).log $(PROJ).out $(PROJ).toc

clean-all: clean
	rm -f $(PROJ).pdf
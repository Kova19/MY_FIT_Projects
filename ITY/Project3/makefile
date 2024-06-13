########################################
# K suboru: proj2.tex                  #
# Datum:    31.3.2024                  #
# Autor:    Martin Kovacik (xkovacm01) #
# Projekt:  ITY projekt 3              #
########################################

PROJ=proj3

$(PROJ).pdf: $(PROJ).tex
	latex $(PROJ).tex
	latex $(PROJ).tex
	dvips -t a4 $(PROJ).dvi
	ps2pdf -sPAPERSIZE=a4 $(PROJ).ps

clean:
	rm -f $(PROJ).aux $(PROJ).dvi $(PROJ).log $(PROJ).ps $(PROJ).out

clean-all: clean
	rm -f $(PROJ).pdf

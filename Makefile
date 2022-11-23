DIROBJ := obj/
DIREXE := exec/
DIRHEA := include/
DIRSRC := src/
DIRRESULTS := Results/

CFLAGS := -I$(DIRHEA) -c -Wall -std=c++11
LDLIBS := -lpthread -lrt
CC := g++

all : dirs manager

dirs:
	mkdir -p $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRRESULTS)

rm_results:
	rm -f $(DIRRESULTS)*

manager:  $(DIROBJ)manager.o $(DIROBJ)client.o $(DIROBJ)thread_searcher.o $(DIROBJ)request.o $(DIROBJ)searcher.o $(DIROBJ)payment_service.o
	$(CC) -o $(DIREXE)$@ $(DIROBJ)manager.o $(DIROBJ)searcher.o $(DIROBJ)client.o $(DIROBJ)request.o $(DIROBJ)thread_searcher.o $(DIROBJ)payment_service.o $(LDLIBS)

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CC) $(CFLAGS) $^ -o $@

test_1: rm_results
	./$(DIREXE)manager 4 Libros/17-LEYES-DEL-TRABJO-EN-EQUIPO.txt

test_2: rm_results
	./$(DIREXE)manager 10 Libros/ACTITUD-DE-VENDEDOR.txt

test_3: rm_results
	./$(DIREXE)manager 50 Libros/La-última-sirena.txt


clean : rm_results
	rm -rf *~ core $(DIROBJ) $(DIREXE) $(DIRDEBUG) $(DIRHEA)*~ $(DIRSRC)*~

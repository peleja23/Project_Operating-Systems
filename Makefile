CC = gcc
CFLAGS = -Wall -g

all: sort stats report

sort: my_sort.o sort.o
stats: my_stats.o stats.o
report: my_report.o report.o

sort.o: sort.c
stats.o: stats.c
report.o: report.c

my_sort.o: my_sort.c my_sort.h def_struct.h
my_stats.o: my_stats.c my_stats.h def_struct.h
my_report.o: my_report.c my_report.h def_struct.h

clean:
	rm -f *.o sort stats report
CC = gcc
CFLAGS = -Wall -g

generate_sensor_data: generate_sensor_data.o main.o

generate_sensor_data.o: generate_sensor_data.c generate_sensor_data.h

main.o: main.c

clean:
	rm *.o generate_sensor_data

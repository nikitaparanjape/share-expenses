project: project.o
  cc project.c -o project
project.o: project.c project.h
  cc -c project.c

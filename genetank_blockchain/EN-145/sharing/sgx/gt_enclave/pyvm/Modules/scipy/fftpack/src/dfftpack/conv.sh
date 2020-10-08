#!bin/bash

CONV := /home/slowloris/f90toC/ff90

for f in ./*.f do
	$(CONV) <$f> $f.c
done
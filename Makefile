# Makefile for the bank program
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
CXXLINK = $(CXX)
OBJS = main.o Bank.o ATM.o LogManager.o Account.o Operation.o Helpers.o
TARGET = bank
RM = rm -f
# Creating the  executable
bank: $(OBJS)
	$(CXXLINK) -o bank $(OBJS)
# Creating the object files
main.o: main.cpp 
Bank.o: Bank.cpp Bank.hpp
ATM.o: ATM.cpp ATM.hpp
LogManager.o: LogManager.cpp LogManager.hpp
Account.o: Account.cpp Account.hpp
Operation.o: Operation.cpp Operation.hpp
Helpers.o: Helpers.cpp Helpers.hpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.*


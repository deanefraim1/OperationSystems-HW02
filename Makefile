# Makefile for the bank program
CXX = g++
CXXFLAGS = -g -Wall -std=c++11
CXXLINK = $(CXX)
OBJS = main.o Bank.o ATM.o LogManager.o Account.o Operation.o Helpers.o IThreadSafe.o
TARGET = bank
RM = rm -f
# Creating the  executable
bank: $(OBJS)
	$(CXXLINK) -o bank $(OBJS)
# Creating the object files
main.o: main.cpp 
Bank.o: Bank.cpp Bank.hpp
ATM.o: ATM.cpp ATM.hpp Bank.hpp Account.hpp Operation.hpp LogManager.hpp
LogManager.o: LogManager.cpp LogManager.hpp Helpers.hpp IThreadSafe.hpp
Account.o: Account.cpp Account.hpp IThreadSafe.hpp
Operation.o: Operation.cpp Operation.hpp
Helpers.o: Helpers.cpp Helpers.hpp LogManager.hpp ATM.hpp
IThreadSafe.o: IThreadSafe.cpp IThreadSafe.hpp Helpers.hpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ "#"* core.* log.txt


CC=g++
CFLAGS=-std=c++11
LibDir=libs
LibIncludeDir=$(LibDir)/include
OutDir=Release
BuildDir=$(OutDir)/build

CommonDir=Common
OpenMPDGravDir=OpenMPGrav
MPIGravDir=MPIGrav

CC1=$(CC) $(CFLAGS) -I $(CommonDir) -I $(LibIncludeDir) -L $(BuildDir) -O2

#OPENMP

OpenMPGrav: Common $(BuildDir)/openmpgrav.o $(OpenMPDGravDir)/OpenMPGrav.cpp
	$(CC1) -fopenmp $(OpenMPDGravDir)/OpenMPGrav.cpp $(BuildDir)/openmpgrav.o -lcommon -o $(OutDir)/OpenMPGrav

$(BuildDir)/openmpgrav.o: $(OpenMPDGravDir)/Simulator/GravSim.cpp
	$(CC1) -c -fopenmp $(OpenMPDGravDir)/Simulator/GravSim.cpp -o $(BuildDir)/openmpgrav.o

#MPI

MPIGrav: Common $(BuildDir)/mpigrav.o $(MPIGravDir)/MPIGrav.cpp
	$(CC1) -fopenmp $(MPIGravDir)/MPIGrav.cpp $(BuildDir)/mpigrav.o $(BuildDir)/mpicommon.o -lcommon -o $(OutDir)/MPIGrav

$(BuildDir)/mpigrav.o:
	$(CC1) -c -fopenmp $(MPIGravDir)/Simulator/MpiGravSim.cpp -o $(BuildDir)/mpigrav.o

$(BuildDir)/mpicommon.o:
	$(CC1) -c $(MPIGravDir)/mpicommon.cpp -o $(BuildDir)/mpicommon.o

#COMMON

Common: $(BuildDir)/reader.o $(BuildDir)/writer.o $(BuildDir)/commandlineparser.o
	ar rcs $(BuildDir)/libcommon.a $(BuildDir)/reader.o $(BuildDir)/writer.o $(BuildDir)/commandlineparser.o $(BuildDir)/frame.o


$(BuildDir)/reader.o: $(BuildDir)/frame.o $(CommonDir)/Reader/Reader.cpp
	$(CC1) -c $(CommonDir)/Reader/Reader.cpp -lframe -o $(BuildDir)/reader.o

$(BuildDir)/writer.o: $(BuildDir)/frame.o $(CommonDir)/Writer/Writer.cpp
	$(CC1) -c $(CommonDir)/Writer/Writer.cpp -lframe -o $(BuildDir)/writer.o

$(BuildDir)/commandlineparser.o: $(CommonDir)/SimParams/CommandLineParser.cpp
	$(CC1) -c $(CommonDir)/SimParams/CommandLineParser.cpp -o $(BuildDir)/commandlineparser.o

$(BuildDir)/frame.o: $(CommonDir)/Frame.cpp
	$(CC1) -c $(CommonDir)/Frame.cpp -o $(BuildDir)/frame.o
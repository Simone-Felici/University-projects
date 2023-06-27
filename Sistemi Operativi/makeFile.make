#Variabili dei path
SRC_PATH = src/
INCLUDE_PATH = include/
BIN_DIR = bin/
OBJ_DIR = obj/
CFLAGS = -I$(INCLUDE_PATH)

#Obbietto per compilare il progetto
all: LibrariesObj SourceObj Link

#Prima vengono compilate le librerie e le loro implementazioni (e creati i file oggetto)
LibrariesObj: stringCheckingObj SocketServerObj SocketClientObj logManagementObj executabeObj fileManagement ecuUtilsObj 

stringCheckingObj: $(SRC_PATH)stringChecking.c $(INCLUDE_PATH)stringChecking.h
	cc -c $(SRC_PATH)stringChecking.c $(CFLAGS) -o $(OBJ_DIR)/stringChecking.o

logManagementObj: $(SRC_PATH)logManagement.c $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)logManagement.c $(CFLAGS) -o $(OBJ_DIR)/logManagement.o

SocketServerObj: $(SRC_PATH)socketServer.c $(INCLUDE_PATH)socketServer.h $(INCLUDE_PATH)stringChecking.h
	cc -c $(SRC_PATH)socketServer.c $(CFLAGS) -o $(OBJ_DIR)/socketServer.o

SocketClientObj: $(SRC_PATH)socketClient.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)stringChecking.h
	cc -c $(SRC_PATH)socketClient.c $(CFLAGS) -o $(OBJ_DIR)/socketClient.o

executabeObj: $(SRC_PATH)executable.c $(INCLUDE_PATH)executable.h
	cc -c $(SRC_PATH)executable.c $(CFLAGS) -o $(OBJ_DIR)/executable.o
		
fileManagement:$(SRC_PATH)fileManagement.c $(INCLUDE_PATH)fileManagement.h  $(INCLUDE_PATH)socketClient.h  $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)fileManagement.c $(CFLAGS) -o $(OBJ_DIR)/fileManagement.o
	
ecuUtilsObj:$(SRC_PATH)ecuUtils.c $(INCLUDE_PATH)ecuUtils.h  $(INCLUDE_PATH)socketServer.h  $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)ecuUtils.c $(CFLAGS) -o $(OBJ_DIR)/ecuUtils.o


	
#Poi vengono compilati i file sorgente con le relative dipendenze dai file .h (e creati i file oggetto)
SourceObj: EcuObj InObj OutObj CameraObj BreakObj ThrottleObj SteerObj RadarOBJ parkOBJ surroundOBJ

EcuObj: $(SRC_PATH)Ecu.c $(INCLUDE_PATH)socketServer.h $(INCLUDE_PATH)logManagement.h $(INCLUDE_PATH)executable.h $(INCLUDE_PATH)ecuUtils.h
	cc -c $(SRC_PATH)Ecu.c $(CFLAGS) -o $(OBJ_DIR)/Ecu.o

InObj: $(SRC_PATH)In.c $(INCLUDE_PATH)socketClient.h
	cc -c $(SRC_PATH)In.c $(CFLAGS) -o $(OBJ_DIR)/In.o

OutObj: $(SRC_PATH)Out.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)stringChecking.h
	cc -c $(SRC_PATH)Out.c $(CFLAGS) -o $(OBJ_DIR)/Out.o

CameraObj: $(SRC_PATH)frontWindShieldCamera.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h $(INCLUDE_PATH)fileManagement.h
	cc -c $(SRC_PATH)frontWindShieldCamera.c $(CFLAGS) -o $(OBJ_DIR)/frontWindShieldCamera.o
	
BreakObj: $(SRC_PATH)brakeByWire.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)brakeByWire.c $(CFLAGS) -o $(OBJ_DIR)/brakeByWire.o

ThrottleObj: $(SRC_PATH)throttleControl.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)throttleControl.c $(CFLAGS) -o $(OBJ_DIR)/throttleControl.o
	
SteerObj: $(SRC_PATH)steerByWire.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h
	cc -c $(SRC_PATH)steerByWire.c $(CFLAGS) -o $(OBJ_DIR)/steerByWire.o
	
RadarOBJ:$(SRC_PATH)forwardFacingRadar.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h $(INCLUDE_PATH)fileManagement.h
	cc -c $(SRC_PATH)forwardFacingRadar.c $(CFLAGS) -o $(OBJ_DIR)/forwardFacingRadar.o
	
parkOBJ: $(SRC_PATH)parkAssist.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)socketServer.h $(INCLUDE_PATH)logManagement.h $(INCLUDE_PATH)fileManagement.h $(INCLUDE_PATH)executable.h
	cc -c $(SRC_PATH)parkAssist.c $(CFLAGS) -o $(OBJ_DIR)/parkAssist.o
	
surroundOBJ: $(SRC_PATH)surroundViewCamera.c $(INCLUDE_PATH)socketClient.h $(INCLUDE_PATH)logManagement.h $(INCLUDE_PATH)fileManagement.h
	cc -c $(SRC_PATH)surroundViewCamera.c $(CFLAGS) -o $(OBJ_DIR)/surroundViewCamera.o
	
	
#Infine vengono linkati i file oggetto 
Link:
	cc $(OBJ_DIR)/Ecu.o $(OBJ_DIR)/socketServer.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o $(OBJ_DIR)/executable.o $(OBJ_DIR)/ecuUtils.o -o $(BIN_DIR)/Ecu
	cc $(OBJ_DIR)/In.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o -o $(BIN_DIR)/In
	cc $(OBJ_DIR)/Out.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o -o $(BIN_DIR)/Out
	cc $(OBJ_DIR)/frontWindShieldCamera.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o $(OBJ_DIR)/fileManagement.o -o $(BIN_DIR)/frontWindShieldCamera
	cc $(OBJ_DIR)/brakeByWire.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o -o $(BIN_DIR)/brakeByWire
	cc $(OBJ_DIR)/throttleControl.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o -o $(BIN_DIR)/throttleControl
	cc $(OBJ_DIR)/steerByWire.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o -o $(BIN_DIR)/steerByWire
	cc $(OBJ_DIR)/forwardFacingRadar.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o $(OBJ_DIR)/fileManagement.o -o $(BIN_DIR)/forwardFacingRadar
	cc $(OBJ_DIR)/parkAssist.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/socketServer.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/executable.o $(OBJ_DIR)/logManagement.o $(OBJ_DIR)/fileManagement.o -o $(BIN_DIR)/parkAssist
	cc $(OBJ_DIR)/surroundViewCamera.o $(OBJ_DIR)/socketClient.o $(OBJ_DIR)/stringChecking.o $(OBJ_DIR)/logManagement.o $(OBJ_DIR)/fileManagement.o -o $(BIN_DIR)/surroundViewCamera

#Obbietto per avviare l'esecuzione del progetto
run:
	gnome-terminal -- $(BIN_DIR)"/Out"
	$(BIN_DIR)/Ecu ARTIFICIALE
	
#Obbietto per organizzare i file del progetto
install:
	mkdir include
	mkdir src
	mkdir bin
	mkdir obj
	mkdir log
	mkdir bin/fileUtils
	mv *.c src
	mv *.h include
	mv frontCamera.data bin/fileUtils
	mv urandomARTIFICIALE.binary bin/fileUtils

#Obbietto per pulire la directory del progetto
clean:
	rm -r log
	rm -r include
	rm -r src
	rm -r bin
	rm -r obj
	rm ECU
	rm ParkAssist
	

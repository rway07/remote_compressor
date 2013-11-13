CC=g++
CFLAGS= -Wall -c -MMD -MP -MF $@.d
SERVER_FLAGS= -pthread

CLIENT_DIR=compressor-client
SERVER_DIR=compressor-server
COMMON_DIR=common
HASHLIB_DIR=hashlib2plus

CLIENT_SOURCES= ${CLIENT_DIR}/main.o ${CLIENT_DIR}/paramParser.o ${CLIENT_DIR}/shell.o

CLIENT_COMMON=  \
		${COMMON_DIR}/exception.o \
		${COMMON_DIR}/configuration.o \
		${COMMON_DIR}/fileHandler.o \
		${COMMON_DIR}/network.o \
		${COMMON_DIR}/client.o

SERVER_SOURCES= \
		${SERVER_DIR}/main.o \
		${SERVER_DIR}/paramParser.o \
		${SERVER_DIR}/commandHandler.o \
		${SERVER_DIR}/compressor.o \
		${SERVER_DIR}/server.o \
		${SERVER_DIR}/threadHelper.o

SERVER_COMMON=  \
		${COMMON_DIR}/exception.o \
		${COMMON_DIR}/configuration.o \
		${COMMON_DIR}/serverSideConfiguration.o \
		${COMMON_DIR}/fileHandler.o \
		${COMMON_DIR}/network.o

MD5_HEADERS= ${COMMON_DIR}/hl_md5wrapper.o ${COMMON_DIR}/hl_md5.o
MD5_LIB= ${COMMON_DIR}/${HASHLIB_DIR}/libhl++.a

CLIENT_OUT=client
SERVER_OUT=server

default: all

all:	client server
	
client:	${CLIENT_COMMON} ${MD5_HEADERS} ${CLIENT_SOURCES}
	@${CC} -o ${CLIENT_OUT} ${CLIENT_COMMON} ${MD5_HEADERS} ${MD5_LIB} ${CLIENT_SOURCES}			
	@echo [CC] $@
	@echo "[--] compressor-client compiled!!"

server: ${SERVER_COMMON} ${MD5_HEADERS} ${SERVER_SOURCES}
	@${CC} ${SERVER_FLAGS} -o ${SERVER_OUT} ${SERVER_COMMON} ${MD5_HEADERS} ${MD5_LIB} ${SERVER_SOURCES}
	@echo [CC] $@
	@echo "[--] compressor-server compiled!!"

.PHONY: clean
clean:
	@echo "Cleaning...."
	-@rm ${COMMON_DIR}/*.o 2>/dev/null || true
	-@rm ${COMMON_DIR}/*.o.d 2>/dev/null || true
	-@rm ${CLIENT_DIR}/*.o 2>/dev/null || true
	-@rm ${CLIENT_DIR}/*.o.d 2>/dev/null || true
	-@rm ${SERVER_DIR}/*.o 2>/dev/null || true
	-@rm ${SERVER_DIR}/*.o.d 2>/dev/null || true
	-@rm client server 2>/dev/null || true
	@echo "Done!"	
	
${COMMON_DIR}/network.o: ${COMMON_DIR}/network.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/network.o ${COMMON_DIR}/network.cpp
	@echo [CC] $@
	
${COMMON_DIR}/client.o: ${COMMON_DIR}/client.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/client.o ${COMMON_DIR}/client.cpp
	@echo [CC] $@
	
${COMMON_DIR}/configuration.o: ${COMMON_DIR}/configuration.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/configuration.o ${COMMON_DIR}/configuration.cpp
	@echo [CC] $@
	
${COMMON_DIR}/serverSideConfiguration.o: ${COMMON_DIR}/serverSideConfiguration.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/serverSideConfiguration.o ${COMMON_DIR}/serverSideConfiguration.cpp
	@echo [CC] $@
	
${COMMON_DIR}/exception.o: ${COMMON_DIR}/exception.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/exception.o ${COMMON_DIR}/exception.cpp
	@echo [CC] $@
	
${COMMON_DIR}/fileHandler.o: ${COMMON_DIR}/fileHandler.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/fileHandler.o ${COMMON_DIR}/fileHandler.cpp
	@echo [CC] $@
	
${COMMON_DIR}/hl_md5wrapper.o: ${COMMON_DIR}/${HASHLIB_DIR}/hl_md5wrapper.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/hl_md5wrapper.o ${COMMON_DIR}/${HASHLIB_DIR}/hl_md5wrapper.cpp
	@echo [CC] $@
	
${COMMON_DIR}/hl_md5.o: ${COMMON_DIR}/${HASHLIB_DIR}/hl_md5.cpp
	@${CC} ${CFLAGS} -o ${COMMON_DIR}/hl_md5.o ${COMMON_DIR}/${HASHLIB_DIR}/hl_md5.cpp
	@echo [CC] $@
	
${CLIENT_DIR}/main.o: ${CLIENT_DIR}/main.cpp
	@${CC} ${CFLAGS} -o ${CLIENT_DIR}/main.o ${CLIENT_DIR}/main.cpp
	@echo [CC] $@
	
${CLIENT_DIR}/paramParser.o: ${CLIENT_DIR}/paramParser.cpp
	@${CC} ${CFLAGS} -o ${CLIENT_DIR}/paramParser.o ${CLIENT_DIR}/paramParser.cpp
	@echo [CC] $@
	
${CLIENT_DIR}/shell.o: ${CLIENT_DIR}/shell.cpp
	@${CC} ${CFLAGS} -o ${CLIENT_DIR}/shell.o ${CLIENT_DIR}/shell.cpp
	@echo [CC] $@
	
${SERVER_DIR}/main.o: ${SERVER_DIR}/main.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/main.o ${SERVER_DIR}/main.cpp
	@echo [CC] $@
	
${SERVER_DIR}/paramParser.o: ${SERVER_DIR}/paramParser.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/paramParser.o ${SERVER_DIR}/paramParser.cpp
	@echo [CC] $@
	
${SERVER_DIR}/commandHandler.o: ${SERVER_DIR}/commandHandler.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/commandHandler.o ${SERVER_DIR}/commandHandler.cpp
	@echo [CC] $@
	
${SERVER_DIR}/compressor.o: ${SERVER_DIR}/compressor.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/compressor.o ${SERVER_DIR}/compressor.cpp
	@echo [CC] $@
	
${SERVER_DIR}/server.o: ${SERVER_DIR}/server.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/server.o ${SERVER_DIR}/server.cpp
	@echo [CC] $@
	
${SERVER_DIR}/threadHelper.o: ${SERVER_DIR}/threadHelper.cpp
	@${CC} ${CFLAGS} -o ${SERVER_DIR}/threadHelper.o ${SERVER_DIR}/threadHelper.cpp
	@echo [CC] $@

CFLAGS =
LDFLAGS = -lmysqlclient
CC = gcc

BIN = connect dbop tool stmt multsql time tran

all:$(BIN)

%.o:%.c
	$(CC) $(CFLAGS)  -c $<  -o   $@
connect:connect.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
dbop:dbop.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
tool:tool.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
stmt:stmt.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
multsql:multsql.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
time:time.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
tran:tran.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o  $@
clean:
	-rm -f *.o $(BIN) 

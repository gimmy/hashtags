CC= clang #gcc
CFLAGS=-std=c99
#CFLAGS=-O0 -g -std=c99		# for debug
#CFLAGS=-O0 -g -DDEBUG -std=c99	# for verbose debug
LDFLAGS=
SOURCE=main.c parser.c array.c implic.c utf8.c
HEADER=def.h utf8.h #filter.h
OBJECTS= $(SOURCE:.c=.o)
EXECUTABLE=main
TMPFILES= *~ a.out

# output color:
#COLOR="\033["
WHITE="\033[38;1m"
GREEN="\033[32;1m"
CLOSE="\033[0m"

# prefixes:
WORK=$(GREEN)"::"$(CLOSE)
INFO=$(GREEN)" >"$(CLOSE)

all: $(EXECUTABLE)
	@echo $(INFO)$(CLOSE) $(WHITE)"Compiled"$(CLOSE)

$(EXECUTABLE): $(OBJECTS)
	@echo -n $(WORK) $(WHITE)"Linking..."$(CLOSE)"\t"; $(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@echo -n  "\t"$(GREEN)[$(CLOSE)$(WHITE)"Done"$(CLOSE)$(GREEN)]$(CLOSE)

%.o: %.c $(HEADER)
	@echo -n $(WORK) $(WHITE)"Compiling" $< $(CLOSE); $(CC) $(CFLAGS) -c $< -o $@
	@echo  "\t"$(GREEN)[$(CLOSE)$(WHITE)"Done"$(CLOSE)$(GREEN)]$(CLOSE)

clean:
	@echo $(WORK) $(WHITE)"Clean useless files"$(CLOSE)
	@rm -f $(TMPFILES) $(OBJECTS) $(EXECUTABLE)
run: $(EXECUTABLE)
	@echo $(INFO)$(CLOSE) $(WHITE)"Run ./"$(EXECUTABLE)$(CLOSE)
	@./$(EXECUTABLE) ../it.json

SRC_DIR=source

all:

%:
	make -C $(SRC_DIR) $@

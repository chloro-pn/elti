.PHONY : all
all : 
	$(MAKE) -C ./third_party/varint;\
	$(MAKE) -C ./src;\
	$(MAKE) -C ./examples;

.PHONY : clean
clean : 
	$(MAKE) -C ./third_party/varint clean;\
	$(MAKE) -C ./src clean;\
	$(MAKE) -C ./examples clean;

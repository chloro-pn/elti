.PHONY : all
all : 
	$(MAKE) -C ./third_party/varint;\
	$(MAKE) -C ./src;\
	$(MAKE) -C ./examples;\
	$(MAKE) -C ./test;

.PHONY : clean
clean : 
	$(MAKE) -C ./third_party/varint clean;\
	$(MAKE) -C ./src clean;\
	$(MAKE) -C ./examples clean;\
	$(MAKE) -C ./test clean;


.PHONY : test
test :
	test/elti_test

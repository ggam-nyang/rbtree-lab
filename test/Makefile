.PHONY: test

CFLAGS=-I ../src -Wall -g

test: my-test-rbtree
	./my-test-rbtree

my-test-rbtree: my-test-rbtree.o ../src/rbtree.o

../src/rbtree.o:
	$(MAKE) -C ../src rbtree.o

clean:
	rm -f my-test-rbtree *.o

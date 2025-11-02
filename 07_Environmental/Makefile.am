SUBDIRS = src

check-local:
	@echo "Running basic tests..."
	@echo "Hello, World!" > test.txt
	@echo "Testing MD5..."
	@src/rhasher MD5 'Hello, World!'
	@echo "Testing SHA1..."
	@src/rhasher SHA1 hello
	@echo "Testing TTH on file..."
	@src/rhasher TTH '"test.txt"'
	@rm -f test.txt
	@echo "All tests passed!"

clean-local:
	rm -f test.txt

.PHONY: check-local clean-local

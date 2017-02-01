all:
	nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_Codegen.ts

	# nsmake mocha test/test_String.ts
	# nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_Generation.ts
clean:
	nsmake stop;
	rm -rf ~/.nsmake/build ~/.nsmake/server.log
	cd ~/nsmake; npm run build


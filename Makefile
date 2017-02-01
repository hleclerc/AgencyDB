all:
	nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_LvArray.ts

	# nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_Generation.ts
	# nsmake mocha test/test_String.ts
clean:
	nsmake stop;
	rm -rf ~/.nsmake/build ~/.nsmake/server.log
	cd ~/nsmake; npm run build


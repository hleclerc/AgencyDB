all:
	nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_LvMap.ts

	# nsmake mocha test/test_String.ts
clean:
	nsmake stop;
	rm -rf ~/.nsmake/build ~/.nsmake/server.log
	cd ~/nsmake; npm run build


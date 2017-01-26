all:
	nsmake mocha test/test_Codegen.ts

	# nsmake mocha test/test_String.ts
clean:
	nsmake stop;
	rm -rf ~/.nsmake/build ~/.nsmake/server.log
	cd ~/nsmake; npm run build


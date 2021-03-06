all: 
	nsmake run -o agencydb_tracker Tracker/main_tracker.cpp

tests:
	nsmake mocha test/test_*.ts

	# nsmake mocha test/test_LvDirectory.ts
cg:
	nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_Codegen.ts

# nsmake mocha -DNO_OPERATIONAL_TRANSFORM test/test_Generation.ts
clean:
	nsmake stop;
	rm -rf ~/.nsmake/build
	truncate -s 0 ~/.nsmake/server.log
	cd ~/nsmake; npm run build

pull:
	cd ext/Evel; git pull
	cd ext/Hpipe; git pull
	cd ext/PrepArg; git pull
	
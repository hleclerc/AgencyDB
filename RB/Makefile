GF = cpp/RiotBase/Variable/Core/Methods.h \
     cpp/RiotBase/Variable/Core/Methods.cpp \
     cpp/RiotBase/Variable/Core/TupleTypesDecl.h \
     cpp/RiotBase/Variable/Core/Type_has_surdef_for_cpp.h \
     cpp/RiotBase/Variable/Core/Type_has_surdef_for_h.h \
     cpp/RiotBase/Variable/Core/TypeTempl_DeclDynStat.h \
     cpp/RiotBase/Variable/Core/TypeTempl_DefDynStat.h \
     cpp/RiotBase/Variable/Core/SelfOpIfTermValue.h \
     cpp/RiotBase/Variable/Core/StdOpOxxxx.h \
     cpp/RiotBase/Variable/Symbol/GenSymSelect_sb_extr_defs.h \
     cpp/RiotBase/Variable/Symbol/Op.h \
     cpp/RiotBase/Variable/Symbol/Lazyfier_surdefs.h \
     cpp/RiotBase/Variable/Symbol/SymOp.cpp
	#  cpp/RiotBase/System/KnownInodes.h

TS = ts/RiotBase/Variable/Core/DeclTypesForRW.ts \
     ts/RiotBase/Variable/Ot/PatchTypes.ts \
	 ts/RiotBase/Db/Core/TrackerServerCmds.ts \
	 ts/RiotBase/Db/Core/TrackerClientCmds.ts \
	 ts/RiotBase/Db/Core/DbCmds.ts \
	 ts/RiotBase/System/KnownInodes.ts

# DBD = --valgrind # --gdb #--exec-using "valgrind  --num-callers=24" # --valgrind # --valgrind # --show-leak-kinds=all # --gdb # #  --leak-check=full--valgrind #
# # --exec-using "valgrind --leak-check=full"
# PRG = tests/test_Com.cpp

# IF = -Isrc -I/usr/include/qt -I/usr/include/qt5 -I/usr/include/x86_64-linux-gnu/qt5 -Iext/PrepArg/src -Iext/CImg
# OF = -DDEBUG -g3

# MC = nsmake exe --cpp-flag -Wall --cpp-flag -std=c++1y --cpp-flag -pipe ${IF} ${OF}
# MC = metil_comp --sep-libs -pipe -j8 -Wall -std=c++1y ${IF} ${OF} -ne 

# PRG = tests/test_Com.cpp --sep-libs
# IF = -pipe -lpthread -Icpp -I/usr/include/qt -I/usr/include/qt5 -I/usr/include/x86_64-linux-gnu/qt5 -Iext/PrepArg/src -Iext/crc32c -Iext/OpenSsl/include -I/usr/include/jsoncpp
# DF = -Lext/OpenSsl/lib
# OF = -DDEBUG -g3

# -DINSECURE_DEBUG
# --cxx clang++ --ld clang++
# -O3 --static --no-sep-libs

# MC = export LD=ld.gold && metil_comp -j1 -Wall -std=c++1y ${IF} ${OF} ${DF}
# MC = nsmake exe --verbose --cpp-flag -Wall --cpp-flag -pipe
MC = nsmake run --verbose 

all: riotbase

clean_nsmake:
	cd ~/nsmake; tsc
	nsmake stop
	rm -f ~/.nsmake/server.log
	rm -f ~/.nsmake/build/*

ts: ${TS}
	cd ts && tsc

dist:
	rsync --delete -ruv --exclude '*/compilations' . leclerc@192.168.0.31:/home/leclerc/RiotBase
	ssh leclerc@192.168.0.31 'cd /home/leclerc/RiotBase; make val_test_inc'

riotbase: ext/PrepArg clean_nsmake # ${GF}
	${MC} -o riotbase cpp/riotbase.cpp

http_test_server:
	${MC} bin/HttpTestServer/http_test_server.cpp

setup_babel:
	npm install babel-preset-es2015 babel-cli babel-plugin-transform-es2015-modules-amd --save-dev

setup_openssl:
	git clone https://github.com/openssl/openssl ext/openssl-src || true
	cd ext/openssl-src; ./config -DPURIFY --prefix=${PWD}/ext/OpenSsl shared no-asm no-sse2
	cd ext/openssl-src; make depend
	cd ext/openssl-src; make -j8
	cd ext/openssl-src; make install

# compilation/execution of individual tests
test_%.coffee: # need a working HttpTestServer (that can be launched in // using http_test_server)
	wget -q --no-check-certificate https://localhost:8001/_make_local_copy?tests/test_$*.js -O -
	node local/tests/test_$*.js

# compilation/execution of individual tests
test_%: ${GF} ${TS} ext/crc32c
	mkdir -p /tmp/compilations 2> /dev/null
	${MC} cpp/tests/test_$*.cpp

cmp_test_%: ${GF} ${TS} ext/crc32c
	${MC} -ne cpp/tests/test_$*.cpp

val_test_%: ${GF} ${TS}
	${MC} --exec-using "valgrind --num-callers=48" cpp/tests/test_$*.cpp
	# --vgdb-error=1 --show-leak-kinds=all --vgdb-error=1 --leak-check=full --leak-check=full

gdb_test_%: ${GF} ${TS}
	${MC} --gdb cpp/tests/test_$*.cpp

# generated files
%.h: %.py.h bin/Method_template.py
	python2 bin/Method_template.py $< $@

%.cpp: %.py.cpp bin/Method_template.py
	python2 bin/Method_template.py $< $@

%.ts: %.pyts bin/Method_template.py
	python2 bin/Method_template.py $< $@

# deps
ext/PrepArg:
	test -e ext/PrepArg || (mkdir ext; git clone https://github.com/hleclerc/PrepArg.git ext/PrepArg)

ext/crc32c:
	test -e ext/crc32c || (mkdir ext; git clone https://github.com/ahknight/crc32c.git ext/crc32c)

# build of javascript files
js:
	coffee -o .gen -b -c cpp/RiotBase/Js/pre_js.coffee
	coffee -o .gen -b -c cpp/RiotBase/Js/post_js.coffee
	# source ~/Prog/emsdk_portable/emsdk_env.sh
	emcc --pre-js .gen/pre_js.js --post-js .gen/post_js.js -DRiotBase_JS --bind -std=c++1y -o node_modules/RiotBase/index.js cpp/RiotBase/System/CbQueue.cpp cpp/RiotBase/System/CbString.cpp cpp/RiotBase/DWPMObjects/Number.cpp cpp/RiotBase/DWPMObjects/String.cpp cpp/RiotBase/DWPMObjects/Internals/BinderProxyCreator.cpp

.PHONY: js riotbase ts

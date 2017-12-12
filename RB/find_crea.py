#! /usr/bin/env python2

import os, sys

pwd = os.getcwd()
while True:
    if os.path.exists( "crea.py" ):
        os.system( "python crea.py " + pwd + " " + str.join( " ", sys.argv[1:] ) )
        break
    os.chdir("..")
    if os.getcwd() == '/':
        print( "crea.py not found" )
        break



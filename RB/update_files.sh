N=`ls *.files`

echo update_files.sh > $N
echo Makefile       >> $N
echo SConstruct     >> $N
echo *.md           >> $N

for d in bin cpp ts
do
    for t in 'SConscript' '*.h' '*.cpp' '*.cu' '*.txt' '*.py' '*.js' '*.html' '*.css' '*.files' '*.met' '*.coffee' '*.asm' '*.sipe' '*.md'
    do
        for i in `find $d -name "$t" -a -not -wholename "*/compilations/*" -a -not -wholename "*/old/*"`
        do
            echo $i >> $N
            echo $i
            # git add $i
        done
    done
done

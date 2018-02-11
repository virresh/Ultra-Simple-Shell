all: bin test ush extCommands

only_executables: bin ush extCommands

bin:
	mkdir -p ../bin

test:
	mkdir -p ../test/folderA
	ln -sf test/folderA/ ../"Link to folderA"
	echo "This is some text\n\ncoming from file 1\n\n\n" > ../test/file1.txt
	echo "This is some more text\n\nand still useless\nbut this time\nit's coming from file 2\n\n" > ../test/file2.txt
	echo "A hidden text file !!!" > ../test/.sampleTextDoc
	echo "" > ../test/folderA/emptyFile.txt


ush: ush.c helperFuncs.c cd.c echo.c history.c
	gcc ush.c -o ../bin/ush

extCommands: ls date cat mkdir rm

ls: ls.c
	gcc ls.c -o ../bin/ls

date: date.c
	gcc date.c -o ../bin/date

cat: cat.c
	gcc cat.c -o ../bin/cat

mkdir: makeDir.c helperFuncs.c
	gcc makeDir.c -o ../bin/mkdir

rm: rm.c
	gcc rm.c -o ../bin/rm


.PHONY: clean	
clean:
	rm -rf ../test ../bin
	rm -f "../Link to folderA"
build :
	flex lexical.l
	gcc main.c lex.yy.c -ll -o scanner
test :
	echo "######### Test 1 ###########"
	./scanner ../test/test1.cmm
	echo "######### Test 2 ###########"
	./scanner ../test/test2.cmm
	echo "######### Test 3 ###########"
	./scanner ../test/test3.cmm
	echo "######### Test 4 ###########"
	./scanner ../test/test4.cmm

clean :
	rm lex.yy.c
	rm scanner
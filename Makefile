CPPFILES = astree.cpp auxlib.cpp lyutils.cpp oc.cpp stringset.cpp symtable.cpp typechecking.cpp oilgen.cpp
HEADERS  = astree.h auxlib.h lyutils.h stringset.h sym_switch_functions.h symtable.h typechecking.h oilgen.h oil_switch_functions.h

FLEXSRC  = scanner.l
BISONSRC = parser.y

LEXH     = yylex.h
LEXC     = yylex.cc
PARSEHDR = yyparse.h
PARSECPP = yyparse.cpp
PARSEOUT = yyparse.output
F_B_OUT  = ${LEXH} ${LEXC} ${PARSEHDR} ${PARSECPP} ${PARSEOUT}

EXTRA    = README Makefile check.bash
SOURCE   = ${CPPFILES} ${HEADERS} ${FLEXSRC} ${BISONSRC} ${EXTRA}

OBJECTS  = astree.o auxlib.o lyutils.o stringset.o symtable.o typechecking.o yylex.o yyparse.o oilgen.o
PRODUCTS = *.str *.tok *.ast *.sym *.log

COMPILE  = g++ -g -O0 -Wall -Wextra -std=gnu++14 -o
FLEX     = flex --header-file=${LEXH} --outfile=${LEXC}
BISON    = bison --defines=${PARSEHDR} --output=${PARSECPP}


all: oc

oc: oc.cpp deps
	${COMPILE} $@ $< ${OBJECTS} 

deps: ${PARSEHDR} ${LEXH} ${OBJECTS} 

nice:
	-rm ${PRODUCTS} 2> /dev/null

clean: nice
	-rm ${OBJECTS} ${F_B_OUT} 2> /dev/null

spotless: clean
	-rm oc 2> /dev/null

check: oc check.bash
	./check.bash
	@make -s nice


submit: ${SOURCE} ci
	submit cmps104a-wm.s16 asg5 ${SOURCE}

ci: ${SOURCE}
	git add ${SOURCE}
	git commit -am "${m}" #call with make ci m='blablabla'
	git push origin master


${LEXC}: ${FLEXSRC}
	${FLEX} ${FLEXSRC}

${LEXH}: ${LEXC}

yylex.o: ${LEXC} ${LEXH}
	${COMPILE} $@ -c $< -Wno-sign-compare
	

${PARSECPP}: ${BISONSRC}
	${BISON} ${BISONSRC}
	echo -e '1,$$!cpp -fpreprocessed -dD\nw\nq\n' | ex ${PARSEHDR}

${PARSEHDR}: ${PARSECPP}

#yyparse.o: ${PARSECPP} lyutils.h astree.h
#	${COMPILE} $@ -c $<
#
#astree.h: t.h lyutils.h
#	${COMPILE} $@ -c $<
#
#lyutils.h: auxlib.h astree.h ${LEXH} ${PARSEHDR}
#	${COMPILE} $@ -c $<

%.o: %.cpp %.h
	${COMPILE} $@ -c $<

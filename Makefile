CPPFILES = oc.cpp auxlib.cpp lyutils.cpp astree.cpp stringset.cpp
HEADERS  = auxlib.h lyutils.h astree.h stringset.h tokens.h

FLEXSRC  = scanner.l
BISONSRC = parser.y

LEXH     = yylex.h
LEXC     = yylex.cc
PARSEHDR = yyparse.h
PARSECPP = yyparse.cpp
PARSEOUT = yyparse.output
F_B_OUT  = ${LEXH} ${LEXC} ${PARSEHDR} ${PARSECPP} ${PARSEOUT}

EXTRA    = README Makefile
SOURCE   = ${CPPFILES} ${HEADERS} ${FLEXSRC} ${BISONSRC} ${EXTRA}

OBJECTS  = auxlib.o lyutils.o astree.o stringset.o yylex.o yyparse.o
PRODUCTS = *.str ${F_B_OUT}

COMPILE  = g++ -g -O0 -Wall -Wextra -std=gnu++14 -o
FLEX     = flex --header-file=${LEXH} --outfile=${LEXC}
BISON    = bison --defines=${PARSEHDR} --output=${PARSECPP}


all: oc

oc: oc.cpp deps
	${COMPILE} $@ $< ${OBJECTS} 

clean:
	-rm ${OBJECTS} ${PRODUCTS} 2> /dev/null

spotless: clean
	-rm oc 2> /dev/null

ci: ${SOURCE}
	git add ${SOURCE}
	git commit -am "${m}" #call with make ci m='blablabla'
	git push origin master

deps: ${OBJECTS}

submit: ${SOURCE}
	submit cmps104a-wm.s16 asg2 ${SOURCE}


${LEXH}: ${FLEXSRC}
	${FLEX} ${FLEXSRC}

${LEXC}: ${LEXH}

yylex.o: ${LEXC} ${LEXH}
	${COMPILE} $@ -c $< -Wno-sign-compare
	

${PARSEHDR}: ${BISONSRC}
	${BISON} ${BISONSRC}
	echo -e '1,$$!cpp -fpreprocessed -dD\nw\nq\n' | ex ${PARSEHDR}

${PARSECPP}: ${PARSEHDR}

yyparse.o: ${PARSECPP} lyutils.h astree.h
	${COMPILE} $@ -c $<

astree.o: astree.cpp astree.h stringset.h lyutils.h
	${COMPILE} $@ -c $<

lyutils.o: lyutils.cpp lyutils.h auxlib.h astree.h ${LEXH} ${PARSEHDR}
	${COMPILE} $@ -c $<

%.o: %.cpp %.h
	${COMPILE} $@ -c $<

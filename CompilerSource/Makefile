# DEBUG (faster compile)
SYMBOL_FLAGS       = -g
OPTIMIZATION_FLAGS =

# RELEASE (faster runtime)
#SYMBOL_FLAGS       = -s
#OPTIMIZATION_FLAGS = -O2 # SPEED
#OPTIMIZATION_FLAGS = -Os # SIZE

all: build

objects:
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/cfile_parse.cpp -o cfile_parse/cfile_parse.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/cfile_parse_macros.cpp -o cfile_parse/cfile_parse_macros.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/cfile_pushing.cpp -o cfile_parse/cfile_pushing.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/cparse_components.cpp -o cfile_parse/cparse_components.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/cparse_shared.cpp -o cfile_parse/cparse_shared.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/expression_evaluator.cpp -o cfile_parse/expression_evaluator.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/handle_letters.cpp -o cfile_parse/handle_letters.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/keyword_operator.cpp -o cfile_parse/keyword_operator.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/macro_functions.cpp -o cfile_parse/macro_functions.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/main.cpp -o cfile_parse/main.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/skipping.cpp -o cfile_parse/skipping.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c cfile_parse/template_handlers.cpp -o cfile_parse/template_handlers.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c compiler/compile.cpp -o compiler/compile.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c compiler/compile_includes.cpp -o compiler/compile_includes.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c compiler/output_locals.cpp -o compiler/output_locals.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c compiler/pcs/pcs.cpp -o compiler/pcs/pcs.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c externs/ext_iteration.cpp -o externs/ext_iteration.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c externs/externs.cpp -o externs/externs.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c externs/references.cpp -o externs/references.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c gcc_interface/gcc_backend.cpp -o gcc_interface/gcc_backend.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c general/darray.cpp -o general/darray.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c general/string.cpp -o general/string.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c general/textfile.cpp -o general/textfile.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c main.cpp -o main.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c parser/collect_variables.cpp -o parser/collect_variables.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c parser/object_storage.cpp -o parser/object_storage.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c parser/parser.cpp -o parser/parser.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c parser/parser_components.cpp -o parser/parser_components.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c syntax/checkfile.cpp -o syntax/checkfile.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c syntax/syntax.cpp -o syntax/syntax.o
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c varnames/varnames.cpp -o varnames/varnames.o

standalone-object:
	g++ -Wall ${SYMBOL_FLAGS} ${OPTIMIZATION_FLAGS} -fPIC -c standalone_main.cpp -o standalone_main.o

link:
	g++ -shared -fPIC cfile_parse/cfile_parse.o cfile_parse/cfile_parse_macros.o cfile_parse/cfile_pushing.o cfile_parse/cparse_components.o cfile_parse/cparse_shared.o cfile_parse/expression_evaluator.o cfile_parse/handle_letters.o cfile_parse/keyword_operator.o cfile_parse/macro_functions.o cfile_parse/main.o cfile_parse/skipping.o cfile_parse/template_handlers.o compiler/compile.o compiler/compile_includes.o compiler/output_locals.o compiler/pcs/pcs.o externs/ext_iteration.o externs/externs.o externs/references.o gcc_interface/gcc_backend.o general/darray.o general/string.o general/textfile.o main.o parser/collect_variables.o parser/object_storage.o parser/parser.o parser/parser_components.o syntax/checkfile.o syntax/syntax.o varnames/varnames.o -o ../compileEGMf

link-standalone link-standalone:
	g++ -shared -fPIC cfile_parse/cfile_parse.o cfile_parse/cfile_parse_macros.o cfile_parse/cfile_pushing.o cfile_parse/cparse_components.o cfile_parse/cparse_shared.o cfile_parse/expression_evaluator.o cfile_parse/handle_letters.o cfile_parse/keyword_operator.o cfile_parse/macro_functions.o cfile_parse/main.o cfile_parse/skipping.o cfile_parse/template_handlers.o compiler/compile.o compiler/compile_includes.o compiler/output_locals.o compiler/pcs/pcs.o externs/ext_iteration.o externs/externs.o externs/references.o gcc_interface/gcc_backend.o general/darray.o general/string.o general/textfile.o main.o parser/collect_variables.o parser/object_storage.o parser/parser.o parser/parser_components.o syntax/checkfile.o syntax/syntax.o varnames/varnames.o standalone_main.o -o ../compileEGMf

filename-linux filename-unix:
	mv ../compileEGMf ../libcompileEGMf.so

filename-win:
	move ../compileEGMf ../compileEGMf.dll

build: objects link
build-standalone: objects standalone-object link-standalone

win: build filename-win
linux unix: build filename-linux

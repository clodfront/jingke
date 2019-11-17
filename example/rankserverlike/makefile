EXEFILE = rankserverlike
include ../../makerule/makefile.depend

INC_PROTOC = -I../testprotoc
LIB_PROTOC = -L../testprotoc -ltestprotoc

INC_ALL += ${INC_SERVER} ${INC_NET} ${INC_LIBCURL} ${INC_ASYNC_WORK} ${INC_UTILITY} ${INC_JSON} ${INC_PROTOC} ${INC_PB}
LIB_ALL += ${LIB_SERVER} ${LIB_NET} ${LIB_LIBCURL} ${LIB_ASYNC_WORK} ${LIB_UTILITY} ${LIB_PROTOC} ${LIB_PB}

include ../../makerule/makefile.rule

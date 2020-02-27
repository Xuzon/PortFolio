#pragma once
#ifndef __HELLO_H
#define __HELLO_H

void saluda(const char* name);

#endif

//en las x64 native tools para ver el desesnsamblado
//dumpbin /disasm Hello.obj
//para ver los simbolos de headers
//dumpbin /symbols /headers HolaMundo.obj
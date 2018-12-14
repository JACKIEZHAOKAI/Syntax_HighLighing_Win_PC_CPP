#pragma once
/*
author£º zhaokai xu
email: gzxzk96@gmail.com
description£º this program parse HTML code to C code with color setting using gumbo query
which is a warpped API of gumbo parser
Reference:
Gumbo API https://github.com/google/gumbo-parser
gumbo query API: https://github.com/lazytiger/gumbo-query
gumbo query samples: https://github.com/lazytiger/gumbo-query/blob/master/example/main.cpp
*/


#ifndef GUMBO_PARSE_H
#define GUMBO_PARSE_H
#include <string>
#include <iostream>
#include <vector>
#include <gumbo.h>
#include <Document.h>
#include <Node.h>
using namespace std;

struct HLJS_Color {
	std::string name;
	std::string color;
};

void DecodeHtmlNode(CNode node, string& C_result);

string DecodeDOMTree(string inputCode);

#endif
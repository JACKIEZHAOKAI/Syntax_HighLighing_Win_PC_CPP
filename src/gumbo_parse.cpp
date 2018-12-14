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
#include <string>
#include <iostream>
#include <vector>
#include <gumbo.h>
#include <Document.h>
#include <Node.h>
#include "gumbo_parse.h"
using namespace std;

vector<HLJS_Color> hljs_table = { { "hljs-keyword", "#FF0000" },{ "hljs-number", "#00FF00" },
{ "hljs-function", "#0000FF" },{ "hljs-title", "#000000" },{ "hljs-params", "#A020F0" },
{ "hljs-comment", "#BEBEBE" },{ "hljs-literal", "#FF00FF" },{ "hljs-string", "#696969" }
};

/* this function recursive call nodeDecoder to decode the DOM tree */
void DecodeHtmlNode(CNode node, string& C_result) {

	//detect tag
	if (node.tag() == "span") { 
		for (auto item : hljs_table) {
			//detect attribute
			if (node.attribute("class") == item.name)
			{
				//formating to C with its color by hljs_table
				string added_content = "<c " + item.color + ">" + node.text(true) + "</c>";
				C_result += added_content;
			}
		}
	}
	//detect new line 
	else if (node.tag() == "br") {
		C_result += "\n";
	}
	//detect text
	else {
		size_t count = node.childNum();
		// recursive call to traverse the node 
		if (count > 0) {
			for (unsigned int i = 0; i < count; i++) {
				auto child = node.childAt(i);
				DecodeHtmlNode(child, C_result);
			}
		}
		else {
			// the node is leaf node, so it is a text
			string added_content = "<c #000000>" + node.text(true) + "</c>";
			C_result += added_content;
		}
	}


}

/* this function can decode DOM tree */
string DecodeDOMTree(string inputCode) {

	string C_result = "";
	CDocument doc;

	//parse the DOM tree 
	doc.parse(inputCode);
	CSelection c = doc.find("body");

	//starting from root to traverse the tree
	auto num = c.nodeNum();
	for (int i = 0; i < num; i++) {
		CNode node = c.nodeAt(i);
		DecodeHtmlNode(node, C_result);
	}
	return C_result;
}
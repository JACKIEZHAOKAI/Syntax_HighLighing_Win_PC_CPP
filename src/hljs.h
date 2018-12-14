/*
author： zhaokai xu
email: gzxzk96@gmail.com
description： this program implement the interaction between C++ and Js through V8 API
reference：
https://www.jianshu.com/p/849e354322ff （旧版api）
https://github.com/v8/v8/wiki/Getting-Started-with-Embedding
https://github.com/highlightjs/highlight.js/tree/master/src
https://www.cnblogs.com/Dennis-mi/p/3284216.html
https://www.cnblogs.com/Dennis-mi/p/3284216.html
*/

#ifndef HLJS_H
#define HLJS_H
#include <libplatform/libplatform.h>
#include <v8.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <direct.h>
#include "hljs.h"
using namespace v8;

/* data structure containing detected language name and coresponding xxx.js file */
struct HLJSLanguageInfo {
	std::string name;
	std::string js_content_file;
};

/* 先编译再执行 run and compile js 传入执行脚本 返回结果通过引用传出 */
bool ExecuteScript(v8::Isolate* isolate, Local<String> script, Local<Value>& result);

/* function to read a file  */
bool GetContentFromFile(const std::string& filename, std::string& content);

/* Adding all popular languages xxx.js files into hljs.js library */
std::string ConfigJSLanguage();

/* Parsing  input code to HTML format with implementation on v8 and highlihgt.js */
std::string ConvertToHTML(int argc, char *argv[], std::string& inputCode);

/*recognize newline char in HTML format of highlighting and change to <BR>*/
std::string newLineConvertion(std::string inputString);
#endif
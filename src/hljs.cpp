/*
author： zhaokai xu
email: gzxzk96@gmail.com
description： this program implement the interaction between C++ and Js through V8 API
*/

#include "hljs.h"
using namespace v8;
using namespace std;

/* 先编译再执行 run and compile js 传入执行脚本 返回结果通过引用传出 */
bool ExecuteScript(v8::Isolate* isolate, Local<String> script, Local<Value>& result) {
	HandleScope handle_scope(isolate);

	// We're just about to compile the script; set up an error handler to
	// catch any exceptions the script might throw.
	TryCatch try_catch(isolate);

	Local<Context> context(isolate->GetCurrentContext());

	// Compile the script and check for errors.
	Local<Script> compiled_script;
	if (!Script::Compile(context, script).ToLocal(&compiled_script)) {
		String::Utf8Value error(isolate, try_catch.Exception());
		//Log(*error);
		// The script failed to compile; bail out.
		return false;
	}

	// Run the script!
	if (!compiled_script->Run(context).ToLocal(&result)) {
		// The TryCatch above is still in effect and will have caught the error.
		String::Utf8Value error(isolate, try_catch.Exception());
		// Running the script failed; bail out.
		return false;
	}
	return true;
}



/* function to read a file  */
bool GetContentFromFile(const std::string& filename, std::string& content) {
	try {
		std::ifstream stream(filename, std::ios::in | std::ios::binary);
		if (stream.is_open()) {
			//cerr << "reading success" << endl;
			content = std::string((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
			stream.close();
			return true;
		}
	}
	catch (...) {
	}
	//cerr << "reading failed" << endl;
	return false;
}

/* Adding all popular languages xxx.js files into hljs.js library */
std::string ConfigJSLanguage() {
	std::string result;

	/* list of languaegs will be added to the hljs library  */
	std::vector<HLJSLanguageInfo> g_language_table = {
		{ "java", "java.js" },{ "apache", "apache.js" },{ "bash", "bash.js" },{ "cpp.", "cpp.js" },{ "cs", "cs.js" },
		{ "css", "css.js" },{ "http", "http.js" },{ "javascript", "javascript.js" },{ "json", "json.js" },{ "makefile", "makefile.js" },
		{ "markdown", "markdown.js" },{ "objectivec", "objectivec.js" },{ "perl", "perl.js" },{ "php", "php.js" },
		{ "python", "python.js" },{ "ruby", "ruby.js" },{ "sql", "sql.js" },{ "xml", "xml.js" }
	};


	for (const auto& lang : g_language_table) {
		std::string jsfilePath = "..\\bin\\lang\\" + lang.js_content_file; //相对路径
		std::string jsfilecontent = "";
		bool success = GetContentFromFile(jsfilePath, jsfilecontent);
		result.append("hljs.registerLanguage(\"" + lang.name + "\", " + jsfilecontent + ");");
	}
	return result;
}


/*recognize newline char in HTML format of highlighting and change to <BR>*/
string newLineConvertion(string inputString) {
	string newinputString = "";
	for (int i = 0; i < inputString.size(); i++)
	{
		newinputString += inputString[i];
		if ('\n' == inputString[i])
		{
			newinputString += "<br>";
		}
	}
	return newinputString;
}

/* Parsing  input code to HTML format with implementation on v8 and highlihgt.js */
std::string ConvertToHTML(int argc, char *argv[], std::string& inputCode) {

	std::string Result_HTML;
	v8::V8::InitializeICUDefaultLocation(argv[0]);
	v8::V8::InitializeExternalStartupData(argv[0]);
	auto platform = platform::CreateDefaultPlatform();
	v8::V8::InitializePlatform(platform);
	v8::V8::Initialize();

	//引入hightlighjs 库
	std::string jslib;				// jslib = hljs库文件 + 常用语言包	
	GetContentFromFile("..\\bin\\lang\\hljs.js", jslib);
	jslib += "\n\n";
	jslib += ConfigJSLanguage();   //添加常用语言包

								   //在context的JS环境下运行hljs的高亮转换函数 highlightAuto
	string js_test_func = R"(
			function Call_hljs(content) {
				hljs.initHighlightingOnLoad();
				html = hljs.highlightAuto(content);
				return html.value;
			})";

	// 1 创建v8的VM环境 
	Isolate::CreateParams create_params;
	create_params.array_buffer_allocator =
		ArrayBuffer::Allocator::NewDefaultAllocator();
	Isolate* isolate = Isolate::New(create_params);
	{
		//2 Create a stack-allocated handle scope.
		HandleScope handle_scope(isolate);

		//3 Create a new context. 创建Js运行沙箱并进入
		Local<Context> context = Context::New(isolate);
		Context::Scope context_scope(context);
		{
			//定义context环境内需要的变量
			Local<String> hljs_lib_source;
			Handle<String> js_source;
			Handle<String> js_func_name;
			Handle<Value>  js_func_val;
			Handle<Function> js_func;
			Handle<String> code_context;
			Handle<Value>  argv[1];
			Local<Value> result;
			int argc = 1;


			//4 在context环境引入highlightJs库文件，编译并运行
			hljs_lib_source = String::NewFromUtf8(isolate, jslib.c_str(),
				NewStringType::kNormal).ToLocalChecked();
			ExecuteScript(isolate, hljs_lib_source, result);

			//5 将  Js函数Test 写入context 并且编译跑通
			js_source = String::NewFromUtf8(isolate,
				js_test_func.c_str(), NewStringType::kNormal).ToLocalChecked();
			ExecuteScript(isolate, js_source, result);

			//6 从全局运行环境中进行查找，看看是否存在一个叫做  Call_hljs 的Js函数
			js_func_name = String::NewFromUtf8(isolate, "Call_hljs");
			js_func_val = context->Global()->Get(js_func_name);
			if (!js_func_val->IsFunction())
			{
				cerr << " This function is not found" << endl;
			}
			else
			{
				// 利用handle的强制类型转换，把js_func_val转换成一个函数对象
				js_func = Handle<Function>::Cast(js_func_val);

				// 初始化参数，所有数据都要定义成javascript可以识别的数据类型，例如Integer对象
				// javascript中是没有内建数据类型的（int, char, short是c/c++中的用的类型）
				// 这里通过string传入需要高亮的代码
				const char *cstr = inputCode.c_str();		//convert to c string as input 
				code_context = String::NewFromUtf8(isolate, cstr);

				// 把这些对象放到argv数组中去
				argv[0] = code_context;

				// 利用函数对象去调用该函数，当然需要传入脚本的运行环境，以及参数个数和参数的值。
				Local<Value> html_Result = js_func->Call(context->Global(), argc, argv);
				String::Utf8Value utf8(isolate, html_Result);
				//printf("%s\n", *utf8);
				Result_HTML = string(*utf8);
			}
		}
	}
	//7 Dispose the isolate and tear down V8. 销毁v8虚拟机
	isolate->Dispose();
	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();
	delete platform;

	return newLineConvertion(Result_HTML);
}

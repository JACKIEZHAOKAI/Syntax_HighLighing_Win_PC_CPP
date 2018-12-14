
#include "hljs.h"
#include "gumbo_parse.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
	
	string test = R"(<span class="hljs-keyword">class</span> <span class="hljs-title">MainDemo</span>

<br>{
<br>    <span class="hljs-function"><span class="hljs-keyword">public</span> <sp
an class="hljs-keyword">static</span> <span class="hljs-keyword">void</span> <sp
an class="hljs-title">main</span>(<span class="hljs-params">String[] args</span>
)
<br>    </span>{
<br>            System.<span class="hljs-keyword">out</span>.println(args);
<br>            System.<span class="hljs-keyword">out</span>.println(args.length
);
<br>            <span class="hljs-keyword">for</span>(<span class="hljs-keyword"
>int</span> i = <span class="hljs-number">0</span>; i &lt; args.length; i++)
<br>                    System.<span class="hljs-keyword">out</span>.println(arg
s[i]);
<br>    }
<br>}
<br>)";

std::string inputCode = R"(class MainDemo
{
	public static void main(String[] args)  
	{
		System.out.println(args); 
		System.out.println(args.length); 
		for(int i = 0; i < args.length; i++)
			System.out.println(args[i]); 
	}
}
)";
	// taking input code and covert it to html format with highlight.js
	//string result_HTML = ConvertToHTML(argc, argv, inputCode);
	//cout << result_HTML << endl;

	// parsing HTML code with assigned color using  Gumbo parser
	string result_C = DecodeDOMTree(test);
	printf("%s", result_C.c_str());

	return 0;
}
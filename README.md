# HighlightJs_VS15


This highlightJS project import google V8 to interact C++ with JS,

Tnside the context environment of  a V8 isolate, the program calls highlight.js lib to parse an input text code 
and transfering to its highlighting HTML format. The process is completed uder src/hljs.cpp, called ConvertToHTML in main.

Then, the HTML foramt code will be send to gumbo parser, and the DecodeDoMTREE funciton  in main will handle the job
of parsing the DOM tree, converting it to be DUI lib recognizable format.

The DUI will present the syntax highlighting effect on Windows System.

_________________________
Notice that the program requires a V8 engine to be imported and compilied successfully.
you can V8 under deps, and includes the lib in Visual studio.

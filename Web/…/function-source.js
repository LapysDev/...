function f(){}
"function f(){}"

class A { a(){} }
"class A { a(){} }"

function* g(){}
"function* g(){}"

a => a
"a => a"

({ a(){} }.a)
"a(){}"

({ *a(){} }.a)
"*a(){}"

({ [0](){} }[0])
"[0](){}"

Object.getOwnPropertyDescriptor({get a(){}}, 'a').get
"get a(){}"

Object.getOwnPropertyDescriptor({ set a(x){} }, 'a').set
"set a(x){}"

Function.prototype.toString
"function toString() { [native code] }"

(function f(){}.bind(0))
"function () { [native code] }"

Function('a', 'b')
"function anonymous(a\n) {\nb\n}"

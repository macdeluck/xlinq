xLinq is header only library allowing to use LINQ like expressions in C++ code. It was inspired by [cpplinq](https://github.com/mrange/cpplinq) project. Xlinq uses similar syntax like cpplinq but it defines IEnumerable interface, thus it allows to grab enumerable object without need to gather elements.

This library needs to have c++11 standard enabled during compilation. Library was tested on MS Visual C++ Compiler and C++.

To start use xlinq u need only download code, add the include folder to your include path and voila!

Here u may see some examples how to use xlinq:

Iterate over array:
<p><code>
int numbers[] = { 1, 2, 3, 4, 5 };<br>
for (auto it = from(numbers) &gt;&gt; select([](int n){ return n * 2; }) &gt;&gt; getEnumerator(); it-&gt;next();)<br>
{<br>
    std::cout &lt;&lt; it-&gt;current() &lt;&lt; " ";<br>
}<br>
// Result: 2 4 6 8 10 <br>
</code></p>

Returning last element of vector:

<p><code>
std::vector&lt;std::string&gt; vec = { "Eeny", "Meeny", "Miny", "Moe" };<br>
auto lastOne = from(vec) &gt;&gt; last();<br>
std::cout &lt;&lt; lastOne;<br>
// Result: Moe <br>
</code></p>

Returning enumerable from other function:
<p><code>
std::shared_ptr&lt;IEnumerable&lt;size_t&gt;&gt; nameLengths()<br>
{<br>
  std::vector&lt;std::string&gt; vec = { "Ken", "John", "David", "Thomas", "Richard" };<br>
  return from(vec) &gt;&gt; select([](std::string name) { return name.size(); }) &gt;&gt; gather();<br>
  //gather is needed here, because vec will be deallocated when function will exit. <br>
}<br>
<br>
void printNameLengths()<br>
{<br>
  auto lengths = nameLengths();<br>
  for (auto it = lengths &gt;&gt; getEnumerator(); it-&gt;next();)<br>
  std::cout &lt;&lt; it-&gt;current() &lt;&lt; " ";<br>
}<br>
// Result: 3 4 5 6 7 <br>
</code></p>
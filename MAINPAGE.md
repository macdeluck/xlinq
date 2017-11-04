xLinq is header only library allowing to use LINQ like expressions in C++ code. It was inspired by [cpplinq](https://github.com/mrange/cpplinq) project. Xlinq uses similar syntax like cpplinq but it defines IEnumerable interface, thus it allows to grab enumerable object without need to gather elements.

This library needs to have c++11 standard enabled during compilation. Library was tested on MS Visual C++ Compiler 19.10 and G++ 4.8.4.

To start use xlinq you need only download code, add the include folder to your include path and voila!

Here you may see some examples how to use xlinq:

Iterate over array:
<p><code>
#include "xlinq/all.h"

int numbers[] = { 1, 2, 3, 4, 5 };<br/>
for (auto it = from(numbers) &gt;&gt; select([](int n){ return n * 2; }) &gt;&gt; getEnumerator(); it-&gt;next();)<br/>
{<br/>
&nbsp; std::cout &lt;&lt; it-&gt;current() &lt;&lt; " ";<br/>
}<br/>
// Result: 2 4 6 8 10 <br/>
</code></p>

or even:
<p><code>
#include "xlinq/all.h"

int numbers[] = { 1, 2, 3, 4, 5 };<br/>
for (auto val : from(numbers) &gt;&gt; select([](int n){ return n * 2; }) &gt;&gt; stl())<br/>
{<br/>
&nbsp; std::cout &lt;&lt; val &lt;&lt; " ";<br/>
}<br/>
// Result: 2 4 6 8 10 <br/>
</code></p>

Returning last element of vector:

<p><code>
#include "xlinq/all.h"

std::vector&lt;std::string&gt; vec = { "Eeny", "Meeny", "Miny", "Moe" };<br/>
auto lastOne = from(vec) &gt;&gt; last();<br/>
std::cout &lt;&lt; lastOne;<br/>
// Result: Moe <br/>
</code></p>

Returning enumerable from other function:
<p><code>
#include "xlinq/all.h"

std::shared_ptr&lt;IEnumerable&lt;size_t&gt;&gt; nameLengths()<br/>
{<br/>
&nbsp; std::vector&lt;std::string&gt; vec = { "Ken", "John", "David", "Thomas", "Richard" };<br/>
&nbsp; return from(vec) &gt;&gt; select([](std::string name) { return name.size(); }) &gt;&gt; gather();<br/>
&nbsp; //gather is needed here, because vec will be deallocated when function will exit. <br/>
}<br/>
<br/>
void printNameLengths()<br/>
{<br/>
&nbsp; auto lengths = nameLengths();<br/>
&nbsp; for (auto it = lengths &gt;&gt; getEnumerator(); it-&gt;next();)<br/>
&nbsp; std::cout &lt;&lt; it-&gt;current() &lt;&lt; " ";<br/>
}<br/>
// Result: 3 4 5 6 7 <br/>
</code></p>

# Supported operations:

* aggregate()
* all()
* any()
* avg()
* concat()
* count()
* distinct()
* distinct_by()
* element_at()
* element_at_or_default()
* except()
* first()
* first_or_default()
* from_array()
* from()
* gather()
* lazy_gather()
* group_by()
* intersect()
* join()
* last()
* last_or_default()
* max()
* min()
* reverse()
* select()
* select_many()
* sequence_equals()
* skip_while()
* skip()
* sort()
* stl()
* sum()
* take_while()
* take()
* to_vector()
* to_list()
* to_forward_list()
* to_set()
* to_multiset()
* to_map()
* to_unordered_set()
* to_unordered_multiset()
* to_unordered_map()
* union_with()
* where()
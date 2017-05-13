# xLinq

xLinq is header only library allowing to use LINQ like expressions in C++ code. It was inspired by [cpplinq](https://github.com/mrange/cpplinq) project. Xlinq uses similar syntax like cpplinq but it defines IEnumerable interface, thus it allows to grab enumerable object without need to gather elements.

This library needs to have c++11 standard enabled during compilation. Library was tested on MS Visual C++ Compiler 19.10 and G++ 4.8.4.

To start use xlinq you need only download code, add the include folder to your include path and voila!

### Examples

Here you may see some examples how to use xlinq:

Iterate over array:
```C++
int numbers[] = { 1, 2, 3, 4, 5 };
for (auto it = from(numbers) >> select([](int n){ return n * 2; }) >> getEnumerator(); it->next();)
{
    std::cout << it->current() << " ";
}
/* Result: 2 4 6 8 10 */
```

Returning last element of vector:

```C++
std::vector<std::string> vec = { "Eeny", "Meeny", "Miny", "Moe" };
auto lastOne = from(vec) >> last();
std::cout << lastOne;
/* Result: Moe */
```

Returning enumerable from other function:
```C++
std::shared_ptr<IEnumerable<size_t>> nameLengths()
{
    std::vector<std::string> vec = { "Ken", "John", "David", "Thomas", "Richard" };
    return from(vec) >> select([](std::string name) { return name.size(); }) >> gather();
    /*gather is needed here, because vec will be deallocated when function will exit. */
}

void printNameLengths()
{
    auto lengths = nameLengths();
    for (auto it = lengths >> getEnumerator(); it->next();)
    std::cout << it->current() << " ";
}
/* Result: 3 4 5 6 7 */
```

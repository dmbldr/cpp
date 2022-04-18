# Лекция 3 Указатели, массивы, функции, константы

# Лекция 5 Конструкторы и деструкторы

```C++
class String {
private:
    char* str = nullptr;
    size_t sz = 0;
public:
    // Конструктор
    String(size_t n, char ch) 
    {
        this->sz = n;
        str = new char[sz];
        memset(str, ch, sz);
    }
}
```



memset и memcpy - эффективнее обычных циклов с присваиванием.

Компилятор сам создаёт конструктор копирования, оператор присваивания и деструктор - **правило трёх**. Но он делает это "по-глупому" - просто делает присваивание каждого поля. Это называется **Shallow copy** - например, скопировали указатели, но не скопировали то, что лежит под ними, не выделили память и не освободили память.

**Deep copy** - выделение новой, копирование того, что лежит под указателем, освобождение памяти  и др. нетривиальные вещи.

```c++
/// Конструктор копирования
String(const String& other)
{
    sz = other.sz;
    str = new char[sz];
    memcpy(str, other.str, sz);
}
```



Деструктор - то, что вызывается, когда необходимо уничтожение объекта.

**Важно:** поля класса сами создаются перед входом в конструктор, и уничтожаются после вызова деструктор. Нам нужно написать в конструкторе и деструкторе, что сделать помимо этого.  В деструкторе - что сделать до уничтожения объекта.

```c++
struct A {
    A() { cout << "A\n"; }
    ~A() { cout << "~A\n"; }
}
struct B {
    A a;
    B() { cout << "B\n"; }
    ~B() { cout << "~B\n"; }
}

int main() {
    B b;
}

/* Вывод:
A
B
~B
~A
*/
```



```с++
/// Деструктор
~String()
{
	delete[] str; // освобождение памяти
}
```



#### [Список инициализации](https://en.cppreference.com/w/cpp/utility/initializer_list) (list initialization)

Не путать со списком инициализации членов класса!!!!

```c++
String(const std::initializer_list<char>& lst) {
    sz = lst.size();
    str = new char[sz];
    std::copy(lst.begin(), lst.end(), str);
}

int main() {
    String s('n', 'o', 'n');
    String s{'h', 'e', 'l', 'l', 'o'};
}
```

#### 

#### Aggregate initialization

Данная инициализация работает для структур, где все поля публичные.

```c++
struct S {
    int x;
    double y;
    std::string str;
}

int main() {
    S s = {0, 1.0, "abc"};
}
```



**Важно:** если вы сами определяется какой-либо конструктор, то компилятор не будет создавать конструктор по-умолчанию.

```c++
public:
	String() = default;
```

Но если компилятор не знает как проинициализировать поле по-умолчанию, то выдаст CE.

```c++
struct A {
    int x;
    
   	A(int x) : x(x) {}; 
}

struct S {
    int& x;
    const int y;
    A a; // т.к. нет конструктора по-умолчанию
}

int main() {
    S s;
}
```



Для других конструкторов так же можно использовать *default*.

```c++
struct S {
    S(const S& s) = default;
    ~S() = default; // это бесмыслено
}
```

Деструктор генерируется всегда (кроме случаев если вы сами его создали)

Можно запрещать любые методы и функции

```c++
int f(int x) = delete;

struct S {
    S(S& other) = delete;
}
```



#### Делегирующий конструктор

Копипаст - почти всегда зло!!! Ниже просто неэффективный пример, т.к. лишний memset.

```c++
String(size_t n, char ch) 
{
    this->sz = n;
    str = new char[sz];
    memset(str, ch, sz);
}

String(const String& s) : String(s.sz, 0)
{
    memcpy(str, s.str, sz);
}


```



## 3.3. Operators overloading. Assigment operator. The Rule of Three

```c++
/// НЕ эффективный оператор присваивания
String& operator=(const String& s)
{
    if(this == &s) 
    {
        return *this;
    }
    delete[] str;
    sz = s.sz;
    str = new char[sz];
    memcpy(str, s.str, sz);
    return *this;
}
```

Снова копипаст!!! Для данного класса немного строк копипастим. Но для больших объектов хуже.

#### Copy and swap

swap работает за O(1) т.к. просто перестановка указателей.

```c++
void swap(String& s) 
{
    std::swap(str, s.str);
    std::swap(sz, s.sz);
}

String& operator=(const String& s)
{
    if(this == &s) 
    {
        return *this;
    }
    String new_s = s;
    swap(new_s);
    return *this;
}
```

В этом случае можно и не делать проверку на присваивания себе.

А так просто меньше кода, но не обязательно так делать:

```c++
String& operator=(String s) {
    swap(s);
    retunr *this;
}
```



**The Rule of Three** - если явно был определён деструктор, оператор присваивания или конструктор копирования, то необходимо самостоятельно определить все 3 метода.

# Лекция 6. Перегрузка операторов

## 3.4. Member initializer list 

Когда мы заходим в тело конструктора все поля уже созданы и инициализированы по-умолчанию.

```c++
struct A;
struct S {
    int x;
    A y;
    S() 
    {
        // тут x и y уже созданы и инициализированы
    }
}

S() : x(0), y(...) // тогда мы экономим копирование для каждого объекта
{
    
}
```



Для класса String:

```c++
class String {
private:
    char* str = nullptr;
    size_t sz = 0;
public:
	String(size_t sz, char ch) 
    	: sz(sz), str(new char[sz])
	{
    	memset(str, ch, sz)
	}  
};
```

**Важно:** компилятор не гарантирует порядок вызова конструкторов в списке инициализации!

Поля генерируются в том порядке, в котором они написаны в классе!!!!

```c++
class String {
private:
    size_t sz = 0;
    char* str = nullptr;
public:
    String(size_t sz, char ch) 
    	: sz(sz), str(new char[sz]) // теперь мы гарантируем порядок
	{
    	memset(str, ch, sz)
	}
}
```



## 3.5. Arithmetic operators overloading



```c++
class Complex 
{
    double re;
    double im;
public:
	// Тут есть уже есть все конструкторы
}
```



### +=, +

Сначала всегда определяем +=, а не + и =.

```c++
Complex& operator+=(const Complex& rhs)
{
    re += rhs.re;
    im += rhs.im;
    return *this;
}

Complex operator+(const Complex& rhs) 
{
    Complex sum = *this;
    return sum += rhs;
}
```

В данном случае произойдёт два раза копирование:

```C++
int main()
{
    Complex s(1.0, 2.0);
    Complex tmp = s + s;
}
```



### RVO - Return Value Optimization

Иногда компилятор помогает нам избежать лишних копирований

```C++
Complex operator+(const Complex& rhs)
{
    Complex sum = *this; // создаём объект
    sum += rhs;			 // проводим манипуляции с объектом
    return sum;			 // возвращаем по значению этот объект
}

int main() {
    Complex s(1.0, 2.0);
    // тут ещё есть Copy Elision (подробнее будет в будущем)
    Complex tmp = s + s;
}
```

Такой шаблон написания функции позволяет компилятору сразу создавать объект типа Complex в нужном месте (в точке main с tmp) и в операторе сложения один раз вызовется конструктор копирования от *this. 



Для такого оператора + существует проблема: левый операнд всегда должен быть класс Complex. В таком случае не получится сделать:

```c++
int main()
{
    Complex c(1.0, 2.0);
    Complex x = 5.0 + c;
}
```

Даже если есть неявное преобразование из ```double```:

```c++
public:
	Complex(double x) : Complex(x, 0.0) {};
```



На самом деле оператор + надо определять вне класса

```c++
Complex operator+(const Complex& lhs, const Complex& rhs)
{
    Complex sum = lhs;
    sum += rhs;
    return sum;
}
```



### ++obj и obj++

```C++
// Префиксный инкремент
Complex& operator++()
{
    return *this += 1.0;
}

// Постфиксный инкремент
Complex operator++(int) // костыль от создателей языка
{
    Complex copy = *this;
    ++*this;
    return copy;
}
```



Остальные операторы аналогично (-=, -, /=, / и др.)



## 3.6. Const методы и оператор []

Метод константный, если он не изменяет никакие приватные поля (кроме mutable)

```c++
class String
{
	size_t sz = 0;
	char* str = nullptr;
public:
	char& operator[](size_t index)
	{
		return str[index];
	}
}
```

Для const String это скомпилируется, но будет RE.

```C++
char operaator[](size_t index) const
{
    return str[index];
}
```

Для тяжелых объектов

```c++
const T& operator[](size_t index) const
{
    return member[index];
}
```

const методы класса могут вызываться как от константный, так и не константый объект класса.

## 3.7. Stream i/o overloading

Определяется вне класса

```c++
std::ostream& operator<<(std::ostream& out, const String& s)
{
    for(size_t i = 0; i < size(); ++i)
    {
        out << s[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& s)
{
    // тут должны менять строку
}
```

Иногда ввод из потока меняет приватные поля класса. Тогда надо объявить эту функцию другом.

```C++
public:
	friend std::istream& operator>>(std::istream& in, String& s);
```

Не стоит везде пихать friend везде, т.к. это нарушение инкапсуляции. Он позволяет получить доступ к приватным полям и методам класса, классу, который объявлен как друг.

Дружба не транзитивна: если A - друг B и B - друг C, то A - НЕ друг C.
# Лекция 7. Перегрузка операторов (продолжение)

## 3.8. Static fields and methods

Статическое поле - поле, общее для всего класса, т.е. не принадлежит какому-то конкретному объекту.
Точно так же со статическим методом.
Неконстантные статические поля нельзя инициализировать по-умолчанию. Только вне класса. Поскольку в таком случае после создания очередного экземпляра статическая память перезаписывалась бы значением по умолчанию.
```c++
/// Пример паттерна Singleton
class C {
public:
    static const int x = 1;
    static C& GetObject() {
        if(obj) return *obj;
        obj = new C();
        return *obj;
    }
    static void destroy() {
        delete obj;
    }
private:
    static C* obj;
    C() {

    }
};

C* C::obj = nullptr;
```

## 3.9. Type conversion overloading and explicit keyword

Конструктор определяет приведение типа и позволяет делать static_cast.
```c++
class UserId {
    int id = 0;

public:
    UserId(int id) : id(id) {}
}

int main() {
    auto id1 = static_cast<UserId>(1);
    UserId id2 = 2; // неявный каст
    UserId id3(3);
}
```

Существует также оператор приведения класса к какому-то типу.
```c++
class UserId {
    int id = 0;

public:
    UserId(int id) : id(id) {}

    operator int() {
        return id;
    }
}

int main() {
    UserId id = 5;
    std::cout << id + 1; // неявный каст
}
```
Если перегрузить для ```UserId``` оператор сложения, то будет ошибка компиляции, т.к. компилятору будет одинакого плохо приводить ```int --> UserId``` и ```UserId --> int```.

Надо помнить, что внутреннии приведения языка более предпочтительны с точки зрения компилятора, чем пользовательские.

Неявные касты плохо! Наприме, есть два класса: метры и секунды. Оба класса могут быть приведены к типу ```int```. И в какой-то момент можно было бы сложить метры и секунды, а это нарушение логики. 
Поэтому есть ключевое слово explicit.
```c++
class UserId {
    int x;
public:
    explicit UserId(int id) : id(id) { }
    explicit operator int() { return id; }
}

int main() {
    //UserId id = 3; Ошибка компиляции
    //std::cout << id + 1; Ошибка компиляции
    UserId id(3);
    std::cout << static_cast<int>(id) + 1; 
}
```


# Лекция 13. vtables, шаблоны

## 5.8. Virtual functions with virtual inheritance

```
      Granny
     /       \
    / virtual \  
 Mother       Father
    \         /
     \ usual /  
        Son
```

```c++
struct Granny {
    virtual void foo();
    int g;
}

struct Mother : public virtual Granny {
    int m;
}

struct Father : public virtual Granny {
    int f;
}

struct Son : public Father, public Mother 
{
    int s;
}
```

В памяти это будет лежать так: [vptr][m] [vptr][f] [s] [vptr][g]

И 3 виртуальные таблицы (для папы и мамы): [virt_base_offset] [top_offset] [typeinfo] [&foo]

virt_base_offset - смещение вправо, чтобы дойти до виртуальных предков. Т.е. до бабушки. Все виртуальные предки лежат подряд в конце. 

top_offset - смещение относительно начала области памяти лежит объект. Т.е. для мамы это будет 0 (возможно) и она понимает, что лежит первой. Для папы где он лежит относительно начал

typeinfo - указатель на информацию о типе


## 6. Templates
## 6.1. Idea of template

Может быть много разных функций, которые делают разные вещи для разных типов.

```c++
int max(int a, int b) {
    return a > b ? a : b;
}

double max(double a, double b) {
     // сравнение с какой-то точностью
}
```

И тогда можем:
```c++
template<typename T>
T mymax(T a, T b) {
    return a > b ? a : b;
}
```

Когда мы сделаем реальный вызов функции max компилятор сгенерирует для этого типа код функции. 

```c++
int main() {
    mymax(1, 2); // Тут комплиятор сгенерирует функцию с типом int
    mymax(std::cout, std::cout); // CE, т.к. он сгенерирует функцию для типа std::ostream& увидит, что их не получается сравнивать
}
```

При этом адреса функций будут разные, т.к. max<int>  и max<double> разные функции:
```c++
template<typename T>
T mymax(T a, T b) {
    return a > b ? a : b;
}

int main() {
    int (*pfi)(int, int) = &mymax<int>;
    double (*pfd)(double, double) = &mymax<double>;
    std:cout << (int*)pfi << "\n" << (int*)pfd << '\n';
}
/// Выведет 2 разных адреса
```

Что можно делать шаблонным?
```c++
/// Функции
template<typename T>
T foo(T a, T b) {}

/// Классы, структуры
template<typename T>
struct S {}

/// Методы внутри структур
class C {
    template<typename T>
    void f();
}

template<typename T>
void C::f() {}

/// Alias since c++11
template<typename T>
using my_tree = tree<T, null_type, less<T>, rb_tree_tag>;

// Переменные since c++14
template<typename T>
T pi = 3.14;
```

## 6.2. 
















































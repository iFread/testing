#include "calc_v2.h"
#include <cmath>
void Token_stream::putback(Token t)
{
  if(full) error("putback(), is not empty\n");
 buffer=t;
 full=true;
}

Token Token_stream::get()
{
   if(full)
   {
      full=false;
      return buffer;
   }
  char ch;
  std::cin>>ch;
 switch(ch)
 {
   case '+': case '-': case '(': case ')': case '*': case '/': case '=': case '%': case '!': case ':' :
    case quit:
   case print:
     return Token(ch);
   case '.':
 case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
  { std::cin.putback(ch);
     double val;
     std::cin>>val;
     return Token(number,val);
        }
 default:
     if(isalpha(ch))
     {
    //  std::cin.putback(ch);
      std::string s;
      s+=ch;
      while(std::cin.get(ch) &&(isalpha(ch)||isdigit(ch))) s+=ch;
      std::cin.putback(ch);

      if(s==declkey)
      {
          return Token(let);  // Token('L')   // будет вызываться declaration()
      }
     // if(s==func)  // нужен вектор ключевых слов, чтобы проверит по нему

      return Token(name,s); // Token('a', name)
                  // если за Token('a',name) следует'=' нужно присвоить  токену новое значение
      /*
  2 варианта: 1.определить функцию, как переменную, т.е
  обрабатывать ее как Token('a',name);
  2. разграничить функции от переменных, передавая ее как Token(func,name);
  в этом случае нужно определить список(вектор ключевых слов)

  */
     }
     error("Bad token");
 }
}
void Token_stream::ignore(char ch)
{ // сначала проверяем буфер на наличие ch
  if(full && ch==buffer.kind)
  {
    full=false;
    return;
  }
  char c=0;
 while(std::cin>>c)
  if(c==ch) return;
}
//****************************
Token_stream ts;  // для доступа к get(), ignore(),putback()
std::vector<Variable> var_table;
//**************************

bool is_declared(std::string s)
{
  for(size_t i=0;i<var_table.size();++i)
  {
    if(var_table[i].name==s) return true;
  }
  return false;
}

double define_name(std::string s,double val)
{
  if(is_declared(s)) error("Повторное объявление, "+s);
var_table.push_back(Variable(s,val));
return  val;
}

double declaration()
// считаем что встретили "let"
// Обрабатываем Имя= Выражение
// Объявление переменной с Именем и начальным значением заданным Выражением
{
  Token t=ts.get();
  if(t.kind!=name) error("exepted variable's name");
  std::string var_name=t.name;
  Token t2=ts.get();
  if(t2.kind!='=') error("exepted '=' in diclaration");
  double d=expression();
  define_name(var_name,d);
  return d;
}

double func_expr()
{  // проходимся по ключевым словам и в зависимости от t.name вычисляем результат
    //pow(2,5)  нужно обработать выражение пока не ','
    // обработать выражение после запятой,
    //применить pow(expression,expression);
    // вернуть результат.

}

double statement()  // здесь можно включить обработку функций
{                   // добавить case func: (const char func='f')
                  // и по этому case анализировать входную строку на ключевые слова pow(), sqrt() и т.д.
 /* примечание, функции должны выполнятся внутри выражения,
  * т.е. обрабатываться в primary(), а не в statement()

*/
   Token t=ts.get();
   switch (t.kind)
   {
     case let:
       return declaration();
   default:
       ts.putback(t);
       return expression();
      }
    }

void set_value(std::string s,double val)
{for( Variable &v:var_table)
    {
    if(v.name==s)
    {v.value=val;
        return;
       }
    }
    error("set : неопределенная переменая "+s);
   }

double get_value(std::string s)
{
  for(const Variable &v:var_table)
  {
    if(v.name==s) return v.value;
  }
  error("get: неопределенная переменная "+s);
  }


double expression()
{
    double left=0;
    left=term();
    Token t=ts.get();
    while (true)
    { switch (t.kind)
      {
   case'-':
       left-=term();
       t=ts.get();
       break;
   case '+':
       left+=term();
       t=ts.get();
         break;
      default:
      ts.putback(t);
      return left;
        }
    }
}

double term()
{
    double left=0;
    left=primary();
    Token t=ts.get();
  while(true)
  {
     switch(t.kind)
    {
     case '*':
      left*=primary();
      t=ts.get();
         break;
     case '/':
     {
         double d=primary();
         if(d==0) error("divided by zero");
        left/=d;
        t=ts.get();
        break;
     }
      case '%':
     {
         double d=0;
         if(d==0) error("divided by zero");
         left=fmod(left,d);
         t=ts.get();
         break;
     }
     default:
      ts.putback(t);
         return left;
     }
  }
}

double primary()
{ double d=0;
   Token t=ts.get();
   switch (t.kind)
   {

    case '(':
     { d=expression();
      t=ts.get();
      if(t.kind!=')') error("')' exepted");
      long res=get_fact(d);
       if(res) return res;

       return d;
   }
   case '-': return -primary();
  case '+': return primary();

   case number:
    { long res=get_fact(t.value);
         if(res) return res;
       else
      return t.value;
   }
   case quit:
   case print:
    ts.putback(t);
       break;
  case name:
   {
    Token t1=ts.get();
     if(t1.kind=='=')
     { char ch;
         std::cin.get(ch);
         if(ch=='\n')
         {
             return get_value(t.name);
         } else if(ch=='='){
             //std::cin.putback(ch);
             double d=0;
             d=expression();
          set_value(t.name,d);
          return d;} else
             std::cin.putback(ch);
     } else
     {
       ts.putback(t1);
      }
       //if(is_declared(t.name))
            return get_value(t.name);
  }
   default:
       error(" primary exepted");
  }
}
// вспомогательные функции
double get_fact(double x)
{
   Token t=ts.get();  // смотрим следующий символ
    if(t.kind=='!')
    {
      long res=1;
      for(int i=1;i<=int(x);++i)
      {
        res*=i;
      }
      return res;
    } else {
     ts.putback(t);
     return 0;}
}

void error(const std::string &s)
{
  throw std::runtime_error(s);
}
 // вместо функции expression() в calculate() будет вызываться statement()
void calculate()
{
  while(std::cin)
  {
    try
      {
      std::cout<<promt;
      Token t=ts.get();
      while(t.kind==print) t=ts.get();
      if(t.kind==quit) break;
      ts.putback(t);
     std::cout<<": "<<statement()<<'\n'; // раньше expression()

      } catch(std::exception& ex)
      {
        std::cerr<<ex.what()<<'\n';
      clean_up_mass();
      }
  }

}
void clean_up_mass()
{
  ts.ignore(print);
}
// реализация переменных


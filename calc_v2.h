#ifndef CALC_V2_H
#define CALC_V2_H
/*
 простой калькулятор глава 6-7 Строуструп Принципы и практика
  // 19.09.21

 Грамматика

  Вычисление:
    Инструкция
    Вывод
    Выход
    Вычисление Инструкция
  Инструкция:
    Объявление
    Выражение
  Объявление
    "let" Имя "=" Выражение
  Выражение:
     Терм
     Выражение'+'терм
     Выражение'-' терм
 Терм:
    Первичное выражение
    Терм'*'первичное выражение
    Терм'/' первичное выражение
    Терм'%' первичное выражение
  Первичное выражение:
     Число
     '('Выражение')'
   Число:
      Литерал с плавающей точкой

    Вычисление - это новая продукция грамматики верхнего уровня, она будет выражать цикл в функции
    calculate(), который позволит выполнять несколько вычислений в ходе одного сеанса программы
При обработке выражений и объявлений это правило опирается на правило Инструкция (double statement())

declaration() - должна убедится что после ключевого слова let следует имя переменной, за ним символ '='
 и Выражение
 Должна добавлять в вектор var_table, объект Variable с именем name и значением выражения
Определение переменной будет состоять из двух частей:
    1. Проверка наличия в векторе var_table объекта с именем var
    2. Добавление объекта, если такого нет
 Т.к. неинициализированные переменные не будут использованы
     определены функции is_diclared(string var) и define_name(string var, double val)
*/
#include <iostream>
#include <vector>

const char number='8';
const char quit='q';
const char print=';';
const char promt='>';
const char let='L';
const char name='a';
const char func='f';
const std::string declkey="let";

// задать константные ключевые слова типа pow(expression,expression);
// sqrt(expression)
// и т.д
const std::string pow_="pow";
const std::string sqrt_="sqrt";


class Token
 {
   public:
    char kind;
    double value;
    std::string name;
    Token(char ch):kind(ch){}
    Token(char ch,double val):kind(ch),value(val){}
    Token (char ch,std::string nm): Token(ch,0){name=nm;}

 };

 class Token_stream
 {
  public:
     Token_stream():full(false),buffer(0) {}
     void putback(Token t);
     Token get();
    void ignore(char ch);
 private:
   bool full;
   Token buffer;
 };
//************************
            // реализация переменных
 class Variable
 {
 public:
   std::string name;
   double value;
   Variable(std::string s,double val):name(s),value(val){}

 };

 double get_value(std::string s); // поиск переменной в векторе
 void set_value(std::string s,double d);
 double statement();
 double declaration();
 bool is_declared(std::string var);
 double define_name(std::string var,double val);
 //*****************************

 extern Token_stream ts;  // для доступа к get(), ignore(),putback()
 extern std::vector<Variable> var_table;



 void error(const std::string& s);

 double expression();
double term();
double primary();

void calculate();
double get_fact(double d);
void clean_up_mass();

#endif // CALC_V2_H

#ifndef UNCOPYABLE_H_INCLUDED
#define UNCOPYABLE_H_INCLUDED


/*
Inheriting from this class makes classes uncopyable. This is useful because
a lot of the time we don't want to be able to copy stuff (like the Window or the Example class)
*/

class Uncopyable
{
  protected:
    Uncopyable () {}
    ~Uncopyable () {}
  private:
    Uncopyable (const Uncopyable&);
    Uncopyable& operator=(const Uncopyable &);
};

#endif // NONCOPYABLE_H_INCLUDED

#!/usr/bin/env python
# encoding: utf-8

import os

def calc(numbers):
    sum = 0
    for n in numbers:
        sum = sum + n * n
    return sum

print calc([1, 2, 3])

def person(name, age, **kw):
    print 'name:', name, 'age;', age, 'other:', kw

person('Michael', 30)
person('Bob', 35, city='Beijing')

def test(a, b, c=0, *args, **kw):
    print 'a', a, 'b', b, 'c', c, 'args', args, 'kw', kw
l = [1, 2, 3, 4]
d = {'x':99}
test(1, 2, 3, 4, x=99)
test(*l, **d)

for x, y in [(1, 1), (2, 4), (3, 9)]:
    print x, y
#列出当前目录下的文件
[d for d in os.listdir('.')]
print d

#小写转换成小写,判断变量是否是字符串
L = ['Hello', 'World', 18, 'Apple', 'None']
print [s.lower() for s in L if isinstance(s, str) == True]

#创建一个generator 然后利用for来迭代
g = (x * x for x in range(10))
for n in g:
    print n
#generator函数
def fib(max):
    n, a, b = 0, 0, 1
    while n < max:
        yield b
        a, b = b, a + b
        n = n + 1
#利用for来迭代generator函数
for x in fib(6):
    print x

def str2int(s):
    def fn(x, y):
        return x * 10 + y
    def char2num(s):
        return {'0':0, '1':1, '2':2, '3':3, '4':4, '5':5, '6':6, '7':7, '8':8, '9':9, '0':0}[s]
    return reduce(fn, map(char2num, s))

print str2int('13579')

def func(s):
    k = ''
    for i, x in enumerate(s):
        if (i == 0):
            x = s[i].upper()
            k += x;
        else:
            x = s[i].lower()
            k += x;
    return k

print map(func, ['adam', 'LISA', 'barT'])

def prod(l):
    def fn(x, y):
        return x * y
    return reduce(fn, l)
print prod([1, 2, 3, 4])

#删除1-100的素数
def Isprime(n):
    for i in range(2, n):
        if n % i == 0 and i != n:
            return True
    return False
print filter(Isprime, range(1, 101))
#删除1-100的素数

#返回函数
def count():
    fd = []
    for i in range(1, 4):
        def f():
            return i * i
        fd.append(f)
    return fd
f1, f2, f3 = count()
print f1()
print f2()
print f3()

#返回函数引用循环变量
def count1():
    fd = []
    for i in range(1, 4):
        def f(j):
            def g():
                return j * j
            return g
        fd.append(f(i))
    return fd
f1, f2 ,f3 = count()
print f1()
print f2()
print f3()
print f1.__name__

#装饰器
import functools
def log(text=None):
    def decorator(func=None):
        #将原始函数的属性复制到wrapper函数中
        if not callable(text):
            @functools.wraps(func)
            def wrapper(*args, **kw):
                print 'begin call'
                print '%s %s():' % (text, func.__name__)
                res = func(*args, **kw)
                print 'end call'
                return res
        else:
            def wrapper(*args, **kw):
                print 'begin call'
                print 'call %s():' %(func.__name__)
                res = func(*args, **kw)
                print 'end call'
                return res
        return wrapper
    if not callable(text):
        return decorator
    else:
        return decorator(text)

print ('带execute')
@log('time')
def now():
    print '2016-1-16'
now()
print now.__name__

print ('不带execute')
@log
def time():
    print '2016-1-16 20:58:00'
time()
#装饰器

#!/usr/bin/env python
# coding=utf-8

import os

SDKpath = '/home/chenzhengwei/esp/gitlab_esp/ESP8266_RTOS_SDK'
esp8266path = '/home/chenzhengwei/esp/esp8266.sh'

esp8266 = '. ' + esp8266path
print esp8266
os.system(esp8266)

os.chdir(SDKpath)
os.system('export IDF_PATH=$PWD')
# os.system('$IDF_PATH')

app = raw_input("输入要打开的程序组如（ledc）：")

# example

examplepath = SDKpath + '/examples/peripherals/' + app + '/main'
example = 'code ' + app + '_example_main.c'
os.chdir(examplepath)
print(os.getcwd())
os.system(example)
print(example)

# head

headpath = SDKpath + '/components/esp8266/include/driver'
head = 'code ' + app + '.h'
os.chdir(headpath)
print(os.getcwd())
os.system(head)
print(head)

# code

codepath = SDKpath + '/components/esp8266/driver'
code = 'code ' + app + '.c'
os.chdir(codepath)
print(os.getcwd())
os.system(code)
print(code)

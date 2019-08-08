# ledc-8266 使用指南

vscode 测试

markdown 测试

要关联一个远程库，使用命令 git remote add origin git@server-name:path/repo-name.git；

关联后，使用命令 git push -u origin master 第一次推送 master 分支的所有内容；

此后，每次本地提交后，只要有必要，就可以使用命令 git push origin master 推送最新修改；

分布式版本系统的最大好处之一是在本地工作完全不需要考虑远程库的存在，也就是有没有联网都可以正常工作，而 SVN 在没有联网的时候是拒绝干活的！当有网络的时候，再把本地提交推送一下就完成了同步，真是太方便了！

---

正确步骤：

推送到远端 git 仓库

> 1. git init //初始化仓库
> 2. git add .(文件 name) //添加文件到本地仓库
> 3. git commit -m "first commit" //添加文件描述信息
> 4. git remote add origin + 远程仓库地址 //链接远程仓库，创建主分支
> 5. git pull origin master // 把本地仓库的变化连接到远程仓库主分支
> 6. git push -u origin master //把本地仓库的文件推送到远程仓库

---

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>

# 测试部分

## 表格

| 职业 |   技能   |           说明 |
| ---- | :------: | -------------: |
| 牧师 | 思维窃取 |           偷牌 |
| 法师 |   炎爆   | 造成 10 点伤害 |

## 代码

`nothing`
**`printf("hello world\n");`**

_`斜体`_

**_`粗斜体`_**

~~you are fool~~

```c
#include <stdio.h>
void main(void)
    {
        printf("do nothing!!!\n");
    }
```

```python
print "hello world!!\n"

```

```html
<html>
  <h5>hello world</h5>
</html>
```

H^3^

```flow
st=>start: Start
op=>operation: Your Operation
cond=>condition: Yes or No?
e=>end
st->op->cond
cond(yes)->e
cond(no)->op
```

# git-分布式版本控制系统入门

## 0.仓库
+ 本地仓库(个人)
+ 远程仓库

## 1. SSH连接环境配置
### 1.1 SSH和HTTPS连接的区别
推荐使用 SSH 的情况：
+ 频繁推送代码（无需重复输入密码）
+ 需要更高安全性（密钥认证比密码更难破解）
+ 企业内网环境（允许 SSH 端口时）
  

推荐使用 HTTPS 的情况：
+ 临时克隆公开仓库（快速访问）
+ 公司网络限制 SSH 端口时
+ 不熟悉 SSH 密钥配置的初学者

### 1.2 SSH配置
步骤：
1. 本地在 git bash 中，获取加密传输的公钥：
```
ssh-keygen -t rsa -C "注册账号的邮箱名字"
```
2. 本地创建公钥，github新建SSH公钥连接
   
一直回车键，就可以看到公钥放到了 /c/Users/pc/.ssh/id_rsa.pub 路径下。
将id_rsa.pub内容复制，在github中的 Setting->SSH and GPG keys的菜单中选择 'New SSH key'，粘贴即可。<br>
Authentication Keys（认证密钥）和 Signing Keys（签名密钥），选择前者就行。

3. 验证连接：
~~~
ssh -T git@github.com
~~~

4. 配置邮箱和用户名，以后在github上提交的代码都会附带邮箱和用户名信息
~~~
git config --global user.name "名"
git config --global user.email "邮箱"

#查看
git config --list | grep user
~~~

## 2. git常用命令(按照流程)
### 2.1 创建一个仓库，然后将项目地址发给其他小伙伴克隆到本地
### 2.2 克隆到本地
~~~
git clone 地址
# 自动创建了两个东西：远程仓库 origin； 本地生成仓库 master(追踪origin/master)；
# 本地代码的代码目录就是工作区
~~~
### 2.3 查看项目日志——git log
### 2.4 查看项目状态——git status(看工作区的状态：add、修改等)
### 2.5 提交流程
完成了工作区的代码改动之后，提交代码：
~~~
git add "文件名"
# 从工作区提交到暂存区

git commit -m "提交的信息"
# 将暂存区内容提交到 **本地仓库的代码分支** 上,这个时候git status就可以看到本地代码超前了远程的仓库

git push origin master
# 将本地仓库中master分支的代码推送到远程仓库origin/master中，这个时候可以用git log查看改动
# 现在master好像改为了main，不过应该都一样

~~~

### 2.6 查看本地代码分支——git branch
### 2.7 更新拉取代码——git pull

## 3. 各阶段回退方法
1. 情况一：工作区代码写错了，不想要了，回退到本地仓库版本
```
git checkout -- <filename>
# <本地仓库/分支> => <当前工作区修改的内容/文件>覆盖掉
```

2. 情况二：工作区的代码已经add到暂存区，但是不想要了
```
git reset HEAD <filename>
# 取消暂存,可以不加文件名
```

3. 情况三：错误commit提交了，本地分支main领先远程分支origin/main
```
git reset --hard <commit ID(前面几位就行)>
# HEAD指针会指向最新的修改，这个操作是移动了HEAD指针，将本地仓库的内容回退
git log
# 提交历史（commit 历史），查看项目的正式提交记录
git reflog
# 所有 HEAD 的变动记录（包括 reset/rebase），查看你在本地做的所有操作轨迹，哪怕已被覆盖
```

4. **情况四**：如果将代码推送到远程仓库，如果错了就要挨骂了
先回退到之前正确的版本，然后重新推送上去。
```
## !!!注意
git push origin main
# 本地回退一版之后，没有办法推送，因为你是落后于远程版本的所以
git push -f origin main
# 趁别人没有发现，快点提交了，如果别人在回退之前提交了，你就等着挨骂吧
```

5. 总结
```
git checkout -- <filename>
git reset HEAD <filename>
git reset --hard <commit ID(前面几位就行)>
git reflog  # HEAD 的变动记录
git push -f origin main
git diff HEAD -- <file> # 查看工作区file文件和仓库中该文件最新版本的区别，代码合并可能才用
```

## 4. 代码冲突怎么解决(合作)
每次开始工作前都可以拉取一下远程代码
```
git pull
```
合作开发，远程仓库已经被别人提交了一个版本，代码产生冲突
git push origin main 报错
这时需要重新拉取，再重新推送。

需要手动解决的情况，改的是同一个地方(也会提示)。
修改完，再提交。

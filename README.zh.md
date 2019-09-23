# 19 级图灵班《高级程序设计》(上学期）
中文 | [英文](README.md)

这是 19 级图灵班《高级程序设计》上学期提交作业使用的 GitHub 仓库，请各位同学 Fork 本仓库到自己的账户下操作，并使用 Pull Request 提交到主仓库。

首先，根据 GitHub 的官方教程[生成新 SSH 密钥](https://help.github.com/cn/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent)与[新增 SSH 密钥到 GitHub 帐户](https://help.github.com/cn/articles/adding-a-new-ssh-key-to-your-github-account)，完成对 SSH 密钥的配置。执行 `ssh git@github.com`，出现如下输出则表明配置成功：

```
$ ssh git@github.com
The authenticity of host 'github.com (13.250.177.223)' can't be established.
RSA key fingerprint is SHA256:nThbg6kXUpJWGl7E1IGOCspRomTxdCARLviKw6E5SY8.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added 'github.com,13.250.177.223' (RSA) to the list of known hosts.
Hi Menci! You've successfully authenticated, but GitHub does not provide shell access.
Connection to github.com closed.
```

点击 Fork 主仓库到自己的账户：

<center><img src=".readme/fork.png" width="990"></center>

Fork 后打开 `https://github.com/USER/TuringAdvancedProgramming19A`（其中 `USER` 替换为自己的用户名，下同）即可查看自己账户下的仓库。

我们将需要 `git` 命令行工具，使用以下命令在 Ubuntu 中安装它（其它发行版或操作系统请参阅其文档）：

```bash
sudo apt install git -y
```

使用以下命令将仓库下载到本地（会在当前目录下创建 `TuringAdvancedProgramming19A` 文件夹）：

```bash
git clone git@github.com:USER/TuringAdvancedProgramming19A.git
```

在对应的任务文件夹（如 `Task 0`）内创建以自己学号和姓名命名的文件夹（如 `2019114514张三`），并放入需要提交的文件。使用以下命令向本地 `git` 提交这些修改：

```bash
# cd "TuringAdvancedProgramming19A/Task 0/"
git add 2019114514张三
git commit
```

在打开的编辑器中输入提交信息（不可为空）后保存并退出编辑器，即可完成本地提交。使用以下命令将提交推送到远程：

```bash
git push
```

如果出现如下输出，表示推送成功：

```bash
$ git push
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Delta compression using up to 4 threads
Compressing objects: 100% (4/4), done.
Writing objects: 100% (4/4), 2.57 KiB | 97.00 KiB/s, done.
Total 4 (delta 0), reused 0 (delta 0)
To github.com:Menci/TuringAdvancedProgramming19A.git
 + bdd68d8...1efda49 master -> master
```

最后，到 GitHub 上点击创建 Pull Request：

<center><img src=".readme/pr1.png" width="995"></center>

在接下来的页面中点击「compare across forks」，并在左边选择主仓库（`Menci/TuringAdvancedProgramming19A`），右边选择你的仓库：

<center><img src=".readme/pr2.png" width="990"></center>

输入标题和正文（可为空）后点击最下方的「Create pull request」即可。你的代码将在被审阅之后合并到主仓库。

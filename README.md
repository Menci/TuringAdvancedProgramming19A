# Turing Class 2019's Advanced Programming (first term)
[中文](README.zh.md) | English

This is the GitHub repository for Turing Class 2019's Advanced Programming lecture's tasks. Please fork it to your account and submit to the main repository with Pull Request.

Firstly, configure the SSH key with GitHub's offcial tutorial[Generating a new SSH key](https://help.github.com/en/articles/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) and [Adding a new SSH key to your GitHub account](https://help.github.com/en/articles/adding-a-new-ssh-key-to-your-github-account). Execute `ssh git@github.com` and you're successful if the output is like this:

```
$ ssh git@github.com
The authenticity of host 'github.com (13.250.177.223)' can't be established.
RSA key fingerprint is SHA256:nThbg6kXUpJWGl7E1IGOCspRomTxdCARLviKw6E5SY8.
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added 'github.com,13.250.177.223' (RSA) to the list of known hosts.
Hi Menci! You've successfully authenticated, but GitHub does not provide shell access.
Connection to github.com closed.
```

(I am the picture!)

After forking, open `https://github.com/USER/TuringAdvancedProgramming19A` (replace `USER` with your username, the same below) to view your repository.

Download the repository to local with this command (A folder named `TuringAdvancedProgramming19A` will be created in current directory):

```bash
git clone git@github.com:USER/TuringAdvancedProgramming19A.git
```

Create a folder with the name of your student ID and real name (e.g. `2019114514张三`) in the corresponding task directory (e.g. `Task 0`) and put your files to upload there. Use this command to commit your changes to the local `git`:

```bash
# cd "TuringAdvancedProgramming19A/Task 0/"
git add 2019114514张三
git commit
```

Enter the commit message (non-empty), then save and exit the editor to finish committing locally. Push the commit to the remote with this command:

```bash
git push
```

You're successful if the output is like this:

```bash
(I am the output!)
```

Finally create a Pull Request to me on GitHub. Your code will be merged to the main repository.

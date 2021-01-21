# Git.md
## I. Commit
```
git add .
git commit -m "message here"
git push <upstream name> <branch>
``` 
## II. Remote
1. add remote
```
git remote add <upstream name> <url>
```
2. change remote
```
git remote set-url <upstream name> <url>
```
## III. Branch / Tag
1. git create new branch
```
git branch <branch name>
```
or
```
git checkout -b <branch name>
```
checkout can be used to switch between branches
2. git create tag
```
git tag -a <tag name> -m <message here>
```
3. git change to tag
```
git checkout tags/<tag name>
```
4. git checkout
```
git checkout = git switch + git restore
```
## IV. git config
1. bare vs mirror, cannot be pushed
```
git clone --bare
git clone --mirror
```
a mirrored alone includes all remote branches and tags, but all local references will be overwritten each time you fetch, so it will always the same as the original repository
2. fetch tags
```
git config remote.origin.fetch 'refs/heads/*:refs/heads/*'
```
this allows `git fetch --all` to update tags info, mirror = bare plus this step

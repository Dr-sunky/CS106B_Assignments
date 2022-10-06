# CS106B_Assignments
## The assignments of c++ course from 斯坦福
## code for git
#### create a new repository on the command line

    echo "# 123" >> README.md
    git init
    git add README.md
    git commit -m "first commit"
    git branch -M main
    git remote add origin git@github.com:Dr-sunky/123.git
    git push -u origin main

#### update repositorie's file

    git status
    git add -A
    git commit -a -m "update"
    git push origin master -f

#### creat brunch
    git branch new_branch
    git checkout new-brunch

#### set tracking information for local branch with remote branch
    git branch --set-upstream-to=origin/<branch> release

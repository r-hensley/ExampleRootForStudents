To install git, type `sudo apt-get install git`.

Once you've installed git, you'll want to clone the repository so you can get updated files easily. Go to the directory you want to create the repository in, and type `git clone https://github.com/ryry013/ExampleRootForStudents.git`.

This should create a folder in your directory and download all the files. 

Next, whenever you want to download new files, type `git pull` while inside that directory and it'll update all the files. Be careful this doesn't overwrite any changes you made.

If you want to write something and upload it, try typing `git config`. 

To compare your files and the ones in the github repository, type `git status`, it'll tell you which ones have changed. 

To see specifically *what* has changed, type `git diff`, and then use the arrow keys to scroll all the way down and you'll see what changed. Press "q" to exit it. 

Eventually when you get to wanting to add to the github repository, you'll write `git status` to view what has changed, `git add <filename>` to add specific files that you want to upload (or `git add .` to add all edited files), `git commit` will ask you for a message describing what you changed, and then finally `git push` to upload them. It might ask you for some username/password info for github, I don't remember the details of what that part of the process looks like. You can google "How to push to github from git".  

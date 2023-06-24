# Guidelines

## Code

### Include guardes:

```cpp
#define LTYPE_MODULE_CLASSNAME_H 
```

where MODULE = MENU, DATABASE, GAME...\
and CLASSNAME = well... the class name duh, in all caps

### Namespaces:

All code should be in a namespace named after the corresponding module.\
Exemple: if a class Player is defined inside the module User, the class is defined as\

```cpp
namespace User { class Player { ... }; }
```

Only the main.cpp is not enclosed in a namespace (we use the global one)

### Coding conventions:

* variables and class attributes = snake_case
* classes, structs, enums, filenames = PascalCase
* functions and methods = camelCase
* constants = SCREAMING_SNAKE_CASE
* keep code somewhat readable: (avoid really long names and long bodies)
* comments: explain *why* you do something, not *what* is it doing \
  (unless it is really technical)
* indent is 4 spaces
* Every non-obvious class, struct or methode/function declaration should have an accompanying comment that describes
  what it is for and how it should be used. Syntax is Doxygen \
  (https://www-numi.fnal.gov/offline_software/srt_public_context/WebDocs/doxygen-howto.html)
* semaphores are banned, use mutexes, unless really valid reason
* DRY (Don't repeat yourself)
* KISS (Keep it simple, stupid)
* DWLC (Don't write like a cochon :D)

### Git, Github and Branches

#### Branches

branches names = kebab-case

* main = main branch of the project, all code on this branch is garanteed to work correctly, YOU DO NOT PUSH UNDER ANY
  CIRCUMSTANCES DIRECTLY ONTO IT, ONLY **TEST** CAN!
* test = the main working branch (branched out of *main*), from **everyone** branches from, this is where we merge and
  test for any bug, once a commit is deemed stable, it is merged onto *main*
* feature-name = a branch of *test* that implement one particular feature or module in question, **delete** the branch
  if it will not be worked on anymore.
* bugfix-### = a branch of *test* that is supposed to fix a particular issue with number ###

#### PullRequests and Worklow

Once you've implemented a feature or fixed a bug on your local branch, it is time to share it with the others:

* first push your local branch onto the remote branch that you are working on
* then, on github's website, open a pull request onto *test* (for more
  info: https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request). Its
  title should be descriptive of the changes you are making, just like a commit message. If the PR solves an issues, use
  the issue number in the title.
* Another person will check your changed and will review the PR. DO NOT MERGE YOUR OWN REQUESTS. You can tag people in
  particular if you want their attention, or spam on discord.
* After the merge into *test*, if the branch is not necessary, **delete** it, all existing branches should be under
  active development

If there are conflics: the person submitting should first resolve them locally. Github's docs have tutorial on "merge
conflicts".

Priorities: Issues > Pull Requests > normal task No non-issue PR are merged until issues are solved. PR are treated
FIFO.

**Important**: KEEP YOUR LOCAL COPY UP TO DATE. Everytime there is a new push on test, you should update your local test
branch (git pull or git fetch+merge). And if you updated test, merge the new test into your feature-branch (check Github
docs on Merges)
Use Github diff tools to see changes, so you can avoid any conflicts. Don't forget you can rewind your local repo if
necassary (failed pull or error)

Don't push editor config files (such as .vscode, .idea, add those to the gitignore)
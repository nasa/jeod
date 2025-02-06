#!/bin/bash

files_to_edit=$(find . -name '*.py' | \
  grep -v '/Exercises/' | \
  grep -v '/regression/' | \
  xargs grep -l '\.name *=')
sed_script='s/\.name *= *\(".*"\)/.set_name(\1)/'

if ! git diff-index --quiet HEAD -- ; then
  echo Your branch is dirty. Commit changes and retry.
  exit
fi

if ! test -z $(git status --porcelain) ; then
  echo Your branch has untracked changes. Commit new files and retry.
  exit
fi

echo Editing files:
for f in $files_to_edit ; do
  echo $f
  sed -i -e "$sed_script" $f
done

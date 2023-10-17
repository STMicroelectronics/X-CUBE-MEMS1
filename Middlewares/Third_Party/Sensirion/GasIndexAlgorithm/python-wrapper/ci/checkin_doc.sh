#!/bin/bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -euv -o pipefail

# clone the repo into a subfolder html, checkout the gh-pages into this folder and commit the freshly generated html
git clone "git@gitlab:${CI_PROJECT_PATH}.git"  html
cd html
git checkout gh-pages
rm -f empty.txt
rm -f *.html
rm -f *.js
rm -rf _*
cd ..
# make sure to copy .nojekyll
cp -rf public/.[!.]*  public/* html
cd html
git add .

if git diff-index --quiet HEAD --
then
  exit 0
fi

git commit -m"Automatic doc update: ${CI_COMMIT_SHORT_SHA}"
git push

#!/bin/bash

# set shell settings (see https://sipb.mit.edu/doc/safe-shell/)
set -eufv -o pipefail

# install ssh private key (set as CI variable in GitLab project settings)
mkdir -p ~/.ssh
echo "$SSH_DEPLOY_KEY" > ~/.ssh/id_rsa
chmod 400 ~/.ssh/id_rsa

# change remote URL to SSH to allow pushing with SSH
git remote set-url --push origin "git@gitlab:${CI_PROJECT_PATH}.git"

# set git author
git config --global user.name "GitLab-CI"
git config --global user.email "<>"

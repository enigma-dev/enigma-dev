#!/bin/bash +x
set -e #exit if any command fails

export TEST_HARNESS_MASTER_DIR="$1"

GIT_BRANCH=$(git branch | grep \* | cut -d ' ' -f2)
if [ -z $(git status | grep detached) ]; then
  GIT_DETATCHED="FALSE"
else
  GIT_DETATCHED="TRUE"
fi

if [ -d "${TEST_HARNESS_MASTER_DIR}" ]; then
  read -p "Remove existing directory ${TEST_HARNESS_MASTER_DIR} (y/n)? " CONT
  if [ "$CONT" = "y" ]; then
    rm -rf "${TEST_HARNESS_MASTER_DIR}"
  fi
fi

echo "Copying ${PWD} to ${TEST_HARNESS_MASTER_DIR}"
cp -p -r "${PWD}" "${TEST_HARNESS_MASTER_DIR}"

pushd "${TEST_HARNESS_MASTER_DIR}"

if [[ "${GIT_BRANCH}" == "master" && "${GIT_DETACHED}" == "FALSE" ]]; then
  echo "You appear to be on branch master with no changes. Checking out HEAD~1 for the comparison"
  git checkout HEAD~1
else
  echo "You appear to be on branch ${GIT_BRANCH}. Checking out branch master for the comparison"
  git stash
  git checkout master
fi

make all #rebuild emake and plugin incase we changed something there
./test-runner --gtest_filter=Regression.draw_test

popd

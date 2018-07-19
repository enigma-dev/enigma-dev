#!/bin/bash +x
set -e #exit if any command fails

export TEST_HARNESS_MASTER_DIR="$1"

GIT_BRANCH=$(git branch | grep \* | cut -d ' ' -f2)
if [ -z "$(git status | grep detached)" ]; then
  GIT_DETACHED="FALSE"
else
  GIT_DETACHED="TRUE"
fi

if [ -d "${TEST_HARNESS_MASTER_DIR}" ]; then
  read -p "Remove existing directory ${TEST_HARNESS_MASTER_DIR} (y/n)? " CONT
  if [ "$CONT" = "y" ]; then
    rm -rf "${TEST_HARNESS_MASTER_DIR}"
  fi
fi

mkdir -p "${PWD}/test-harness-out"

echo "Copying ${PWD} to ${TEST_HARNESS_MASTER_DIR}"
cp -p -r "${PWD}" "${TEST_HARNESS_MASTER_DIR}"

pushd "${TEST_HARNESS_MASTER_DIR}"

if [[ "${PWD}" == "${TEST_HARNESS_MASTER_DIR}" ]]; then
  git stash

  if [[ -n "$TRAVIS_PULL_REQUEST_SHA" ]] && [[ -n "$TRAVIS_BRANCH" ]]; then
    echo "This appears to be a Travis pull request integration run; checking out '$TRAVIS_BRANCH' for the comparison."
    git reset --hard "$TRAVIS_BRANCH"
  elif [[ -n "$TRAVIS_COMMIT_RANGE" ]]; then
    prev=${TRAVIS_COMMIT_RANGE%%.*}
    echo "This appears to be a Travis push integration run; checking out '$prev' for the comparison."
    git reset --hard "$prev"
  elif [[ "${GIT_BRANCH}" == "master" && "${GIT_DETACHED}" == "FALSE" ]]; then
    echo "You appear to be on branch master with no changes. Checking out HEAD~1 for the comparison"
    git reset --hard HEAD~1
  else
    echo "You appear to be on branch ${GIT_BRANCH}. Checking out branch master for the comparison"
    git reset --hard master
  fi

  git clean -f -d

  echo "Rebuilding plugin and harness from last commit..."
  make all
  echo "Generating regression comparison images..."
  mkdir -p "${PWD}/test-harness-out"
  ./test-runner --gtest_filter=Regression.draw_test

  popd
else
  echo "Failed to change directory to ${TEST_HARNESS_MASTER_DIR}. Something is horribly wrong..."
  echo "Aborting!"
  travis_terminate 1
fi

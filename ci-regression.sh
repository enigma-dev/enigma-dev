#!/bin/bash +x

set -e  # exit if any command fails

if [[ "$TRAVIS" -eq "true" ]]; then
  xfwm4 --repace & # We need a wm for these tests
fi

if [ -z "$1" ]; then
  echo "No directory specified to check out master for regression tests."
  echo "Aborting!"
  exit 1
fi
export TEST_HARNESS_MASTER_DIR="$1"

if [ -n "$2" ]; then
  MAKE_JOBS=$2
  echo "Make parallelism set to '$MAKE_JOBS'"
else
  MAKE_JOBS=2  # all machines are dual-core or better as of 2015, so sayeth the king
  echo "Make parallelism defaulted to 2."
fi

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

PREVIOUS_PWD=${PWD}
pushd "${TEST_HARNESS_MASTER_DIR}"
make all -j$MAKE_JOBS
./test-runner
if [[ "$TRAVIS" -eq "true" ]]; then
  # upload coverage report before running regression tests
  bash <(curl -s https://codecov.io/bash) -f "*.info"
fi
# move output to safe space until we can compare
mv ./test-harness-out ${PREVIOUS_PWD}
# clean before we checkout
make clean

if [[ "${PWD}" == "${TEST_HARNESS_MASTER_DIR}" ]]; then
  git stash

  if [[ -n "$TRAVIS_PULL_REQUEST_SHA" ]] && [[ -n "$TRAVIS_BRANCH" ]]; then
    echo "This appears to be a Travis pull request integration run; checking out '$TRAVIS_BRANCH' for the comparison."
    git checkout "$TRAVIS_BRANCH"
  elif [[ -n "$TRAVIS_COMMIT_RANGE" ]]; then
    prev=${TRAVIS_COMMIT_RANGE%%.*}
    echo "This appears to be a Travis push integration run; checking out '$prev' for the comparison."
    git checkout "$prev"
  elif [[ "${GIT_BRANCH}" == "master" && "${GIT_DETACHED}" == "FALSE" ]]; then
    echo "You appear to be on branch master with no changes. Checking out HEAD~1 for the comparison"
    git checkout HEAD~1
  else
    echo "You appear to be on branch ${GIT_BRANCH}. Checking out branch master for the comparison"
    git checkout master
  fi
  git clean -f -d

  # re-install deps incase they've changed
  echo "Reinstalling deps"
  ./CI/install_emake_deps.sh && ./CI/split_jobs.sh install

  echo "Rebuilding plugin and harness from last commit..."
  make all -j$MAKE_JOBS
  echo "Generating regression comparison images..."
  mkdir -p "${PWD}/test-harness-out"
  ./test-runner --gtest_filter=Regression.*

  popd
else
  echo "Failed to change directory to ${TEST_HARNESS_MASTER_DIR}. Something is horribly wrong..."
  echo "Aborting!"
  exit 1
fi

./CommandLine/testing/GitHub-ImageDiff.sh

#!/bin/bash +x

set -e  # exit if any command fails

 streaming() {
     INRES="1024x768" # input resolution
     OUTRES="1024x768" # output resolution
     FPS="39" # target FPS
     GOP="30" # i-frame interval, should be double of FPS, 
     GOPMIN="30" # min i-frame interval, should be equal to fps, 
     THREADS="2" # max 6
     CBR="1000k" # constant bitrate (should be between 1000k - 3000k)
     QUALITY="ultrafast"  # one of the many FFMPEG preset
     AUDIO_RATE="44100"
     STREAM_KEY="live_160175943_ZfI2WfvNIC1NOMvLJgzjz9SYKRz9R6" # use the terminal command Streaming streamkeyhere to stream your video to twitch or justin
     SERVER="live-sjc" # twitch server in California, see https://bashtech.net/twitch/ingest.php to change 
     
     ffmpeg -f x11grab -s "$INRES" -r "$FPS" -i :99.0 -f alsa -i pulse -f flv -ac 2 -ar $AUDIO_RATE \
       -vcodec libx264 -g $GOP -keyint_min $GOPMIN -b:v $CBR -minrate $CBR -maxrate $CBR -pix_fmt yuv420p\
       -s $OUTRES -preset $QUALITY -tune film -acodec libmp3lame -threads $THREADS -strict normal \
       -bufsize $CBR "rtmp://$SERVER.twitch.tv/app/$STREAM_KEY"
 }

if [[ "$TRAVIS" -eq "true" ]]; then
  export DISPLAY=:99.0
  Xvfb :99 -s "-screen 0 1024x768x24" &
  openbox-session &
  sleep 5
  streaming &
  # We need a wm for these tests
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
  bash <(curl -s https://codecov.io/bash) -f "*.info" -t "$_CODECOV_UPLOAD_TOKEN"
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

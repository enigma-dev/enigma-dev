#!/bin/bash
# Used to have EnigmaBot comment on GitHub pull requests with image analysis/benchmark results.

# GitHub information
gh_organization='enigma-dev'  # This is the user or organization who owns the repository we're interested in.
gh_repository='enigma-dev'    # This is the simple name of the repository.

gh_api='https://api.github.com/'                          # This is the GitHub API URL. We need this to actually probe GitHub for data.
orgrepo=$gh_organization'/'$gh_repository                 # This is just YOURORGANIZATION/YOURREPOSITORY, for convenience.
repo=$gh_api'repos/'$orgrepo                              # This is the full URL that fetches us repository information.
pullrequest=$repo"/issues/$TRAVIS_PULL_REQUEST/comments"  # Full URL to current pullrequest.

echo $pullrequest

imgur_api='https://api.imgur.com/3/image'  # This is the Imgur API URL. We need this to upload images.

function imgur_upload {
  echo $(curl --request POST \
              --url $imgur_api \
              --header "Authorization: Client-ID $imgur_client_id" \
              --form "image=@$1")
}

imgur_response=$( imgur_upload './enigma_draw_test.png' )
imgur_master_response=$( imgur_upload '/tmp/enigma-master/enigma_draw_test.png' )
imgur_diff_response=$( imgur_upload '/tmp/enigma_draw_diff.png' )

imgur_url=$(echo $imgur_response | jq --raw-output '.data."link"' )
imgur_master_url=$(echo $imgur_master_response | jq --raw-output '.data."link"' )
imgur_diff_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )

echo $imgur_url
echo $imgur_master_url
echo $imgur_diff_url

gh_comment="Regression tests have indicated that graphical changes have been introduced. \
Carefully review the following image comparison for anomalies and adjust the changeset accordingly.\n\
\n\
$TRAVIS_PULL_REQUEST_SHA | Master | Diff\n\
--- | --- | ---\n\
<a href='$imgur_url'><img alt='Image Diff' src='$imgur_url' width='200'/></a>|\
<a href='$imgur_master_url'><img alt='Image Diff' src='$imgur_master_url' width='200'/></a>|\
<a href='$imgur_diff_url'><img alt='Screen Save' src='$imgur_diff_url' width='200'/></a>\n"

curl -u $bot_user':'$bot_password \
  --header "Content-Type: application/json" \
  --request POST \
  --data '{"body":"'"$gh_comment"'"}' \
  $pullrequest

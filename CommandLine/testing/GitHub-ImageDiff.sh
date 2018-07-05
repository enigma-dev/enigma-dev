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

imgur_diff_response=$( imgur_upload '/tmp/enigma_draw_diff.png' )
imgur_response=$( imgur_upload './enigma_draw_test.png' )

imgur_diff_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )
imgur_url=$(echo $imgur_response | jq --raw-output '.data."link"' )

echo $imgur_diff_url
echo $imgur_url

gh_comment="Graphics fidelity seems to have been compromised by changes in $TRAVIS_PULL_REQUEST_SHA. \
Carefully review the following image comparison for anomalies and adjust the changeset accordingly.\n\
\n\
Image Diff | Screen Save\n\
--- | ---\n\
<a href='$imgur_diff_url'><img alt='Image Diff' src='$imgur_diff_url'/></a>|\
<a href='$imgur_url'><img alt='Screen Save' src='$imgur_url'/></a>\n"

curl -u $bot_user':'$bot_password \
  --header "Content-Type: application/json" \
  --request POST \
  --data '{"body":"'"$gh_comment"'"}' \
  $pullrequest

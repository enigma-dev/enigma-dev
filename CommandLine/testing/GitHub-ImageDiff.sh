#!/bin/bash
# Used to have EnigmaBot comment on GitHub pull requests with image analysis/benchmark results.

# GitHub information
gh_organization='enigma-dev'  # This is the user or organization who owns the repository we're interested in.
gh_repository='enigma-dev'    # This is the simple name of the repository.

gh_api='https://api.github.com/'                          # This is the GitHub API URL. We need this to actually probe GitHub for data.
orgrepo=$gh_organization'/'$gh_repository                 # This is just YOURORGANIZATION/YOURREPOSITORY, for convenience.
repo=$gh_api'repos/'$orgrepo                              # This is the full URL that fetches us repository information.
pullrequest=$repo"/issues/$TRAVIS_PULL_REQUEST/comments"  # Full URL to current pullrequest.

imgur_api='https://api.imgur.com/3/image'  # This is the Imgur API URL. We need this to upload images.

function imgur_upload {
  echo $(curl --request POST \
              --url $imgur_api \
              --header "Authorization: Client-ID $imgur_client_id" \
              --form "image=@$1")
}

echo $pullrequest

# temporarily caching the imgur upload until the rest is worked out to preserve rate limit
# imgur_diff_response=$( imgur_upload '/tmp/diff.png' )
# imgur_response=$( imgur_upload '/tmp/test.png' )
imgur_diff_response='{"data":{"id":"k1Y5Mqe","title":null,"description":null,"datetime":1530076880,"type":"image\/png","animated":false,"width":640,"height":480,"size":2441,"views":0,"bandwidth":0,"vote":null,"favorite":false,"nsfw":null,"section":null,"account_url":null,"account_id":0,"is_ad":false,"in_most_viral":false,"has_sound":false,"tags":[],"ad_type":0,"ad_url":"","in_gallery":false,"deletehash":"qQYUJmlu0ilBr1W","name":"","link":"https:\/\/i.imgur.com\/k1Y5Mqe.png"},"success":true,"status":200}'
imgur_response=$imgur_diff_response

imgur_diff_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )
imgur_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )

echo $imgur_url

gh_comment="Graphics fidelity seems to have been compromised by changes in this pull request. \
Carefully review the following image comparison for anomalies and adjust the changeset accordingly.\n\
### Diff\n\
![Diff Graphics Results]($imgur_diff_url)\n\
### Actual Results\n\
![Actual Graphics Results]($imgur_url)\n"

curl -u $bot_user':'$bot_password \
  --header "Content-Type: application/json" \
  --request POST \
  --data '{"body":"'"$gh_comment"'"}' \
  $pullrequest

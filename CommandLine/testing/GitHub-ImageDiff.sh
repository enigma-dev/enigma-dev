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

gh_comment_header="Regression tests have indicated that graphical changes have been introduced. \
Carefully review the following image comparison for anomalies and adjust the changeset accordingly.\n\
\n\
$TRAVIS_PULL_REQUEST_SHA | Master | Diff\n\
--- | --- | ---\n"

gh_comment_images=""

images=$(ls ./test-harness-out/*.png)

if [[ -z "${images}" ]]; then
  echo "Error: Comparison image folder is empty. Something is horribly wrong..."
else
  for image in "${images}"
  do
    diffname="/tmp/"$(basename "$image" .png)"_diff.png"
          
    echo "Comparing ${image} to /tmp/enigma-master/${image}..."
    compare -metric AE "${image}" "/tmp/enigma-master/${image}" "${diffname}"
    result=$?
    echo ""
    if [[ "$result" -eq "0" ]]; then
      echo "No differences detected :)"
    else
      echo "Mismatches detected :("
      echo "Uploading images..."
      
      imgur_response=$(imgur_upload "${image}")
      imgur_master_response=$(imgur_upload "/tmp/enigma-master/${image}")
      imgur_diff_response=$(imgur_upload "/tmp/${diffname}")
      
      imgur_url=$(echo $imgur_response | jq --raw-output '.data."link"' )
      imgur_master_url=$(echo $imgur_master_response | jq --raw-output '.data."link"' )
      imgur_diff_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )
      
      echo $imgur_url
      
      gh_comment_images="${gh_comment_images}\
        <a href='$imgur_url'><img alt='Image Diff' src='$imgur_url' width='200'/></a>|\
        <a href='$imgur_master_url'><img alt='Image Diff' src='$imgur_master_url' width='200'/></a>|\
        <a href='$imgur_diff_url'><img alt='Screen Save' src='$imgur_diff_url' width='200'/></a>\n"
      
    fi
  done
fi

if [[ ! -z "${gh_comment_images}" ]]; then
  curl -u $bot_user':'$bot_password \
    --header "Content-Type: application/json" \
    --request POST \
    --data '{"body":"'"${gh_comment_header}${gh_comment_images}"'"}' \
    $pullrequest
    
  travis_terminate 1
fi

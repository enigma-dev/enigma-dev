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

function enigmabot_post_comment {
  echo $(curl --H "Authorization: token $bot_comment_token" \
              --H "Content-Type: application/json" \
              --request POST \
              --data '{"body":"'"$1"'"}' \
              "$pullrequest")
}

gh_comment_header="Regression tests have indicated that graphical changes have been introduced. \
Carefully review the following image comparison for anomalies and adjust the changeset accordingly.\n\
\n\
$TRAVIS_PULL_REQUEST_SHA | Master | Diff\n\
--- | --- | ---\n"

gh_comment_images=""

master_dir="/tmp/enigma-master/test-harness-out"
pr_dir="${PWD}/test-harness-out"
diff_dir="/tmp"

master_images=$(ls ${master_dir}/*.png | xargs -n1 basename | sort)
pr_images=$(ls ${pr_dir}/*.png | xargs -n1 basename | sort)

# GitHub/JSON don't like tabs which comm can output, so delete them with tr
com_master_pr=$(comm -2 -3 <(echo "$master_images") <(echo "$pr_images") | tr -d '\t')
com_pr_master=$(comm -1 -3 <(echo "$master_images") <(echo "$pr_images") | tr -d '\t')

if [[ ! -z "${com_master_pr}" ]]; then
  deleted_images_comment="Error: The following images are found in master but not the pull request:\n\
  ${com_master_pr}\n"

  echo -e "${deleted_images_comment}"
  echo "Aborting!"
  if [[ "$TRAVIS" -eq "true" ]]; then
    enigmabot_post_comment "${deleted_images_comment}"
  fi
else
  if [[ ! -z "${com_pr_master}" ]]; then
    new_images_comment="Warning: The following images are found in the pull request but not master (new tests?):"

    echo "${new_images_comment}"
    echo "${com_pr_master}"
    echo "Continuing..."
    if [[ "$TRAVIS" -eq "true" ]]; then
      new_images_comment+="\n"
      while read -r image; do
        imgur_response=$(imgur_upload "${pr_dir}/${image}")
        imgur_url=$(echo $imgur_response | jq --raw-output '.data."link"' )
        new_images_comment+="### ${image}\n<a href='$imgur_url'><img alt='${image}' src='$imgur_url' width='200'/></a>\n"
      done <<< "${com_pr_master}"
      enigmabot_post_comment "${new_images_comment}"
    fi
  fi

  if [[ -z "${master_images}" ]]; then
    echo "Error: Comparison image folder is empty. Something is horribly wrong..."
  else
    while read -r image; do
      diffname="${diff_dir}"/$(basename "${image}" .png)"_diff.png"
      echo "Comparing ${master_dir}/${image} to ${pr_dir}/${image}..."
      result=$(compare -metric AE "${master_dir}/${image}" "${pr_dir}/${image}" "${diffname}" 2>&1 >/dev/null)
      echo "$result"
      if [[ "$result" -eq "0" ]]; then
        echo "No differences detected :)"
      else
        echo "Mismatches detected :("

        if [[ "$TRAVIS" -eq "true" ]]; then
          echo "Uploading images..."

          imgur_response=$(imgur_upload "${pr_dir}/${image}")
          imgur_master_response=$(imgur_upload "${master_dir}/${image}")
          imgur_diff_response=$(imgur_upload "${diffname}")

          imgur_url=$(echo $imgur_response | jq --raw-output '.data."link"' )
          imgur_master_url=$(echo $imgur_master_response | jq --raw-output '.data."link"' )
          imgur_diff_url=$(echo $imgur_diff_response | jq --raw-output '.data."link"' )

          echo $imgur_url

          gh_comment_images="<a href='$imgur_url'><img alt='Image Diff' src='$imgur_url' width='200'/></a>|\
          <a href='$imgur_master_url'><img alt='Image Diff' src='$imgur_master_url' width='200'/></a>|\
          <a href='$imgur_diff_url'><img alt='Screen Save' src='$imgur_diff_url' width='200'/></a>\n"
        fi
      fi
    done <<< "${master_images}"
  fi

  if [[ "$TRAVIS" -eq "true" ]] && [[ ! -z "${gh_comment_images}" ]]; then
    enigmabot_post_comment "${gh_comment_header}${gh_comment_images}"
  fi
fi

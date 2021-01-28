# Git Log
## I. `git tag --list --sort=creatordate`
show all the tags
## II. `git log --pretty="..." --merges --first-parent tag1..tag2`
1. tag1..tag2 means commits that tag2 has while tag1 doesn't have
2. tag1...tag2 means commits tag2 has tag1 doesn't have + tag1 has tag2 doesn't have

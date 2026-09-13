#!/bin/bash


first_file="text.txt"
blob_mode="100644"
new_file="new.txt"

echo "In this eg we will see the blob and tree git objects in depth"
echo "blob is a git object that coresponds to a normal Unix file"
echo "tree is the git object that corresponds to a Unix directory/subdirectory"

echo ""



echo "to see how this two git objects work let's create a directory called MyDir"
mkdir MyDir
echo ""

echo "now let initialise it as a git repository" 

cd MyDir 
git init
echo ""

echo "then let's see what git creates at init for objects" 
find .git/objects 
echo ""

echo "git has initialized the objects directory and created pack and info subdirectories in it"
echo "now let's create a file and save its content in object database"
echo "version 1" > $first_file
sha_1_v1=$(git hash-object -w $first_file)
echo ""

echo "now we will update the content $first_file"
echo "version 2" > $first_file
sha_1_v2=$(git hash-object -w $first_file)
echo ""

echo "now the file has 2 versions on the objects database"
find .git/objects -type f
echo ""

echo "git builds tree objects from staged element but since we do not have any staging area yet we will use the plumbing command update-index"
echo "git update index --add --cachefile mode SHA-1 file_name"
echo "--add option to add a file to staging area"
echo "--cacheinfo to grab the element from the database not form workarea"
echo "so let's add the first version of the $first_file to staging area"
git update-index --add --cacheinfo $blob_mode $sha_1_v1 $first_file
echo ""

echo "now let's build a tree object that contain that blob"
tree_sha=$(git write-tree)
echo ""

echo "after creating a tree that has the v1 if the $first_file, now lets update the $first_file and add a new file and add all that in a new tree"
git update-index --cacheinfo $blob_mode $sha_1_v2 $first_file
echo "new file content" > $new_file
git update-index --add $new_file
root_tree_sha=$(git write-tree)
echo ""

echo "now we will add the previous tree as a subtree to the latest tree using the read-tree command with --prefix option"
git read-tree --prefix=subdir $tree_sha
top_tree_sha=$(git write-tree)
echo ""

echo "finally we can see what is the structure that we built"
git cat-file -p $top_tree_sha

